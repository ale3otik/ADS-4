//
//  Structs.h
//  RayTracing
//
//  Created by Alex on 15.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#ifndef Structs_h
#define Structs_h

struct color {
    color(){};
    color(int  r_ , int g_ , int b_) {
        r = r_;
        g = g_;
        b = b_;
    }
    int r , g, b;
};

struct crd {
    crd(){};
    crd(double x_ , double y_ , double z_) {
        x = x_;
        y = y_;
        z = z_;
    }
    double x,y,z;
};

struct Triangle {
    crd vertices[3];
    color color;
    
    /* .. another params ..*/
};

//struct PolygonShape {
//    std::vector<Triangle> polygons;
//};


#endif /* Structs_h */
