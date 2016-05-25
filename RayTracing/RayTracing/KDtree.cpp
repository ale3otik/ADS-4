//
//  KDtree.cpp
//  RayTracing
//
//  Created by Alex on 08.05.16.
//  Copyright © 2016 Alex. All rights reserved.
//

#include <cmath>
#include "KDtree.hpp"
#include <cassert>
using std::vector;
using std::pair;
using std::make_pair;
using std::shared_ptr;
#define size(x) (int)(x).size()
typedef long double ld;
typedef pair<ld,ld> Range;
const crd BASE_VECTORS[3] = {crd(1,0,0),crd(0,1,0),crd(0,0,1)};
/************BoundBox*************/
inline short get_bit(unsigned short mask, unsigned short i) {
    return (mask & (1<<i)) > 0 ? 1 : 0;
}

vector<crd> KDtree::BoundBox::get_rect_(short id) const {
    unsigned short mask = 1 << id;
    vector<crd> rect(3);
    rect[0] = crd(get_bit(mask,0) == 0 ? lims[0].first : lims[0].second, //X
                  get_bit(mask,2) == 0 ? lims[1].first : lims[1].second,
                  get_bit(mask,4) == 0 ? lims[2].first : lims[2].second);
    
    rect[1] = crd(get_bit(mask,1) == 0 ? lims[0].second : lims[0].first,
                  get_bit(mask,3) == 0 ? lims[1].second : lims[1].first,
                  get_bit(mask,5) == 0 ? lims[2].second : lims[2].first);
    
    rect[2] = crd(((get_bit(mask,2) + get_bit(mask,3) == 0) ? rect[0].x : rect[1].x),
                   rect[1].y, rect[0].z);
//    assert((mult(rect[2] - rect[0], rect[1] - rect[0])).len2() < EPS/100.0 == false);
    return rect;
}

pair<ld,ld> KDtree::BoundBox::get_intersection(const Ray & ray) const {
    ld dists[2] = {-1,-1};
    int k = 0;
    for(int id = 0; id < 6; ++id) {
        dim null_dim = id / 2;
        vector<crd> rect = get_rect_(id);
//
        crd normal = crd(0,0,0);
        normal.setDimCrd(null_dim, 1);
        ld cos = std::abs(ray.dir.getDimCrd(null_dim));
        if(cos < EPS) continue;
//
        ld D = -rect[0].getDimCrd(null_dim); // = (n , rect[0])
        ld dist = std::abs(ray.pt.getDimCrd(null_dim) + D);
        dist /= cos;
        crd expected_pt = ray.pt + dist * ray.dir;
        ld dim_crd_val = expected_pt.getDimCrd(null_dim);
        if(std::abs(dim_crd_val + D) > 3.0 * EPS) continue;
        
        dim dims[] = {Dim::next(null_dim),Dim::next(Dim::next(null_dim))};
        bool miss = false;
        for(auto d : dims) {
            ld dval = expected_pt.getDimCrd(d);
            if(!(lims[d].first - EPS <= dval && lims[d].second + EPS >= dval)) {
                miss = true;
                break;
            }
        }
//
        if(!miss){
            dists[k] = dist;
            ++k;
            if(k == 2) break;
        }
    }
    if(dists[0] > dists[1]) std::swap(dists[0],dists[1]);
    return make_pair(dists[0],dists[1]);
}

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
//        b1.second = std::min(b1.second,bounds_.second);
        Range b2 = p2->getBoundRange(dim_);
        b2.first = std::max(b2.first,bounds_.first);
//        b2.second = std::min(b2.second,bounds_.second);
        
        return b1.first <  b2.first;
    }
    
};

/****************************BUILD*************************/

KDtree & KDtree::buildTree(const vector<shared_ptr<Shape> > & shapes) {
    nodes_.reserve(size(shapes));
    
    Node node;
    node.height = 0;
    for(int dim = 0; dim < 3; ++dim) {
        if(size(shapes) > 0) {
            node.box.lims[dim] = shapes[0]->getBoundRange(dim);
        }
        for(int j = 1; j < size(shapes); ++j) {
            Range bounds = shapes[j]->getBoundRange(dim);
            node.box.lims[dim].first = std::min(node.box.lims[dim].first,bounds.first);
            node.box.lims[dim].second = std::max(node.box.lims[dim].second,bounds.second);
        }
        if(node.box.lims[dim].second - node.box.lims[dim].first < EPS) {
            node.box.lims[dim].second += EPS/2.0;
            node.box.lims[dim].first -= EPS/2.0;
        }
    }
    node.parent = -1;
    node.shapes_ = shapes;
    nodes_.push_back(node);
    
    build_node_(0, Dim::x, 0);
    return *this;
}

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
                        ld mid,
                        int id_mid,
                        dim dim) const {
    const ld PREDICT_LENGTH = 2.0 / 3.0;
    left.clear();
    right.clear();
    left.reserve(PREDICT_LENGTH *  size(data));
    right.reserve(PREDICT_LENGTH * size(data));
    
    for(int i = 0; i < size(data); ++i) {
        
        const auto & p = data[i];
        if(i == id_mid) {
            right.push_back(p);
            continue;
        }
        int k = 0;
        Range bound = p->getBoundRange(dim);
        if(bound.first <= mid) {
            left.push_back(p);
            ++k;
        }
        if(bound.second >= mid) {
            right.push_back(p);
            ++k;
        }
//        if(k == 2 && i == id_mid) {
//            
//        }
    }
}

void KDtree::build_node_(int node_idx, dim div_dim, int empty_partition_cnt) {
    
    memset(nodes_[node_idx].childs, 0 , 2 * sizeof(int));
    if(nodes_[node_idx].parent > 0) {
//        memcpy(node.box.lims, nodes_[node.parent].box.lims, sizeof(nodes_[node.parent].box.lims));
        nodes_[node_idx].height = nodes_[nodes_[node_idx].parent].height + 1;
    }
    nodes_[node_idx].div_dim = div_dim;
    
    if(is_time_to_stop_(node_idx,empty_partition_cnt)) return;
    
    int id_l = size(nodes_);
    int id_r = size(nodes_) + 1;
    nodes_.resize(size(nodes_) + 2);
    Node & node = nodes_[node_idx];
    
    node.childs[0] = id_l;
    node.childs[1] = id_r;
    
    std::sort(node.shapes_.begin(), node.shapes_.end(),ShapeCmp(div_dim, node.box.lims[div_dim]));
    
    int mid_id = size(node.shapes_) / 2;
    Range bounds = node.shapes_[mid_id]->getBoundRange(div_dim);
    bounds.first = std::max(bounds.first, node.box.lims[div_dim].first);
    bounds.second = std::min(bounds.second, node.box.lims[div_dim].second);
    ld mid = bounds.first - 2.0 * EPS;
    node.mid = mid;
    
    Node * l = &nodes_[id_l];
    Node * r = &nodes_[id_r];
    l->parent = node_idx;
    r->parent = node_idx;
    
    partition_(node.shapes_, l->shapes_, r->shapes_, mid, mid_id , div_dim);
    
    memcpy(l->box.lims,node.box.lims, sizeof(node.box.lims));
    memcpy(r->box.lims,node.box.lims, sizeof(node.box.lims));
    l->box.lims[div_dim].second = mid;
    r->box.lims[div_dim].first = mid;
    vector<Node *> childs = {l,r};
//    for(const auto & pt : childs) {
//        if(l->box.lims[div_dim].second - pt->box.lims[div_dim].first < EPS) {
//            l->box.lims[div_dim].first -= EPS;
//            l->box.lims[div_dim].second += EPS;
//        }
//    }
    
    build_node_(id_l, Dim::next(div_dim), size(node.shapes_) == size(l->shapes_)?empty_partition_cnt+1:0);
    build_node_(id_r, Dim::next(div_dim), size(node.shapes_) == size(r->shapes_)?empty_partition_cnt+1:0);
}

/************************TRACE RAY**************************/


pair<ld , shared_ptr<Shape> > KDtree::intersect_with_shapes_(const vector<shared_ptr<Shape> > & shapes,
                                                  const Ray & ray) const {
    vector<pair<ld,int> > dists;
    for(int i = 0; i < size(shapes); ++i) {
        pair<bool, ld> res = shapes[i]->getIntersection(ray);
        if(!res.first) continue;
        dists.push_back(std::make_pair(res.second,i));
    }
    
    if(size(dists) == 0) {
        return std::make_pair(-1 , shared_ptr<Shape> ());
    }
    
    int minid = 0;
    for(int i = 1; i < size(dists) ; ++i) {
        if(dists[i].first < dists[minid].first) {
            minid = i;
        }
    }
    return std::make_pair(dists[minid].first , shapes[dists[minid].second]);
}

pair<ld, shared_ptr<Shape> > KDtree::findIntrsection(const Ray & ray) const  {
    pair<ld,ld> dists = nodes_[0].box.get_intersection(ray);
    if(dists.second < 0) return make_pair(-1,shared_ptr<Shape> ());
    return find_intersection_(0,dists,ray);
}

pair<ld, shared_ptr<Shape> > KDtree::find_intersection_(int id, pair<ld,ld> intersection_dists ,const Ray & ray) const {
    const Node & node = nodes_[id];
    if(node.childs[0] <= 0) {
        return intersect_with_shapes_(node.shapes_, ray);
    }
    dim div_dim = node.div_dim;
    pair<crd,crd> section_pts = make_pair(ray.pt + intersection_dists.first * ray.dir ,
                                                  ray.pt + intersection_dists.second * ray.dir );
    if(intersection_dists.first < 0) {
        section_pts.first = ray.pt;
    }
    
    pair<ld,ld> section_dim_crds = make_pair(section_pts.first.getDimCrd(div_dim), section_pts.second.getDimCrd(div_dim));
    int ids[] = {0,1};
    if(section_dim_crds.first > section_dim_crds.second) {
        std::swap(section_dim_crds.first,section_dim_crds.second);
        std::swap(ids[0],ids[1]);
    }
    
    
    ld mid = node.mid;
    
    if(section_dim_crds.first < mid + EPS && section_dim_crds.second < mid + EPS) {
        return find_intersection_(node.childs[0],intersection_dists,ray);
    }
    
    if(section_dim_crds.first >= mid - EPS && section_dim_crds.second >= mid - EPS) {
        return find_intersection_(node.childs[1],intersection_dists,ray);
    }
    
    crd pt_mid = crd(node.box.lims[0].first,node.box.lims[1].first,node.box.lims[2].first);
    pt_mid.setDimCrd(div_dim, mid);
    crd v1 =  BASE_VECTORS[Dim::next(div_dim)] ;
    crd v2 = BASE_VECTORS[Dim::next(Dim::next(div_dim))];
    
    ld dist = get_plane_intersection(pt_mid, v1, v2, ray);
    if(dist - intersection_dists.first < EPS) {
        intersection_dists.first -= EPS;
    }
    
    pair<ld, shared_ptr<Shape> > res1 = find_intersection_(node.childs[ids[0]],make_pair(intersection_dists.first,dist),ray);
    
    if(res1.first > 0 && res1.first < dist) return res1;
    
    if(intersection_dists.second  - dist < EPS) {
        intersection_dists.second += EPS;
    }
    
    return find_intersection_(node.childs[ids[1]],make_pair(dist,intersection_dists.second),ray);
}
