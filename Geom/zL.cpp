#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <memory.h>
#include <cassert>

using namespace std;
typedef long long ll;
#define mp make_pair 
#define ft first
#define sd second
#define pb push_back
#define pii pair<int,int>
#define pll pair<ll,ll>
#define loop(i, n) for(int (i) = 0; (i) < (n) ; ++ (i))
#define forn(i, l, r) for(int (i) = (l); (i) < (r) ; ++ (i))
#define vec vector 
#define vii vector<int> 

const int maxn = 100000;
int t[maxn];
int ans[maxn];
bool used[maxn];
int n , m;

void add(int i , int x) {
    for(; i  <  n ; i = (i | (i+1))) {
        t[i] += x;
    }
}

int sum(int i) {
    if(i  <  0 ) return 0;
    int res = 0;
    for(; i >= 0 ; i = (i & (i+1)) - 1) {
        res += t[i];
    }

    return res;
}

void solve() {
    scanf("%d%d" , &n , &m);
    pii v[maxn];
    memset(t, 0 ,sizeof(int) * n);
    memset(used, 0 ,sizeof(bool) * n);
    memset(ans, 0 ,sizeof(int) * m);

    loop(i , n) {
        scanf("%d" , &v[i].ft);
        v[i].sd = i;
    }

    sort(v,v + n);

    
    pair <pii , pii> qr[2 * maxn]; // < k  , id > < x , y>
    int x, y , k , l;
    loop(i , m) {
        scanf("%d%d%d%d",&x,&y,&k,&l);
        --x;
        --y;
        qr[(i<<1)] = mp(mp(k - 1,i) , mp(x,y));
        qr[(i<<1) + 1] = mp(mp(l,i) , mp(x,y));
    }

    sort(qr,qr + (m<<1));

    int cur = 0;
    loop(i , (m << 1)) {
        int val = qr[i].ft.ft;
        int id = qr[i].ft.sd;
        int x =  qr[i].sd.ft , y =  qr[i].sd.sd;
        while(true) {
            if(cur >= n) break;
            if(v[cur].ft > val) break;
            add(v[cur].sd , 1);
            ++cur;
        }
        int res = sum(y) - sum(x - 1);
        if(!used[id]) {
            ans[id] -= res;
            used[id] = true;
        } else {
            ans[id] += res;
        }
    }

    loop(i , m) {
        printf("%d\n" , ans[i]);
    }   
}

int main()
{
    // freopen("in.txt","r", stdin);
    freopen("permutation.in","r", stdin);
    freopen("permutation.out","w", stdout);
    // cout << fixed;
    // cout.precision(3);
    // ios_base::sync_with_stdio(false);   

    solve();
    return 0;
}
