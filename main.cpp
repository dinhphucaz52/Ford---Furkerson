#include <iostream>
#include <queue>
#include <fstream>
#include <algorithm>


using namespace std;

const int MAXM = 2e3;

template <typename T>
ostream &operator<<(ostream &os, vector<T> list)
{
    cerr << "{";
    for (auto x : list)
        cerr << x << ", ";
    cerr << "}";
    return os;
}

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
    int **curCapacity; // curCapacity[u][v]
    int **tmp; 
    int *par; //par[u]
    vector<AugmentingPath> augmentingPathList;
    vector<Edge> edgeList;

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
                if (!vst[v] && this->curCapacity[u][v] > 0)
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

public:
    Graph(int size)
    {
        this->n = size;
        this->curCapacity = new int *[size];
        this->tmp = new int *[size];
        for (int i = 0; i < size; i++)
        {
            this->curCapacity[i] = new int[size];
            this->tmp[i] = new int[size];
        }
        for (int u = 0; u < size; u++)
            for (int v = 0; v < size; v++)
            {
                this->curCapacity[u][v] = this->tmp[u][v] = 0;
            }
        this->par = new int[size];
        this->augmentingPathList = vector<AugmentingPath>();
        this->edgeList = vector<Edge>();
    }

    Graph(int size, vector<Edge> edgeList) : Graph(size)
    {
        for (auto [u, v, w] : edgeList)
        {
            curCapacity[u][v] = tmp[u][v] = w;
        }
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
                minFlow = min(minFlow, curCapacity[u][v]);
            }

            this->traceBack(minFlow, s, t); // Chi de phuc vu preview do thi

            for (int v = t; v != s; v = par[v])
            {
                int u = par[v];
                curCapacity[u][v] -= minFlow;
                curCapacity[v][u] += minFlow;
            }
            res += minFlow;
        }
        return res;
    }
    void printForDotFile(const char *path, const int augmentingPathCnt, bool pre = false)
    {
        ofstream ofs;
        ofs.open(path, ios::out);
        ofs << "/*\n";
        int cur = 0;
        vector<vector<int>> adj(this->n, vector<int>(this->n, 0));
        vector<vector<bool>> isColor(this->n, vector<bool>(this->n, 0));
        for (int i = 0; i < this->augmentingPathList.size(); i++)
        {
            AugmentingPath path = this->augmentingPathList[i];
            if (i + 1 == augmentingPathCnt)
            {
                // cerr << "Res: " << cur << " + " << " " << this->augmentingPathList[i].flow;
                // cerr << endl;
            }
            cur += this->augmentingPathList[i].flow;
            ofs << path.flow << " : ";
            ofs << path.vertices[0] << " ";
            for (int j = 1; j < path.vertices.size(); j++)
            {
                int u = path.vertices[j - 1];
                int v = path.vertices[j];
                ofs << v << " ";
                if (i < augmentingPathCnt)
                {
                    adj[u][v] += path.flow;
                    isColor[u][v] = (i + 1 == augmentingPathCnt);
                }
            }
            ofs << endl;
        }
        ofs << "*/\n";
        ofs << "digraph G {" << "\n";
        ofs << "0 [fillcolor = aqua;style = \"filled\";];\n";
        ofs << n - 1 << " [fillcolor = aqua;style = \"filled\";];\n";
        for (auto [u, v, w] : this->edgeList)
        {
            if (augmentingPathCnt)
            {
                if (isColor[v][u])
                {
                    ofs << u << " -> " << v << " "
                        << (isColor[u][v] && pre ? "[color=red]" : "")
                        << "[label=\"" << adj[u][v] - this->augmentingPathList[augmentingPathCnt - 1].flow
                        << "," << w << "\"]\n";
                    ofs << v << " -> " << u << " "
                        << "[color= aquamarine]"
                        << "\n";
                }
                else
                {
                    ofs << u << " -> " << v << " "
                        << (isColor[u][v] && pre ? "[color=red]" : "")
                        << "[label=\"" << adj[u][v]
                        << "," << w << "\"]\n";
                }
            }
            else
                ofs << u << " -> " << v << " "
                    << "[label=\"" << w << "\"]\n";
        }
        ofs << "0 -> 4 [style = invis;];\n";
        ofs << "}\n";
        ofs.close();
    }
    void printMinCut(int s, int t)
    {
        this->fordFulkerson(s, t);
        vector<bool> vst(this->n);
        vector<int> setS = {s};
        queue<int> q;
        vst[s] = true;
        q.push(s);

        while (!q.empty())
        {
            int u = q.front();
            q.pop();
            for (int v = 0; v < this->n; v++)
            {
                if (!vst[v] && this->curCapacity[u][v])
                {
                    q.push(v);
                    vst[v] = true;
                    setS.push_back(v);
                }
            }
        }
        vector<int> setT = {};
        for (int u = 0; u < this->n; u++)
            if (!vst[u])
                setT.push_back(u);
                
        cerr << "setS : " << setS << "\n";
        cerr << "setT : " << setT << "\n";
        
        for (int u : setS)
        {
            for (int v : setT)
            {
                if (tmp[u][v])
                    cerr << u << " " << v << " " << this->curCapacity[u][v] + this->curCapacity[v][u] << "\n";
            }
        }
    }
    ~Graph()
    {
        free(this->curCapacity);
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

    cerr << "Max flow:" << graph.fordFulkerson(0, n - 1) << endl;

    cerr << "Min cut:\n";
    graph.printMinCut(0, n - 1);

    int step = 0;

    for (step = -1; step < 5; step++)
    {
        graph.printForDotFile("dot.dot", step == -1 ? 0 : step);
        graph.printForDotFile("dot1.dot", step + 1, true);
    }
    return 0;
}

/*
8 : 0 1 3 5
2 : 0 1 4 5
2 : 0 2 3 5
6 : 0 2 3 4 5
1 : 0 2 3 1 4 5
*/
