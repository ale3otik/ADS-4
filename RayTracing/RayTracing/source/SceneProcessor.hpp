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
    SceneProcessor(const std::vector<Triangle> & data);
    SceneProcessor & setScreenPosition(crd corner , crd a , crd b);
    SceneProcessor & setObserverPosition(crd pos);
    std::vector<std::vector<Color> > buildScene();
private:
    std::vector<Triangle> data_;
    
    crd scr_a_;
    crd scr_b_;
    crd scr_corner_;
    
    crd obsr_pos;
    
};

#endif /* SceneProcessor_hpp */
