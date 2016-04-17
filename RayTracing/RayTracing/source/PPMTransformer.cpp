//
//  PPMTransformer.cpp
//  RayTracing
//
//  Created by Alex on 15.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include "PPMTransformer.hpp"
#include <cassert>
typedef long long i64;
void PPMTransformer::transformToPPM(std::vector<std::vector<Color> > data , const std::string & fname) {
    assert(data.size() > 0);
    i64 ysize = data.size();
    i64 xsize = data[0].size();
    
    std::ofstream file(fname);
    assert(file.is_open());
    
    file << "P3 \n 255 \n";
    file << xsize << " " << ysize << "\n";
    for(const auto & line : data) {
        for(const auto & pix : line) {
            file << pix.r << " " << pix.g << " " << pix.b <<"\n";
        }
    }
    file << "\n";
    file.close();
}

std::vector<Triangle> PPMTransformer::getShapesFromFile(const std::string & fname) {
    std::ifstream file(fname);
    assert(file.is_open());
    
    int qnt;
    file >> qnt;
    assert(qnt > 0);
    std::vector<Triangle> result(qnt);

    for(int i = 0 ; i < qnt; ++i) {
        Color clr;
        file >> clr.r >> clr.g >> clr.b;
        crd vertices[3];
        for(int j = 0; j < 3; ++j) {
            file >> vertices[j].x >> vertices[j].y >> vertices[j].z;
        }
        
        Triangle t(vertices);
        t.setColor(clr);
        
        result[i] = t;
    }
    return result;
}