//
//  KDtree.cpp
//  RayTracing
//
//  Created by Alex on 08.05.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include "KDtree.hpp"
using std::vector;
using std::pair;
using std::make_pair;
using std::shared_ptr;

#define size(x) (int)(x).size()
typedef pair<double,double> Range;

struct ShapeCmp {
    dim dim_;
    Range bounds_;
    ShapeCmp(dim dim , Range bounds) {
        this->dim_ = dim;
        this->bounds_ = bounds;
    }
    
    bool operator() (const shared_ptr<Shape> & p1 , const shared_ptr<Shape> & p2) {
        Range b1 = p1->getBoundRange(dim_);
        b1.first = std::max(b1.first,bounds_.first);
        b1.second = std::min(b1.second,bounds_.second);
        Range b2 = p2->getBoundRange(dim_);
        b2.first = std::max(b2.first,bounds_.first);
        b2.second = std::min(b2.second,bounds_.second);
        
        return (b1.first + b1.second) <  (b2.first + b2.second);
    }
};

KDtree & KDtree::buildTree(const vector<shared_ptr<Shape> > & shapes) {
    nodes_.reserve(size(shapes));
    
    Node node;
    node.height = 0;
    for(int dim = 0; dim < 3; ++dim) {
        if(size(shapes) > 0) {
            node.bounds[dim] = shapes[0]->getBoundRange(dim);
        }
        for(int j = 1; j < size(shapes); ++j) {
            Range bounds = shapes[j]->getBoundRange(dim);
            node.bounds[dim].first = std::min(node.bounds[dim].first,bounds.first);
            node.bounds[dim].second = std::max(node.bounds[dim].second,bounds.second);
        }
        if(node.bounds[dim].second - node.bounds[dim].first < EPS) {
            node.bounds[dim].second += EPS/2.0;
            node.bounds[dim].first -= EPS/2.0;
        }
    }
    
    node.parent = -1;
    node.shapes_ = shapes;
    nodes_.push_back(node);
    
    build_node_(0, Dim::x, 0);
    return *this;
}

/************************private*************************/

inline bool KDtree::is_time_to_stop_(int node_idx, int empty_partition_cnt) const {
    if(empty_partition_cnt >= EMPTY_PARTITION_LIMIT_) return true;
    
    const Node & node = nodes_[node_idx];
    if(size(node.shapes_) < 3) return true;
    
    static const short height_limit = 10;
    if(node.height > height_limit) return true;
    
    return false;
}

void KDtree::partition_(const vector<shared_ptr<Shape> > & data,
                        vector<shared_ptr<Shape> > & left ,
                        vector<shared_ptr<Shape> > & right,
                        double mid,
                        dim dim) const {
    const double PREDICT_LENGTH = 2.0 / 3.0;
    left.clear();
    right.clear();
    left.reserve(PREDICT_LENGTH *  size(data));
    right.reserve(PREDICT_LENGTH * size(data));
    for(const auto & p : data) {
        Range bound = p->getBoundRange(dim);
        if(bound.first <= mid) {
            left.push_back(p);
        }
        if(bound.second >= mid) {
            right.push_back(p);
        }
    }
}
void KDtree::build_node_(int node_idx, dim div_dim, int empty_partition_cnt) {
    Node & node = nodes_[node_idx];
    memset(node.childs, 0 , 2 * sizeof(int));
    if(node.parent > 0) {
        memcpy(node.bounds, nodes_[node.parent].bounds, sizeof(nodes_[node.parent].bounds));
        node.height = nodes_[node.parent].height + 1;
    }
    
    if(is_time_to_stop_(node_idx,empty_partition_cnt)) return;
    node.div_dim = div_dim;
    
    int id_l = size(nodes_);
    int id_r = size(nodes_) + 1;
    node.childs[0] = id_l;
    node.childs[1] = id_r;
    
    std::sort(node.shapes_.begin(), node.shapes_.end(),ShapeCmp(div_dim, node.bounds[div_dim]));
    
    int mid_id = size(node.shapes_) / 2;
    Range bounds = node.shapes_[mid_id]->getBoundRange(div_dim);
    bounds.first = std::max(bounds.first, node.bounds[div_dim].first);
    bounds.second = std::min(bounds.second, node.bounds[div_dim].second);
    double mid = (bounds.first + bounds.second)/2.0;
    
    nodes_.resize(size(nodes_) + 2);
    Node * l = &nodes_[id_l];
    Node * r = &nodes_[id_r];
    l->parent = node_idx;
    r->parent = node_idx;
    partition_(node.shapes_, l->shapes_, r->shapes_, mid, div_dim);
    
    //refresh bound-box size
    vector<Node * > childs = {l,r};
    for(const auto & pt : childs) {
        for(int i = 0; i < size(pt->shapes_); ++i) {
            Range bounds = pt->shapes_[i]->getBoundRange(div_dim);
            pt->bounds[div_dim].first = std::min(pt->bounds[div_dim].first,bounds.first);
            pt->bounds[div_dim].second = std::max(pt->bounds[div_dim].second,bounds.second);
        }
        if(pt->bounds[div_dim].second - pt->bounds[div_dim].first < EPS) {
            pt->bounds[div_dim].first -= EPS/2.0;
            pt->bounds[div_dim].second += EPS/2.0;
        }
    }
    l->bounds[div_dim].second = std::min(mid,l->bounds[div_dim].second);
    r->bounds[div_dim].first = std::max(mid,r->bounds[div_dim].first);
    
    build_node_(id_l, Dim::next(div_dim), size(node.shapes_) == size(l->shapes_)?empty_partition_cnt+1:0);
    build_node_(id_r, Dim::next(div_dim), size(node.shapes_) == size(r->shapes_)?empty_partition_cnt+1:0);
}

pair<shared_ptr<Shape> , double> KDtree::findIntrsection(const Ray & ray) const  {
    return make_pair(shared_ptr<Shape>(),false);
}
