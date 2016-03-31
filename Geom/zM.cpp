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

    int get_min(int id, range dx, range dy) {
        Node & cur = nodes[id];
        if(nonintersect(cur, dx, dy) || cur.min == MAXMIN) return MAXMIN;
        if(cur.mod != -1) push(cur , cur.mod);
        cur.mod = -1;

        if(cur.children[0] == 0) {
            if(check(cur.pnt , dx , dy)) return cur.min;
            return MAXMIN;
        } 
        if(inside(cur, dx, dy)) return cur.min;

        int ans = MAXMIN;
        loop(i,2) {
            ans = min(ans , get_min(cur.children[i] , dx , dy));
        }
        return ans;
    }

    int modify(int id, range dx, range dy, int mod) {
        Node & cur = nodes[id];
        if(nonintersect(cur, dx,dy)) return cur.min;
        if(cur.min == MAXMIN) return MAXMIN;
        if(cur.children[0] == 0) {
            if(check(cur.pnt, dx, dy)) {
                cur.min = mod;
            } 
            return cur.min;
        }

        if(cur.mod != -1) push(cur , cur.mod);
        cur.mod = -1;
        if(inside(cur , dx , dy)) {
            cur.mod = mod;
            cur.min = mod;
        } else {
            cur.min = MAXMIN;
            loop(i,2) {
                cur.min = min(cur.min , modify(cur.children[i], dx, dy, mod));
            }
        }
        return cur.min;
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
        return cur.dx.l > dx.r || cur.dx.r < dx.l
            || cur.dy.l > dy.r || cur.dy.r < dy.l;
    }

    int build_node(int id , vector<point>::iterator begin ,
                            vector<point>::iterator last ,
                            bool xdiv , range second_d) {
        if(last - begin == 0) {
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
            int size = last - begin + 1;
            vector<point>::iterator mid = begin + size / 2;
            int k = nodes.size();
            loop(i,2) {
                nodes[id].children[0] = k + i;
                nodes.push_back(!xdiv);
            }

            nodes[id].min = min(nodes[id].min , 
                        build_node(k, begin , mid, !xdiv,range(begin->x , mid->x)));
            nodes[id].min = min(nodes[id].min ,
                        build_node(k + 1, mid + 1 ,last , !xdiv, range((mid+1)->x , last->x)));
        }
        return nodes[id].min;
    }

};

void solve() {
    
    int n,m;
    cin >> n;
    vector<point> pts(n);
    loop(i,n) {
        cin >> pts[i].x >> pts[i].y >> pts[i].val;
    }
    
    d2_tree tree;
    tree.build(pts);
    cin >> m;
    vector<int> ans;

    loop(i,m) {
        char c;
        cin >> c;
        range xr , yr;
        cin >> xr.l >> yr.l >> xr.r >> yr.r;
        
        if(c == '=') {
            int val;
            cin >> val;
            tree.modify(0, xr, yr, val);
        } else {
            cout << tree.get_min(0,xr,yr) <<endl;
        }
    }

    loop(i,ans.size()) {
        if(ans[i] == MAXMIN) {
            cout <<"NO\n";
        } else {
            cout << ans[i] <<"\n";
        }
    }
}

int main()
{
    freopen("in.txt","r", stdin);
    // freopen("permutation.in","r", stdin);
    // freopen("permutation.out","w", stdout);
    // cout << fixed;
    // cout.precision(3);
    // ios_base::sync_with_stdio(false);   

    solve();
    return 0;
}
