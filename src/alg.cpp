// Copyright 2022 NNTU-CS
#include "tree.h"

#include <vector>

// Деструктор узла дерева
PMTreeNode::~PMTreeNode() {
    for (auto child : children) {
        delete child;
    }
}

// Рекурсивная функция построения дерева перестановок
void PMTree::buildTree(PMTreeNode* node,
                       const std::vector<char>& available) {
    if (available.empty()) return;

    for (char c : available) {
        PMTreeNode* child = new PMTreeNode(c);
        node->children.push_back(child);

        std::vector<char> remaining;
        for (char elem : available) {
            if (elem != c) {
                remaining.push_back(elem);
            }
        }

        buildTree(child, remaining);
    }
}

// Конструктор дерева
PMTree::PMTree(const std::vector<char>& elements) {
    for (char c : elements) {
        PMTreeNode* root = new PMTreeNode(c);
        roots.push_back(root);

        std::vector<char> remaining;
        for (char elem : elements) {
            if (elem != c) {
                remaining.push_back(elem);
            }
        }
        buildTree(root, remaining);
    }
}

PMTree::~PMTree() {
    for (auto root : roots) {
        delete root;
    }
}

// Вспомогательная функция для обхода дерева и сбора перестановок
void collectAllPerms(PMTreeNode* node, std::vector<char>& current,
                     std::vector<std::vector<char>>& result) {
    if (!node) return;

    current.push_back(node->value);

    if (node->children.empty()) {
        result.push_back(current);
    } else {
        for (auto child : node->children) {
            collectAllPerms(child, current, result);
        }
    }

    current.pop_back();
}

// Функция получения всех перестановок
std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> result;
    std::vector<char> current;

    for (auto root : tree.getRoots()) {
        collectAllPerms(root, current, result);
    }

    return result;
}

// Первый способ: обход всех перестановок до нужной
std::vector<char> getPerm1(PMTree& tree, int num) {
    if (num <= 0) return {};

    auto allPerms = getAllPerms(tree);

    if (num > static_cast<int>(allPerms.size())) {
        return {};
    }

    return allPerms[num - 1];
}

// Подсчёт количества листьев в поддереве
int countLeaves(PMTreeNode* node) {
    if (!node) return 0;
    if (node->children.empty()) return 1;

    int count = 0;
    for (auto child : node->children) {
        count += countLeaves(child);
    }
    return count;
}

// Второй способ: навигация по дереву
std::vector<char> getPerm2(PMTree& tree, int num) {
    if (num <= 0) return {};

    const auto& roots = tree.getRoots();
    if (roots.empty()) return {};

    // Находим нужный корневой узел
    int cumulative = 0;
    PMTreeNode* current = nullptr;

    for (auto root : roots) {
        int leavesInSubtree = countLeaves(root);
        if (cumulative + leavesInSubtree >= num) {
            current = root;
            break;
        }
        cumulative += leavesInSubtree;
    }

    if (!current) return {};

    std::vector<char> result;
    int targetNum = num - cumulative;

    // Идём по дереву до листа
    while (current) {
        result.push_back(current->value);

        if (current->children.empty()) break;

        cumulative = 0;
        PMTreeNode* next = nullptr;

        for (auto child : current->children) {
            int leavesInSubtree = countLeaves(child);
            if (cumulative + leavesInSubtree >= targetNum) {
                next = child;
                break;
            }
            cumulative += leavesInSubtree;
        }

        if (!next) return {};

        targetNum -= cumulative;
        current = next;
    }

    return result;
}
