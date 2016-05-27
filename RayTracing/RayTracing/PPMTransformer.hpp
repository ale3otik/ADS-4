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
#include <map>
#include "Structs.h"

class PPMTransformer {
public:
    static void transformToPPM(std::vector<std::vector<Color> > data , const std::string & fname);
    static std::vector<std::shared_ptr<Shape> > getShapesFromFile(const std::string & fname);
    static std::vector<std::shared_ptr<Shape> > scanDataFromASCISTL(const std::string & fname);
    static std::vector<std::shared_ptr<Light> > getLightFromFile(const std::string & fname);
    
    static Texture scanTextureFromPPM(const std::string & fname,
                                      const std::string texture_name,
                                      const int new_id,
                                      std::map<std::string,int> & texture_id);
    
    static void scanTextureInfoFromFile(const std::string & fname,
                                        const std::map<std::string,int> & texture_id,
                                        const std::vector<std::shared_ptr<Shape> > & shapes);
    
    static camera_info getCameraInfo(const std::string & fname);
};


#endif /* PPMTransformer_hpp */
