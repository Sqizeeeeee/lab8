/**
 * @file config.h
 * @brief Конфигурационные параметры для проекта сравнения алгоритмов поиска пути
 * 
 * Этот файл содержит все основные настройки и константы, используемые
 * в алгоритмах поиска пути и их тестировании.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>

/**
 * @namespace config
 * @brief Пространство имен для конфигурационных параметров
 */
namespace config {

/**
 * @defgroup GridSettings Настройки сетки
 * @{
 */
constexpr int GRID_WIDTH = 100;          ///< Ширина сетки в клетках
constexpr int GRID_HEIGHT = 100;         ///< Высота сетки в клетках
constexpr int CELL_SIZE = 1;             ///< Размер клетки в условных единицах
/** @} */

/**
 * @defgroup AgentSettings Настройки агента (трубы)
 * @{
 */
constexpr double AGENT_RADIUS = 0.1;     ///< Радиус трубы в условных единицах
constexpr double SAFETY_MARGIN = 0.0;    ///< Дополнительный запас безопасности
/** @} */

/**
 * @defgroup AlgorithmSettings Настройки алгоритмов
 * @{
 */
constexpr bool ALLOW_DIAGONAL_MOVEMENT = true;  ///< Разрешить диагональное перемещение
constexpr double DIAGONAL_COST = M_SQRT2;       ///< Стоимость диагонального перемещения
constexpr double HEURISTIC_WEIGHT = 1.0;        ///< Вес эвристики в A*
/** @} */

/**
 * @defgroup TestingSettings Настройки тестирования
 * @{
 */
constexpr int NUM_TEST_RUNS = 10;        ///< Количество запусков для каждого теста
constexpr int MAX_PATHFINDING_ITERATIONS = 100000; ///< Максимальное число итераций поиска
/** @} */

/**
 * @defgroup MetricsSettings Настройки метрик
 * @{
 */
constexpr double ANGLE_EPSILON = 1e-5;   ///< Точность для вычисления углов
constexpr double MIN_OBSTACLE_DISTANCE = 0.1; ///< Минимальное расстояние до препятствия
/** @} */

/**
 * @defgroup ScenarioSettings Настройки сценариев
 * @{
 */
constexpr int NARROW_CORRIDOR_WIDTH = 3; ///< Ширина узкого коридора в клетках
constexpr int OBSTACLE_DENSITY = 30;     ///< Плотность препятствий в процентах
/** @} */

} // namespace config

#endif // CONFIG_H