// Copyright 2022 NNTU-CS
#include "tree.h"
#include <vector>

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
        // Корневой узел имеет реальное значение, начинаем с него
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

// Конструктор дерева — создаёт корневые узлы
PMTree::PMTree(const std::vector<char>& elements) {
    // Создаем реальный корневой узел с первым элементом
    // Для этого нужно создать несколько корневых узлов
    // Проще создать фиктивный корень, но при обходе его пропускать
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

    // Пропускаем фиктивный корневой узел
    if (current->value == '\0' && !current->children.empty()) {
        // Нам нужно выбрать правильного потомка фиктивного корня
        int cumulative = 0;
        PMTreeNode* next = nullptr;
        
        for (auto child : current->children) {
            int leavesInSubtree = countLeaves(child);
            if (cumulative + leavesInSubtree >= num) {
                next = child;
                break;
            }
            cumulative += leavesInSubtree;
        }
        
        if (!next) return {};
        num -= cumulative;
        current = next;
    }
    
    // Теперь идем по дереву до листа
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