/**
 * @file metrics_calculator.h
 * @brief Калькулятор метрик для оценки качества путей
 * 
 * Содержит функции для вычисления всех метрик оптимальности пути,
 * вычислительной эффективности и качества траектории
 */

#ifndef METRICS_CALCULATOR_H
#define METRICS_CALCULATOR_H

#include "grid/grid.h"
#include "../../config.h"

#include <vector>
#include <cmath>

/**
 * @struct PathMetrics
 * @brief Структура для хранения всех метрик пути
 */
struct PathMetrics {
    // Метрики оптимальности пути
    double path_length;                 ///< Длина найденного пути
    double optimality_coefficient;      ///< Коэффициент оптимальности
    double path_deviation;              ///< Отклонение от оптимального пути (%)
    double smoothness;                  ///< Гладкость пути
    double total_turn_angle;            ///< Суммарный угол поворотов
    
    // Метрики вычислительной эффективности
    int nodes_expanded;                 ///< Количество раскрытых узлов
    double search_efficiency;           ///< Эффективность поиска
    double branching_factor;            ///< Фактор ветвления
    
    // Метрики качества траектории
    double min_obstacle_distance;       ///< Минимальное расстояние до препятствий
    double avg_obstacle_distance;       ///< Среднее расстояние до препятствий
    double max_curvature;               ///< Максимальная кривизна пути
    
    // Статистические метрики
    double execution_time;              ///< Время выполнения (мс)
    bool success;                       ///< Успешность поиска
};

/**
 * @namespace metrics
 * @brief Пространство имен для функций вычисления метрик
 */
namespace metrics {

/**
 * @brief Вычислить все метрики для пути
 * @param path Найденный путь
 * @param grid Сетка
 * @param start_x Начальная X координата
 * @param start_y Начальная Y координата
 * @param end_x Конечная X координата
 * @param end_y Конечная Y координата
 * @param nodes_expanded Количество раскрытых узлов
 * @param execution_time Время выполнения
 * @return Структура с вычисленными метриками
 */
PathMetrics calculateAllMetrics(const std::vector<Node*>& path,
                               const Grid& grid,
                               int start_x, int start_y,
                               int end_x, int end_y,
                               int nodes_expanded,
                               double execution_time);

/**
 * @brief Вычислить длину пути
 * @param path Вектор узлов пути
 * @return Длина пути
 */
double calculatePathLength(const std::vector<Node*>& path);

/**
 * @brief Вычислить гладкость пути (суммарный угол поворотов)
 * @param path Вектор узлов пути
 * @return Суммарный угол поворотов в радианах
 */
double calculateSmoothness(const std::vector<Node*>& path);

/**
 * @brief Вычислить минимальное и среднее расстояние до препятствий
 * @param path Вектор узлов пути
 * @param grid Сетка
 * @return Пара: минимальное расстояние, среднее расстояние
 */
std::pair<double, double> calculateObstacleDistances(const std::vector<Node*>& path,
                                                    const Grid& grid);

/**
 * @brief Вычислить кривизну пути
 * @param path Вектор узлов пути
 * @return Максимальная кривизна пути
 */
double calculateMaxCurvature(const std::vector<Node*>& path);

/**
 * @brief Вычислить евклидово расстояние между двумя узлами
 * @param a Первый узел
 * @param b Второй узел
 * @return Евклидово расстояние
 */
double euclideanDistance(const Node& a, const Node& b);

/**
 * @brief Вычислить угол между тремя точками
 * @param a Первая точка
 * @param b Центральная точка
 * @param c Третья точка
 * @return Угол в радианах
 */
double calculateAngle(const Node& a, const Node& b, const Node& c);

} // namespace metrics

#endif // METRICS_CALCULATOR_H