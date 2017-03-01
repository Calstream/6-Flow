#include<fstream>
#include<vector>
#include<climits>

using namespace std;

string const iname = "input.txt";
string const oname = "output.txt";

class Graph
{
private:
	struct edge
	{
		int to;
		int from;
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

	int mages_n; // number of mages - G
	vector<int> levels;
	vector<mage> mages;
	vector<edge> adj; // 

	void make_f_sink()
	{
		for (int i = 1; i < mages_n; i++)
			adj.push_back({mages_n, i, -1, mages[i].req_e, -1});
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

		for (int i = 0; i < mages_n; i++)
		{
			for (int j = 0; j < mages[i].n; j++)
			{
				int tt, tc;
				input >> tt;
				input >> tc;
				adj.push_back({tt, i, -1, tc, -1});
			}

			//if (mages[i].dg)
			//	adj.push_back({ mages_n, i, -1, INT_MAX, -1 });
		}
		input.close();
	}

	void out()
	{
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