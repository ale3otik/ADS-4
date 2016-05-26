//
//  KDtree.hpp
//  RayTracing
//
//  Created by Alex on 08.05.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include "Structs.h"
#ifndef KDtree_hpp
#define KDtree_hpp


class KDtree {
public:
    KDtree & buildTree(const std::vector<std::shared_ptr<Shape> > & shapes);
    std::pair<long double, std::shared_ptr<Shape> > findIntrsection(const Ray & ray) const;
private:
    
    class BoundBox {
    public:
        BoundBox(){};
        std::pair<long double , long double> lims[3];
        
        std::pair<long double,long double> get_intersection(const Ray & ray) const;
    private:
        std::vector<crd> get_rect_(short id) const;
    };
    
    
    class Node {
    public:
        dim div_dim;
        double mid;
        BoundBox box;
        std::vector<std::shared_ptr<Shape> > shapes_;
        short height;
        int childs[2];
        int parent;
    };
    
    void build_node_(int node, dim div_dim,int empty_partition_cnt, int qnt);
    bool is_time_to_stop_(int nodem, int empty_partition_cnt) const;
    void partition_(const std::vector<std::shared_ptr<Shape> > & data,
                    std::vector<std::shared_ptr<Shape> > & left ,
                    std::vector<std::shared_ptr<Shape> > & right,
                    long double mid,
                    int id_mid,
                    dim dim) const;
    
    
    std::pair<long double, std::shared_ptr<Shape> >
    find_intersection_(int id,
                       std::pair<long double ,long double > intersection_dists ,
                       const Ray & ray) const;
    
    
    std::pair<long double , std::shared_ptr<Shape> >intersect_with_shapes_(const std::vector<std::shared_ptr<Shape> > & shapes,
                                                   const Ray & ray) const ;
    
    std::vector<Node> nodes_;
    static const int EMPTY_PARTITION_LIMIT_ = 3;
};

#endif /* KDtree_hpp */
