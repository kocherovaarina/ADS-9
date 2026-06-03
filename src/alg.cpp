// Copyright 2022 NNTU-CS
#include  <iostream>
#include  <fstream>
#include  <locale>
#include  <cstdlib>
#include  "tree.h"

// Деструктор узла дерева — рекурсивно удаляет всех потомков
PMTreeNode::~PMTreeNode() {
    for (auto child : children) {
        delete child;
    }
}

// Вспомогательная функция для обхода дерева и сбора всех перестановок
void collectAllPerms(PMTreeNode* node, std::vector<char>& current,
                     std::vector<std::vector<char>>& result) {
    if (!node) return;

    current.push_back(node->value);

    // Если это лист — добавляем текущую перестановку в результат
    if (node->children.empty()) {
        result.push_back(current);
    } else {
        // Рекурсивно обходим всех потомков
        for (auto child : node->children) {
            collectAllPerms(child, current, result);
        }
    }

    // Возвращаем состояние для других ветвей обхода
    current.pop_back();
}

// Функция получения всех перестановок из дерева
std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> result;
    std::vector<char> current;

    if (tree.getRoot()) {
        collectAllPerms(tree.getRoot(), current, result);
    }

    return result;
}

// Рекурсивная функция построения дерева перестановок
void PMTree::buildTree(PMTreeNode* node, const std::vector<char>& available) {
    // Базовый случай: если доступных элементов нет — останавливаемся
    if (available.empty()) return;

    // Создаём потомков для текущего узла из доступных элементов
    for (char c : available) {
        PMTreeNode* child = new PMTreeNode(c);
        node->children.push_back(child);

        // Создаём новый вектор доступных элементов без текущего
        std::vector<char> remaining;
        for (char elem : available) {
            if (elem != c) {
                remaining.push_back(elem);
            }
        }

        // Рекурсивно строим поддерево
        buildTree(child, remaining);
    }
}

// Конструктор дерева — создаёт корневые узлы и запускает построение
PMTree::PMTree(const std::vector<char>& elements) {
    root = new PMTreeNode('\0'); // Временный корневой узел
    buildTree(root, elements);
}

PMTree::~PMTree() {
    delete root;
}

// Первый способ: обход всех перестановок до нужной
std::vector<char> getPerm1(PMTree& tree, int num) {
    if (num <= 0) return {};

    auto allPerms = getAllPerms(tree);

    if (num > static_cast<int>(allPerms.size())) {
        return {}; // Если номер больше количества перестановок
    }

    return allPerms[num - 1]; // Нумерация с 1
}

// Вспомогательная функция для подсчёта количества листьев в поддереве
int countLeaves(PMTreeNode* node) {
    if (!node) return 0;
    if (node->children.empty()) return 1;

    int count = 0;
    for (auto child : node->children) {
        count += countLeaves(child);
    }
    return count;
}

// Второй способ: навигация по дереву с выбором нужного пути
std::vector<char> getPerm2(PMTree& tree, int num) {
    if (num <= 0 || !tree.getRoot()) return {};

    std::vector<char> result;
    PMTreeNode* current = tree.getRoot();

    // Пропускаем корневой узел (временный)
    if (!current->children.empty()) {
        current = current->children[0]; // Начинаем с первого реального узла
    }

    while (current) {
        result.push_back(current->value);

        if (current->children.empty()) break; // Достигли листа

        int cumulative = 0;
        PMTreeNode* next = nullptr;

        // Ищем нужное поддерево для перехода
        for (auto child : current->children) {
            int leavesInSubtree = countLeaves(child);
            if (cumulative + leavesInSubtree >= num) {
                next = child;
                break;
            }
            cumulative += leavesInSubtree;
        }

        if (!next) return {}; // Ошибка: номер вне диапазона

        num -= cumulative; // Корректируем номер для следующего уровня
        current = next;
    }

    return result;
}
