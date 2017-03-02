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
		int to;
		int flow;
		int cap;
		int rev; // rev --
	};

	struct mage
	{
		int req_e;
		bool dg;
		int n;
	};

	// A DFS based function to send flow after BFS has
	// figured out that there is a possible flow and
	// constructed levels. This function called multiple
	// times for a single call of BFS.
	// flow : Current flow send by parent function call
	// start[] : To keep track of next edge to be explored.
	//           start[i] stores  count of edges explored
	//           from i.
	//  u : Current vertex
	//  t : Sink

	int sendFlow(int u, int flow, int t, int start[])
	{
		// Sink reached
		if (u == t)
			return flow;

		// Traverse all adjacent edges one -by - one.
		for (; start[u] < adj[u].size(); start[u]++)
		{
			// Pick next edge from adjacency list of u
			edge &e = adj[u][start[u]];

			if (levels[e.to] == levels[u] + 1 && e.flow < e.cap)
			{
				// find minimum flow from u to t
				int curr_flow = min(flow, e.cap - e.flow);

				int temp_flow = sendFlow(e.to, curr_flow, t, start);

				// flow is greater than zero
				if (temp_flow > 0)
				{
					// add flow  to current edge
					e.flow += temp_flow;

					// subtract flow from reverse edge
					// of current edge
					adj[e.to][e.rev].flow -= temp_flow;
					return temp_flow;
				}
			}
		}

		return 0;
	}

	int mages_n; // number of mages - G
	vector<int> levels;
	vector<mage> mages;
	vector<vector<edge>> adj; // 

	void make_f_sink()
	{
		for (int i = 1; i < mages_n; i++)
			adj[i].push_back({mages_n, 0, mages[i].req_e, 0});
	}

	bool BFS(int s, int t)
	{
		for (int i = 0; i < mages_n; i++) // careful!
			levels[i] = -1;

		levels[s] = 0;  // Level of source vertex

					   // Create a queue, enqueue source vertex
					   // and mark source vertex as visited here
					   // level[] array works as visited array also.
		list< int > q;
		q.push_back(s);

		vector<edge>::iterator i;
		while (!q.empty())
		{
			int u = q.front();
			q.pop_front();
			for (i = adj[u].begin(); i != adj[u].end(); i++)
			{
				edge &e = *i;
				if (levels[e.to] < 0 && e.flow < e.cap)
				{
					// Level of current vertex is,
					// level of parent + 1
					levels[e.to] = levels[u] + 1;

					q.push_back(e.to);
				}
			}
		}

		// IF we can not reach to the sink we
		// return false else true
		return levels[t] < 0 ? false : true;
	}

	// Returns maximum flow in graph
	int DinicMaxflow(int s, int t)
	{
		// Corner case
		if (s == t)
			return -1;

		int total = 0;  // Initialize result

						// Augment the flow while there is path
						// from source to sink
		while (BFS(s, t) == true)
		{
			// store how many edges are visited
			// from V { 0 to V }
			int *start = new int[mages_n + 1];

			// while flow is not zero in graph from S to D
			while (int flow = sendFlow(s, INT_MAX, t, start))

				// Add path flow to overall flow
				total += flow;
		}

		// return maximum flow
		return total;
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

		adj.resize(mages_n);
		levels.resize(mages_n+1);

		for (int i = 0; i < mages_n; i++)
		{
			for (int j = 0; j < mages[i].n; j++)
			{
				int tt, tc;
				input >> tt;
				input >> tc;
				adj[i].push_back({tt, 0, tc, 0});
			}

			//if (mages[i].dg)
			//	adj[i].push_back({ mages_n, -1, INT_MAX, -1 });
		}
		input.close();
	}

	void out()
	{

		make_f_sink();
		/*bool BFS(int s, int t);
		int sendFlow(int s, int flow, int t, int ptr[]);
		int DinicMaxflow(int s, int t);*/
		int t = DinicMaxflow(0, mages_n);
		ofstream output;
		output.open(oname);
		output.clear();
		output.close();
	}
};



int main()
{
	Graph graph = Graph();
	graph.out();
	getchar();

}