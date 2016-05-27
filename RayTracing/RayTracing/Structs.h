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

const long double EPS = 1e-9;

typedef short dim;
struct Dim {
    static dim next(dim a) {
        return (a + 1)%3;
    }
    static const dim x = 0;
    static const dim y = 1;
    static const dim z = 2;
};


struct Color {
    Color(){};
    Color(int  r_ , int g_ , int b_) {
        r = r_;
        g = g_;
        b = b_;
    }
    void limit();
    int r , g, b;
};

Color operator * (long double val , const Color & clr);
Color operator + (const Color & a , const Color & b);

typedef std::vector<std::vector<Color> > Texture;

struct crd {
    crd(){};
    crd(long double x_ , long double y_ , long double z_) {
        x = x_;
        y = y_;
        z = z_;
    }
    void setDimCrd(dim dim, long double value);
    long double getDimCrd(dim dim) const;
    long double length() const;
    long double len2() const;
    long double x,y,z;
};

struct Light {
    Light(){};
    Light(const crd & pos_ , long double rate_) {
        pos = pos_;
        rate = rate_;
    };
    
    crd pos;
    long double rate; // >= 0;
};

crd operator * (const long double & cof , const crd & v);
crd operator -(const crd & a , const crd & b);
crd operator +(const crd & a , const crd & b);

crd solveMatrix3(const std::vector<crd> & columns , crd b);

crd normalize(const crd & vc);
crd getNormVec(const crd & v);
long double scal(const crd & a , const crd & b);
crd mult(const crd & a , const crd & b);


struct Ray {
    Ray(){}
    Ray(const crd & pt_ , const crd & dir_);
    
    crd dir;
    crd pt;
};

class Material {
public:
    Color color;
};

class Shape {
public:
    virtual std::pair<bool, long double> getIntersection(const Ray & ray) const = 0;
    virtual crd getNormal(const crd & point) const = 0;
    virtual Color getColor() const = 0;
    virtual Color getTextureColor(const crd & pt , const Texture & texture) const = 0;
    virtual void setColor(const Color & color) = 0;
    virtual void move(crd dir, long double length) = 0;
    virtual void rescale(long double rate) = 0;
    virtual std::pair<long double, long double> getBoundRange(dim dim) const = 0;
    virtual crd getWeightCenter() const = 0;
    virtual void setMirrorRate(long double val) = 0;
    virtual long double getMirrorRate() const = 0;
    virtual int getTextureId() const = 0;
    virtual long double getTextureRate() const = 0;
    virtual void setTextureId(int) = 0;
    virtual void setTextureRate(long double) = 0;
};

class Triangle : public Shape {
public:
    Triangle(){};
    Triangle(crd vertices_[3]);
    
    std::pair<bool , long double> getIntersection(const Ray & ray) const;
    crd getNormal(const crd & point) const;
    Color getColor() const;
    Color getTextureColor(const crd & pt , const Texture & texture) const;
    void setColor(const Color & color);
    std::pair<long double, long double> getBoundRange(dim dim) const;
    void move(crd dir, long double length);
    void rescale(long double rate);
    crd getWeightCenter() const;
    void  setMirrorRate(long double val);
    long double  getMirrorRate() const;
    
    int getTextureId() const;
    long double getTextureRate() const;
    void setTextureId(int);
    void setTextureRate(long double);
    
private:
    bool is_inside_(const crd & point) const;
    long double get_dist_(const crd & point) const;
    
    crd normal_;
    long double D_; // from Ax + By + Cz + D = 0
    crd vertices_[3];
    Material material_;
    long double mirror_rate_;
    
    long double texture_rate_;
    int texture_id_;
    
    /* .. another params ..*/
};

class Sphere : public Shape {
public:
    Sphere(){};
    Sphere(crd center, long double radius);
    
    std::pair<bool , long double> getIntersection(const Ray & ray) const;
    crd getNormal(const crd & point) const;
    Color getColor() const;
    Color getTextureColor(const crd & pt , const Texture & texture) const;
    void setColor(const Color & color);
    std::pair<long double, long double> getBoundRange(dim dim) const;
    void move(crd dir, long double length);
    void rescale(long double rate);
    crd getWeightCenter() const;
    void  setMirrorRate(long double val);
    long double  getMirrorRate() const;
    
    int getTextureId() const;
    long double getTextureRate() const;
    void setTextureId(int);
    void setTextureRate(long double);
private:
    crd center_;
    long double radius_;
    Material material_;
    long double mirror_rate_;
    
    long double texture_rate_;
    int texture_id_;
};


long double get_plane_intersection(const crd & pt , const crd & v1, const crd & v2, const Ray & ray);
#endif /* Structs_h */
