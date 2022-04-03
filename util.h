#ifndef UTIL_H
#define UTIL_H
#include <iostream>
#include <algorithm>
#include <chrono>
#include <math.h>
#include "Eigen/Dense"
#define pi (3.141593f)
#define arc (57.29582f)
#define iarc (0.017453f) //inverse arc
#define CONVEXPOLYGON 0
#define CIRCLE 1
#define ELLIPSE 2
using namespace std::chrono_literals;

template<typename duration_type=std::chrono::nanoseconds>
struct T{
    std::string hint;
    std::chrono::high_resolution_clock::time_point start;
    T():hint("Time(ns) elapsed:"){
        start = std::chrono::high_resolution_clock::now();
    }
    T(std::string& hint):hint(hint){
        start = std::chrono::high_resolution_clock::now();
    }
    ~T(){
        size_t time_elapsed = (size_t)std::chrono::duration_cast<duration_type>(std::chrono::high_resolution_clock::now() - start).count();
        std::cout << hint << time_elapsed << "\n";
    }
};

struct Shape{
    int type;
    Shape(int type): type(type){}
};

Eigen::MatrixXf getRotateMatrix(float theta){
    float costheta = cos(theta), sintheta = sin(theta);
    Eigen::MatrixXf m(2, 2);
    m << costheta, -sintheta,
         sintheta, costheta;
    return m;
}

#endif