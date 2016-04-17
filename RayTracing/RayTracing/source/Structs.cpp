//
//  Structs.cpp
//  RayTracing
//
//  Created by Alex on 17.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include "Structs.h"
#include <vector>
#include <cmath>
#include <cassert>
const int EPS = 1e-12;
using std::pair;

double crd::length() const {
    return sqrt(x * x + y*y + z * z);
}

crd operator -(const crd & a , const crd & b) {
    return crd(a.x - b.x , a.y - b.y , a.z - b.z);
}

crd operator +(const crd & a , const crd & b) {
    return crd(a.x + b.x , a.y + b.y , a.z + b.z);
}

crd operator * (const double & cof , const crd & v) {
    return crd(cof * v.x ,cof * v.y ,cof * v.z);
}
crd getNormVec(const crd & vc) {
    double len = vc.length();
    assert(len != 0);
    return (1.0/len) * vc;
}

double scal(const crd & a , const crd & b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

crd mult(const crd & a , const crd & b) {
    return crd(a.z * b.y - a.y * b.z,
               a.x * b.z - a.z * b.x,
               a.y * b.x - a.x * b.y);
}

/**************Triangle***************/

Triangle::Triangle(crd nvertices_[3]) {
    memcpy(vertices_, nvertices_, 3 * sizeof(crd));
    normal_ = mult(vertices_[1] - vertices_[0], vertices_[2] - vertices_[0]);
    
    crd & v = vertices_[0];
    D_ = - (normal_.x *  v.x + normal_.y * v.y + normal_.z * v.z);
    assert(normal_.length() > EPS);
}

Color Triangle::getColor() const {
    return material_.color;
}

void Triangle::setColor(const Color & color) {
    material_.color = color;
}


std::pair<bool , crd> Triangle::getIntersection(const Ray & ray) const {
    
    return std::make_pair(true , crd(0,0,0));
}

crd Triangle::getNormal(const crd & point) const {
    
    return crd(0,0,0);
}

bool Triangle::is_inside_(const crd & point) const {
    if(get_dist_(point) > EPS) return false;
    
    return true;
}

double Triangle::get_dist_(const crd & pt) const {
    return fabs(normal_.x * pt.x + normal_.y * pt.y + normal_.z * pt.z);
}
/***************Sphere****************/

//Color Sphere::getColor() const {
//    
//    return material.color;
//}
