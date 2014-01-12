#include "construct_tree.h"
#include "encode.h"
#include "bit_stream.h"
#include <climits>
#include <cstddef>
#include <cstdio>
#include <algorithm>

using namespace std;

void ComputeCodes(size_t dictionarySize, vector<int> const &codeLengths, vector<int> &codes)
{
    int maxCodeLength = *max_element(codeLengths.begin(), codeLengths.end());
    vector<int> numberLeafs(maxCodeLength + 1, 0);
    for (size_t i = 0; i < dictionarySize; ++i) {
        if (codeLengths[i])
            ++numberLeafs[codeLengths[i]];
    }

    vector<int> S(maxCodeLength + 1, 0);
    for (int i = maxCodeLength - 1; i > 0; --i) {
        S[i] = (S[i + 1] + numberLeafs[i + 1]) >> 1;
    }

    codes.assign(dictionarySize, -1);
    for (size_t i = 0; i < dictionarySize; ++i) {
        if (codeLengths[i]) {
            codes[i] = S[codeLengths[i]];
            S[codeLengths[i]]++;
        }
    }
}

void encode(string const &inputFileName, string const &out_f)
{
    ifstream inputFile(inputFileName.c_str(), ifstream::binary | ifstream::in);
    vector<size_t> weights((size_t)1 << CHAR_BIT * sizeof(unsigned char), 0);
    size_t fileSize = 0;
    for (char byte; inputFile.get(byte); ++weights[(unsigned char)byte], ++fileSize);

    inputFile.clear();
    inputFile.seekg(0);

    BitStream out_file(out_f, BitStream::wb);
    out_file.write(fileSize);
    out_file.write(weights.size());
    for (size_t i = 0; i < weights.size(); ++i) {
        out_file.write(weights[i]);
    }

    vector<int> const lengths = construct_tree(weights);
    vector<int> codes;
    ComputeCodes(weights.size(), lengths, codes);
    char byte;
    while (inputFile.get(byte)) {
        unsigned char c = (unsigned char)byte;
        int code = codes[c];
        for (int k = lengths[c] - 1; k >= 0; --k) {
            out_file.writeBit(code & (1 << k));
        }
    }
}