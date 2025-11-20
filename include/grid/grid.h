/**
 * @file grid.h
 * @brief Класс сетки для представления карты строительной площадки
 * 
 * Содержит двумерную сетку узлов, методы для работы с препятствиями
 * и инфляции препятствий с учетом радиуса трубы
 */

#ifndef GRID_H
#define GRID_H

#include "../../config.h"
#include "node.h"

#include <vector>
#include <memory>
#include <stdexcept>

/**
 * @class Grid
 * @brief Класс для представления карты в виде сетки узлов
 */
class Grid {
public:
    /**
     * @brief Конструктор сетки
     * @param width Ширина сетки
     * @param height Высота сетки
     */
    Grid(int width = config::GRID_WIDTH, int height = config::GRID_HEIGHT);
    
    /**
     * @brief Деструктор
     */
    ~Grid() = default;
    
    /**
     * @brief Получить узел по координатам
     * @param x Координата X
     * @param y Координата Y
     * @return Ссылка на узел
     * @throw std::out_of_range если координаты вне сетки
     */
    Node& getNode(int x, int y);
    
    /**
     * @brief Получить узел по координатам (константная версия)
     * @param x Координата X
     * @param y Координата Y
     * @return Константная ссылка на узел
     * @throw std::out_of_range если координаты вне сетки
     */
    const Node& getNode(int x, int y) const;
    
    /**
     * @brief Проверить валидность координат
     * @param x Координата X
     * @param y Координата Y
     * @return true если координаты в пределах сетки
     */
    bool isValidCoordinate(int x, int y) const;
    
    /**
     * @brief Установить препятствие в указанной клетке
     * @param x Координата X
     * @param y Координата Y
     */
    void setObstacle(int x, int y);
    
    /**
     * @brief Убрать препятствие из указанной клетки
     * @param x Координата X
     * @param y Координата Y
     */
    void clearObstacle(int x, int y);
    
    /**
     * @brief Проверить, является ли клетка препятствием
     * @param x Координата X
     * @param y Координата Y
     * @return true если клетка является препятствием
     */
    bool isObstacle(int x, int y) const;
    
    /**
     * @brief Получить ширину сетки
     * @return Ширина сетки
     */
    int getWidth() const { return width_; }
    
    /**
     * @brief Получить высоту сетки
     * @return Высота сетки
     */
    int getHeight() const { return height_; }
    
    /**
     * @brief "Раздуть" препятствия с учетом радиуса агента
     * @param agent_radius Радиус агента (трубы)
     */
    void inflateObstacles(double agent_radius = config::AGENT_RADIUS);
    
    /**
     * @brief Сбросить все параметры поиска во всех узлах
     */
    void resetSearchData();
    
    /**
     * @brief Получить соседей узла
     * @param node Узел, для которого ищутся соседи
     * @return Вектор указателей на соседние узлы
     */
    std::vector<Node*> getNeighbors(const Node& node);

private:
    int width_;                                     ///< Ширина сетки
    int height_;                                    ///< Высота сетки
    std::vector<std::vector<Node>> nodes_;          ///< Двумерный массив узлов
    
    /**
     * @brief Инициализировать сетку
     */
    void initializeGrid();
};

#endif // GRID_H