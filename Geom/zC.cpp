#include <iostream>
#include <fstream>
#include <cmath> 
#include <cassert>
 using namespace std;
typedef long double ld;
typedef long long ll;
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

bool check(Point p, Point a, Point b) {
    ll ac,bc,pc;
    if(a.x == b.x) {
        ac = a.y;
        bc = b.y;
        pc = p.y;
        if(ac > bc) swap(ac,bc);
    } else {
        ac = a.x;
        bc = b.x;
        pc = p.x;
        if(ac > bc) swap(ac,bc);
    }

    if(pc > bc || pc < ac) return false;
    return true;
}

void solve() {
    Point o[2][2];
    for(int i = 0; i < 2; ++i)
        cin >> o[i][0].x >> o[i][0].y >> o[i][1].x >> o[i][1].y;
    ll A[2],B[2],C[2];
    for(int i = 0 ; i < 2; ++i) {
        A[i] = o[i][0].y - o[i][1].y;
        B[i] = o[i][1].x - o[i][0].x;
        C[i] = o[i][0].x * o[i][1].y - o[i][0].y * o[i][1].x;
        if(B[i] < 0) {
            A[i] *= -1;
            B[i] *= -1;
            C[i] *= -1;
        } 
        if(B[i] == 0 && A[i] < 0) {
            A[i] *= -1;
            C[i] *= -1;
        }
    }
    

    ll det = gdet(A[0],B[0],A[1],B[1]);
    bool ans;
    if(det == 0) {
        ans = false;
        if(A[0] * o[1][0].x + B[0] * o[1][0].y + C[0] == 0) {
            for(int i = 0; i < 2; ++i) {
                for(int j = 0 ; j < 2; ++j){
                    if(check(o[(i+1)%2][j], o[i][0], o[i][1])) {
                        ans = true;
                        break;
                    }
                }
                if(ans) break;
            }
        } else {
            // assert(0);
        }
    } else {
        int dx = gdet(-C[0],B[0],-C[1],B[1]);
        int dy = gdet(A[0],-C[0],A[1],-C[1]);

        ans = true;
        for(int i = 0 ; i < 2; ++i) {
            if(!check(Point(dx,dy) , Point(o[i][1].x * det , o[i][1].y * det), Point(o[i][0].x * det,o[i][0].y * det))){
                ans = false;
                break;
            }
        }
    }

    cout <<( ans ? "YES" : "NO" );
}

int main() {
    // freopen("in.txt","r", stdin);
    freopen("intersec2.in","r", stdin);
    freopen("intersec2.out","w", stdout);


    ios_base::sync_with_stdio(false);    
    cout << fixed;
    cout.precision(10);

    solve();
}