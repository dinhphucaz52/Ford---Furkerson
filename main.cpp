#include <iostream>
#include <queue>
#include <algorithm>
#include <stack>

using namespace std;

const int MAXM = 2e3;

struct Edge
{
    int u, v, w;
};

struct AugmentingPath
{
    int flow;
    vector<int> vertices;
    AugmentingPath() : flow(0), vertices(vector<int>()) {}
    AugmentingPath(int flow, vector<int> vertices) : flow(flow), vertices(vertices) {}
    void pushVertex(int vertex)
    {
        this->vertices.push_back(vertex);
    }
    void finish()
    {
        reverse(this->vertices.begin(), this->vertices.end());
    }
};

class Graph
{
private:
    int n;
    int **capacity;
    int *par;
    vector<AugmentingPath> augmentingPathList;
    vector<Edge> edgeList;

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
                if (!vst[v] && this->capacity[u][v] > 0)
                {
                    if (v == t)
                        return this->par[v] = u, true;
                    q.push(v);
                    this->par[v] = u;
                    vst[v] = true;
                }
            }
        }
        return false;
    }

    void traceBack(int minFlow, int s, int t)
    {
        AugmentingPath tmp;
        tmp.flow = minFlow;
        for (int v = t; v != s; v = par[v])
            tmp.pushVertex(v);
        tmp.pushVertex(s);
        tmp.finish();
        this->augmentingPathList.push_back(tmp);
    }

public:
    Graph(int size)
    {
        this->n = size;
        this->capacity = new int *[size];
        for (int i = 0; i < size; i++)
            this->capacity[i] = new int[size];
        for (int u = 0; u < size; u++)
            for (int v = 0; v < size; v++)
                this->capacity[u][v] = 0;
        this->par = new int[size];
        this->augmentingPathList = vector<AugmentingPath>();
        this->edgeList = vector<Edge>();
    }

    Graph(int size, vector<Edge> edgeList) : Graph(size)
    {
        for (auto [u, v, w] : edgeList)
            capacity[u][v] = w;
        this->edgeList = edgeList;
    }

    int fordFulkerson(int s, int t)
    {
        int res = 0;
        while (this->bfs(s, t))
        {
            int minFlow = INT_MAX;
            for (int v = t; v != s; v = par[v])
            {
                int u = par[v];
                minFlow = min(minFlow, capacity[u][v]);
            }
            this->traceBack(minFlow, s, t);

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
    void printForDotFile(const int augmentingPathCnt)
    {
        freopen("test.dot", "w", stdout);
        cout << "/*\n";
        vector<vector<int>> adj(this->n, vector<int>(this->n, 0));
        vector<vector<bool>> isColor(this->n, vector<bool>(this->n, 0));
        for (int i = 0; i < this->augmentingPathList.size(); i++)
        {
            AugmentingPath path = this->augmentingPathList[i];
            cout << path.flow << " : ";
            cout << path.vertices[0] << " ";
            for (int j = 1; j < path.vertices.size(); j++)
            {
                int u = path.vertices[j - 1];
                int v = path.vertices[j];
                cout << v << " ";
                if (i < augmentingPathCnt)
                {
                    adj[u][v] += path.flow;
                    isColor[u][v] = (i + 1 == augmentingPathCnt);
                }
            }
            cout << endl;
        }
        cout << "*/\n";
        cout << "digraph G {" << "\n";
        cout << "0 [fillcolor = aqua;style = \"filled\";];\n";
        cout << n - 1 << " [fillcolor = aqua;style = \"filled\";];\n";
        for (auto [u, v, w] : this->edgeList)
        {
            if (augmentingPathCnt)
                cout << u << " -> " << v << " "
                     << (isColor[u][v] ? "[color=red]" : "")
                     << "[label=\"" << adj[u][v]
                     << "," << w << "\"]\n";
            else
                cout << u << " -> " << v << " "
                     << "[label=\"" << w << "\"]\n";
        }
        cout << "0 -> 4 [style = invis;];\n";
        cout << "}\n";
    }
    ~Graph()
    {
        free(this->capacity);
        free(this->par);
    }
};

void readFile(int &n, vector<Edge> &edgeList)
{
    freopen("data.txt", "r", stdin);
    int m;
    int u, v, w;
    cin >> n >> m;
    while (m--)
    {
        cin >> u >> v >> w;
        edgeList.push_back({u, v, w});
    }
}

// int main(int argc, char *argv[])
int main()
{
    int n;
    vector<Edge> edgeList;
    readFile(n, edgeList);

    Graph graph = Graph(n, edgeList);

    graph.fordFulkerson(0, n - 1);
    // cout << argv[1][0] - '0';
    graph.printForDotFile(3);
    return 0;
}
/*
8 : 0 1 3 5
2 : 0 1 4 5
2 : 0 2 3 5
6 : 0 2 3 4 5
1 : 0 2 3 1 4 5
*/
