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

void solve() {
    int n;
    cin >> n;
    vector<Point> p(n);
    ll minx = 1e4 + 1, miny = 1e4 + 1;
    loop(i,n) {
        cin >> p[i].x >> p[i].y;
        minx = min(p[i].x, minx);
        miny = min(p[i].y, miny);
    }
    Point c(minx-1,miny-1);

    ll ans = 0;
    loop(i,n-1) {
        ans += mult(p[i] - c,p[i+1] - c);
    }
    ans += mult(p[n-1] - c, p[0] - c);
    
    cout << abs(ans) / 2.0;
}

int main() {
    // freopen("in.txt","r", stdin);
    freopen("area.in","r", stdin);
    freopen("area.out","w", stdout);


    ios_base::sync_with_stdio(false);    
    cout << fixed;
    cout.precision(10);

    solve();
}