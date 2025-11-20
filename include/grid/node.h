/**
 * @file node.h
 * @brief Структура узла сетки для алгоритмов поиска пути
 * 
 * Определяет структуру Node, которая представляет собой ячейку сетки
 * и хранит информацию, необходимую для работы алгоритмов A*, A*PS и Theta*
 */

#ifndef NODE_H
#define NODE_H

#include "../../config.h"
#include <memory>
#include <cmath>

/**
 * @struct Node
 * @brief Узел сетки для алгоритмов поиска пути
 */
struct Node {
    int x;                          ///< Координата X на сетке
    int y;                          ///< Координата Y на сетке
    
    bool walkable;                  ///< Доступность узла (true - проходимый)
    
    double g_cost;                  ///< Стоимость пути от старта до этого узла
    double h_cost;                  ///< Эвристическая оценка до цели
    double f_cost;                  ///< Общая стоимость: f = g + h
    
    Node* parent;                   ///< Родительский узел для восстановления пути
    
    /**
     * @brief Конструктор узла
     * @param x_coord Координата X
     * @param y_coord Координата Y
     * @param is_walkable Флаг проходимости
     */
    Node(int x_coord, int y_coord, bool is_walkable = true)
        : x(x_coord), y(y_coord), walkable(is_walkable),
          g_cost(0.0), h_cost(0.0), f_cost(0.0), parent(nullptr) {}
    
    /**
     * @brief Сбрасывает параметры поиска узла
     */
    void reset() {
        g_cost = 0.0;
        h_cost = 0.0;
        f_cost = 0.0;
        parent = nullptr;
    }
    
    /**
     * @brief Вычисляет эвристическое расстояние до целевого узла
     * @param target Целевой узел
     * @return Эвристическое расстояние
     */
    double calculateHeuristic(const Node& target) const {
        if (config::ALLOW_DIAGONAL_MOVEMENT) {
            // Евклидово расстояние для диагонального движения
            double dx = static_cast<double>(x - target.x);
            double dy = static_cast<double>(y - target.y);
            return std::sqrt(dx * dx + dy * dy);
        } else {
            // Манхэттенское расстояние для ортогонального движения
            return std::abs(x - target.x) + std::abs(y - target.y);
        }
    }
    
    /**
     * @brief Вычисляет стоимость перемещения к соседнему узлу
     * @param neighbor Соседний узел
     * @return Стоимость перемещения
     */
    double calculateMoveCost(const Node& neighbor) const {
        if (x == neighbor.x || y == neighbor.y) {
            // Ортогональное перемещение
            return 1.0;
        } else {
            // Диагональное перемещение
            return config::DIAGONAL_COST;
        }
    }
    
    /**
     * @brief Оператор сравнения для приоритетной очереди
     */
    bool operator>(const Node& other) const {
        return f_cost > other.f_cost;
    }
    
    /**
     * @brief Проверяет, совпадают ли координаты узлов
     */
    bool operator==(const Node& other) const {
        return x == other.x && y == other.y;
    }
    
    /**
     * @brief Проверяет, различны ли координаты узлов
     */
    bool operator!=(const Node& other) const {
        return !(*this == other);
    }
};

#endif // NODE_H