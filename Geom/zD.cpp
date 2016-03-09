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
    
    ll r1,r2;
    ll x1,x2,y1,y2;
    cin >> x1 >> y1 >> r1 >> x2 >> y2 >> r2;
    ll d2 = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    
    if(d2 > (r1 + r2) * (r1 + r2) || d2 < (r1 - r2) * (r1 - r2)) {
        cout << "0\n";
        return;
    }

    if(d2 == 0 && r1 == r2) {
        cout << "3\n";
        return;
    }
    
    Point o1(x1,y1),o2(x2,y2);
    ld d = dist(o1,o2);
    if(d2 == (r1 + r2) * (r1 + r2) || d2 == (r1 - r2) * (r1 - r2)) {
        if(r1 < r2) {
            swap(r1,r2);
            swap(o1,o2);
        }
        Point a = o2 - o1;
        a.x /= d;
        a.y /= d;
        a.x *= r1;
        a.y *= r1;
        Point p = o1 + a;
        cout << "1\n" << p.x << " " << p.y << "\n";
        return;
    }
    
    bool swapped = false;
    if(r1 < r2) {
        swap(r1,r2);
        swap(o1,o2);
        swapped = true;
    }

    Point a = o2 - o1;
    a.x /= d;
    a.y /= d;
    Point n(-a.y, a.x);

    ld cosa = (r1 * r1 + d*d - r2 * r2) / (2 * r1 * d);
    ld OH = r1 * cosa;
    ld PH = r1 * sqrt(1 - cosa * cosa);

    a.x *= OH;
    a.y *= OH;
    n.x *= PH;
    n.y *= PH;
    Point H = o1 + a;
    Point p1 = H + n;
    Point p2 = H - n;
    cout << "2\n" << H.x << " " << H.y << "\n";
    cout << (swapped ? abs(d - OH) : OH) << " " << PH << "\n";
    cout << p1.x << " " << p1.y << "\n" << p2.x << " " << p2.y <<"\n";
}

int main() {
    freopen("in.txt","r", stdin);
    // freopen("intersec4.in","r", stdin);
    // freopen("intersec4.out","w", stdout);


    ios_base::sync_with_stdio(false);    
    cout << fixed;
    cout.precision(10);
    int t;
    cin >> t;
    loop(i,t) solve();
}