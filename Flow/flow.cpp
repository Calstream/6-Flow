#include<fstream>
#include<vector>

using namespace std;

string const iname = "input.txt";
string const oname = "output.txt";

class Graph
{
public:
	Graph()
	{
		ifstream input;
		input.open(iname);

	}

	void out()
	{
		ofstream output;
		output.open(oname);
		output.clear();
	}
};



int main()
{
	Graph graph = Graph();
	graph.out();
	getchar();

}