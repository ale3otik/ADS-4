//
//  SceneProcessor.cpp
//  RayTracing
//
//  Created by Alex on 15.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include "SceneProcessor.hpp"

using std::vector;
using std::pair;

SceneProcessor::SceneProcessor(const std::vector<std::shared_ptr<Shape> > & data) {
    shapes_ = data;
}

vector<vector<Color> > SceneProcessor::buildScene() {
    vector<vector<Color> > result(height_ , vector<Color> (width_));
    for(int y = 0; y < height_; ++ y) {
        for(int x = 0; x < width_; ++ x) {
            crd pix_pos = scr_corner_ + (x + 1) * scr_a_ + (y + 1) * scr_b_;
            Ray r(pix_pos , pix_pos - obsr_pos_);
            result[y][x] = trace_ray_(r);
        }
    }
    return result;
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

Color SceneProcessor::trace_ray_(const Ray & ray) const {
    vector<pair<double,int> > dists;
    for(int i = 0; i < shapes_.size(); ++i) {
        pair<bool, double> res = shapes_[i]->getIntersection(ray);
        if(!res.first) continue;
        dists.push_back(std::make_pair(res.second,i));
    }
    
    if(dists.size() == 0) {
        return Color(0,0,0);
    }
    
    int minid = 0;
    for(int i = 1; i < dists.size() ; ++i) {
        if(dists[i].first < dists[minid].first) {
            minid = i;
        }
    }
    
    return shapes_[dists[minid].second]->getColor();
}
