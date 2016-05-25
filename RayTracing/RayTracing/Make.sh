#!/bin/sh

#  Make.sh
#  RayTracing
#
#  Created by Alex on 05.05.16.
#  Copyright © 2016 Alex. All rights reserved.

g++ -Wc++11-extensions main.cpp PPMTransformer.cpp SceneProcessor.cpp Structs.cpp KDtree.cpp -o raytracer
