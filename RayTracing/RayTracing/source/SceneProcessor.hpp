//
//  SceneProcessor.hpp
//  RayTracing
//
//  Created by Alex on 15.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#ifndef SceneProcessor_hpp
#define SceneProcessor_hpp
#include "Structs.h"
#include <vector>

class SceneProcessor {
public:
    SceneProcessor(const std::vector<std::shared_ptr<Shape> > & data);
    SceneProcessor & setScreenPosition(crd corner , crd a , crd b, int width , int height);
    SceneProcessor & setObserverPosition(crd pos);
    std::vector<std::vector<Color> > buildScene();
private:
    std::vector<std::shared_ptr<Shape> > shapes_;
    
    crd scr_a_;
    crd scr_b_;
    crd scr_corner_;
    int width_;
    int height_;
    
    crd obsr_pos_;
    
    Color trace_ray_(const Ray & ray) const;
};

#endif /* SceneProcessor_hpp */
