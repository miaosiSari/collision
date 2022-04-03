#ifndef POLYGON_H
#define POLYGON_H
#include <vector>
#include "pybind11/pybind11.h"
#include "vertex.h"
#include "util.h"

namespace py = pybind11;

#define    DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&);                \
    TypeName& operator=(const TypeName&);

struct Polygon: public Shape{
    std::vector<Vertex2d> vertices;
    //matrix是一个2*(vertices.size())类型的矩阵。每一列是一个顶点, 顶点顺序和vertices中的顶点顺序相同, 必须保证是逆时针(Counter Clockwise, CCW)
    Eigen::MatrixXf* matrix;
    py::list vertices_pylist;
    bool is_vertices_pylist_initialized;
    //vertices以逆时针方向初始化
    Polygon(std::vector<Vertex2d>& vertices):Shape(CONVEXPOLYGON), vertices(vertices), is_vertices_pylist_initialized(false){
        matrix = new (std::nothrow) Eigen::MatrixXf(2, this->vertices.size());
    }
    Polygon(std::vector<Vertex2d>&& vertices):Shape(CONVEXPOLYGON),vertices(std::move(vertices)), is_vertices_pylist_initialized(false){
        matrix = new (std::nothrow) Eigen::MatrixXf(2, this->vertices.size());
    }
    Polygon(py::list vertices_pylist):Shape(CONVEXPOLYGON), vertices_pylist(vertices_pylist), is_vertices_pylist_initialized(true){
        for (py::handle obj : vertices_pylist) {  // iterators!
            py::tuple t = obj.cast<py::tuple>();
            (this -> vertices).emplace_back(t[0].cast<float>(), t[1].cast<float>());
        }
        matrix = new (std::nothrow) Eigen::MatrixXf(2, this->vertices.size());
    }
    Polygon(Eigen::MatrixXf&& verticesmatrix, float biasx, float biasy): Shape(CONVEXPOLYGON), is_vertices_pylist_initialized(false){
        for(int i = 0; i < verticesmatrix.cols(); ++i){
            vertices.emplace_back(verticesmatrix(0, i) + biasx, verticesmatrix(1, i) + biasy);
        }
        matrix = new (std::nothrow) Eigen::MatrixXf(2, this->vertices.size());
    }
    ~Polygon(){
        if(matrix){
            delete matrix;
            matrix = nullptr;
        }
    }

    Polygon* rotate(float theta, float biasx=0.0f, float biasy=0.0f){
        //assign eigen
        bool theta_is_null = (::abs(theta) < 1e-9);
        if(!matrix){
            return nullptr;
        }
        Eigen::MatrixXf rotatematrix = getRotateMatrix(theta);
        for(size_t i = 0; i < vertices.size(); ++i){
            (*matrix)(0, i) = vertices[i][0];
            (*matrix)(1, i) = vertices[i][1];
        }
        return new Polygon(theta_is_null ? *matrix : rotatematrix * (*matrix), biasx, biasy);
    }

    void getunitedges(std::vector<Vertex2d>& edges) const{
        //计算所有边, 并单位化
        //unit函数: 将向量单位化
        //虽然是二重循环, 但边并不多, 不会慢
        for(size_t i = 0; i < vertices.size() && i + 1 < vertices.size(); ++i){
            Vertex2d unit = (vertices[i+1] - vertices[i]).unit();
            bool find = false;
            for(const Vertex2d& edge: edges){
                if(edge == unit || edge == -unit){
                    find = true;
                    break;
                }
            }
            if(!find){
                edges.push_back(unit);
            }
        }
        Vertex2d unit = vertices[0] - vertices[vertices.size() - 1];
        bool find = false;
        for(const Vertex2d& edge: edges){
            if(edge == unit || edge == -unit){
                find = true;
                break;
            }
        }
    }

    bool collide(Polygon& another) const{
        std::vector<Vertex2d> e;
        //分别计算出所有可能的分离轴
        getunitedges(e); 
        another.getunitedges(e);
        for(const Vertex2d& side: e){
            std::vector<Vertex2d> perpendiculars1, perpendiculars2;
            for(const Vertex2d& v: vertices){
                perpendiculars1.push_back(v.unitprojection(side));
            }
            for(const Vertex2d& v: another.vertices){
                perpendiculars2.push_back(v.unitprojection(side));
            }
            bool iscollide = collide_helper(perpendiculars1, perpendiculars2);
            if(!iscollide){
                return false; //Separating Axis Theorem (SAT): 找到一条分离轴
            }
        }
        //没有找到任何分离轴
        return true;
    }

    std::string __str__() const{
        std::string returnstr = "";
        size_t index = 0;
        for(const auto& v: vertices){
            returnstr += ("Vertex " + std::to_string(index) + ": " + v.__str__() + ";\n");
            ++index; 
        }   
        return returnstr;
    }

    py::list get(){
        if(is_vertices_pylist_initialized){return vertices_pylist;}
        py::list newlist;
        for(const Vertex2d& v: vertices){
            newlist.append(py::make_tuple(v[0], v[1]));
        }
        return newlist;
    }

private:
    DISALLOW_COPY_AND_ASSIGN(Polygon) //没必要拷贝多边形, matrix是个指针, 浅拷贝会出问题
};

std::ostream& operator<<(std::ostream& os, Polygon& p){
    size_t index = 0;
    for(const auto& v: p.vertices){
        os << "Vertex " << index << ": " << v << "\n";
        ++index;
    }
    return os;
}

#endif