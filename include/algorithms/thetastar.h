/**
 * @file thetastar.h
 * @brief Реализация алгоритма Theta* для поиска пути
 * 
 * Алгоритм Theta* - это вариант A*, который интегрирует сглаживание пути
 * непосредственно в процесс поиска, проверяя прямую видимость между узлами
 * для создания более оптимальных путей
 */

#ifndef THETASTAR_H
#define THETASTAR_H

#include "grid/grid.h"
#include "../utils/line_of_sight.h"
#include "../../config.h"

#include <vector>
#include <memory>
#include <queue>
#include <functional>
#include <unordered_set>

/**
 * @class ThetaStar
 * @brief Реализация алгоритма Theta* для поиска пути
 */
class ThetaStar {
public:
    /**
     * @brief Конструктор алгоритма Theta*
     * @param grid Ссылка на сетку для поиска
     */
    explicit ThetaStar(Grid& grid);
    
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
     * @brief Обновить вершину с проверкой прямой видимости от родителя родителя
     * @param current Текущий узел
     * @param neighbor Соседний узел для обновления
     * @param end_node Целевой узел
     */
    void updateVertex(Node* current, Node* neighbor, Node* end_node);
    
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

#endif // THETASTAR_H