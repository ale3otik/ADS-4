//
//  Object.cpp
//  RayTracing
//
//  Created by Alex on 25.05.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include "Object.hpp"

using std::vector;
using std::shared_ptr;
void Object::rescale(vector<shared_ptr<Shape> > & shapes , long double rate) {
    for(auto & pt : shapes) {
        pt->rescale(rate);
    }
}

void Object::move(vector<shared_ptr<Shape> > & shapes , crd dir , long double length) {
    for(auto & pt : shapes) {
        pt->move(dir, length);
    }
}
void Object::setPosition(vector<shared_ptr<Shape> > & shapes , crd pos) {
    crd center = crd(0,0,0);
    for(auto & pt : shapes) {
        center = center + pt->getWeightCenter();
    }
    center = 1.0 / (long double)(shapes.size()) * center;
    Object::move(shapes, pos - center, (pos-center).length());
}

//void Object::turn90degrees(std::vector<std::shared_ptr<Shape> > & shapes , dim d) {
//    
//}