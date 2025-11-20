import matplotlib.pyplot as plt
import numpy as np
from matplotlib import colors
import os
import sys

def create_directories():
    """Создает необходимые директории если их нет"""
    os.makedirs('python_visualization', exist_ok=True)
    os.makedirs('results/plots', exist_ok=True)

def load_maze_data(file_path):
    """
    Загружает данные лабиринта из файла
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Выполняем код из файла в отдельном пространстве имен
        maze_namespace = {}
        exec(content, maze_namespace)
        
        return {
            'width': maze_namespace['width'],
            'height': maze_namespace['height'],
            'start': maze_namespace['start'],
            'end': maze_namespace['end'],
            'obstacles': maze_namespace['obstacles']
        }
    except Exception as e:
        print(f"Ошибка при загрузке файла {file_path}: {e}")
        return None

def create_maze_grid(maze_data):
    """Создает сетку лабиринта из данных"""
    width = maze_data['width']
    height = maze_data['height']
    obstacles = maze_data['obstacles']
    
    maze_grid = np.zeros((height, width))
    
    for x, y in obstacles:
        if 0 <= x < width and 0 <= y < height:
            maze_grid[y, x] = 1  # 1 - препятствие
    
    return maze_grid

def visualize_all_mazes():
    """
    Основная функция для визуализации всех лабиринтов
    """
    # Список файлов с лабиринтами
    maze_files = [
        'python_visualization/maze.py',
        'python_visualization/narrow_corridors.py', 
        'python_visualization/obstacles.py',
        'python_visualization/open_space.py'
    ]
    
    titles = [
        'Complex Maze',
        'Narrow Corridors', 
        'Obstacles Field',
        'Open Space'
    ]
    
    # Проверяем существование файлов
    missing_files = []
    for file_path in maze_files:
        if not os.path.exists(file_path):
            missing_files.append(file_path)
    
    if missing_files:
        print("ВНИМАНИЕ: Не найдены следующие файлы:")
        for file in missing_files:
            print(f"  - {file}")
        print("\nУбедитесь, что файлы находятся в папке 'python_visualization/'")
        return
    
    # Загружаем данные всех лабиринтов
    mazes_data = []
    valid_files = []
    valid_titles = []
    
    for file_path, title in zip(maze_files, titles):
        maze_data = load_maze_data(file_path)
        if maze_data is not None:
            mazes_data.append(maze_data)
            valid_files.append(file_path)
            valid_titles.append(title)
    
    if not mazes_data:
        print("Не удалось загрузить ни один лабиринт!")
        return
    
    # Создаем визуализацию
    n_mazes = len(mazes_data)
    
    # Определяем размер сетки в зависимости от количества лабиринтов
    if n_mazes <= 2:
        fig, axes = plt.subplots(1, n_mazes, figsize=(6*n_mazes, 6))
        if n_mazes == 1:
            axes = [axes]
    else:
        fig, axes = plt.subplots(2, 2, figsize=(16, 12))
        axes = axes.flatten()
    
    # Цветовая схема
    cmap = colors.ListedColormap(['white', 'black', 'green', 'red'])
    bounds = [0, 0.5, 1.5, 2.5, 3.5]
    norm = colors.BoundaryNorm(bounds, cmap.N)
    
    maze_stats = []
    
    for i, (maze_data, title) in enumerate(zip(mazes_data, valid_titles)):
        if i >= len(axes):  # На случай если осей меньше чем лабиринтов
            break
            
        maze_grid = create_maze_grid(maze_data)
        
        # Добавляем стартовую и конечную точки
        visual_maze = maze_grid.copy()
        start_x, start_y = maze_data['start']
        end_x, end_y = maze_data['end']
        visual_maze[start_y, start_x] = 2  # Старт
        visual_maze[end_y, end_x] = 3      # Финиш
        
        # Визуализируем
        im = axes[i].imshow(visual_maze, cmap=cmap, norm=norm, origin='lower')
        axes[i].set_title(title, fontsize=14, fontweight='bold')
        axes[i].set_xlabel('X Coordinate')
        axes[i].set_ylabel('Y Coordinate')
        axes[i].grid(True, alpha=0.3)
        
        # Добавляем аннотации
        axes[i].plot(start_x, start_y, 'go', markersize=8, label='Start')
        axes[i].plot(end_x, end_y, 'ro', markersize=8, label='End')
        axes[i].legend()
        
        # Собираем статистику
        total_cells = maze_data['width'] * maze_data['height']
        obstacle_cells = len(maze_data['obstacles'])
        free_cells = total_cells - obstacle_cells
        density = obstacle_cells / total_cells * 100
        
        maze_stats.append({
            'title': title,
            'size': f"{maze_data['width']}x{maze_data['height']}",
            'start': maze_data['start'],
            'end': maze_data['end'],
            'obstacles': obstacle_cells,
            'free': free_cells,
            'density': density
        })
    
    # Удаляем пустые subplot'ы
    for i in range(len(mazes_data), len(axes)):
        fig.delaxes(axes[i])
    
    # Добавляем общую цветовую легенду
    if len(mazes_data) > 1:
        cbar = fig.colorbar(im, ax=axes, shrink=0.8)
        cbar.set_ticks([0.25, 1.0, 2.0, 3.0])
        cbar.set_ticklabels(['Free', 'Obstacle', 'Start', 'End'])
    
    plt.tight_layout()
    
    # Сохраняем график
    output_path = 'results/plots/all_mazes_comparison.png'
    plt.savefig(output_path, dpi=300, bbox_inches='tight')
    print(f"График сохранен как: {output_path}")
    
    plt.show()
    
    # Выводим статистику
    print("\n" + "="*50)
    print("MAZE STATISTICS")
    print("="*50)
    for stat in maze_stats:
        print(f"\n📊 {stat['title']}:")
        print(f"   📏 Size: {stat['size']}")
        print(f"   🎯 Start: {stat['start']}, End: {stat['end']}")
        print(f"   ⛰️  Obstacles: {stat['obstacles']:,}")
        print(f"   🟢 Free cells: {stat['free']:,}")
        print(f"   📈 Obstacle density: {stat['density']:.1f}%")

def create_individual_plots():
    """
    Создает отдельные графики для каждого лабиринта
    """
    maze_files = [
        'python_visualization/maze.py',
        'python_visualization/narrow_corridors.py',
        'python_visualization/obstacles.py', 
        'python_visualization/open_space.py'
    ]
    
    titles = [
        'Complex Maze',
        'Narrow Corridors',
        'Obstacles Field',
        'Open Space'
    ]
    
    for file_path, title in zip(maze_files, titles):
        if not os.path.exists(file_path):
            print(f"Файл не найден: {file_path}")
            continue
            
        maze_data = load_maze_data(file_path)
        if maze_data is None:
            continue
            
        # Создаем отдельный график для каждого лабиринта
        fig, ax = plt.subplots(figsize=(10, 10))
        
        maze_grid = create_maze_grid(maze_data)
        
        # Цветовая схема
        cmap = colors.ListedColormap(['white', 'black', 'green', 'red'])
        bounds = [0, 0.5, 1.5, 2.5, 3.5]
        norm = colors.BoundaryNorm(bounds, cmap.N)
        
        # Добавляем точки
        visual_maze = maze_grid.copy()
        start_x, start_y = maze_data['start']
        end_x, end_y = maze_data['end']
        visual_maze[start_y, start_x] = 2
        visual_maze[end_y, end_x] = 3
        
        # Визуализируем
        im = ax.imshow(visual_maze, cmap=cmap, norm=norm, origin='lower')
        ax.set_title(title, fontsize=16, fontweight='bold')
        ax.set_xlabel('X Coordinate')
        ax.set_ylabel('Y Coordinate')
        ax.grid(True, alpha=0.3)
        
        # Аннотации
        ax.plot(start_x, start_y, 'go', markersize=10, label='Start')
        ax.plot(end_x, end_y, 'ro', markersize=10, label='End')
        ax.legend(fontsize=12)
        
        # Цветовая легенда
        cbar = fig.colorbar(im, ax=ax, shrink=0.8)
        cbar.set_ticks([0.25, 1.0, 2.0, 3.0])
        cbar.set_ticklabels(['Free', 'Obstacle', 'Start', 'End'])
        
        plt.tight_layout()
        
        # Сохраняем индивидуальный график
        filename = title.lower().replace(' ', '_')
        output_path = f'results/plots/{filename}.png'
        plt.savefig(output_path, dpi=300, bbox_inches='tight')
        print(f"Индивидуальный график сохранен: {output_path}")
        
        plt.close()  # Закрываем график чтобы освободить память

if __name__ == "__main__":
    # Создаем директории
    create_directories()
    
    print("🚀 Starting maze visualization...")
    print("📁 Looking for maze files in 'python_visualization/' folder")
    
    # Создаем комбинированный график
    visualize_all_mazes()
    
    # Создаем индивидуальные графики
    print("\n📊 Creating individual plots...")
    create_individual_plots()
    
    print("\n✅ All visualizations completed!")
    print("📂 Check the 'results/plots/' folder for output files")