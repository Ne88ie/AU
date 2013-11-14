#include <fstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include <map>

using namespace std;

int main(int argc, char *argv[])
{
	vector<int> weight(0x100, 0);
	string s_in= argv[1];
	string s_out= argv[2];
	ifstream f_in(s_in.c_str());
	while (!f_in.eof())
	{
		char ch;
		f_in.read((char *) &ch, sizeof(ch));
		++weight[ch];
	}		
	
	multimap<int, int> sortedWeight;

	struct Node
	{
		char ch;
		int parent;
		int zero;
		int one;
		bool branch;
	};

	vector<Node>;
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
	f_in.clear();
	f_in.seekg(0);
	while(!f_in.eof())
	{	++fileSize;
		char ch;
		f_in.read((char *)&ch, sizeof(ch));
		auto n = tree[charMap[ch]];
		vector<bool> compressedChar;
		while(n.parent != -1)
		{
			compressedChar.push_back(n.branch);
			n = tree[n.parent];
		}
		data.insert(data.end(), compressedChar.rbegin(),compressedChar.rend());
	}
	ofstream f_out(s_out.c_str());
	int treeSize = tree.size();
	fileSize += treeSize + sizeof(fileSize) + sizeof(treeSize);
	f_out.write((char *) &fileSize, sizeof(fileSize));
	f_out.write((char *) &treeSize, sizeof(treeSize));

	for (auto i: tree)
		f_out.write((char *)&i, sizeof(i));
	for (size_t i = 0; i <= data.size() / CHAR_BIT; ++i)
	{		
		char ch = 0;
		for (int j = 0; j < CHAR_BIT; ++j)
			if (data[i * CHAR_BIT + j])
				ch |= (1 << j);
		f_out.write((char *) &ch, sizeof(ch));
	}

	// ofstream f_out(argv[2]);
	// cout << EOF;
	for (int i: weight)
		cout << i << endl;

	return 0;	
}