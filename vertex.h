#ifndef VERTEX_H
#define VERTEX_H
#include "util.h"

struct Vertex3d{
    float v[3];
    Vertex3d(float x, float y, float z){
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }
};

std::ostream& operator<< (std::ostream& os, const Vertex3d& v){
    os << "(" << v.v[0] << ", " << v.v[1] << ", " << v.v[2] << ")";
    return os;
}

struct Vertex2d{
    float v[2];
    Vertex2d(float x, float y){
        v[0] = x;
        v[1] = y;
    }

    Vertex2d operator+(const Vertex2d& x) const{
        return Vertex2d(v[0] + x.v[0], v[1] + x.v[1]);
    }

    Vertex2d operator-(const Vertex2d& x) const{
        return Vertex2d(v[0] - x.v[0], v[1] - x.v[1]);
    }

    Vertex2d operator-(){
        return Vertex2d(-v[0], -v[1]);
    }

    Vertex3d operator*(const Vertex2d& x) const{
        //cross product
        return Vertex3d(0, 0, v[0] * x.v[1] - v[1] * x.v[0]);
    }

    bool operator==(const Vertex2d& x) const{
        return distance(x) < 1e-9;
    }

    float operator[](size_t i) const{
        return v[i];
    }

    inline float dot() const {
        return v[0] * v[0] + v[1] * v[1];
    }

    inline float dot(const Vertex2d& x) const{
        return v[0] * x.v[0] + v[1] * x.v[1];
    }

    inline float abs() const{
        return sqrt(dot());
    }

    inline float distance(const Vertex2d& x) const{
        return sqrt((v[0] - x.v[0]) * (v[0] - x.v[0]) + (v[1] - x.v[1]) * (v[1] - x.v[1]));
    }

    inline Vertex2d scalar(float c) const{
        return Vertex2d(c*v[0], c*v[1]);
    }

    inline Vertex2d projection(const Vertex2d& x) const{
        return x.scalar(dot(x) / x.dot());
    }

    inline Vertex2d unitprojection(const Vertex2d& x) const{
        return x.scalar(dot(x)); //note: you have to guarantee that x is a unit vector!
    }

    inline Vertex2d unit() const{
        float len = abs();
        if(len < 1e-9){
            return Vertex2d(v[0], v[1]);
        }
        return Vertex2d(v[0]/len, v[1]/len);
    }

    std::string __str__() const{
        return "(" + std::to_string(v[0]) + ", " + std::to_string(v[1]) + ")";
    }
};

std::ostream& operator<< (std::ostream& os, const Vertex2d& v){
    os << "(" << v.v[0] << ", " << v.v[1] << ")";
    return os;
}


bool collide_helper(std::vector<Vertex2d>& perpendiculars1, std::vector<Vertex2d>& perpendiculars2){
    //perpendicular: 垂足
    //也就是各个投影点的坐标
    //由于垂足共线, 用x坐标判断即可
    float minx1 = std::numeric_limits<float>::infinity();
    float maxx1 = -std::numeric_limits<float>::infinity();
    float minx2 = std::numeric_limits<float>::infinity();
    float maxx2 = -std::numeric_limits<float>::infinity();
    for(const Vertex2d& vertex: perpendiculars1){
        if(vertex[0] < minx1){
            minx1 = vertex[0];
        }
        if(vertex[0] > maxx1){
            maxx1 = vertex[0];
        }
    }
    for(const Vertex2d& vertex: perpendiculars2){
        if(vertex[0] < minx2){
            minx2 = vertex[0];
        }
        if(vertex[0] > maxx2){
            maxx2 = vertex[0];
        }
    }
    if((minx1 > maxx2) || (minx2 > maxx1)){
        return false;
    }
    return true;
}

#endif