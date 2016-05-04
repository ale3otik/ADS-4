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
    SceneProcessor(): base_light(0.1) {};
    
    SceneProcessor & setScene(const std::vector<std::shared_ptr<Shape> > & shapes ,
                              const std::vector<std::shared_ptr<Light> > & light);
    
    SceneProcessor & setScreenPosition(crd corner, crd a, crd b, int width , int height);
    SceneProcessor & setObserverPosition(crd pos);
    SceneProcessor & setBaseIntensity(double value);
    std::vector<std::vector<Color> > buildScene();
    
private:
    std::vector<std::shared_ptr<Shape> > shapes_;
    std::vector<std::shared_ptr<Light> > light_;
    
    crd scr_a_;
    crd scr_b_;
    crd scr_corner_;
    int width_;
    int height_;
    
    crd obsr_pos_;
    
    double base_light;
    
    std::pair<int , double> find_ray_intersection_(const Ray & ray) const;
    Color get_pix_color_(int id , double dist , const Ray & ray) const;
    double get_intensity_(const crd & pt, const crd & normal) const;
    Color transform_color_(Color color , double ext_intensity) const;
};

#endif /* SceneProcessor_hpp */
