#include "decode.h"
#include "construct_tree.h"
#include "bit_stream.h"
#include <algorithm>

using namespace std;

bool compareCodes(pair<int, int> x, pair<int, int> y)
{
    if (x.second != y.second)
        return x.second > y.second;
    else
        return x.first < y.first;
}

void decode(string const &in_f, string const &out_f)
{
    BitStream in_file(in_f, BitStream::rb);

    size_t sourceFileSize;
    size_t dictionary_size;
    in_file.read(sourceFileSize);
    in_file.read(dictionary_size);

    vector<size_t> weights(dictionary_size, 0);
    for (size_t i = 0; i < dictionary_size; ++i) {
        in_file.read(weights[i]);
    }

    vector<int> const codeLengths = construct_tree(weights);
    int maxCodeLength = *max_element(codeLengths.begin(), codeLengths.end());
    vector<int> numberInternals = getNumberInternals();
    if (numberInternals.size() < size_t(maxCodeLength + 1))
        numberInternals.resize(maxCodeLength + 1, 0);

    vector<pair<int, int> > sorted_symbols;
    for (size_t i = 0; i < dictionary_size; ++i) {
        if (codeLengths[i])
            sorted_symbols.push_back(make_pair(i, codeLengths[i]));
    }
    sort(sorted_symbols.begin(), sorted_symbols.end(), compareCodes);

    vector<int> offset (maxCodeLength + 1, -1);
    for (int i = 0; i < (int)sorted_symbols.size(); ++i) {
        int len = sorted_symbols[i].second;
        if (offset[len] == -1)
            offset[len] = i;
    }

    ofstream out_file(out_f.c_str());

    for (size_t i = 0; i < sourceFileSize; ++i) {
        int code = in_file.readBit();
        int length = 1;
        while (code < numberInternals[length]) {
            code <<= 1;
            code += in_file.readBit();
            ++length;
        }
        out_file << (unsigned char)(sorted_symbols
                                          [offset[length] + code - numberInternals[length]].first);
    }
}