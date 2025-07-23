# Mazeli-2.0 - Interactive Maze Solver

![License](https://img.shields.io/badge/license-AGPL--3.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![C++](https://img.shields.io/badge/language-C%2B%2B-blue.svg)
![wxWidgets](https://img.shields.io/badge/GUI-wxWidgets-green.svg)

Mazeli-2.0 is an interactive maze solver application that visualizes various pathfinding algorithms in real-time. Built with C++ and wxWidgets, it provides an educational platform for understanding how different search algorithms work on grid-based mazes.

## 🚀 Features

### Pathfinding Algorithms
- **Breadth-First Search (BFS)** - Guarantees shortest path
- **Depth-First Search (DFS)** - Memory efficient exploration
- **A* Search** - Optimal pathfinding with heuristics
- **Greedy Best-First Search** - Fast but not always optimal
- **Bidirectional BFS** - Searches from both start and goal simultaneously

### Interactive Features
- **Visual Maze Editor** - Draw and erase walls with mouse
- **Real-time Visualization** - Watch algorithms explore the maze step by step
- **Flexible Start/Goal Placement** - Click to set custom start and goal positions
- **Random Maze Generation** - Generate complex mazes automatically
- **Algorithm Statistics** - Track nodes explored and solution path length
- **Simulation Controls** - Start, stop, and reset simulations

### User Interface
- **Intuitive Controls** - Easy-to-use mouse-based interface
- **Color-coded Visualization** - Different colors represent exploration depth
- **Side Panel** - Algorithm selection and statistics display
- **Help System** - Built-in instructions and feature explanations

## 🛠️ Requirements

### System Requirements
- **Operating System**: Windows (10 or later recommended)
- **Compiler**: Visual Studio 2019 or later with MSVC v143 toolset
- **Framework**: .NET Framework (for Visual Studio)

### Dependencies
- **wxWidgets**: GUI framework for cross-platform applications
  - Download from [wxWidgets official website](https://www.wxwidgets.org/)
  - Set `WXWIN` environment variable to wxWidgets installation directory

## 📦 Installation

### Option 1: Build from Source

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/Mazeli-2.0.git
   cd Mazeli-2.0
   ```

2. **Install wxWidgets**
   - Download and install wxWidgets
   - Set the `WXWIN` environment variable to your wxWidgets installation path
   - Example: `WXWIN=C:\wxWidgets-3.2.0`

3. **Open in Visual Studio**
   - Open `Mazeli-2.0.sln` in Visual Studio
   - Ensure the project is configured for your target platform (x64 recommended)

4. **Build the project**
   - Select your desired configuration (Debug/Release)
   - Build the solution (Ctrl+Shift+B)

5. **Run the application**
   - Press F5 or click Start to run the application

### Option 2: Download Pre-built Binary
*Pre-built binaries will be available in the Releases section once published.*

## 🎮 Usage

### Getting Started

1. **Launch the application** - Run the executable file
2. **Create a maze**:
   - Left-click and drag to draw walls (black squares)
   - Right-click and drag to erase walls
   - Use "Generate Random Maze" for automatic maze creation

3. **Set start and goal points**:
   - Click "Set Start" then click on the grid to place the yellow start point
   - Click "Set Goal" then click on the grid to place the green goal point

4. **Choose an algorithm** from the dropdown menu

5. **Start simulation** - Click "Start Simulation >>" to begin pathfinding

### Controls Reference

| Action | Control |
|--------|---------|
| Draw walls | Left mouse button + drag |
| Erase walls | Right mouse button + drag |
| Set start point | Click "Set Start" → Click on grid |
| Set goal point | Click "Set Goal" → Click on grid |
| Clear everything | "Clear All" button |
| Clear search results | "Clear Search" button |
| Generate random maze | "Generate Random Maze" button |
| Start/Stop simulation | "Start/Stop Simulation" button |

### Visual Legend

| Color | Meaning |
|-------|---------|
| 🟨 Yellow | Start position |
| 🟩 Green | Goal position |
| ⬜ White | Empty space |
| ⬛ Black | Wall/Obstacle |
| 🟦 Blue (varying shades) | Explored nodes (darker = explored later) |
| 🟧 Orange | Final solution path |

## 🔧 Algorithm Details

### Breadth-First Search (BFS)
- **Time Complexity**: O(V + E)
- **Space Complexity**: O(V)
- **Guarantees**: Shortest path in unweighted graphs
- **Best for**: Finding optimal solutions

### Depth-First Search (DFS)
- **Time Complexity**: O(V + E)
- **Space Complexity**: O(V)
- **Guarantees**: Will find a path if one exists
- **Best for**: Memory-constrained environments

### A* Search
- **Time Complexity**: O(b^d) where b is branching factor, d is depth
- **Space Complexity**: O(b^d)
- **Guarantees**: Optimal path with admissible heuristic
- **Best for**: Optimal pathfinding with good heuristics

### Greedy Best-First Search
- **Time Complexity**: O(b^m) where m is maximum depth
- **Space Complexity**: O(b^m)
- **Guarantees**: Fast but not always optimal
- **Best for**: Quick solutions when optimality isn't critical

### Bidirectional BFS
- **Time Complexity**: O(b^(d/2))
- **Space Complexity**: O(b^(d/2))
- **Guarantees**: Optimal path (like BFS)
- **Best for**: Large search spaces

## 🏗️ Project Structure

```
Mazeli-2.0/
├── Mazeli-2.0
│   ├── .gitignore
│   ├── Mazeli-2.0.aps
│   ├── Mazeli-2.0.vcxproj
│   ├── Mazeli-2.0.vcxproj.filters
│   ├── mainApp.cpp
│   ├── mainApp.h
│   ├── maze.cpp
│   ├── maze.h
│   ├── mazeSolverAlgo.cpp
│   └── x64
│       ├── Debug
│       │   ├── Mazeli-2.0.exe.recipe
│       │   ├── Mazeli-2.0.tlog
│       │   │   ├── CL.command.1.tlog
│       │   │   ├── CL.read.1.tlog
│       │   │   ├── CL.write.1.tlog
│       │   │   ├── Cl.items.tlog
│       │   │   ├── Mazeli-2.0.lastbuildstate
│       │   │   ├── link.command.1.tlog
│       │   │   ├── link.read.1.tlog
│       │   │   ├── link.secondary.1.tlog
│       │   │   └── link.write.1.tlog
│       │   └── vc143.idb
│       └── Release
│           └── Mazeli-2.0.tlog
│               ├── CL.command.1.tlog
│               ├── Mazeli-2.0.lastbuildstate
│               └── unsuccessfulbuild
├── .gitattributes
├── .gitignore
├── LICENSE
├── Mazeli-2.0.sln
└── README.md
```

## 🤝 Contributing

We welcome contributions to Mazeli-2.0! Here's how you can help:

### Reporting Issues
- Use the GitHub issue tracker
- Provide detailed description of the problem
- Include steps to reproduce
- Specify your operating system and Visual Studio version

### Suggesting Features
- Open an issue with the "enhancement" label
- Describe the feature and its benefits
- Provide use cases and examples

### Code Contributions
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Follow the existing code style
5. Test your changes thoroughly
6. Commit your changes (`git commit -m 'Add amazing feature'`)
7. Push to the branch (`git push origin feature/amazing-feature`)
8. Open a Pull Request

### Development Guidelines
- Follow C++ best practices
- Maintain consistent code formatting
- Add comments for complex algorithms
- Update documentation when adding features
- Ensure thread safety for UI updates

## 📄 License

This project is licensed under the GNU Affero General Public License v3.0 (AGPL-3.0). See the [LICENSE](https://github.com/president-xd/Mazeli-2.0/blob/master/LICENSE) file for details.

### Key License Points
- ✅ Commercial use allowed
- ✅ Modification allowed
- ✅ Distribution allowed
- ✅ Private use allowed
- ❗ Must include license and copyright notice
- ❗ Must disclose source code
- ❗ Network use is distribution (AGPL specific)

## 👥 Team HAVELI

This project was developed by **Team HAVELI**, a dedicated group of computer science enthusiasts passionate about algorithm visualization and educational software development.

### Core Team Members

| S.No | Name | GitHub |
|------|------|-------------|
| 1 | Abdul Wahab Khan | [babullay](https://github.com/wahabkhanwk) |
| 2 | Mohsin Mukhtiar | [president-xd](https://github.com/president-xd)|
| 3 | Talha Bilal | [k4Karlal](https://github.com/k4Karlal) |
| 4 | Qazi Muhammad Awais | [qazi awais](https://github.com/qaziawais)| 

### Team Philosophy
*"Making complex algorithms accessible through interactive visualization"*

Team HAVELI believes in the power of visual learning and hands-on exploration. Our goal is to bridge the gap between theoretical computer science concepts and practical understanding through engaging, interactive applications.

---

*If you're a Team HAVELI member and would like to update your information, please open a pull request or contact the team leads.*

## 🏆 Acknowledgments

- **Team HAVELI** - Original development team and ongoing maintenance
- **wxWidgets Community** - For the excellent cross-platform GUI framework
- **Algorithm Research** - Based on classical computer science pathfinding algorithms
- **Open Source Community** - For inspiration and collaborative development practices

## 📞 Support

- **Documentation**: Check this README and in-app help
- **Issues**: Report bugs via GitHub Issues
- **Questions**: Open a discussion in the GitHub Discussions tab

## 🗺️ Roadmap

### Planned Features
- [ ] Additional pathfinding algorithms (Dijkstra, Jump Point Search)
- [ ] Save/Load maze functionality
- [ ] Custom heuristic functions
- [ ] Performance benchmarking tools
- [ ] Cross-platform support (Linux, macOS)
- [ ] Weighted graphs support
- [ ] Algorithm animation speed controls

### Future Enhancements
- [ ] 3D maze visualization
- [ ] Multi-agent pathfinding
- [ ] Educational mode with step-by-step explanations
- [ ] Export functionality for results

---

**Developed by Team HAVELI** | **Powered by wxWidgets** | **Licensed under AGPL-3.0**