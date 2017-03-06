﻿#include<fstream>
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
		int indegree;
	};

	int mages_n; // number of mages
	int src;
	int snk;
	vector<edge> edges_init; 
	vector<vector<int>> adj_init; 
	vector<edge> edges;
	vector<vector<int>> adj;
	vector<int> queue; 
	vector<int> levels; 
	int valid_edges;
	vector<int> ptr;
	vector<mage> mages;
	//vector<int> outdegree;

	int outdg;
	int in;

	void recount_caps()
	{
		for (int i = 0; i < edges_init.size(); i += 2)
			edges_init[i].cap -= edges[i].flow;
	}

	void add_dg()
	{
		edges.clear();
		adj.clear();
		edges = edges_init;
		adj = adj_init;
		for (int i = 0; i < mages_n; i++)
			if (mages[i].dg)
				add_edge_and_rev(i, mages_n, INT_MAX, false);
	}

	void make_f_sink()
	{
		/*edges = edges_init;
		adj = adj_init;*/
		for (int i = 1; i < mages_n; i++)
			add_edge_and_rev(i, mages_n, mages[i].req_e, false);
	}

	//DONE
	void add_edge_and_rev(int from, int to, int cap, bool init)
	{
		edge e1 = { from, to, cap, 0 }; // forward
		edge e2 = { to, from, 0, 0 }; // reverse
		if (init)
		{
			adj_init[from].push_back((int)edges_init.size());
			edges_init.push_back(e1);
			adj_init[to].push_back((int)edges_init.size());
			edges_init.push_back(e2);
		}
		else
		{
			adj[from].push_back((int)edges.size());
			edges.push_back(e1);
			adj[to].push_back((int)edges.size());
			edges.push_back(e2);
		}
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
				int id = adj[v][i];
				int to = edges[id].to;
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
				to = edges[id].to;
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
		src = 0;
		snk = mages_n;

		adj.resize(mages_n + 1);
		adj_init.resize(mages_n + 1);
		levels.resize(mages_n + 1);
		ptr.resize(mages_n + 1);
		queue.resize(mages_n + 1);

		for (int i = 0; i < mages_n; i++)
		{
			int neighb = mages[i].n;
			if (neighb == 0)
			{
				input.ignore();
				continue;
			}
			for (int j = 0; j < mages[i].n; j++)
			{
				int to, cap;
				input >> to;
				input >> cap;
				mages[to].indegree += 1;
				add_edge_and_rev(i, to, cap, true);
				valid_edges++;
			}
		}
		input.close();
	}
	
	void second_dinic_out()
	{
		recount_caps();
		add_dg();
		for (int i = 0; i < queue.size(); i++)
			queue[i] = 0;
		int t = dinic();
		ofstream output;
		output.open(oname);
		output.clear();
		output << t;
		output.close();
	}

	void out()
	{
		edges = edges_init;
		adj = adj_init;
		make_f_sink();
		dinic();
		bool d = false;
		for (int i = valid_edges*2; i < edges.size(); i+=2)
			if (edges[i].flow != edges[i].cap) // если по какому-то из ребер в фиктивный сток прошло меньше 
			{								   // энергии чем capacity, значит кто-то из магов умер
				ofstream output;
				output.open(oname);
				output.clear();
				output << -1;
				d = true;
			}
		if (!d)
		{
			vector<int> outdg0;
			for(int i = 0; i < mages.size(); i++)
				if (mages[i].n == 0 && mages[i].dg == 0 && mages[i].indegree > 1)
				{
					outdg0.push_back(i);
				}
			if (outdg0.empty()) // vse ok
				second_dinic_out();
			else                // VSE OCHEN PLOHO
			{
				
			}
		}
	}
};

int main()
{
	Graph graph = Graph();
	graph.out();
}