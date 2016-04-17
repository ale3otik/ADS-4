//
//  Structs.cpp
//  RayTracing
//
//  Created by Alex on 17.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include "Structs.h"
#include <vector>
using std::pair;
/**************Triangle***************/

Triangle::Triangle(crd vertices_[3]) {
    memcpy(vertices, vertices_, 3 * sizeof(crd));
}

Color Triangle::getColor() const {
    return material.color;
}

void Triangle::setColor(const Color & color) {
    material.color = color;
}

std::pair<bool , crd> Triangle::getIntersection(const Ray & ray) const {
    
    
    return std::make_pair(true , crd(0,0,0));
}

crd Triangle::getNormal(const crd & point) const {
    return crd(0,0,0);
}


/***************Sphere****************/

//Color Sphere::getColor() const {
//    
//    return material.color;
//}
