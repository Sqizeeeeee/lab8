/**
 * @file astar_ps.h
 * @brief Реализация алгоритма A* с пост-сглаживанием (A*PS)
 * 
 * Алгоритм A* с последующим сглаживанием пути методом "натягивания веревки"
 * для получения более плавной и оптимальной траектории
 */

#ifndef ASTAR_PS_H
#define ASTAR_PS_H

#include "astar.h"
#include "utils/line_of_sight.h"

#include <vector>

/**
 * @class AStarPS
 * @brief Реализация алгоритма A* с пост-сглаживанием
 */
class AStarPS {
public:
    /**
     * @brief Конструктор алгоритма A*PS
     * @param grid Ссылка на сетку для поиска
     */
    explicit AStarPS(Grid& grid);
    
    /**
     * @brief Найти путь от начальной до конечной точки со сглаживанием
     * @param start_x Начальная координата X
     * @param start_y Начальная координата Y
     * @param end_x Конечная координата X
     * @param end_y Конечная координата Y
     * @return Вектор узлов, представляющий сглаженный путь
     * @throw std::runtime_error если путь не найден
     */
    std::vector<Node*> findPath(int start_x, int start_y, int end_x, int end_y);
    
    /**
     * @brief Получить количество раскрытых узлов в последнем поиске
     * @return Количество раскрытых узлов
     */
    int getNodesExpanded() const { return astar_.getNodesExpanded(); }
    
    /**
     * @brief Получить длину исходного пути (до сглаживания)
     * @return Длина исходного пути
     */
    double getOriginalPathLength() const { return original_path_length_; }
    
    /**
     * @brief Получить длину сглаженного пути
     * @return Длина сглаженного пути
     */
    double getSmoothedPathLength() const { return smoothed_path_length_; }
    
    /**
     * @brief Сбросить статистику алгоритма
     */
    void resetStatistics();

private:
    Grid& grid_;                                    ///< Ссылка на сетку для проверки видимости
    AStar astar_;                                   ///< Базовый алгоритм A*
    double original_path_length_;                   ///< Длина пути до сглаживания
    double smoothed_path_length_;                   ///< Длина пути после сглаживания
    
    /**
     * @brief Сгладить путь методом "натягивания веревки"
     * @param original_path Исходный путь, найденный A*
     * @return Вектор узлов сглаженного пути
     */
    std::vector<Node*> smoothPath(const std::vector<Node*>& original_path);
    
    /**
     * @brief Вычислить длину сглаженного пути
     * @param path Вектор узлов сглаженного пути
     * @return Длина пути
     */
    double calculateSmoothedPathLength(const std::vector<Node*>& path) const;
};

#endif // ASTAR_PS_H