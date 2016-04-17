//
//  Structs.h
//  RayTracing
//
//  Created by Alex on 15.04.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#ifndef Structs_h
#define Structs_h

#include <vector>

struct Color {
    Color(){};
    Color(int  r_ , int g_ , int b_) {
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
    double length() const;
    
    double x,y,z;
};

crd operator * (const double & cof , const crd & v);
crd operator -(const crd & a , const crd & b);
crd operator +(const crd & a , const crd & b);

crd getNormVec(const crd & v);
double scal(const crd & a , const crd & b);
crd mult(const crd & a , const crd & b);


struct Ray {
    Ray(){}
    Ray(const crd & pt_ , const crd & dir_) {
        dir = dir_;
        pt = pt_;
    }
    
    crd dir;
    crd pt;
};

class Material {
public:
    Color color;
};

class Shape {
public:
    virtual std::pair<bool, crd> getIntersection(const Ray & ray) const = 0;
    virtual crd getNormal(const crd & point) const = 0;
    virtual Color getColor() const = 0;
    virtual void setColor(const Color & color) = 0;
};

class Triangle : public Shape {
public:
    Triangle(){};
    Triangle(crd vertices_[3]);
    
    std::pair<bool , crd> getIntersection(const Ray & ray) const;
    crd getNormal(const crd & point) const;
    Color getColor() const;
    void setColor(const Color & color);
    
private:
    bool is_inside_(const crd & point) const;
    double get_dist_(const crd & point) const;
    
    crd normal_;
    double D_; // from Ax + By + Cz + D = 0
    crd vertices_[3];
    Material material_;
    
    /* .. another params ..*/
};

//class Sphere : Shape {
//public:
//    crd getIntrsectPoint(const Ray & ray) const;
//    crd getNormal(const crd & point) const;
//    Color getColor() const;
//private:
//    crd center;
//    double radius;
//    Material material;
//};

#endif /* Structs_h */
