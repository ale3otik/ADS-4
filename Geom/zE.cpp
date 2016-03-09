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
    Point(ld x_, ld y_) {
        x = x_;
        y = y_;
    }
    Point(){};

    ld x;
    ld y;
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
ld dist(const Point & a, const Point & b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
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
    
    ll ox,oy,px,py;
    ll r;
    cin >> ox >> oy >> r >> px >> py;
    ll d2 = (ox - px) * (ox - px) + (oy - py) * (oy - py);
    if(d2 < r * r) {
        cout << 0;
        return;
    }
    if(d2 == r*r) {
        cout << "1\n" << px << " " << py;
        return;
    }
    
    Point o(ox,oy),p(px,py);
    ld R = r;
    ld d = sqrt(d2);
    ld x = sqrt(d2 - R*R );
    ld P1P3,P2P3;
    P1P3 = d - R*R / d;
    P2P3 = (R * x) / d;
    // cout << sqrt(x * x - P1P3 * P1P3) << "\n";
    // cout << P1P3 << " " << P2P3 << "..." <<endl;
    Point a(o.x - p.x, o.y - p.y);
    a.x /= d;
    a.y /= d;

    Point n(-a.y,a.x);
    a.x *= P1P3; a.y *= P1P3;
    n.x *= P2P3; n.y *= P2P3;
    Point p3 = p + a;
    Point p2 = p + a + n;
    Point p22 = p + a - n;

    cout << "2\n" << p3.x << " " <<p3.y << "\n" << P1P3 << " " << P2P3 << "\n";
    cout << p2.x << " " << p2.y << "\n" << p22.x << " " << p22.y << endl;
}

int main() {
    // freopen("in.txt","r", stdin);
    freopen("tangent.in","r", stdin);
    freopen("tangent.out","w", stdout);


    ios_base::sync_with_stdio(false);    
    cout << fixed;
    cout.precision(6);

    solve();
}