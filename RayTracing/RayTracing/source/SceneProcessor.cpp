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

SceneProcessor & SceneProcessor::setBaseIntensity(double value) {
    assert(value >= 0);
    this->base_light = value;
    return *this;
}

vector<vector<Color> > SceneProcessor::buildScene() {
    vector<vector<Color> > result(height_ , vector<Color> (width_));
    for(int y = 0; y < height_; ++ y) {
        for(int x = 0; x < width_; ++ x) {
            crd pix_pos = scr_corner_ + (double)(x + 1) * scr_a_ + (double)(y + 1) * scr_b_;
            Ray r(pix_pos , pix_pos - obsr_pos_);
            pair<int , double> intrs = find_ray_intersection_(r);
            if(intrs.first < 0) {
                result[y][x] = Color(0,0,0);
            }
            else {
                result[y][x] = get_pix_color_(intrs.first, intrs.second, r);
            }
        }
    }
    return result;
}

double SceneProcessor::get_intensity_(const crd & pt, const crd & normal) const {
    double intensity = 0.0;
    for(int i = 0; i < light_.size(); ++i) {
        crd vec = light_[i]->pos - pt;
        crd nvec = normalize(vec);
        if(vec.len2() < 100 * 100) {
            
        }
        if(scal(nvec, normal) < 0.0) continue;
        
        // |>|>--D-- KOSTYL!!???//
        Ray r(pt + (2.0 * EPS) * nvec + (2 * EPS) * normal, nvec);
        
        pair <int , double> intrs = find_ray_intersection_(r);
        if(intrs.first >= 0 && intrs.second <= nvec.length()) continue;
        
        if(vec.len2() < 100 * 100) {
            
        }
        
        intensity += light_[i]->rate * fabs(scal(nvec , normal)) / vec.len2();
    }
    return intensity;
}

Color SceneProcessor::transform_color_(Color color , double intensity) const {
    intensity += base_light;
    color.r = std::min((int)(intensity * (double)color.r + 0.5), 255);
    color.g = std::min((int)(intensity * (double)color.g + 0.5), 255);
    color.b = std::min((int)(intensity * (double)color.b + 0.5), 255);
    return color;
}

pair<int , double > SceneProcessor::find_ray_intersection_(const Ray & ray) const {
    vector<pair<double,int> > dists;
    for(int i = 0; i < shapes_.size(); ++i) {
        pair<bool, double> res = shapes_[i]->getIntersection(ray);
        if(!res.first) continue;
        dists.push_back(std::make_pair(res.second,i));
    }
    
    if(dists.size() == 0) {
        return std::make_pair(-1 , 0);
    }
    
    int minid = 0;
    for(int i = 1; i < dists.size() ; ++i) {
        if(dists[i].first < dists[minid].first) {
            minid = i;
        }
    }
    return std::make_pair(dists[minid].second, dists[minid].first);
}

Color SceneProcessor::get_pix_color_(int id , double dist , const Ray & ray) const {
    
    crd pt = ray.pt + dist * ray.dir;
    crd normal = shapes_[id]->getNormal(pt);
    if(scal(-1*ray.dir , normal) < 0.0) {
        normal = -1 * normal;
    }
    
    double intensity = get_intensity_(pt , normal);
//    if(fabs(intensity) > EPS) {
//        std::cout << intensity <<std::endl;
//    }
    return transform_color_(shapes_[id]->getColor() , intensity);
}
