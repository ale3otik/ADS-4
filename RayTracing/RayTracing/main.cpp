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
#include <map>
#include "Object.hpp"
#include "KDtree.hpp"
#include "Structs.h"

#include "PPMTransformer.hpp"
#include "SceneProcessor.hpp"

#include <chrono>


using std::vector;
using std::shared_ptr;
using std::cout;
using std::endl;
using std::map;
int main() {
    
//    vector<std::shared_ptr<Shape> > shapesGirya = PPMTransformer::scanDataFromASCISTL("stl/girya.stl");
//    vector<std::shared_ptr<Shape> > shapesChrist = PPMTransformer::scanDataFromASCISTL("stl/chri.stl");
//    
//    Object::rescale(shapesGirya, 25);
//    Object::setPosition(shapesGirya, crd(0,0,-200));
//    Object::rescale(shapesChrist, 1e-1);
//    Object::setPosition(shapesChrist, crd(-40,0,100));
//    
//    vector<std::shared_ptr<Shape> > shapes(shapesGirya.size() + shapesChrist.size());
//    
//    std::merge(shapesChrist.begin(),shapesChrist.end(),shapesGirya.begin(),shapesGirya.end(),shapes.begin());
//
    
//    vector<std::shared_ptr<Shape> > shapes = PPMTransformer::scanDataFromASCISTL("stl/gnomik.stl");
//        Object::rescale(shapes, 25);
//    Object::setPosition(shapes, crd(300,-150,-200));
    
//    vector<std::shared_ptr<Shape> > shapes = PPMTransformer::scanDataFromASCISTL("stl/panther_at_the_beach.stl");
//    Object::rescale(shapes, 7);
//    Object::setPosition(shapes, crd(0,0,-200));

//    vector<std::shared_ptr<Shape> > shapesFlats = PPMTransformer::getShapesFromFile("stl/flats.txt");
//    vector<std::shared_ptr<Shape> > shapesGirl = PPMTransformer::scanDataFromASCISTL("stl/nudegirl4.stl");
//    vector<std::shared_ptr<Shape> > shapes(shapesFlats.size() + shapesGirl.size());
//    Object::rescale(shapesGirl, 2);
//    Object::setPosition(shapesGirl, crd(0,0,0));
//    std::merge(shapesFlats.begin(),shapesFlats.end(),shapesGirl.begin(),shapesGirl.end(),shapes.begin());
//    Object::rescale(shapes, 1e-1);
    
    vector<std::shared_ptr<Shape> > shapes = PPMTransformer::getShapesFromFile("stl/serg_scene.txt");
    vector<Texture> textures(6);
    map<std::string, int> texture_id;
    textures[0] = PPMTransformer::scanTextureFromPPM("stl/images-2.txt","brick",0,texture_id);
    textures[1] = PPMTransformer::scanTextureFromPPM("stl/images-3.txt","brown_wood",1,texture_id);
    textures[2] = PPMTransformer::scanTextureFromPPM("stl/images-4.txt","sand",2,texture_id);
    textures[3] = PPMTransformer::scanTextureFromPPM("stl/images-5.txt","grey_wood",3,texture_id);
    textures[4] = PPMTransformer::scanTextureFromPPM("stl/images-6.txt","chess_field",4,texture_id);
    textures[5] = PPMTransformer::scanTextureFromPPM("stl/alena.ppm","alena", 5 ,texture_id);
    
    PPMTransformer::scanTextureInfoFromFile("stl/texture_info.txt", texture_id, shapes);
    
    vector<std::shared_ptr<Light> > light = PPMTransformer::getLightFromFile("stl/serg_light.txt");
    
    
    
    int width = 2000;
    int height = 1500;
    
//    crd corner(-width/20.0 , height/20.0 , -40);
       crd corner(-width/2 , height/2 , -800);
    crd nline = normalize(crd(0.2 , -0.2, 1.0));
    crd a(1, -0.1 ,-0.2);
    crd b =  0.9 * normalize(mult(nline, a));
    cout << b.x << " " << b.y << " " <<b.z <<endl;
    a =  0.9 * normalize(mult(b,nline));
    cout << a.x << " " << a.y << " " << a.z <<endl;
    cout << scal(a, b) <<endl;
    long double distobs = -3000;
    crd obs = corner + (width/2.0) * a + (height/2.0) * b + distobs * nline;
    
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    
//    KDtree tree = KDtree().buildTree(shapes);
    
    
    vector<vector<Color> > result = SceneProcessor()
                                    .setScene(shapes, light)
                                    .setScreenPosition(corner, a, b , width , height)
                                    .setObserverPosition(obs)
                                    .setTextures(textures)
                                    .buildScene();

    PPMTransformer::transformToPPM(result, "out.ppm");
    
    end = std::chrono::system_clock::now();
    long double x = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() / 1000.0;
    cout << x <<endl;
    
    cout << "OK" <<endl;
    return 0;
}