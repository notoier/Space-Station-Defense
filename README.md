# Space Station Defense

Space Station Defense is a simple game developed in **C++** using **SFML** as part of a final assignment.

The objective of the project is to apply core C++ programming concepts, game architecture, and resource management following an iterative development approach.

## Game Description

Space Station Defense is a 2D defense game where the player must protect a space station from incoming enemy waves.

Enemies spawn around the map and move towards the central station. The player must strategically manage defenses and resources to prevent the station from being destroyed.

The game focuses on clean architecture, efficient resource management, and an iterative development process, prioritizing technical quality over visual complexity.

## Controls
- **Mouse** – Interact / Shoot
- 
*(Controls may change as development progresses)*

## Technical Features
- C++17
- SFML
- CMake-based project
- Modular architecture
- Data-oriented design where possible

## Project Structure
- /assets → Game assets (textures, fonts, sounds)
- /include → Header files
- /src → Source files
- /build → Build directory (ignored by Git)

## Build Instructions (Visual Studio)

### Requirements
- Visual Studio 2022
- CMake
- SFML installed or fetched via CMake

### Steps
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
```
Then:

1. Open the generated .sln file in Visual Studio
2. Select Debug or Release
3. Build and run the project from Visual Studio

##Author

Developed by Oier Álvarez as part of a C++ & SFML academic project.