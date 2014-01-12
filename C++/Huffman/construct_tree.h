#ifndef CONSTRUCT_TREE_H_
#define CONSTRUCT_TREE_H_

#include <vector>
#include <cstddef>

struct Node {
    Node();
    Node(size_t weight, int symbol = -1, Node *left = NULL, Node *right = NULL)
        : weight_(weight), symbol_(symbol), left_(left), right_(right) { }
    Node(Node *left, Node *right, int symbol = -1)
        : weight_(left->weight_ + right->weight_),
          symbol_(symbol),
          left_(left),
          right_(right) { }

    ~Node() {
        delete left_;
        delete right_;
    }

    size_t weight_;
    int symbol_;
    Node *left_;
    Node *right_;
};

struct NodeComparer {
        bool operator()(Node const *left, Node const *right) const {
            return left->weight_ > right->weight_;
        }
    };

std::vector<int> & construct_tree(std::vector<size_t> const &);
std::vector<int> & getNumberInternals();

#endif