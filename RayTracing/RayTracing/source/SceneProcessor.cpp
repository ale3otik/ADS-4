//
//  SceneProcessor.cpp
//  RayTracing
//
//  Created by Alex on 15.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include "SceneProcessor.hpp"

using std::vector;
SceneProcessor::SceneProcessor(const std::vector<Triangle> & data) {
    this->data_ = data;
}

vector<vector<Color> > SceneProcessor::buildScene() {
    return vector<vector<Color> > ();
}

SceneProcessor & SceneProcessor::setScreenPosition(crd corner , crd a , crd b) {
    this->scr_corner_ = corner;
    this->scr_a_ = a;
    this->scr_b_ = b;
    return *this;
}

SceneProcessor & SceneProcessor::setObserverPosition(crd pos) {
    this->obsr_pos_ = pos;
    return *this;
}
