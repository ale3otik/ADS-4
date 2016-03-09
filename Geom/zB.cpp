#include <iostream>
#include <fstream>
#include <cmath> 
#include <cassert>
 using namespace std;
typedef long double ld;

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
    return {a.x - b.x, a.y - b.y};
}
Point operator +(const Point & a, const Point & b) {
    return {a.x + b.x, a.y + b.y};
}
ld dist(const Point & a, const Point & b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
ld gdet(ld a, ld b, ld c, ld d) {
    return a*d - b*c;
}

void solve() {
    ld A[2],B[2],C[2];
    for(int i = 0 ; i < 2; ++i) {
        cin >> A[i] >> B[i] >> C[i];
        C[i] *= -1;
    }

    ld det = gdet(A[0],B[0],A[1],B[1]);
    assert(det != 0.0);
    ld detx = gdet(C[0],B[0],C[1],B[1]);
    ld dety = gdet(A[0],C[0],A[1],C[1]);
    cout << detx/det << " " << dety/det;
}

int main() {
    // freopen("in.txt","r", stdin);
    freopen("intersec1.in","r", stdin);
    freopen("intersec1.out","w", stdout);


    ios_base::sync_with_stdio(false);    
    cout << fixed;
    cout.precision(10);

    solve();
}