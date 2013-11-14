#include "decoder.h"
#include <fstream>
#include <vector>
#include <map>
#include <climits>

void decoder(string s_in, string s_out)
{	
	ifstream f_in(s_in.c_str());
	long fileSize;
	f_in.read((char *)&fileSize, sizeof(fileSize));
	if (fileSize == 1)
	{
		ofstream f_out(s_out.c_str());
	}
	else if (fileSize == 2)
	{
		ofstream f_out(s_out.c_str());
		unsigned char ch;
		f_in.read((char *) &ch, sizeof(ch));
		f_out.write((char *) &ch, sizeof(ch));
	}
	else
	{
		long treeSize;
		f_in.read((char *)&treeSize, sizeof(treeSize));

		struct Node
		{
			unsigned char ch;
			long parent; 
			long zero;
			long one;
			bool branch;
		};
		vector<Node> tree;
		for (long i = 0; i < treeSize; ++i)
		{
			Node n;
			f_in.read((char *)&n, sizeof(n));
			tree.push_back(n); 
		}
		vector<bool> data;
		while(!f_in.eof())
		{	
			unsigned char ch;
			f_in.read((char *)&ch, sizeof(ch));
			for (int i = 0; i < CHAR_BIT; ++i)
				data.push_back((ch & (1 << i)) != 0);
		}
		auto n = tree.size() - 1;
		ofstream f_out(s_out.c_str());
		long c = 0;
		for (auto i: data)
		{	
			if (c == fileSize-1) break;
			if (i)
				n = tree[n].one;
			else
				n = tree[n].zero;
			if (tree[n].one == -1)
			{	
				f_out.write((char *)& tree.at(n).ch, sizeof(char));
				n = tree.size() - 1;
				++c;
			}
		}
	}
}
