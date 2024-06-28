#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

#define elif else if
#define int long long
#define ii pair<int, int>
#define sz(a) (int)(a.size())
#define bit(i, j) ((i >> j) & 1)
#define left_node id << 1, l, mid
#define right_node id << 1 | 1, mid + 1, r
#define all(a) (a).begin(), (a).end()
#define what_is(x) cout << #x << " is " << x << endl

const int dx[8] = {1, 0, -1, 0, 1, 1, -1, -1};
const int dy[8] = {0, -1, 0, 1, 1, -1, -1, 1};

template <typename T>
bool maximize(T &res, const T &val)
{
    if (res < val)
    {
        res = val;
        return true;
    }
    return false;
}
template <typename T>
bool minimize(T &res, const T &val)
{
    if (val < res)
    {
        res = val;
        return true;
    }
    return false;
}

const int INF = 1000;
const int BASE = 311;
const int MOD = 1e9 + 7;

#define ii pair<int, int>

void solve()
{
    freopen("data.txt", "r", stdin);
    int n, m;
    cin >> n >> m;
    vector<vector<ii>> adj(n, vector<ii>(n, {0, 0}));
    int u, v, w;
    while (m--)
    {
        cin >> u >> v >> w;
        adj[u][v] = {0, w};
    }
    freopen("log.txt", "r", stdin);
    for (int i = 0; i < 4; i++)
    {
        int flow;
        cin >> flow;
        string tmp;
        cin >> tmp;
        int size;
        cin >> size;
        char c;
        cin >> c;
        vector<int> vertices(size);
        for (auto &x : vertices)
            cin >> x;
        for (int i = 1; i < vertices.size(); i++)
            adj[vertices[i-1]][vertices[i]].first += flow;
    }
    freopen("data.txt", "r", stdin);
    cin >> n >> m;
    while (m--)
    {
        
    }
}

signed main()
{
    cin.tie(0), cout.tie(0)->ios::sync_with_stdio(0);
    int test = 1;
    // cin >> test;
    while (test--)
        solve();
    return 0;
}