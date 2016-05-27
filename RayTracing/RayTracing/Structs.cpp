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
typedef long long i64;
#define PI (ld)3.1415926535

void  Color::limit() {
    r = std::min(r,255);
    g = std::min(g,255);
    b = std::min(b,255);
}

Color operator * (long double val , const Color & clr) {
    return Color((ld)clr.r * val, (ld)clr.g * val, (ld)clr.b * val);
}

Color operator + (const Color & a , const Color & b) {
    return Color(a.r + b.r,a.g + b.g,a.b + b.b);
}



long double crd::length() const {
    return sqrt(x * x + y*y + z * z);
}

long double crd::len2() const {
    return x * x + y*y + z * z;
}

crd operator -(const crd & a , const crd & b) {
    return crd(a.x - b.x , a.y - b.y , a.z - b.z);
}

crd operator +(const crd & a , const crd & b) {
    return crd(a.x + b.x , a.y + b.y , a.z + b.z);
}

crd operator * (const long double & cof , const crd & v) {
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

crd solveMatrix3(const std::vector<crd> & columns , crd b) {
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

crd normalize(const crd & vc) {
    long double len = vc.length();
    if(len < EPS) return crd(0,0,0);
    return (1.0/len) * vc;
}

long double scal(const crd & a , const crd & b) {
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
    texture_id_ = -1;
    memcpy(vertices_, nvertices_, 3 * sizeof(crd));
    normal_ = normalize(mult(vertices_[1] - vertices_[0], vertices_[2] - vertices_[0]));
    mirror_rate_ = 0;
    
    crd & v = vertices_[0];
    D_ = - scal(normal_,v);
//    assert(normal_.length() > EPS);
}

Color Triangle::getColor() const {
    return material_.color;
}

void Triangle::setColor(const Color & color) {
    material_.color = color;
}


Color Triangle::getTextureColor(const crd & pt , const Texture & texture) const {
    crd v1 = vertices_[1] - vertices_[0];
    crd v2 = vertices_[2] - vertices_[0];
    crd v = pt - vertices_[0];
    
    i64 x_len = texture.size();
    i64 y_len = texture[0].size();
    
    i64 crd1 = (i64)(scal(v, v1)/(v1.length()));
    crd e2 = v2 - (scal(v1,v2)/v1.len2()) * v1;
    i64 crd2 = (i64)(scal(v, e2)/e2.length());
    return texture[crd1 % x_len][crd2 % y_len];
}

void Triangle::move(crd dir, long double length) {
    dir = normalize(dir);
    dir = length * dir;
    for(int i = 0; i < 3; ++i) {
        vertices_[i] = dir + vertices_[i];
    }
    
    Triangle t(vertices_);
    t.setColor(this->getColor());
    *this = t;
}
void Triangle::rescale(long double rate) {
    for(int i = 0; i < 3; ++i) {
        vertices_[i] = rate * vertices_[i];
    }
    Triangle t(vertices_);
    t.setColor(this->getColor());
    *this = t;
}

crd Triangle::getWeightCenter() const {
    crd res = crd(0,0,0);
    for(int i = 0; i < 3; ++i) {
        res = res + vertices_[i];
    }
    return (1.0/3.0) * res;
}

void  Triangle::setMirrorRate(long double val) {
    assert(val >= 0 - EPS && val <= 1 + EPS);
    mirror_rate_ = val;
}
long double Triangle::getMirrorRate() const {
    return mirror_rate_;
}

int Triangle::getTextureId() const{
    return texture_id_;
}
long double Triangle::getTextureRate() const {
    return texture_rate_;
}
void Triangle::setTextureId(int val) {
    texture_id_ = val;
}
void Triangle::setTextureRate(long double rate) {
    texture_rate_ = rate;
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
    texture_id_ = -1;
    center_ = center;
    radius_ = radius;
    mirror_rate_ = 0;
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

void Sphere::move(crd dir, long double length) {
    dir = normalize(dir);
    dir = length * dir;
    center_ = dir + center_;
}
void Sphere::rescale(long double rate) {
    radius_ *= rate;
}

crd Sphere::getWeightCenter() const {
    return center_;
}

int Sphere::getTextureId() const{
    return texture_id_;
}
long double Sphere::getTextureRate() const {
    return texture_rate_;
}
void Sphere::setTextureId(int val) {
    texture_id_ = val;
}
void Sphere::setTextureRate(long double rate) {
    texture_rate_ = rate;
}


Color Sphere::getColor() const {
    return material_.color;
}

Color Sphere::getTextureColor(const crd & point , const Texture & texture) const {
    i64 ymax = texture.size();
    i64 xmax = texture[0].size();
    
    crd pt = point - center_;
    ld theta = acos(pt.x/sqrtl(pt.z * pt.z + pt.x*pt.x + pt.y * pt.y));
    ld phi = atan(pt.x/pt.z);
    if(pt.z < 0) phi += PI;
    phi += PI/2.0;
    
    return texture[(i64) ((theta/PI) * ymax) % ymax][(i64) ( (phi/(2.0* PI))* xmax) % xmax];
}

void Sphere::setColor(const Color & color) {
    material_.color = color;
}

void  Sphere::setMirrorRate(long double val) {
    assert(val >= -EPS && val <= 1 + EPS);
    mirror_rate_ = val;
}
long double Sphere::getMirrorRate() const {
    return mirror_rate_;
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
