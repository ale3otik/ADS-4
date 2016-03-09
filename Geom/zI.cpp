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
#define MOD (ld)1e9 + 7
#define eps 1e-12
#define PI (ld)3.14159265358979324
#define mp make_pair 
#define ft first
#define pb push_back
#define sd second
#define sz(a) a.size()
#define pii pair<int,int>
#define pld pair<long long,long long>
#define loop(i, n) for(int (i) = 0; (i) < (n) ; ++ (i))
#define forn(i, l, r) for(int (i) = (l); (i) < (r) ; ++ (i))
#define vec vector 
#define str string
#define vii vector<int> 
#define vld vector<ld>

/*******************************************************/

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
void operator /= (Point & a, const ld x) {
    assert(x != 0);
    a.x /= x;
    a.y /= x;
}

ld dist(const Point & a, const Point & b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}
ld dist(const Point & a) {
    return (a.x * a.x + a.y * a.y);
}
ld gdet(ld a, ld b, ld c, ld d) {
    return a*d - b*c;
}

ld mult(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

/***********************************/
vec<Point> p;

ld calc_square(const Point & pr) {
    Point prn(-pr.y , pr.x);
    ld max1 = -1e15;
    ld max2 = -1e15;
    ld min1 = 1e15;
    ld min2 = 1e15;
    loop(i,p.size()) {
        max1 = max(pr * p[i], max1);
        min1 = min(pr * p[i],min1);
        max2 = max(prn * p[i], max2);
        min2 = min(prn * p[i], min2);

    }
    return abs((max1 - min1) * (max2 - min2));
}

ld search3(ld l , ld r , ld sign) {
    ld m1v = -1e9;
    ld m2v;
    ld cur;
    while(r - l > 1e-12){
        ld m1 = l + (r - l) / 3.0;
        ld m2 = r - (r - l) / 3.0;
        Point m1pr = Point(cos(m1) , sin(m1));
        Point m2pr = Point(cos(m2) , sin(m2));
        m1v = sign * calc_square(m1pr);
        m2v = sign * calc_square(m2pr);
        if(m1v == m2v) {
            l = m1;
            r = m2;
            cur = m1v;
        } else if(m1v < m2v) {
            l = m1;
            cur = m2v;
        } else {
            cur = m1v;
            r = m2;
        }
    }  
    return sign * cur;
}

void solve(int n) {
    int x,y;
    p.resize(n);
    loop(i,n) {
        cin >> x >> y;
        p[i] = Point(x,y);
    }

    ld max_fi = (2.0 * PI) / 3.0;
    ld len = max_fi / 10;
    ld mmax = 0;
    ld mmin = 1e12;
    loop(i,20) {
        ld cur = search3(len * (ld)i , len * (ld)(i+1) , +1);
        mmax = max(cur,mmax);

        cur = search3(len * (ld)i , len * (ld) (i+1) , -1);
        mmin = min(cur, mmin);
    }
    cout << "Minimum area = " << mmin << endl;
    cout << "Maximum area = " << mmax << endl;
}


int main()
{
    // freopen("in.txt","r", stdin);
    freopen("gift.in","r", stdin);
    freopen("gift.out","w", stdout);
    
    cout << fixed;
    cout.precision(3);
    ios_base::sync_with_stdio(false);   
    int n;
    int i = 1;
    while(true) {
        cin >> n;
        if(n != 0) {
            cout << "Gift " << i << endl;
            ++i;
            solve(n);
            cout << endl;
        } else break;
    }
    return 0;
}