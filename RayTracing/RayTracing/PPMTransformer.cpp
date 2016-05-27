//  PPMTransformer.cpp
//  RayTracing
//
//  Created by Alex on 15.04.16.
//  Copyright © 2016 Alex. All rights reserved.

#include "PPMTransformer.hpp"
#include <cassert>
#include <iostream>
typedef long long i64;
using std::string;
using std::vector;
using std::map;
using std::pair;


void PPMTransformer::transformToPPM(std::vector<std::vector<Color> > data , const std::string & fname) {
    assert(data.size() > 0);
    i64 ysize = data.size();
    i64 xsize = data[0].size();
    
    std::ofstream file(fname);
    assert(file.is_open());
    
    
    file << "P3 \n";
    file << xsize << " " << ysize << "\n";
    file << "255 \n";
    
    for(const auto & line : data) {
        for(const auto & pix : line) {
            file << pix.r << " " << pix.g << " " << pix.b <<"\n";
        }
    }
    file << "\n";
    file.close();
}

std::vector<std::shared_ptr<Shape> > PPMTransformer::getShapesFromFile(const std::string & fname) {
    std::ifstream file(fname);
    assert(file.is_open());
    
    int qnt;
    file >> qnt;
    assert(qnt >= 0);
    std::vector<std::shared_ptr<Shape> > result(qnt);

    for(int i = 0 ; i < qnt; ++i) {
        int id;
        file >> id;
        Color clr;
        long double mirror_rate;
        file >> clr.r >> clr.g >> clr.b;
        file >> mirror_rate;
        clr.r = std::max(5,clr.r);
        clr.g = std::max(5,clr.g);
        clr.b = std::max(5,clr.b);
        if(id == 1) {  // triangle
            crd vertices[3];
            for(int j = 0; j < 3; ++j) {
                file >> vertices[j].x >> vertices[j].y >> vertices[j].z;
            }
            
            std::shared_ptr<Triangle> t(new Triangle(vertices));
            t->setColor(clr);
            t->setMirrorRate(mirror_rate);
            result[i] = t;
        } else if(id == 2) {
            crd center;
            double radius;
            file >> center.x >> center.y >> center.z;
            file >> radius;
            std::shared_ptr<Sphere> s(new Sphere(center,radius));
            s->setColor(clr);
            s->setMirrorRate(mirror_rate);
            result[i] = s;
        } else {
            return result;
        }
    }
    
    file.close();
    return result;
}

std::vector<std::shared_ptr<Light> > PPMTransformer::getLightFromFile(const std::string & fname) {
    std::ifstream file(fname);
    assert(file.is_open());
    
    int qnt;
    file >> qnt;
    assert(qnt >= 0);
    std::vector<std::shared_ptr<Light> > result(qnt);
    
    for(int i = 0 ; i < qnt; ++i) {
        crd pos;
        int intense;
        file >> pos.x >> pos.y >> pos.z;
        file >> intense;
        std::shared_ptr<Light> l(new  Light(pos,intense));
        result[i] = l;
    }
    
    file.close();
    return result;
}

std::vector<std::shared_ptr<Shape> > PPMTransformer::scanDataFromASCISTL(const std::string & fname) {
    std::vector<std::shared_ptr<Shape> > shapes;
    std::ifstream file(fname);
    assert(file.is_open());
    std::string s;
    file >> s;
    if(s != "solid") {
        assert(0);
    }
    file >> s >> s;
    while(s != "endsolid") {
        file >> s;
        crd norm;
        file >> norm.x >> norm.y >> norm.z;
        file >> s >> s;
        crd v[3];
        for(int i = 0;i < 3;++i) {
            file >> s;
            file >> v[i].x >> v[i].z >> v[i].y;
//            v[i].x *= 1e2; v[i].y *= 1e2; v[i].z *= 1e2;
        }
        std::shared_ptr<Shape> tr = std::shared_ptr<Shape>(new Triangle(v));
        tr->setColor(Color(100,100,100));
        shapes.push_back(tr);
        file >> s >> s >> s;
    }
    file.close();
    return shapes;
}



Texture PPMTransformer::scanTextureFromPPM(const std::string & fname,
                                           const std::string texture_name,
                                           const int new_id,
                                           map<string,int> & texture_id) {;
    FILE * in = freopen(fname.data(), "r", stdin);
    string s;
    std::cin >> s;
    if(s != "P6") {
        assert(0);
    }
    
    int len, high;
    std::cin >> len >> high;
    texture_id[texture_name] = new_id;
    Texture result(high, std::vector <Color> (len));
    
    int maxcolor;
    std::cin >> maxcolor;
    getchar();
    
    for(int y = 0; y < high; ++y) {
        for(int x = 0; x < len; ++x) {
            result[y][x].r = (int)getchar();
            result[y][x].g = (int)getchar();
            result[y][x].b = (int)getchar();
        }
    }
    
    fclose(in);
    return result;
}

void PPMTransformer::scanTextureInfoFromFile(const std::string & fname,
                                             const map<std::string,int> & texture_id,
                                             const vector<std::shared_ptr<Shape> > & shapes) {
    std::ifstream file(fname);
    assert(file.is_open());
    int qnt, shape_id;
    long double rate;
    string texture_name;
    file >> qnt;
    for(int i = 0; i < qnt; ++i) {
        file >> shape_id >> texture_name >> rate;
        if(shape_id >= 0 && shape_id < shapes.size()) {
            if(texture_id.find(texture_name)!= texture_id.end()) {
                int x = texture_id.find(texture_name)->second;
                shapes[shape_id]->setTextureId(x);
                shapes[shape_id]->setTextureRate(rate);
            }
        }
    }
    
    file.close();
}

camera_info PPMTransformer::getCameraInfo(const std::string & fname) {
    
    std::ifstream file(fname.c_str());
    assert(file.is_open());
    camera_info result;
    std::string s;
    file >> s;
    file >> result.width;
    file >> s;
    file >> result.height;
    file >> s;
    file >> result.corner.x >> result.corner.y >> result.corner.z;
    file >> s;
    file >> result.nline.x >> result.nline.y >> result.nline.z;
    result.nline = normalize(result.nline);
    file >> s;
    file >> result.a.x >> result.a.y >> result.a.z;
    file >> s;
    file >> result.coef;
    file >> s;
    file >> result.distobs;
    file.close();
    return result;
}
