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
    std::vector<std::vector<color> > buildScene();
private:
    std::vector<Triangle> data_;
};

#endif /* SceneProcessor_hpp */
