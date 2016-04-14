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
#include "Structs.h"
using namespace std;
int main() {
    vector<Triangle> triangles = PPMTransformer::getShapesFromFile("elements.txt");
    
    
    return 0;
}