/**
 * @file test_scenarios.cpp
 * @brief Реализация тестовых сценариев
 */

#include "scenarios/test_scenarios.h"
#include "config.h"

#include <fstream>
#include <filesystem>
#include <random>
#include <sys/stat.h>
#include <dirent.h>   
#include <iostream>

namespace scenarios {

// Вспомогательная функция для создания директории
bool createDirectory(const std::string& path) {
#ifdef _WIN32
    return _mkdir(path.c_str()) == 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

// Вспомогательная функция для проверки существования директории
bool directoryExists(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (dir) {
        closedir(dir);
        return true;
    }
    return false;
}

std::vector<TestScenario> createAllScenarios() {
    std::vector<TestScenario> scenarios;
    
    // Сценарий 1: Открытое пространство
    TestScenario open_space("open_space", config::GRID_WIDTH, config::GRID_HEIGHT);
    createOpenSpace(open_space.grid, open_space.start_x, open_space.start_y, 
                   open_space.end_x, open_space.end_y);
    scenarios.push_back(open_space);
    
    // Сценарий 2: Сложный лабиринт
    TestScenario maze("maze", config::GRID_WIDTH, config::GRID_HEIGHT);
    createComplexMaze(maze.grid, maze.start_x, maze.start_y, maze.end_x, maze.end_y);
    scenarios.push_back(maze);
    
    // Сценарий 3: Множество препятствий
    TestScenario obstacles("obstacles", config::GRID_WIDTH, config::GRID_HEIGHT);
    createManyObstacles(obstacles.grid, obstacles.start_x, obstacles.start_y,
                       obstacles.end_x, obstacles.end_y, config::OBSTACLE_DENSITY);
    scenarios.push_back(obstacles);
    
    // Сценарий 4: Узкие коридоры
    TestScenario corridors("narrow_corridors", config::GRID_WIDTH, config::GRID_HEIGHT);
    createNarrowCorridors(corridors.grid, corridors.start_x, corridors.start_y,
                         corridors.end_x, corridors.end_y, config::NARROW_CORRIDOR_WIDTH);
    scenarios.push_back(corridors);
    
    return scenarios;
}

void saveScenario(const TestScenario& scenario, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    
    // Сохраняем метаданные
    file << "NAME:" << scenario.name << "\n";
    file << "WIDTH:" << scenario.grid.getWidth() << "\n";
    file << "HEIGHT:" << scenario.grid.getHeight() << "\n";
    file << "START:" << scenario.start_x << "," << scenario.start_y << "\n";
    file << "END:" << scenario.end_x << "," << scenario.end_y << "\n";
    file << "GRID_DATA:\n";
    
    // Сохраняем сетку
    for (int y = 0; y < scenario.grid.getHeight(); ++y) {
        for (int x = 0; x < scenario.grid.getWidth(); ++x) {
            file << (scenario.grid.isObstacle(x, y) ? '1' : '0');
        }
        file << "\n";
    }
}

TestScenario loadScenario(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }
    
    std::string line;
    std::string name;
    int width = 0, height = 0;
    int start_x = 0, start_y = 0, end_x = 0, end_y = 0;
    
    // Читаем метаданные
    while (std::getline(file, line) && line != "GRID_DATA:") {
        if (line.find("NAME:") == 0) {
            name = line.substr(5);
        } else if (line.find("WIDTH:") == 0) {
            width = std::stoi(line.substr(6));
        } else if (line.find("HEIGHT:") == 0) {
            height = std::stoi(line.substr(7));
        } else if (line.find("START:") == 0) {
            size_t comma_pos = line.find(',', 6);
            start_x = std::stoi(line.substr(6, comma_pos - 6));
            start_y = std::stoi(line.substr(comma_pos + 1));
        } else if (line.find("END:") == 0) {
            size_t comma_pos = line.find(',', 4);
            end_x = std::stoi(line.substr(4, comma_pos - 4));
            end_y = std::stoi(line.substr(comma_pos + 1));
        }
    }
    
    // Создаем сценарий
    TestScenario scenario(name, width, height);
    scenario.start_x = start_x;
    scenario.start_y = start_y;
    scenario.end_x = end_x;
    scenario.end_y = end_y;
    
    // Читаем данные сетки
    int y = 0;
    while (std::getline(file, line) && y < height) {
        for (int x = 0; x < width && x < static_cast<int>(line.size()); ++x) {
            if (line[x] == '1') {
                scenario.grid.setObstacle(x, y);
            }
        }
        ++y;
    }
    
    return scenario;
}

void saveAllScenarios(const std::vector<TestScenario>& scenarios, 
                     const std::string& directory) {
    // Создаем директорию, если не существует
    if (!directoryExists(directory)) {
        if (!createDirectory(directory)) {
            std::cerr << "Warning: Cannot create directory " << directory << std::endl;
            return;
        }
    }
    
    for (const auto& scenario : scenarios) {
        std::string filename = directory + "/" + scenario.name + ".scenario";
        saveScenario(scenario, filename);
    }
}

std::vector<TestScenario> loadAllScenarios(const std::string& directory) {
    std::vector<TestScenario> scenarios;
    
    if (!directoryExists(directory)) {
        std::cout << "Directory " << directory << " does not exist. Creating default scenarios." << std::endl;
        auto default_scenarios = createAllScenarios();
        saveAllScenarios(default_scenarios, directory);
        return default_scenarios;
    }
    
    // Простая реализация - загружаем только известные сценарии
    std::vector<std::string> known_scenarios = {
        "open_space.scenario",
        "maze.scenario", 
        "obstacles.scenario",
        "narrow_corridors.scenario"
    };
    
    for (const auto& scenario_file : known_scenarios) {
        std::string filename = directory + "/" + scenario_file;
        try {
            scenarios.push_back(loadScenario(filename));
        } catch (const std::exception& e) {
            std::cerr << "Error loading scenario " << filename << ": " << e.what() << std::endl;
        }
    }
    
    return scenarios;
}

void exportForPython(const TestScenario& scenario, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    
    // Экспортируем в простом формате для Python
    file << "# Test scenario: " << scenario.name << "\n";
    file << "width = " << scenario.grid.getWidth() << "\n";
    file << "height = " << scenario.grid.getHeight() << "\n";
    file << "start = (" << scenario.start_x << ", " << scenario.start_y << ")\n";
    file << "end = (" << scenario.end_x << ", " << scenario.end_y << ")\n";
    file << "obstacles = [\n";
    
    // Сохраняем препятствия как список координат
    for (int y = 0; y < scenario.grid.getHeight(); ++y) {
        for (int x = 0; x < scenario.grid.getWidth(); ++x) {
            if (scenario.grid.isObstacle(x, y)) {
                file << "    (" << x << ", " << y << "),\n";
            }
        }
    }
    file << "]\n";
}





bool isPathPossible(const Grid& grid, int start_x, int start_y, int end_x, int end_y) {
    // Простая проверка - старт и финиш должны быть доступны
    if (!grid.isValidCoordinate(start_x, start_y) || grid.isObstacle(start_x, start_y)) {
        return false;
    }
    if (!grid.isValidCoordinate(end_x, end_y) || grid.isObstacle(end_x, end_y)) {
        return false;
    }
    return true;
}

std::string getScenarioName(int index) {
    switch (index) {
        case 0: return "open_space";
        case 1: return "maze";
        case 2: return "obstacles";
        case 3: return "narrow_corridors";
        default: return "unknown";
    }
}

/**
 * @brief Инициализировать и сохранить все сценарии один раз
 * @param force_recreate Принудительно пересоздать сценарии
 */
void initializeScenarios(bool force_recreate) {
    const std::string scenario_dir = "test_scenarios";
    
    if (force_recreate || !directoryExists(scenario_dir)) {
        std::cout << "Creating test scenarios..." << std::endl;
        auto scenarios = createAllScenarios();
        saveAllScenarios(scenarios, scenario_dir);
        
        // Также экспортируем для Python визуализации
        std::string python_dir = "python_visualization";
        createDirectory(python_dir);
        for (const auto& scenario : scenarios) {
            std::string python_file = python_dir + "/" + scenario.name + ".py";
            exportForPython(scenario, python_file);
        }
        std::cout << "Scenarios created and saved to " << scenario_dir << std::endl;
    } else {
        std::cout << "Using existing scenarios from " << scenario_dir << std::endl;
    }
}

/**
 * @brief Загрузить все сценарии (создать если не существуют)
 * @return Вектор загруженных сценариев
 */
std::vector<TestScenario> getScenarios() {
    initializeScenarios(false); // Создает если не существуют
    return loadAllScenarios("test_scenarios");
}

} // namespace scenarios
