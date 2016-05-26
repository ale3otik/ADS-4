//
//  SceneProcessor.cpp
//  RayTracing
//
//  Created by Alex on 15.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include "SceneProcessor.hpp"
#include <cmath>
#include <cassert>
#include <iostream> 

using std::vector;
using std::pair;
using std::shared_ptr;
typedef long double ld;
SceneProcessor & SceneProcessor::setScene(const std::vector<std::shared_ptr<Shape> > & shapes,
                                          const std::vector<std::shared_ptr<Light> > & light) {
    shapes_ = shapes;
    light_ = light;
    return *this;
}

SceneProcessor & SceneProcessor::setScreenPosition(crd corner , crd a , crd b , int width , int height) {
    scr_corner_ = corner;
    scr_a_ = a;
    scr_b_ = b;
    width_ = width;
    height_ = height;
    return *this;
}

SceneProcessor & SceneProcessor::setObserverPosition(crd pos) {
    this->obsr_pos_ = pos;
    return *this;
}

SceneProcessor & SceneProcessor::setBaseIntensity(long double value) {
    assert(value >= 0);
    this->base_light = value;
    return *this;
}

vector<vector<Color> > SceneProcessor::buildScene() {
    tree_.buildTree(shapes_);
    vector<vector<Color> > result(height_ , vector<Color> (width_));
    for(int y = 0; y < height_; ++ y) {
        for(int x = 0; x < width_; ++ x) {
            crd pix_pos = scr_corner_ + (long double)(x + 1) * scr_a_ + (long double)(y + 1) * scr_b_;
            Ray r(pix_pos , pix_pos - obsr_pos_);
            pair<long double, std::shared_ptr<Shape> > intrs = tree_.findIntrsection(r);
            if(intrs.first < 0) {
                result[y][x] = Color(0,0,0);
            }
            else {
                result[y][x] = get_pix_color_(intrs.second, intrs.first, r,1.0);
            }
        }
    }
    return result;
}

long double SceneProcessor::get_intensity_(const crd & pt, const crd & normal) const {
    long double intensity = 0.0;
    for(int i = 0; i < light_.size(); ++i) {
        crd vec = light_[i]->pos - pt;
        crd nvec = normalize(vec);

        if(scal(nvec, normal) < 0.0) continue;
        
        // |>|>--D-- KOSTYL!!???//
        /** TO NOT INTERSECT CURRENT SHAPE **/
        Ray r(pt + (2.0 * EPS) * nvec + (2.5 * EPS) * normal, nvec);
        
        pair <long double,shared_ptr<Shape> > intrs = tree_.findIntrsection(r);
        if(intrs.first >= 0 && intrs.first <= vec.length()) continue;
        intensity += light_[i]->rate * std::abs(scal(nvec , normal)) / (long double) vec.len2();
    }
    return intensity;
}

Color SceneProcessor::transform_color_(std::shared_ptr<Shape> shape, Color reflexed_color , long double intensity) const {
    intensity += base_light;
    Color color = shape->getColor();
    ld rate = shape->getMirrorRate();
    
    color.r = std::min((int)((1.0 - rate) * intensity * (ld)color.r + rate * (ld)reflexed_color.r + 0.5), 255);
    color.g = std::min((int)((1.0 - rate) * intensity * (ld)color.g + rate * (ld)reflexed_color.g + 0.5), 255);
    color.b = std::min((int)((1.0 - rate) * intensity * (ld)color.b + rate * (ld)reflexed_color.b + 0.5), 255);
    return color;
}

crd SceneProcessor::get_reflected_ray(crd normal , const crd & ray) const {
    ld cos = scal(normal, (-1) * ray);
    if(cos < EPS) return crd(0,0,0);
    normal = cos *  normal;
    crd offset = normal + ray;
    return normalize(normal + offset);
}

Color SceneProcessor::get_pix_color_(shared_ptr<Shape> shape, long double dist, const Ray & ray, double rate) const {
    
    crd pt = ray.pt + dist * ray.dir;
    crd normal = shape->getNormal(pt);
    if(scal(-1*ray.dir , normal) < 0.0) {
        normal = -1 * normal;
    }
    
    long double intensity = get_intensity_(pt , normal);
    if(shape->getMirrorRate() < EPS) {
        
    }
    Color mirror_clr = Color(0,0,0);
    if(rate * shape->getMirrorRate() > 0.1) {
        crd reflected_dir = get_reflected_ray(normal, ray.dir);
        crd reflected_pt = ray.pt + dist * ray.dir + 2 * EPS * normal + 2 * EPS * reflected_dir;
        Ray reflected_ray(reflected_pt,reflected_dir);
        
        pair<long double, std::shared_ptr<Shape> > intrs = tree_.findIntrsection(reflected_ray);
        
        if(intrs.first >= 0) {
            mirror_clr = get_pix_color_(intrs.second, intrs.first, reflected_ray, rate * shape->getMirrorRate());
        }
    }

    return transform_color_(shape, mirror_clr, intensity);
}
