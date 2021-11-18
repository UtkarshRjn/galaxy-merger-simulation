#include "Vector.h"

//--------------------------------------------------------------------------------------------
Vector::Vector(double argx, double argy){
    x = argx;
    y = argy;
};

Vector operator + (Vector const &vec1,Vector const &vec2){
    Vector res;
    res.x = vec1.x + vec2.x;
    res.y = vec1.y + vec2.y;
    return res;
};

Vector operator - (Vector const &vec1,Vector const &vec2){
    Vector res;
    res.x = vec1.x - vec2.x;
    res.y = vec1.y - vec2.y;
    return res;
};

Vector operator / (Vector const &vec, double fac){
    Vector res;
    res.x = vec.x/fac;
    res.y = vec.y/fac;
    return res;
};

Vector operator * (double fac,Vector const &vec){
    Vector res;
    res.x = vec.x * fac;
    res.y = vec.y * fac;
    return res;    
};

double Vector::getX(){
    return x;
}

double Vector::getX(){
    return y;
}