#include <iostream>
#include <fstream>
#include <cmath> 
#include <cassert>
 using namespace std;
typedef long double ld;

struct Point {
    ld x;
    ld y;
};
ld operator *(const Point & a, const Point & b) {
    return a.x * b.x + a.y * b.y;
}
Point operator -(const Point & a, const Point & b) {
    return {a.x - b.x, a.y - b.y};
}
Point operator +(const Point & a, const Point & b) {
    return {a.x + b.x, a.y + b.y};
}
ld dist(const Point & a, const Point & b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void solve() {
    Point p,p1,p2;
    cin >> p.x >> p.y >> p1.x >> p1.y >> p2.x >> p2.y;

    Point v1,v2,v;
    v1 = p1 - p;
    v2 = p2 - p;
    v = p2 - p1;

    if((v1 * v) * (v2 * v) < 0) { // inside
        ld A,B,C;
        A = p1.y - p2.y;
        B = p2.x - p1.x;
        C = p1.x * p2.y - p1.y * p2.x;
        ld d = sqrt(A*A + B*B);
        A/= d; B/= d; C /= d;
        cout << abs(A * p.x + B * p.y + C);
    } else { 
        cout << min(dist(p1,p),dist(p2,p));
    }
}

int main() {
    // freopen("in.txt","r", stdin);
    freopen("input.txt","r", stdin);
    freopen("output.txt","w", stdout);

    ios_base::sync_with_stdio(false);    
    cout << fixed;
    cout.precision(10);

    solve();
}