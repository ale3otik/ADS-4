//
//  Object.hpp
//  RayTracing
//
//  Created by Alex on 25.05.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include "Structs.h"
class Object {
public:
    static void rescale(std::vector<std::shared_ptr<Shape> > & shapes , long double rate);
    static void move(std::vector<std::shared_ptr<Shape> > & shapes , crd dir , long double length);
    static void setPosition(std::vector<std::shared_ptr<Shape> > & shapes , crd pos);
//    static void turn90degrees(std::vector<std::shared_ptr<Shape> > & shapes, dim d);
};

#endif /* Object_hpp */
