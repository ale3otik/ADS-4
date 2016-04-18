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
    
    crd corner(-width/2 , height/2 , -1000);
    crd nline = normalize(crd( 0.2 , -0.2, 1.0));
    crd a(1, -0.1 ,-0.2);
    crd b = normalize(mult(nline, a));
    cout << b.x << " " << b.y << " " << b.z <<endl;
    a = normalize(mult(b,nline));
    cout << a.x << " " << a.y << " " << a.z <<endl;
    cout << scal(a, b) <<endl;
//    crd nline = normalize(mult(b, a));
    double distobs = -3000;
    crd obs = corner + (width/2.0) * a + (height/2.0) * b + distobs * nline;
    
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