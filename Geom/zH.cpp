#include <iostream>
#include <fstream>
#include <cmath> 
#include <cassert>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stack>

 using namespace std;
typedef long long ld;
#define loop(i, n) for(int (i) = 0; (i) < (n) ; ++ (i))

struct Point {
    
    Point(ld x_, ld y_) {
        x = x_;
        y = y_;
    }
    Point(){};

    ld x;
    ld y;
    int i;
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

ld mult(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

Point test(1e9 + 1, 1e9 + 1);

bool cmp(const Point & a, const Point & b) {
    Point d1 = a - test;
    Point d2 = b - test;
    if(d1.y * d2.x == d2.y * d1.x) {
        if(d1.x == d2.x) {
            return d1.y < d2.y;
        } else {
            return d1.x < d2.x;
        }
    } else {
        return d1.y * d2.x < d2.y * d1.x;
    }
}

void hull(vector <Point> & p, vector<int> & ans) {
    loop(i , p.size()) {
        if(p[i].x < test.x || (p[i].x == test.x && p[i].y < test.y)) {
            test = p[i];
        }
    }  

    sort(p.begin(),p.end(),cmp);
    stack<Point> q;

    int i = 1;

    while(p[i].x == p[0].x && p[i].y == p[0].y) {
        ++i;
    }

    q.push(p[0]);
    q.push(p[i]);
    Point p2;
    ++i;
    for(;i < p.size(); ++i) {
        p2 = q.top();
        q.pop();
        while(true) {
            Point p1 = q.top();
            // cout << "(" << p1.x << "," << p1.y << "|" << p2.x << "," <<p2.y << endl;

            // cout << i << endl;
            // cout <<"(" << p2.x << " " << p1.x <<")" << endl;
            Point v1 = Point(p2.x - p1.x, p2.y - p1.y);
            Point v2 = Point(p[i].x - p2.x,p[i].y - p2.y);
            // cout << v1.x << " " << v1.y << "|" << v2.x << v2.y << endl;
            // cout << v1.x << " " << v1.y << " _ " << v2.x << " " << v2.y << endl;
            // cout << mult(v1,v2) << ".\n";
            if(mult(v1,v2) < 0) {
                p2 = p1;
                if(q.size() == 1) break;
                q.pop();
                continue;
            }

            if(mult(v1,v2) != 0) q.push(p2);
            break;
        }
        q.push(p[i]);
    }

    ans.clear();
    ans.reserve(q.size());
    while(q.size() > 0) {
        ans.push_back(q.top().i);
        q.pop();
    }
}

int main() {
    freopen("in.txt","r", stdin);
    // freopen("hull.in","r", stdin);
    // freopen("hull.out","w", stdout);


    ios_base::sync_with_stdio(false);    
    // cout << fixed;
    // cout.precision(10);

    // solve();
}