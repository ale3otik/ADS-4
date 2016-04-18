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
#include <iostream>
const double EPS = 1e-9;
using std::pair;
using std::make_pair;
using std::vector;
double crd::length() const {
    return sqrt(x * x + y*y + z * z);
}

double crd::len2() const {
    return x * x + y*y + z * z;
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

double get_det3(const vector<crd> & cs) {
    assert(cs.size() == 3);
    
    // dangerous O_o !!!
    return    cs[0].x * (cs[1].y *cs[2].z - cs[1].z*cs[2].y)
            - cs[1].x * (cs[0].y *cs[2].z - cs[0].z*cs[2].y)
            + cs[2].x * (cs[0].y *cs[1].z - cs[0].z*cs[1].y);

}

crd solveMatrix3(const std::vector<crd> & columns , crd b) {
    assert(columns.size() == 3);
    vector<crd> new_columns = columns;
    double det = get_det3(new_columns);
    assert(fabs(det) > EPS);
    
    double d[3];
    for(int i = 0; i < 3; ++i) {
        new_columns[i] = b;
        d[i] = get_det3(new_columns);
        new_columns[i] = columns[i];
    }
    return crd(d[0]/det , d[1] / det , d[2] / det);
}

crd normalize(const crd & vc) {
    double len = vc.length();
    assert(len > EPS);
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


Ray::Ray(const crd & pt_ , const crd & dir_) {
    dir = normalize(dir_);
    pt = pt_;
}

/**************Triangle***************/

Triangle::Triangle(crd nvertices_[3]) {
    memcpy(vertices_, nvertices_, 3 * sizeof(crd));
    normal_ = normalize(mult(vertices_[1] - vertices_[0], vertices_[2] - vertices_[0]));
    
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


std::pair<bool , double> Triangle::getIntersection(const Ray & ray) const {
    double dist = get_dist_(ray.pt);
    double cos = fabs(scal(ray.dir, normal_));
    if(cos < EPS) return make_pair(false , 0);
    
    dist /= cos;
    crd expected_pt = ray.pt + dist * ray.dir;
    if(is_inside_(expected_pt)) {
        return make_pair(true, dist);
    }
    return make_pair(false , 0);
}

crd Triangle::getNormal(const crd & point) const {
    return normal_;
}

bool Triangle::is_inside_(const crd & point) const {
    if(get_dist_(point) > EPS) return false;
    vector<crd> v(3);
    v[0] = vertices_[1] - vertices_[0];
    v[1] = vertices_[2] - vertices_[0];
    v[2] = normal_;
    crd b = point - vertices_[0];
    crd coords = solveMatrix3(v, b);
    assert(fabs(coords.z) < EPS);
    return coords.x >= -EPS && coords.y >= -EPS && coords.x + coords.y <= (1.0 + EPS);
}

double Triangle::get_dist_(const crd & pt) const {
    return fabs(normal_.x * pt.x + normal_.y * pt.y + normal_.z * pt.z + D_);
}
/***************Sphere****************/

//Color Sphere::getColor() const {
//    
//    return material.color;
//}
