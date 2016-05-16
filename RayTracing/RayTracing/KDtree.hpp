//
//  KDtree.hpp
//  RayTracing
//
//  Created by Alex on 08.05.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#ifndef KDtree_hpp
#define KDtree_hpp
#include "Structs.h"

class KDtree {
public:
    KDtree & buildTree(const std::vector<std::shared_ptr<Shape> > & shapes);
    std::pair<std::shared_ptr<Shape> , double> findIntrsection(const Ray & ray) const;
private:
    class Node {
    public:
        dim div_dim;
        std::pair<double , double> bounds[3];
        std::vector<std::shared_ptr<Shape> > shapes_;
        short height;
        int childs[2];
        int parent;
    };
    
    void build_node_(int node, dim div_dim,int empty_partition_cnt);
    bool is_time_to_stop_(int nodem, int empty_partition_cnt) const;
    void partition_(const std::vector<std::shared_ptr<Shape> > & data,
                    std::vector<std::shared_ptr<Shape> > & left ,
                    std::vector<std::shared_ptr<Shape> > & right,
                    double mid,
                    dim dim) const;
    
    std::vector<Node> nodes_;
    static const int EMPTY_PARTITION_LIMIT_ = 3;
};

#endif /* KDtree_hpp */
