//
//  PPMtransformer.hpp
//  RayTracing
//
//  Created by Alex on 15.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#ifndef PPMTransformer_hpp
#define PPMTransformer_hpp

#include <vector>
#include <string>
#include <fstream>
#include "Structs.h"

class PPMTransformer {
public:
    static void transformToPPM(std::vector<std::vector<Color> > data , const std::string & fname);
    static std::vector<Triangle> getShapesFromFile(const std::string & fname);
};


#endif /* PPMTransformer_hpp */
