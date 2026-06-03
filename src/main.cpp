// Copyright 2022 NNTU-CS
#include "tree.h"
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>

// Функция для вычисления факториала (чтобы определить количество перестановок)
unsigned long long factorial(int n) {
    if (n <= 1) return 1;
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Функция для генерации случайного номера перестановки в допустимом диапазоне
int generateRandomPermNumber(int n) {
    unsigned long long totalPerms = factorial(n);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, static_cast<int>(totalPerms));
    return dist(gen);
}

int main() {
    // Параметры эксперимента
    std::vector<int> n_values = {2, 3, 4, 5, 6}; // Размеры алфавита
    int num_trials = 5; // Количество испытаний для каждого n

    // Векторы для хранения результатов
    std::vector<double> n_list;
    std::vector<double> time_getAllPerms;
    std::vector<double> time_getPerm1;
    std::vector<double> time_getPerm2;

    std::cout << "Начинаем вычислительный эксперимент...\n";

    for (int n : n_values) {
        std::cout << "\nТестирование для n = " << n << ":\n";

        // Создаём входной вектор символов
        std::vector<char> in;
        for (int i = 0; i < n; ++i) {
            in.push_back('A' + i); // Используем буквы A, B, C, ...
        }

        double avg_getAllPerms = 0.0;
        double avg_getPerm1 = 0.0;
        double avg_getPerm2 = 0.0;

        for (int trial = 0; trial < num_trials; ++trial) {
            // Создаём дерево
            PMTree tree(in);

            // Замер времени для getAllPerms
            auto start_getAllPerms = std::chrono::high_resolution_clock::now();
            auto perms = getAllPerms(tree);
            auto end_getAllPerms = std::chrono::high_resolution_clock::now();
            auto duration_getAllPerms = std::chrono::duration_cast<std::chrono::microseconds>(end_getAllPerms - start_getAllPerms);
            avg_getAllPerms += duration_getAllPerms.count();

            // Генерируем случайный номер перестановки
            int random_num = generateRandomPermNumber(n);

            // Замер времени для getPerm1
            auto start_getPerm1 = std::chrono::high_resolution_clock::now();
            auto result1 = getPerm1(tree, random_num);
            auto end_getPerm1 = std::chrono::high_resolution_clock::now();
            auto duration_getPerm1 = std::chrono::duration_cast<std::chrono::microseconds>(end_getPerm1 - start_getPerm1);
            avg_getPerm1 += duration_getPerm1.count();

            // Замер времени для getPerm2
            auto start_getPerm2 = std::chrono::high_resolution_clock::now();
            auto result2 = getPerm2(tree, random_num);
            auto end_getPerm2 = std::chrono::high_resolution_clock::now();
            auto duration_getPerm2 = std::chrono::duration_cast<std::chrono::microseconds>(end_getPerm2 - start_getPerm2);
            avg_getPerm2 += duration_getPerm2.count();
        }

        // Усредняем результаты
        avg_getAllPerms /= num_trials;
        avg_getPerm1 /= num_trials;
        avg_getPerm2 /= num_trials;

        // Сохраняем результаты
        n_list.push_back(n);
        time_getAllPerms.push_back(avg_getAllPerms);
        time_getPerm1.push_back(avg_getPerm1);
        time_getPerm2.push_back(avg_getPerm2);

        std::cout << "Среднее время getAllPerms: " << avg_getAllPerms << " мкс\n";
        std::cout << "Среднее время getPerm1: " << avg_getPerm1 << " мкс\n";
        std::cout << "Среднее время getPerm2: " << avg_getPerm2 << " мкс\n";

        return 0;
    }
  }