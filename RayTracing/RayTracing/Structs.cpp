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
using std::pair;
using std::make_pair;
using std::vector;
typedef long double ld;

long double crd::length() const {
    return sqrt(x * x + y*y + z * z);
}

long double crd::len2() const {
    return x * x + y*y + z * z;
}

inline crd operator -(const crd & a , const crd & b) {
    return crd(a.x - b.x , a.y - b.y , a.z - b.z);
}

inline crd operator +(const crd & a , const crd & b) {
    return crd(a.x + b.x , a.y + b.y , a.z + b.z);
}

inline  crd operator * (const long double & cof , const crd & v) {
    return crd(cof * v.x ,cof * v.y ,cof * v.z);
}

void crd::setDimCrd(dim dim , long double value) {
    switch (dim) {
        case Dim::x:
            x = value;
            break;
        case Dim::y:
            y = value;
            break;
        case Dim::z:
            z = value;
            break;
        default:
            assert(0);
    }
}

long double crd::getDimCrd(dim dim) const {
    switch (dim) {
        case Dim::x:
            return x;
            break;
        case Dim::y:
            return y;
            break;
        case Dim::z:
            return z;
            break;
        default:
            assert(0);
    }
}

inline long double get_pt_dist(const crd & a, const crd & b) {
    return (b-a).length();
}

inline long double get_pt_dist2(const crd & a, const crd & b) {
    return (b-a).len2();
}

inline  long double get_det3(const vector<crd> & cs) {
    assert(cs.size() == 3);
    
    // dangerous O_o !!!
    return    cs[0].x * (cs[1].y *cs[2].z - cs[1].z*cs[2].y)
            - cs[1].x * (cs[0].y *cs[2].z - cs[0].z*cs[2].y)
            + cs[2].x * (cs[0].y *cs[1].z - cs[0].z*cs[1].y);

}

inline crd solveMatrix3(const std::vector<crd> & columns , crd b) {
    assert(columns.size() == 3);
    vector<crd> new_columns = columns;
    long double det = get_det3(new_columns);
    assert(std::abs(det) > EPS);
    
    long double d[3];
    for(int i = 0; i < 3; ++i) {
        new_columns[i] = b;
        d[i] = get_det3(new_columns);
        new_columns[i] = columns[i];
    }
    return crd(d[0]/det , d[1] / det , d[2] / det);
}

inline crd normalize(const crd & vc) {
    long double len = vc.length();
    if(len < EPS) return crd(0,0,0);
    return (1.0/len) * vc;
}

inline long double scal(const crd & a , const crd & b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline crd mult(const crd & a , const crd & b) {
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
    D_ = - scal(normal_,v);
    assert(normal_.length() > EPS);
}

Color Triangle::getColor() const {
    return material_.color;
}

void Triangle::setColor(const Color & color) {
    material_.color = color;
}


std::pair<bool , long double> Triangle::getIntersection(const Ray & ray) const {
    long double dist = get_dist_(ray.pt);
    long double cos = std::abs(scal(ray.dir, normal_));
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
//    assert(fabs(coords.z) <  2 * EPS);
    return coords.x >= -EPS && coords.y >= -EPS && coords.x + coords.y <= (1.0 + EPS);
}

long double Triangle::get_dist_(const crd & pt) const {
    return std::abs(scal(normal_, pt) + D_);
}

pair<long double, long double> Triangle::getBoundRange(dim dim) const {
    long double dims[3];
    for(int i = 0; i < 3; ++i) {
        dims[i] = vertices_[i].getDimCrd(dim);
    }
    long double max = std::max(dims[0],std::max(dims[1],dims[2]));
    long double min = std::min(dims[0],std::min(dims[1],dims[2]));
    return make_pair(min - EPS,max + EPS);
}

/***************Sphere****************/

Sphere::Sphere(crd center, long double radius) {
    center_ = center;
    radius_ = radius;
}

pair<long double, long double> Sphere::getBoundRange(dim dim) const {
    long double min = center_.getDimCrd(dim) - radius_ - EPS;
    long double max = center_.getDimCrd(dim) + radius_ + EPS;
    return make_pair(min,max);
}

std::pair<bool , long double> Sphere::getIntersection(const Ray & ray) const {
    crd normal = -1 * ray.dir;
    long double D = - scal(normal,center_);
    long double dist = std::abs(scal(normal,ray.pt) + D);
    crd pt_intrs = ray.pt + dist * ray.dir;
    long double center_to_pt2 = get_pt_dist2(center_ , pt_intrs);
    
    if(center_to_pt2 > radius_ * radius_) {
        return make_pair(false , 0);
    }
    
    long double ret_dist = sqrt(radius_ * radius_ - center_to_pt2);
    
    crd pt_1 = pt_intrs - ret_dist * ray.dir;
    if(scal((pt_1 - ray.pt) , ray.dir) >= 0) {
        return make_pair(true , dist - ret_dist);
    }
    
    crd pt_2 = pt_intrs + ret_dist * ray.dir;
    if(scal((pt_2 - ray.pt) , ray.dir) >= 0) {
        return make_pair(true , dist + ret_dist);
    }
    
    return make_pair(false , 0);
}

crd Sphere::getNormal(const crd & point) const {
    crd normal = normalize(point - center_);
    if(get_pt_dist2(point, center_) < radius_ * radius_) {
        return -1 * normal;
    }
    return normal;
}

Color Sphere::getColor() const {
    return material_.color;
}

void Sphere::setColor(const Color & color) {
    material_.color = color;
}

/*************************************/

long double get_plane_intersection(const crd & pt , const crd & v1, const crd & v2, const Ray & ray) {
    crd n = normalize(mult(v1,v2));
    ld D_ = - scal(n,pt);
    ld dist = std::abs(scal(n, ray.pt) + D_);
    long double cos = std::abs(scal(ray.dir, n));
    if(cos < EPS) return -1.0;
    return dist/cos;
}
