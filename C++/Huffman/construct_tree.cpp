#include "construct_tree.h"
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>


using namespace std;

void walk(Node *currentNode, int currentDeep = 0);

vector<int> codeLengths;
priority_queue<Node *, vector<Node *>, NodeComparer> nodesQueue;
vector<int> numberInternals;

vector<int> & construct_tree(vector<size_t> const &weights)
{
    for (int i = 0; i < (int)weights.size(); ++i) {
        if (weights[i])
            nodesQueue.push(new Node(weights[i], i));
    }

    // anti single-character-file bugfix: adding dummy nodes
    if (nodesQueue.size() == 0) {
        nodesQueue.push(new Node(0, 0));
        nodesQueue.push(new Node(0, 1));
    }
    if (nodesQueue.size() == 1) {
        if (nodesQueue.top()->symbol_ == 0)
            nodesQueue.push(new Node(0, 1));
        else
            nodesQueue.push(new Node(0, 0));
    }

    while (nodesQueue.size() > 1) {
        Node *childR = nodesQueue.top();
        nodesQueue.pop();
        Node *childL = nodesQueue.top();
        nodesQueue.pop();
        Node *parent = new Node(childL, childR);
        nodesQueue.push(parent);
    }

    codeLengths.assign(weights.size(), 0);
    walk(nodesQueue.top());
    delete nodesQueue.top();
    return codeLengths;

}

vector<int> & getNumberInternals() {
    return numberInternals;
}

void walk(Node *currentNode, int currentDeep) {
    if (currentNode) {
        walk(currentNode->left_, currentDeep + 1);
        if (currentNode->symbol_ != -1)
            codeLengths[currentNode->symbol_] = currentDeep;
        else {
            if (numberInternals.size() < size_t(currentDeep + 1))
                numberInternals.resize(currentDeep + 1, 0);
            ++numberInternals[currentDeep];
        }
        
        walk(currentNode->right_, currentDeep + 1);
    }
}