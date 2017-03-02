#include<fstream>
#include<vector>
#include<climits>
#include<iterator>
#include<list>
#include<algorithm>

using namespace std;

string const iname = "input.txt";
string const oname = "output.txt";

class Graph
{
private:
	struct edge
	{
		int from;
		int to;
		int cap;
		int flow;
	};

	struct mage
	{
		int req_e;
		bool dg;
		int n;
	};

	int mages_n; // number of mages
	int src;
	int snk;
	vector<edge> edges; //e
	vector<vector<int>> adj; //g
	vector<int> queue; //q
	vector<int> levels; //d

	//ptr??
	vector<int> ptr;

	vector<mage> mages;

	void make_f_sink()
	{
		for (int i = 1; i < mages_n; i++)
			add_edge_and_rev(i, mages_n, mages[i].req_e);
	}

	//DONE
	bool bfs()
	{
		int qh = 0, qt = 0;
		queue[qt++] = src;
		for (int i = 0; i <= mages_n; i++)
			levels[i] = -1;
		levels[src] = 0;
		while (qh < qt && levels[snk] == -1) {
			int v = queue[qh++];
			for (size_t i = 0; i < adj[v].size(); ++i) {
				int id = adj[v][i],
					to = edges[id].to; // edges[id].b
				if (levels[to] == -1 && edges[id].flow < edges[id].cap) {
					queue[qt++] = to;
					levels[to] = levels[v] + 1;
				}
			}
		}
		return levels[snk] != -1;
	}

	// DONE
	int dfs(int v, int flow) {
		if (!flow)  return 0;
		if (v == snk)  return flow;
		for (; ptr[v] < (int)adj[v].size(); ++ptr[v]) {
			int id = adj[v][ptr[v]],
				to = edges[id].to; // edges[id].b
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

	//DONE
	int dinic() 
	{
		int flow = 0;
		for (;;) {
			if (!bfs())  break;
			for (int i = 0; i <= mages_n; i++)
				ptr[i] = 0;
			while (int pushed = dfs(src, INT_MAX))
				flow += pushed;
		}
		return flow;
	}

	//DONE
	void add_edge_and_rev(int from, int to, int cap)
	{
		edge e1 = { from, to, cap, 0 }; // forward
		edge e2 = { to, from, 0, 0 }; // reverse
		adj[from].push_back((int)edges.size());
		edges.push_back(e1);
		adj[to].push_back((int)edges.size());
		edges.push_back(e2);
	}



public:
	Graph()
	{
		ifstream input;
		input.open(iname);
		input >> mages_n;
		for (int i = 0; i < mages_n; i++)
		{
			mage m;
			input >> m.req_e;
			input >> m.dg;
			input >> m.n;
			mages.push_back(m);
		}

		//++mages_n;

		src = 0;
		snk = mages_n;

		adj.resize(mages_n + 1);
		levels.resize(mages_n + 1);
		ptr.resize(mages_n + 1);
		queue.resize(mages_n + 1);

		for (int i = 0; i < mages_n; i++)
		{
			for (int j = 0; j < mages[i].n; j++)
			{
				int to, cap;
				input >> to;
				input >> cap;
				add_edge_and_rev(i, to, cap);
			}
		}
		input.close();
	}

	void out()
	{

		make_f_sink();
		/*bool BFS(int s, int t);
		int sendFlow(int s, int flow, int t, int ptr[]);
		int DinicMaxflow(int s, int t);*/
		int t = dinic();
		ofstream output;
		output.open(oname);
		output.clear();
		output << t;
		output.close();
	}
};



int main()
{
	Graph graph = Graph();
	graph.out();
	getchar();

}