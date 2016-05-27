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
#include "KDtree.hpp"
#include <vector>
class SceneProcessor ;
class process_worker {
public:
    process_worker(int y , int x, SceneProcessor * processor);
    void operator()();
    
    private :
    int x_ , y_;
    SceneProcessor * processor_;
};

class SceneProcessor {
public:
    SceneProcessor(): base_light(0.1) {};
    
    SceneProcessor & setScene(const std::vector<std::shared_ptr<Shape> > & shapes ,
                              const std::vector<std::shared_ptr<Light> > & light);
    
    SceneProcessor & setScreenPosition(crd corner, crd a, crd b, int width , int height);
    SceneProcessor & setObserverPosition(crd pos);
    SceneProcessor & setBaseIntensity(long double value);
    SceneProcessor & setTextures(const std::vector<Texture> & data);
    std::vector<std::vector<Color> > buildScene();
    friend process_worker;
    
private:
    std::vector<std::shared_ptr<Shape> > shapes_;
    std::vector<std::shared_ptr<Light> > light_;
    std::vector<Texture> textures_;
    KDtree tree_;
    std::vector<std::vector<Color> > result_;
    
    crd scr_a_;
    crd scr_b_;
    crd scr_corner_;
    int width_;
    int height_;
    
    crd obsr_pos_;
    
    long double base_light;
    
    Color get_pix_color_(std::shared_ptr<Shape> shape , long double dist , const Ray & ray, double rate) const;
    crd get_reflected_ray(const crd normal , const crd & ray) const ;
    long double get_intensity_(const crd & pt, const crd & normal) const;
    Color transform_color_(std::shared_ptr<Shape> shape,
                           const crd & point,
                           Color reflexed_color,
                           long double ext_intensity) const;
    
    void process_pixel_(int y , int x);
    
};

#endif /* SceneProcessor_hpp */
