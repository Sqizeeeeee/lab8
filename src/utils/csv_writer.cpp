/**
 * @file csv_writer.cpp
 * @brief Реализация класса для записи CSV файлов
 */

#include "utils/csv_writer.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <chrono>
#include <ctime>

CSVWriter::CSVWriter(const std::string& output_dir) 
    : output_dir_(output_dir) {
    openFiles();
}

CSVWriter::~CSVWriter() {
    closeFiles();
}

void CSVWriter::openFiles() {
    // Создаем директорию, если она не существует
    // В реальной реализации нужно добавить создание директорий
    
    std::string detailed_filename = output_dir_ + "/" + createTimestampedFilename("detailed_results");
    std::string summary_filename = output_dir_ + "/" + createTimestampedFilename("summary_table");
    
    detailed_file_.open(detailed_filename);
    summary_file_.open(summary_filename);
    
    if (detailed_file_.is_open()) {
        writeCSVHeader(detailed_file_);
    }
    
    if (summary_file_.is_open()) {
        summary_file_ << "Algorithm,Scenario,SuccessRate,AvgPathLength,AvgTime,AvgNodesExpanded\n";
    }
}

void CSVWriter::closeFiles() {
    if (detailed_file_.is_open()) {
        detailed_file_.close();
    }
    if (summary_file_.is_open()) {
        summary_file_.close();
    }
}

void CSVWriter::writeResult(const AlgorithmResult& result) {
    if (detailed_file_.is_open()) {
        writeCSVRow(detailed_file_, result);
        detailed_file_.flush();
    }
}

void CSVWriter::writeResults(const std::vector<AlgorithmResult>& results) {
    for (const auto& result : results) {
        writeResult(result);
    }
}

void CSVWriter::createSummaryTable(const std::vector<AlgorithmResult>& results) {
    if (!summary_file_.is_open()) return;
    
    // Группируем результаты по алгоритмам и сценариям
    std::map<std::string, std::map<std::string, std::vector<PathMetrics>>> grouped;
    
    for (const auto& result : results) {
        grouped[result.algorithm_name][result.test_scenario].push_back(result.metrics);
    }
    
    // Записываем сводную статистику
    summary_file_ << "\nSummary Statistics:\n";
    summary_file_ << "Algorithm,Scenario,SuccessRate(%),AvgPathLength,AvgTime(ms),AvgNodesExpanded,AvgSmoothness\n";
    
    for (const auto& algo_pair : grouped) {
        const std::string& algorithm = algo_pair.first;
        
        for (const auto& scenario_pair : algo_pair.second) {
            const std::string& scenario = scenario_pair.first;
            const auto& metrics_list = scenario_pair.second;
            
            if (metrics_list.empty()) continue;
            
            // Вычисляем средние значения
            double success_count = 0;
            double total_length = 0;
            double total_time = 0;
            double total_nodes = 0;
            double total_smoothness = 0;
            
            for (const auto& metrics : metrics_list) {
                if (metrics.success) success_count++;
                total_length += metrics.path_length;
                total_time += metrics.execution_time;
                total_nodes += metrics.nodes_expanded;
                total_smoothness += metrics.smoothness;
            }
            
            double success_rate = (metrics_list.size() > 0) ? (success_count / metrics_list.size()) * 100.0 : 0.0;
            double avg_length = (metrics_list.size() > 0) ? total_length / metrics_list.size() : 0.0;
            double avg_time = (metrics_list.size() > 0) ? total_time / metrics_list.size() : 0.0;
            double avg_nodes = (metrics_list.size() > 0) ? total_nodes / metrics_list.size() : 0.0;
            double avg_smoothness = (metrics_list.size() > 0) ? total_smoothness / metrics_list.size() : 0.0;
            
            summary_file_ << algorithm << ","
                         << scenario << ","
                         << std::fixed << std::setprecision(2) << success_rate << ","
                         << std::setprecision(4) << avg_length << ","
                         << std::setprecision(2) << avg_time << ","
                         << std::setprecision(1) << avg_nodes << ","
                         << std::setprecision(4) << avg_smoothness << "\n";
        }
    }
}

void CSVWriter::setOutputDirectory(const std::string& output_dir) {
    if (output_dir != output_dir_) {
        closeFiles();
        output_dir_ = output_dir;
        openFiles();
    }
}

void CSVWriter::writeCSVHeader(std::ofstream& file) {
    file << "Timestamp,Algorithm,Scenario,Success,PathLength,OptimalityCoefficient,"
         << "PathDeviation(%),Smoothness,TotalTurnAngle,NodesExpanded,"
         << "SearchEfficiency,BranchingFactor,MinObstacleDistance,"
         << "AvgObstacleDistance,MaxCurvature,ExecutionTime(ms)\n";
}

void CSVWriter::writeCSVRow(std::ofstream& file, const AlgorithmResult& result) {
    // Текущее время
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    
    file << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << ","
         << result.algorithm_name << ","
         << result.test_scenario << ","
         << (result.metrics.success ? "true" : "false") << ","
         << std::fixed << std::setprecision(4) << result.metrics.path_length << ","
         << std::setprecision(4) << result.metrics.optimality_coefficient << ","
         << std::setprecision(2) << result.metrics.path_deviation << ","
         << std::setprecision(4) << result.metrics.smoothness << ","
         << std::setprecision(4) << result.metrics.total_turn_angle << ","
         << result.metrics.nodes_expanded << ","
         << std::setprecision(4) << result.metrics.search_efficiency << ","
         << std::setprecision(4) << result.metrics.branching_factor << ","
         << std::setprecision(4) << result.metrics.min_obstacle_distance << ","
         << std::setprecision(4) << result.metrics.avg_obstacle_distance << ","
         << std::setprecision(4) << result.metrics.max_curvature << ","
         << std::setprecision(2) << result.metrics.execution_time << "\n";
}

std::string CSVWriter::createTimestampedFilename(const std::string& base_name) {
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    
    std::ostringstream oss;
    oss << base_name << "_" 
        << std::put_time(&tm, "%Y%m%d_%H%M%S") << ".csv";
    
    return oss.str();
}