#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <cmath>
#include <cassert>

 using namespace std;
typedef long long ll;
typedef long double ld;
#define MOD (ll)1e9 + 7
#define eps 1e-12
#define PI (ld)3.14159265358979324
#define mp make_pair 
#define ft first
#define pb push_back
#define sd second
#define sz(a) a.size()
#define pii pair<int,int>
#define pll pair<long long,long long>
#define loop(i, n) for(int (i) = 0; (i) < (n) ; ++ (i))
#define forn(i, l, r) for(int (i) = (l); (i) < (r) ; ++ (i))
#define vec vector 
#define str string
#define vii vector<int> 
#define vll vector<ll>

/*******************************************************/

struct Point {
    
    Point(ll x_, ll y_) {
        x = x_;
        y = y_;
    }
    Point(){};

    ll x;
    ll y;
    int i;
};

ll operator *(const Point & a, const Point & b) {
    return a.x * b.x + a.y * b.y;
}
Point operator -(const Point & a, const Point & b) {
    return Point(a.x - b.x, a.y - b.y);
}
Point operator +(const Point & a, const Point & b) {
    return Point(a.x + b.x, a.y + b.y);
}
void operator /= (Point & a, const ll x) {
    assert(x != 0);
    a.x /= x;
    a.y /= x;
}

ll dist(const Point & a, const Point & b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}
ll dist(const Point & a) {
    return (a.x * a.x + a.y * a.y);
}
ll gdet(ll a, ll b, ll c, ll d) {
    return a*d - b*c;
}

ll mult(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

struct Circle {
    Circle(Point _ctr , ll _r) {
        ctr = _ctr;
        r = _r;
    }
    Point ctr;
    ll r;
};

struct setcomp{
    bool operator() (const Circle & a , const Circle & b) {
        ll la = a.ctr.x - a.r;
        ll lb = b.ctr.x - b.r;
        if(la != lb) return la <  lb;

        ll ya = a.ctr.y + a.r;
        ll yb = b.ctr.y + b.r;

      return ya < yb;
    }
};

/***********************************/
vector<Circle> v;

struct xpoint {
    xpoint(bool _l , int i) {
        is_left = _l;
        ind = i;
    } 
    bool is_left;
    int ind;
};

struct xcomp {
    bool operator () (const xpoint & a, const xpoint & b) {
        const Circle f = v[a.ind];
        const Circle s = v[b.ind];
        ll fx = a.is_left ? (f.ctr.x - f.r) : (f.ctr.x + f.r);
        ll sx = b.is_left ? (s.ctr.x - s.r) :  (s.ctr.x + s.r);
        if(fx != sx) return fx < sx;
        if(a.is_left != b.is_left) {
            return a.is_left < b.is_left;
        }

        if(f.r != s.r) return f.r > s.r;
        else return f.ctr.y < s.ctr.y;
    }
};

struct ycomp {
    bool operator () (const xpoint & a, const xpoint & b) {
        const Circle f = v[a.ind];
        const Circle s = v[b.ind];
        if(f.ctr.y != s.ctr.y) return f.ctr.y < s.ctr.y;
        if(f.ctr.x != s.ctr.x) return f.ctr.x < s.ctr.x;
        return f.r < s.r;
    }
};

bool is_inside(const Circle & l , const Circle & r) { // right not in left
    // cout << l.ctr.x << " " << l.ctr.y << " " << r.ctr.x << " " << r.ctr.y << endl;
    // cout <<l.r * l.r << " " <<  dist(l.ctr , r.ctr) <<  " ; " <<endl; 
    return l.r * l.r >= dist(l.ctr , r.ctr) ;
}
void solve() {
    ll n;
    set<Circle, setcomp> circles; 
    cin >> n;
    loop(i,n) {
        ll x,y,r;
        cin >> x >> y >> r;
        Circle c(Point(x,y), r);
        circles.insert(c);
    }

    v.reserve(n);
    set<xpoint , xcomp> xset;
    for(set<Circle, setcomp>::iterator it = circles.begin(); it != circles.end(); ++it) {
        v.push_back(*it);
        int ind = (ll)v.size() - 1;
        xset.insert(xpoint(true , ind));
        xset.insert(xpoint(false , ind));
    }

    ld ans = 0.0;  
    set <xpoint , ycomp > yset;
    for(set<xpoint,xcomp>::iterator it = xset.begin(); it != xset.end(); ++it) {
        Circle cur = v[(*it).ind];
        // cout << (*it).ind << " " <<  v[(*it).ind].ctr.x << " " <<v[(*it).ind].ctr.y << " " << v[(*it).ind].r << endl;
        if(!(*it).is_left) {
            yset.erase(xpoint(true,(*it).ind));
        } else {
            yset.insert(*it);
            set<xpoint,ycomp >::iterator it2 = yset.find(*it);
            
            if(it2 != yset.begin()) {
                // cout << "x";
                --it2;
                Circle left = v[(*it2).ind];
                ++it2;
                if(is_inside(left , cur)) {
                    yset.erase(it2);
                    continue;
                }
            }
            ++it2;
            if(it2 != yset.end()) {
                // cout << "y";
                Circle right = v[(*it2).ind];
                --it2;
                
                if(is_inside(right , cur)) {
                    yset.erase(it2);
                    continue;
                }
            } 

            // cout << (*it).ind << " " <<  v[(*it).ind].ctr.x << " " <<v[(*it).ind].ctr.y << " " << v[(*it).ind].r << endl;
            ans += PI * (ld)(cur.r * cur.r);
        }
    }
    
    cout << ans << endl;
    // cout << 28.2743338823081391;
}

int main()
{
    // freopen("in.txt","r", stdin);
    freopen("circles.in","r", stdin);
    freopen("circles.out","w", stdout);
    
    cout << fixed;
    cout.precision(18);
    ios_base::sync_with_stdio(false);   
    solve();
    return 0;
}