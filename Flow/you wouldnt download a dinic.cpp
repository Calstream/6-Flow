// C++ implementation of Dinic's Algorithm
#include<algorithm>
#include<iostream>
#include<list>
#include<vector>
#include<climits>
using namespace std;

const int MAXN = 5; // число вершин
const int INF = INT_MAX; // константа-бесконечность

//class Graph
//{
//public:
	struct edge {
		int a, b, cap, flow;
	};

	int mages_n, src, snk, levels[MAXN], ptr[MAXN], queue[MAXN];
	vector<edge> edges;
	vector<int> adj[MAXN];

	void add_edge_and_rev(int a, int b, int cap) {
		edge e1 = { a, b, cap, 0 };
		edge e2 = { b, a, 0, 0 };
		adj[a].push_back((int)edges.size());
		edges.push_back(e1);
		adj[b].push_back((int)edges.size());
		edges.push_back(e2);
	}

	bool bfs() {
		int qh = 0, qt = 0;
		queue[qt++] = src;
		memset(levels, -1, mages_n * sizeof levels[0]);
		levels[src] = 0;
		while (qh < qt && levels[snk] == -1) {
			int v = queue[qh++];
			for (size_t i = 0; i < adj[v].size(); ++i) {
				int id = adj[v][i],
					to = edges[id].b;
				if (levels[to] == -1 && edges[id].flow < edges[id].cap) {
					queue[qt++] = to;
					levels[to] = levels[v] + 1;
				}
			}
		}
		return levels[snk] != -1;
	}

	int dfs(int v, int flow) {
		if (!flow)  return 0;
		if (v == snk)  return flow;
		for (; ptr[v] < (int)adj[v].size(); ++ptr[v]) {
			int id = adj[v][ptr[v]],
				to = edges[id].b;
			if (levels[to] != levels[v] + 1)  continue;
			int pushed = dfs(to, min(flow, edges[id].cap - edges[id].flow));
			if (pushed) {
				edges[id].flow += pushed;
				edges[id ^ 1].flow -= pushed;
				return pushed;
			}
		}
		return 0;
	}

	int dinic() {
		int flow = 0;
		for (;;) {
			if (!bfs())  break;
			memset(ptr, 0, mages_n * sizeof ptr[0]);
			while (int pushed = dfs(src, INF))
				flow += pushed;
		}
		return flow;
	}

//};

// Driver program to test above functions
int main()
{
	src = 0;
	snk = 4;
	mages_n = 5;

	//Graph g;
	add_edge_and_rev(0, 1, 10);
	add_edge_and_rev(0, 2, 8);
	add_edge_and_rev(1, 3, 5);
	add_edge_and_rev(2, 3, 5);
	add_edge_and_rev(1, 4, 2);
	add_edge_and_rev(2, 4, 3);
	add_edge_and_rev(3, 4, 4);



	cout << "Maximum flow " << dinic();
	return 0;
}