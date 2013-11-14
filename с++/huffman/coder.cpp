#include "coder.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <climits>

void coder(string s_in, string s_out)
{	
	ifstream f_in(s_in.c_str());
	vector<long> weight(0x100, 0);
	long fileSize = 1;
	unsigned char chs;
	// while (!f_in.eof())
	while (f_in.peek() != EOF)
	{	
		++fileSize;
		f_in.read((char *) &chs, sizeof(chs));
		++weight[chs];
	}
	if (fileSize == 1)
	{
		ofstream f_out(s_out.c_str());
		f_out.write((char *) &fileSize, sizeof(fileSize));	
	}
	else if (fileSize == 2)
	{	
		ofstream f_out(s_out.c_str());
		f_out.write((char *) &fileSize, sizeof(fileSize));
		f_out.write((char *) &chs, sizeof(chs));

	}
	else
	{
		multimap<long /* weight */,
				 long /* index in the tree*/> sortedWeight;

		struct Node
		{
			unsigned char ch;
			long parent;
			long zero;
			long one;
			bool branch;
		};

		vector<Node> tree;
		map<unsigned char, long> charMap;
		for (size_t i = 0; i < 0x100; ++i)
		{
			if (weight[i] > 0)
			{
				tree.push_back(Node{(unsigned char)i, -1, -1, -1, false});
				charMap[i] = tree.size() - 1;
				sortedWeight.insert(make_pair(weight[i], tree.size() - 1));
			}
		}
		while(sortedWeight.size() - 1)
		{
			long w0 = sortedWeight.begin()->first;
			long n0 = sortedWeight.begin()->second;
			sortedWeight.erase(sortedWeight.begin());
			long w1 = sortedWeight.begin()->first;
			long n1 = sortedWeight.begin()->second;
			sortedWeight.erase(sortedWeight.begin());
			tree.push_back(Node{'\0', -1, n0, n1, false});
			tree[n0].parent = tree.size() - 1;
			tree[n0].branch = false;
			tree[n1].parent = tree.size() - 1;
			tree[n1].branch = true;
			sortedWeight.insert(make_pair(w0 + w1, tree.size() - 1));
		}
		
		vector<bool> data;
		f_in.clear();
		f_in.seekg(0);
		while(!f_in.eof())
		// while(f_in.peek() != EOF)
		{	
			unsigned char ch;
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
		long treeSize = tree.size();
		f_out.write((char *) &fileSize, sizeof(fileSize));
		f_out.write((char *) &treeSize, sizeof(treeSize));

		for (auto i: tree)
			f_out.write((char *)&i, sizeof(i));
		for (long i = 0; i <= data.size() / CHAR_BIT; ++i)
		{		
			unsigned char ch = 0;
			for (int j = 0; j < CHAR_BIT; ++j)
				if (data[i * CHAR_BIT + j])
					ch |= (1 << j);
			f_out.write((char *) &ch, sizeof(ch));
		}
	}
}