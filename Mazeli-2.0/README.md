# Mazeli 2.0 - Interactive Maze Solver

A powerful C++ GUI application that visualizes various pathfinding algorithms on customizable mazes. Built with wxWidgets for cross-platform compatibility.

## 🎯 Features

### Core Functionality
- **Interactive Maze Creation**: Draw walls by clicking and dragging with mouse
- **Multiple Pathfinding Algorithms**:
  - Breadth-First Search (BFS)
  - Depth-First Search (DFS)
  - A* (A-Star) with consistent heuristic
  - Greedy Best-First Search
  - Bidirectional BFS
- **Real-time Visualization**: Watch algorithms explore the maze step-by-step
- **Dynamic Start/Goal Positioning**: Click to place start and goal points anywhere
- **Random Maze Generation**: Generate complex mazes automatically
- **Performance Metrics**: Track nodes explored and solution path length

### User Interface
- **Scrollable Grid**: Navigate large mazes with scroll bars
- **Control Panel**: Easy-to-use buttons and algorithm selection
- **Color-coded Visualization**:
  - Yellow: Start point
  - Green: Goal point
  - Black: Obstacles/walls
  - Blue gradient: Search path (darker = explored later)
  - Orange: Final solution path
- **Real-time Statistics**: Monitor algorithm performance

## 🛠️ Technical Specifications

### Dependencies
- **wxWidgets 3.x**: Cross-platform GUI toolkit
- **C++11** or later
- **Visual Studio 2019/2022** (Windows)

### Build Requirements
- Windows 10/11 (primary target)
- Visual Studio with C++ development tools
- wxWidgets library properly configured

### Project Structure
```
Mazeli-2.0/
├── mainApp.cpp/.h          # Application entry point and main class
├── maze.cpp/.h             # Core maze logic and GUI components
├── mazeSolverAlgo.cpp      # Pathfinding algorithm implementations
├── Mazeli-2.0.vcxproj      # Visual Studio project file
└── .gitignore              # Git ignore rules
```

## 🚀 Getting Started

### Prerequisites
1. Install Visual Studio 2019 or later with C++ development workload
2. Download and build wxWidgets:
   ```bash
   # Set environment variable WXWIN to wxWidgets installation path
   set WXWIN=C:\path\to\wxwidgets
   ```

### Building
1. Clone the repository:
   ```bash
   git clone <repository-url>
   cd Mazeli-2.0
   ```

2. Open `Mazeli-2.0.sln` in Visual Studio

3. Configure wxWidgets paths:
   - Ensure `$(WXWIN)` environment variable is set
   - Verify include and library paths in project properties

4. Build the solution (Ctrl+Shift+B)

### Running
- Execute the compiled binary from Debug/Release folder
- Or run directly from Visual Studio (F5)

## 📖 How to Use

### Basic Operations
1. **Drawing Walls**: Left-click and drag to create obstacles
2. **Erasing Walls**: Right-click and drag to remove obstacles
3. **Setting Start Point**: Click "Set Start" button, then click on grid
4. **Setting Goal Point**: Click "Set Goal" button, then click on grid
5. **Running Algorithm**: Select algorithm from dropdown, click "Start Simulation"

### Algorithm Selection
Use the dropdown menu to choose from:
- **BFS**: Guarantees shortest path, explores uniformly
- **DFS**: Memory efficient but may not find shortest path
- **A***: Optimal pathfinding with heuristic guidance
- **Greedy Best-First**: Fast but not always optimal
- **Bidirectional BFS**: Searches from both ends simultaneously

### Controls
- **Clear All**: Remove all walls and search results
- **Clear Search**: Keep walls but clear search visualization
- **Generate Random Maze**: Create complex maze automatically
- **Stop Simulation**: Halt running algorithm
- **Help**: View detailed usage instructions

## 🎮 Keyboard & Mouse Controls

| Action | Control |
|--------|---------|
| Draw walls | Left mouse button + drag |
| Erase walls | Right mouse button + drag |
| Place start/goal | Click after selecting respective button |
| Scroll maze | Mouse wheel or scroll bars |
| Stop algorithm | Click "Stop Simulation" button |

## 🔧 Configuration

### Grid Settings (modifiable in code)
- **Grid Size**: 60×35 cells (adjustable)
- **Cell Size**: 20×20 pixels
- **Window Size**: 1000×1000 pixels
- **Animation Speed**: 10ms per step (configurable)

### Algorithm Parameters
- **Heuristic**: Manhattan distance for A* and Greedy
- **Path Cost**: Uniform (1 per step)
- **Memory**: Dynamic allocation based on maze size

## 📊 Performance Metrics

The application tracks and displays:
- **Nodes Explored**: Total cells visited by algorithm
- **Solution Length**: Number of steps in final path
- **Real-time Updates**: Statistics update during algorithm execution

## 🏗️ Architecture

### Core Classes
- **MainApp**: Application entry point and initialization
- **MainFrame**: Main window container
- **Maze**: Core maze logic, rendering, and user interaction
- **SidePanel**: Control interface and statistics display

### Algorithm Implementation
- Modular design with function pointers for easy algorithm switching
- Thread-based execution for non-blocking UI
- Exception handling for graceful termination
- Template-based priority queue for optimal performance

## 📝 License

This project is licensed under the GNU Affero General Public License v3.0 (AGPL-3.0).

See [LICENSE.txt](LICENSE.txt) for full license text.

## 👥 Development Team

**Team HAVELI** - Original developers and maintainers

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## 🐛 Known Issues

- Windows-specific build configuration
- Limited to static maze dimensions
- No save/load functionality for mazes
- Single-threaded algorithm execution

## 🔮 Future Enhancements

See the "Improvement Areas" section for detailed suggestions on extending this project.

---

*For detailed technical documentation and algorithm explanations, see the inline code comments and algorithm implementations in `mazeSolverAlgo.cpp`.*