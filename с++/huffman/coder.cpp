#include <fstream>
#include <vector>
#include <map>
#include <climits>
#include "coder.h"

using namespace std;

void coder(char const s_in[], char const s_out[])
{	
	vector<int> weight(0x100, 0);
	{	
		ifstream f(s_in);
		while (!f.eof())
		{
			unsigned char ch;
			f.read((char *) &ch, sizeof(ch));
			++weight[ch];
		}

	}

	multimap<int /* weight */,
			 int /* index in the tree*/> sortedWeight;

	struct Node
	{
		char ch;
		int parent; 
		int zero;
		int one;
		bool branch;
	};
	vector<Node> tree;
	map<char, int> charMap;
	for (size_t i = 0; i < 0x100; ++i)
	{
		if (weight[i] > 0)
		{
			tree.push_back(Node{(char)i, -1, -1, -1, false});
			charMap[i] = tree.size() - 1;
			sortedWeight.insert(make_pair(weight[i], tree.size() - 1));
		}
	}
	while(sortedWeight.size() - 1)
	{
		int w0 = sortedWeight.begin()->first;
		int n0 = sortedWeight.begin()->second;
		sortedWeight.erase(sortedWeight.begin());
		int w1 = sortedWeight.begin()->first;
		int n1 = sortedWeight.begin()->second;
		sortedWeight.erase(sortedWeight.begin());
		tree.push_back(Node{'\0', -1, n0, n1, false});
		tree[n0].parent = tree.size() - 1;
		tree[n0].branch = false;
		tree[n1].parent = tree.size() - 1;
		tree[n1].branch = true;
		sortedWeight.insert(make_pair(w0 + w1, tree.size() - 1));
	}
	int fileSize=0;
	vector<bool> data;
	{	
		ifstream f(s_in);
		while(!f.eof())
		{	++fileSize;
			unsigned char ch;
			f.read((char *)&ch, sizeof(ch));
			auto n = tree[charMap[ch]];
			vector<bool> compressedChar;
			while(n.parent != -1)
			{
				compressedChar.push_back(n.branch);
				n = tree[n.parent];
			}
			data.insert(data.end(), compressedChar.rbegin(),compressedChar.rend());
		}
	}
	ofstream f(s_out);
	int treeSize = tree.size();
	f.write((char *) &fileSize, sizeof(fileSize));
	f.write((char *) &treeSize, sizeof(treeSize));
	// for (int i=0: sortedWeight)
	// {
	// 	cout << i.first;
	// }

	for (auto i: tree)
		f.write((char *)&i, sizeof(i));
	for (size_t i = 0; i <= data.size() / CHAR_BIT; ++i)
	{		
		unsigned char ch = 0;
		for (int j = 0; j < CHAR_BIT; ++j)
			if (data[i * CHAR_BIT + j])
				ch |= (1 << j);
		f.write((char *) &ch, sizeof(ch));
	}
}