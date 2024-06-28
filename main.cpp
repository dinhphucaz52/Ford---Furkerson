#include <iostream>
#include <queue>
#include <algorithm>
#include <stack>

using namespace std;

const int MAXM = 2e3;

int n, m;
int capacity[MAXM][MAXM];
int par[MAXM];
bool bfs(int s, int t)
{
    vector<bool> vst(n, 0);
    queue<int> q;
    q.push(s);
    vst[s] = true;
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        for (int v = 0; v < n; v++)
        {
            if (!vst[v] && capacity[u][v] > 0)
            {
                if (v == t)
                    return par[v] = u, true;
                q.push(v);
                par[v] = u;
                vst[v] = true;
            }
        }
    }
    return false;
}

void traceBack(int minFlow, int s, int t)
{
    cerr << minFlow;
    vector<int> res;
    for (int v = t; v != s; v = par[v])
        res.push_back(v);
    res.push_back(s);
    reverse(res.begin(), res.end());
    cerr << " -> " << res.size() << " : ";
    for (int v : res)
        cerr << v << " ";
    cerr << endl;
}

int fordFulkerson(int s, int t)
{
    int res = 0;
    while (bfs(s, t))
    {
        int minFlow = INT_MAX;
        for (int v = t; v != s; v = par[v])
        {
            int u = par[v];
            minFlow = min(minFlow, capacity[u][v]);
        }

        traceBack(minFlow,s, t);

        for (int v = t; v != s; v = par[v])
        {
            int u = par[v];
            capacity[u][v] -= minFlow;
            capacity[v][u] += minFlow;
        }
        res += minFlow;
    }
    return res;
}

int main()
{
    freopen("data.txt", "r", stdin);
    freopen("log.txt", "w", stderr);
    cin >> n >> m;

    while (m--)
    {
        int u, v, w;
        cin >> u >> v >> w;
        capacity[u][v] = w;
    }

    cout << fordFulkerson(0, 5);

    return 0;
}