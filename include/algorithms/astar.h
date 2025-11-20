/**
 * @file astar.h
 * @brief Реализация алгоритма A* для поиска пути
 * 
 * Классический алгоритм A* для нахождения оптимального пути
 * на сетке с обходом препятствий
 */

#ifndef ASTAR_H
#define ASTAR_H

#include "../../config.h"
#include "grid/grid.h"

#include <vector>
#include <memory>
#include <queue>
#include <functional>
#include <unordered_set>

/**
 * @class AStar
 * @brief Реализация алгоритма A* для поиска пути
 */
class AStar {
public:
    /**
     * @brief Конструктор алгоритма A*
     * @param grid Ссылка на сетку для поиска
     */
    explicit AStar(Grid& grid);
    
    /**
     * @brief Найти путь от начальной до конечной точки
     * @param start_x Начальная координата X
     * @param start_y Начальная координата Y
     * @param end_x Конечная координата X
     * @param end_y Конечная координата Y
     * @return Вектор узлов, представляющий найденный путь
     * @throw std::runtime_error если путь не найден
     */
    std::vector<Node*> findPath(int start_x, int start_y, int end_x, int end_y);
    
    /**
     * @brief Получить количество раскрытых узлов в последнем поиске
     * @return Количество раскрытых узлов
     */
    int getNodesExpanded() const { return nodes_expanded_; }
    
    /**
     * @brief Получить длину найденного пути в последнем поиске
     * @return Длина пути
     */
    double getPathLength() const { return path_length_; }
    
    /**
     * @brief Сбросить статистику алгоритма
     */
    void resetStatistics();

private:
    Grid& grid_;                                    ///< Ссылка на рабочую сетку
    int nodes_expanded_;                            ///< Счетчик раскрытых узлов
    double path_length_;                            ///< Длина последнего найденного пути
    
    /**
     * @brief Восстановить путь от конечного узла до начального
     * @param end_node Конечный узел
     * @return Вектор узлов пути (от начала до конца)
     */
    std::vector<Node*> reconstructPath(Node* end_node);
    
    /**
     * @brief Вычислить длину пути
     * @param path Вектор узлов пути
     * @return Длина пути
     */
    double calculatePathLength(const std::vector<Node*>& path) const;
    
    /**
     * @brief Структура для сравнения узлов в приоритетной очереди
     */
    struct NodeCompare {
        bool operator()(const Node* a, const Node* b) const {
            return a->f_cost > b->f_cost;
        }
    };
    
    /**
     * @brief Хэш-функция для узлов
     */
    struct NodeHash {
        std::size_t operator()(const Node* node) const {
            return std::hash<int>()(node->x) ^ (std::hash<int>()(node->y) << 1);
        }
    };
    
    /**
     * @brief Функция сравнения узлов для unordered_set
     */
    struct NodeEqual {
        bool operator()(const Node* a, const Node* b) const {
            return a->x == b->x && a->y == b->y;
        }
    };
    
    using NodeSet = std::unordered_set<Node*, NodeHash, NodeEqual>;
    using PriorityQueue = std::priority_queue<Node*, std::vector<Node*>, NodeCompare>;
};

#endif // ASTAR_H