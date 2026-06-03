// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_
#include <vector>

class PMTreeNode {
public:
    char value;
    std::vector<PMTreeNode*> children;

    PMTreeNode(char val) : value(val) {}
    ~PMTreeNode();
};

class PMTree {
private:
    PMTreeNode* root;
    void buildTree(PMTreeNode* node, const std::vector<char>& available);

public:
    PMTree(const std::vector<char>& elements);
    ~PMTree();
    PMTreeNode* getRoot() const { return root; }
};

std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
