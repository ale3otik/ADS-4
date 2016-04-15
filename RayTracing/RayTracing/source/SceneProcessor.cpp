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

vector<vector<color> > SceneProcessor::buildScene() {
    return vector<vector<color> > ();
}