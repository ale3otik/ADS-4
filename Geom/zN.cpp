

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <memory.h>
#include <cassert>

using namespace std;
typedef long long ll;
#define mp make_pair
#define ft first
#define sd second
#define pb push_back
#define pii pair<int,int>
#define pll pair<ll,ll>
#define loop(i, n) for(int (i) = 0; (i) < (n) ; ++ (i))
#define forn(i, l, r) for(int (i) = (l); (i) < (r) ; ++ (i))
#define vec vector
#define vii vector<int>

const int MAXMIN = 1e9 + 1;
const int MAXBOUND = 1 << 30;
struct point {
    point() {}
    point(int x_, int y_ , int val_) {
        x = x_;
        y = y_;
        val = val_;
    }
    int x , y;
    int val;
};
struct range{
    range() {
        l = -MAXBOUND;
        r = +MAXBOUND;
    };
    range(int l_ , int r_) {
        r = r_;
        l = l_;
    }
    int l;
    int r;
};

bool compx(const point & a , const point & b) {
    if(a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

bool compy(const point & a , const point & b) {
    if(a.y != b.y) return a.y < b.y;
    return a.x < b.x;
}

struct Node {
    Node(bool xdiv_): pnt(0,0,MAXMIN) {
        xdiv = xdiv_;
        mod = -1;
        min = MAXMIN;
        memset(children,0, sizeof(int) * 2);
    }
    
    bool xdiv;
    int mod;
    int min;
    int children[2];
    point pnt;
    range dx , dy;
};

ll get_dist2(const point & a , const point & b) {
    ll dx = a.x - b.x;
    ll dy = a.y - b.y;
    return dx * dx + dy * dy;
}

range actual_range(const Node & node ,bool xdiv) {
    return xdiv ?  node.dx : node.dy;
}

int actual_crd(const point & p , bool xdiv) {
    return xdiv ? p.x : p.y;
}



vector<point> points;
int best_id;
class d2_tree {
public:
    vector<Node> nodes;
    void build(vector<point> & pts) {
        nodes.reserve(pts.size() * 2 + 4);
        nodes.pb(Node(true));
        int ymin = MAXBOUND;
        int ymax = -MAXBOUND;
        loop(i,pts.size()){
            ymin = min(ymin , pts[i].y);
            ymax = max(ymax , pts[i].y);
        }
        build_node(0 , pts.begin() , pts.end() - 1 , true , range(ymin , ymax));
    }
    
    void dfs(int id , int pnt_id) {
        const Node & cur = nodes[id];
        point pnt = points[pnt_id];
        point bestp = points[best_id];
        ll bestdist = get_dist2(bestp,pnt);
        if(cur.children[0] == 0) {
            if(bestdist > get_dist2(cur.pnt, pnt) || (bestdist == get_dist2(cur.pnt, pnt) && cur.pnt.val < best_id)) {
                if(pnt_id != cur.pnt.val) {
                    best_id = cur.pnt.val;
//                    cout << pnt_id << " " << cur.pnt.val<<endl;
                }
            }
            return;
        }
        Node & child1 = nodes[cur.children[0]];
        int k = 1;
        
        if(actual_range(child1, cur.xdiv).r >= actual_crd(pnt, cur.xdiv))  {
            k = 0;
        }
        
        loop(i,2) {
            bestdist = get_dist2(points[best_id],pnt);
            int j = (i + k)%2;
            Node & child = nodes[cur.children[j]];
            range act_range = actual_range(child, cur.xdiv);
            ll act_crd = actual_crd(pnt, cur.xdiv);
            ll dist = min(abs(act_crd - act_range.r) , abs(act_crd - act_range.l));
            dist = min (dist , min(abs(act_crd + act_range.r) , abs(act_crd + act_range.l)));
            if((act_crd <= act_range.r && act_crd >= act_range.l)
               || dist*dist <= bestdist) {
                dfs(cur.children[j] , pnt_id);
            }
        }
    }

    
private:
    
    void push(Node & cur, int val) {
        if(cur.children[0] == 0) return;
        loop(i , 2) {
            int cid = cur.children[i];
            nodes[cid].min = val;
            nodes[cid].mod = val;
        }
    }
    
    bool check(point & pt , range  & dx , range & dy) {
        return pt.x <= dx.r && pt.x >= dx.l && pt.y <= dy.r && pt.y >= dy.l;
    }
    
    bool inside(const Node & cur ,range  & dx , range & dy) {
        return cur.dx.r <= dx.r && cur.dx.l >= dx.l
        && cur.dy.r <= dy.r && cur.dy.l >= dy.l;
    }
    bool nonintersect(const Node & cur ,range  & dx , range & dy) {
        //        if(!(cur.dx.l > dx.r || cur.dx.r < dx.l
        //            || cur.dy.l > dy.r || cur.dy.r < dy.l)) {
        //            cout << cur.dx.l << " " << cur.dx.r << " " << cur.dy.l<<  " " << cur.dy.r <<"|";
        //            cout << dx.l << " " << dx.r << " " << dy.l<<  " " << dy.r <<endl;
        //        }
        return cur.dx.l > dx.r || cur.dx.r < dx.l
        || cur.dy.l > dy.r || cur.dy.r < dy.l;
    }
    
    int build_node(int id , vector<point>::iterator begin ,
                   vector<point>::iterator last ,
                   bool xdiv , range second_d) {
        //        cout << last - begin <<endl;
        if(last == begin) {
            nodes[id].dx = range(begin->x , begin->x);
            nodes[id].dy = range(begin->y , begin->y);
            nodes[id].min = begin->val;
            nodes[id].pnt = *begin;
        }
        else {
            sort(begin , last + 1 , (xdiv ? compx : compy));
            
            if(xdiv) {
                nodes[id].dx = range(begin->x , last->x);
                nodes[id].dy = second_d;
            }
            else {
                nodes[id].dx = second_d;
                nodes[id].dy = range(begin->y , last->y);
            }
            int size = (int)(last - begin) + 1;
            vector<point>::iterator mid = begin + size / 2;
            int k = nodes.size();
            loop(i,2) {
                nodes[id].children[i] = k + i;
                nodes.push_back(!xdiv);
            }
            range next_range = (xdiv ? range(begin->x , (mid-1)->x) : range(begin->y , (mid-1)->y));
            nodes[id].min = min(nodes[id].min ,
                                build_node(k, begin , mid - 1, !xdiv, next_range));
            
            next_range = (xdiv ? range((mid)->x , last->x) : range((mid)->y , last->y));
            nodes[id].min = min(nodes[id].min ,
                                build_node(k + 1, mid ,last , !xdiv, next_range));
        }
        return nodes[id].min;
    }
    
};

void solve2() {
    int n;
    cin >> n;
    points.resize(n);
    loop(i , n) {
        cin >> points[i].x >> points[i].y;
        points[i].val = i;
    }
    
    d2_tree tree;
    vector<point> ptscpy = points;
    tree.build(ptscpy);
    vec<vec<int> > ans(n);
    
    loop(i,n) {
        best_id = (i == 0 ? 1 : 0);
        tree.dfs(0 , i);
        ans[best_id].pb(i);
        
    }
    loop(i,n) {
        cout << i+1 <<": ";
        loop(j,ans[i].size()) {
            cout << ans[i][j] + 1 << " ";
        }
        cout << "\n";
    }
}

int main()
{
//    freopen("in.txt","r", stdin);
     freopen("k.in","r", stdin);
     freopen("k.out","w", stdout);
    // cout << fixed;
    // cout.precision(3);
    
    ios_base::sync_with_stdio(false);
    
    solve2();
    return 0;
}
