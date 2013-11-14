#include <fstream>
#include <vector>
#include <map>
#include "decoder.h"
#include <climits>

using namespace std;


void decoder(char const s_in[], char const s_out[])
{	

	struct Node
	{
		char ch;
		int parent; 
		int zero;
		int one;
		bool branch;
	};
	vector<Node> tree;
	ifstream f(s_in);
	int treeSize;
	int fileSize;
	f.read((char *)&fileSize, sizeof(fileSize));
	f.read((char *)&treeSize, sizeof(treeSize));
	for (int i = 0; i < treeSize; ++i)
	{
		Node n;
		f.read((char *)&n, sizeof(n));
		tree.push_back(n); 
	}
	vector<bool> data;
	while(!f.eof())
	{	
		char ch;
		f.read((char *)&ch, sizeof(ch));
		for (int i = 0; i < CHAR_BIT; ++i)
			data.push_back((ch & (1 << i)) != 0);
	}
	auto n = tree.size() - 1;
	ofstream ff(s_out);
	for (auto i: data)
	{
		if (i)
			n = tree[n].one;
		else
			n = tree[n].zero;
		if (tree[n].one == -1)
		{	
			ff.write((char *)& tree.at(n).ch, sizeof(char));
			n = tree.size() - 1;
		}
	}
}
