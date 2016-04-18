//
//  main.cpp
//  RayTracing
//
//  Created by Alex on 11.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "PPMTransformer.hpp"
#include "SceneProcessor.hpp"
#include "Structs.h"

using namespace std;
int main() {
    
    vector<std::shared_ptr<Shape> > triangles = PPMTransformer::getShapesFromFile("elements.txt");

    int width = 1500;
    int height = 1500;
    
    crd corner(1500 , 0 , 750);
    crd a(0,0,-1);
    crd b(-1,-1,0);
    crd obs(70000, -70000, 20000);
    
//    int width = 200;
//    int height = 200;
//    
//    crd corner(-width/2,-height/2,0);
//    crd a(1,0,0);
//    crd b(0,1,0);
//    
//    crd obs(0,0,-1000);

    
    vector<vector<Color> > result = SceneProcessor(triangles)
                                    .setScreenPosition(corner, a, b , width , height)
                                    .setObserverPosition(obs)
                                    .buildScene();

    PPMTransformer::transformToPPM(result, "out.ppm");
    
//    Ray r(crd(0,1,0) , crd(0,0,1));
//    pair<bool,crd> res = triangles[0].getIntersection(r);
//    bool bl = res.first;
//    crd itr = res.second;
//    cout << bl <<endl;
//    cout << itr.x << " " <<itr.y <<" "<< itr.z << endl;
    cout << "OK" <<endl;
    return 0;
}