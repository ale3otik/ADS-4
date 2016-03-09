#include <iostream>
#include <fstream>
#include <cmath> 
#include <vector>
#include <cassert>
 using namespace std;
typedef long double ld;
typedef long long ll;
#define loop(i, n) for(int (i) = 0; (i) < (n) ; ++ (i))

struct Point {
    Point(ll x_, ll y_) {
        x = x_;
        y = y_;
    }
    Point(){};

    ll x;
    ll y;
};
ld operator *(const Point & a, const Point & b) {
    return a.x * b.x + a.y * b.y;
}
Point operator -(const Point & a, const Point & b) {
    return Point(a.x - b.x, a.y - b.y);
}
Point operator +(const Point & a, const Point & b) {
    return Point(a.x + b.x, a.y + b.y);
}
// ld dist(const Point & a, const Point & b) {
//     return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
// }
ld gdet(ld a, ld b, ld c, ld d) {
    return a*d - b*c;
}
ll gdet(ll a, ll b, ll c, ll d) {
    return a*d - b*c;
}

ll mult(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}
ll mod(ll a, ll p) {
    if(a >= p) return a % p;
    while(a < 0) a += p;
    return a;
}

void solve() {
    int n;
    cin >> n;
    vector<Point> p(n);
    loop(i,n) {
        cin >> p[i].x >> p[i].y;
    }

    ll sign = 0;    
    bool ans = true;
    loop(i,n) {
        Point v1 = p[mod(i,n)] - p[mod(i-1,n)];
        Point v2 = p[mod(i+1,n)] - p[mod(i,n)];
        ll m = mult(v1,v2);
        if(m != 0) {
            if(sign == 0) {
                sign = m > 0 ? 1 : -1;
            } else {
                if(sign != m / abs(m)) {
                    ans = false;
                    break;
                }
            }
        }
    }
    cout <<( ans ? "YES" : "NO" );
}

int main() {
    // freopen("in.txt","r", stdin);
    freopen("polygon.in","r", stdin);
    freopen("polygon.out","w", stdout);

    ios_base::sync_with_stdio(false);    
    cout << fixed;
    cout.precision(10);

    solve();
}