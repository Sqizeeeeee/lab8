/**
 * @file astar.cpp
 * @brief Реализация алгоритма A*
 */

#include "algorithms/astar.h"

#include <stdexcept>
#include <cmath>
#include <algorithm>

AStar::AStar(Grid& grid) 
    : grid_(grid), nodes_expanded_(0), path_length_(0.0) {}

std::vector<Node*> AStar::findPath(int start_x, int start_y, int end_x, int end_y) {
    
    resetStatistics();
    grid_.resetSearchData();
    
    
    if (!grid_.isValidCoordinate(start_x, start_y) || !grid_.isValidCoordinate(end_x, end_y)) {
        throw std::runtime_error("Invalid start or end coordinates");
    }
    
    
    Node& start_node = grid_.getNode(start_x, start_y);
    Node& end_node = grid_.getNode(end_x, end_y);
    
    if (!start_node.walkable) {
        throw std::runtime_error("Start node is not walkable");
    }
    
    if (!end_node.walkable) {
        throw std::runtime_error("End node is not walkable");
    }
    
    
    start_node.g_cost = 0.0;
    start_node.h_cost = start_node.calculateHeuristic(end_node);
    start_node.f_cost = start_node.g_cost + start_node.h_cost;
    
    // Приоритетная очередь для открытых узлов
    PriorityQueue open_set;
    open_set.push(&start_node);
    
    // Множества для отслеживания состояний узлов
    NodeSet open_set_members;
    NodeSet closed_set;
    
    open_set_members.insert(&start_node);
    
    while (!open_set.empty()) {
        // Извлекаем узел с наименьшей f_cost
        Node* current_node = open_set.top();
        open_set.pop();
        open_set_members.erase(current_node);
        
        // Если достигли цели, восстанавливаем путь
        if (current_node->x == end_x && current_node->y == end_y) {
            auto path = reconstructPath(current_node);
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
            
            // Вычисляем новую стоимость пути до соседа
            double tentative_g_cost = current_node->g_cost + current_node->calculateMoveCost(*neighbor);
            
            // Проверяем, является ли этот путь лучше
            bool is_better_path = false;
            
            if (open_set_members.find(neighbor) == open_set_members.end()) {
                // Сосед еще не в открытом множестве
                open_set_members.insert(neighbor);
                is_better_path = true;
            } else if (tentative_g_cost < neighbor->g_cost) {
                // Нашли более короткий путь к уже открытому узлу
                is_better_path = true;
            }
            
            if (is_better_path) {
                // Обновляем параметры соседа
                neighbor->parent = current_node;
                neighbor->g_cost = tentative_g_cost;
                neighbor->h_cost = neighbor->calculateHeuristic(end_node);
                neighbor->f_cost = neighbor->g_cost + config::HEURISTIC_WEIGHT * neighbor->h_cost;
                
                open_set.push(neighbor);
            }
        }
        
        // Защита от бесконечного цикла
        if (nodes_expanded_ > config::MAX_PATHFINDING_ITERATIONS) {
            throw std::runtime_error("Pathfinding exceeded maximum iterations");
        }
    }
    
    throw std::runtime_error("Path not found");
}

std::vector<Node*> AStar::reconstructPath(Node* end_node) {
    std::vector<Node*> path;
    Node* current_node = end_node;
    
    while (current_node != nullptr) {
        path.push_back(current_node);
        current_node = current_node->parent;
    }
    
    // Переворачиваем путь, чтобы он шел от начала к концу
    std::reverse(path.begin(), path.end());
    return path;
}

double AStar::calculatePathLength(const std::vector<Node*>& path) const {
    if (path.size() < 2) {
        return 0.0;
    }
    
    double length = 0.0;
    for (size_t i = 1; i < path.size(); ++i) {
        length += path[i-1]->calculateMoveCost(*path[i]);
    }
    return length;
}

void AStar::resetStatistics() {
    nodes_expanded_ = 0;
    path_length_ = 0.0;
}