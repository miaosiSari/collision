#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "polygon.h"
#include <memory>
#define COLLIDE 1
#define NONCOLLIDE 0
#define INVALID -1

struct Rectangle{
    Rectangle(float centerx, float centery, float h, float w, float dir){
        //h: x轴方向的长度
        //w: y轴方向的长度
        p.reset(new Polygon({{h/2, w/2}, {-h/2, w/2}, {-h/2, -w/2}, {h/2, -w/2}}));
        Polygon* p2 = p -> rotate(dir, centerx, centery);
        if(p2){
            p.reset(p2);
        }
    }
    ~Rectangle(){}
    void getunitedges(std::vector<Vertex2d>& edges) const{
        if(p){
            p -> getunitedges(edges);
        }
    }
    int collide(const Rectangle& another) const{
        if(p && another.p){
            return (int)p -> collide(*another.p);
        }
        return INVALID;
    }
    std::string __str__() const{
        if(p){
            return p -> __str__();
        }
        return "[FATAL]Invalid Polygon Pointer!";
    }
    py::list get(){
        if(p){
            return p -> get();
        }
        py::list empty;
        return empty;
    }
    std::shared_ptr<Polygon> p;
};

std::ostream& operator<<(std::ostream& os, const Rectangle& r){
    if(!r.p){
        return os;
    }
    os << (*r.p);
    return os;
}

#endif