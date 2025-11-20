/**
 * @file thetastar.cpp
 * @brief Реализация алгоритма Theta*
 */

#include "algorithms/thetastar.h"

#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <iostream>

ThetaStar::ThetaStar(Grid& grid) 
    : grid_(grid), nodes_expanded_(0), path_length_(0.0) {}

std::vector<Node*> ThetaStar::findPath(int start_x, int start_y, int end_x, int end_y) {
    // Сброс статистики и данных поиска
    resetStatistics();
    grid_.resetSearchData();
    
    // Проверка валидности координат
    if (!grid_.isValidCoordinate(start_x, start_y) || !grid_.isValidCoordinate(end_x, end_y)) {
        throw std::runtime_error("Invalid start or end coordinates");
    }
    
    // Проверка, что начальная и конечная точки доступны
    Node& start_node = grid_.getNode(start_x, start_y);
    Node& end_node = grid_.getNode(end_x, end_y);
    
    if (!start_node.walkable) {
        throw std::runtime_error("Start node is not walkable");
    }
    
    if (!end_node.walkable) {
        throw std::runtime_error("End node is not walkable");
    }
    
    // Инициализация начального узла
    start_node.g_cost = 0.0;
    start_node.h_cost = start_node.calculateHeuristic(end_node);
    start_node.f_cost = start_node.g_cost + start_node.h_cost;
    start_node.parent = nullptr; // Старт не имеет родителя
    
    // Приоритетная очередь для открытых узлов
    PriorityQueue open_set;
    open_set.push(&start_node);
    
    // Множества для отслеживания состояний узлов
    NodeSet open_set_members;
    NodeSet closed_set;
    
    open_set_members.insert(&start_node);
    
    int safety_counter = 0;
    const int MAX_ITERATIONS = config::MAX_PATHFINDING_ITERATIONS;
    
    while (!open_set.empty() && safety_counter < MAX_ITERATIONS) {
        safety_counter++;
        
        // Извлекаем узел с наименьшей f_cost
        Node* current_node = open_set.top();
        open_set.pop();
        open_set_members.erase(current_node);
        
        // Если достигли цели, восстанавливаем путь
        if (current_node->x == end_x && current_node->y == end_y) {
            auto path = reconstructPath(current_node);
            if (path.empty() || path.size() < 2) {
                throw std::runtime_error("Invalid path found");
            }
            path_length_ = calculatePathLength(path);
            return path;
        }
        
        closed_set.insert(current_node);
        nodes_expanded_++;
        
        // Проверяем всех соседей
        auto neighbors = grid_.getNeighbors(*current_node);
        for (Node* neighbor : neighbors) {
            // Пропускаем уже обработанные узлы
            if (closed_set.find(neighbor) != closed_set.end()) {
                continue;
            }
            
            // Theta*: специальная процедура обновления вершины
            updateVertex(current_node, neighbor, &end_node);
            
            // Если сосед не в открытом множестве, добавляем его
            if (open_set_members.find(neighbor) == open_set_members.end()) {
                // Пересчитываем эвристику для точности
                neighbor->h_cost = neighbor->calculateHeuristic(end_node);
                neighbor->f_cost = neighbor->g_cost + config::HEURISTIC_WEIGHT * neighbor->h_cost;
                open_set_members.insert(neighbor);
                open_set.push(neighbor);
            }
        }
    }
    
    if (safety_counter >= MAX_ITERATIONS) {
        throw std::runtime_error("Pathfinding exceeded maximum iterations");
    }
    
    throw std::runtime_error("Path not found");
}

void ThetaStar::updateVertex(Node* current, Node* neighbor, Node* end_node) {
    // Проблема: вы сначала вычисляете standard_cost, но потом не всегда используете его
    
    // Theta* должен сначала проверять прямую видимость от родителя
    if (current->parent != nullptr) {
        if (line_of_sight::hasLineOfSight(grid_, *current->parent, *neighbor)) {
            double direct_cost = current->parent->g_cost + 
                               current->parent->calculateMoveCost(*neighbor);
            
            if (direct_cost < neighbor->g_cost) {
                neighbor->g_cost = direct_cost;
                neighbor->parent = current->parent;
                return; // Важно: выходим здесь
            }
        }
    }
    
    // Только если прямой путь не сработал, используем стандартный A* подход
    double standard_cost = current->g_cost + current->calculateMoveCost(*neighbor);
    if (standard_cost < neighbor->g_cost || neighbor->parent == nullptr) {
        neighbor->g_cost = standard_cost;
        neighbor->parent = current;
    }
}

std::vector<Node*> ThetaStar::reconstructPath(Node* end_node) {
    std::vector<Node*> path;
    Node* current_node = end_node;
    int max_path_length = grid_.getWidth() * grid_.getHeight(); // Максимальная возможная длина
    
    // Отладочная информация
    std::cout << "Theta* reconstructing path from (" << end_node->x << "," << end_node->y << ")" << std::endl;
    
    int step = 0;
    while (current_node != nullptr && step < max_path_length) {
        path.push_back(current_node);
        std::cout << "Step " << step << ": (" << current_node->x << "," << current_node->y << ")" << std::endl;
        
        // Проверяем цикличность
        if (current_node->parent == current_node) {
            std::cout << "Cycle detected at (" << current_node->x << "," << current_node->y << ")" << std::endl;
            break;
        }
        
        current_node = current_node->parent;
        step++;
    }
    
    if (step >= max_path_length) {
        std::cout << "Path reconstruction too long, possible cycle" << std::endl;
        return std::vector<Node*>(); // Возвращаем пустой путь
    }
    
    std::cout << "Path reconstructed with " << path.size() << " nodes" << std::endl;
    std::reverse(path.begin(), path.end());
    return path;
}

double ThetaStar::calculatePathLength(const std::vector<Node*>& path) const {
    if (path.size() < 2) {
        std::cout << "Warning: Path too short, length = 0" << std::endl;
        return 0.0;
    }
    
    double length = 0.0;
    for (size_t i = 1; i < path.size(); ++i) {
        Node* from = path[i-1];
        Node* to = path[i];
        
        double dx = static_cast<double>(to->x - from->x);
        double dy = static_cast<double>(to->y - from->y);
        double segment_length = std::sqrt(dx * dx + dy * dy);
        length += segment_length;
        
        std::cout << "Segment " << i-1 << "-" << i << ": (" << from->x << "," << from->y 
                  << ") to (" << to->x << "," << to->y << ") = " << segment_length << std::endl;
    }
    
    std::cout << "Total path length: " << length << std::endl;
    return length;
}

void ThetaStar::resetStatistics() {
    nodes_expanded_ = 0;
    path_length_ = 0.0;
}