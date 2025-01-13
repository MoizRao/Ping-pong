# SFML Pong Game 🏓

A modern implementation of the classic Pong game using C++ and SFML (Simple and Fast Multimedia Library). This multiplayer game features smooth animations, collision physics, and progressive difficulty.

## Features

- Two-player gameplay
- Dynamic ball physics
- Progressive difficulty (ball speed increases with each hit)
- Score tracking system
- Semi-transparent graphics
- Custom background support
- Winning condition at 10 points

## Technical Implementation

### Dependencies
- C++ compiler
- SFML library
- pthread library (for multi-threading)

### Required Files
- `game_bg.png`: Background image file
- `Watanabe.ttf`: Font file for text rendering

## Game Controls

### Player 1 (Red Paddle)
- `W`: Move paddle up
- `S`: Move paddle down

### Player 2 (Blue Paddle)
- `↑`: Move paddle up
- `↓`: Move paddle down

### System Controls
- `ESC`: Exit game
- Close window button: Exit game

## Game Mechanics

1. Each player controls a paddle on their side of the screen
2. Ball speed increases after each paddle hit
3. Score points when opponent misses the ball
4. First player to reach 10 points wins
5. Ball direction changes based on where it hits the paddle:
   - Top section: Ball goes upward
   - Middle section: Ball goes straight
   - Bottom section: Ball goes downward

## Technical Approaches

### Multi-threading
- Separate threads for:
  - Paddle 1 movement
  - Paddle 2 movement
  - Ball movement
  - Main game loop

### Physics Implementation
- Dynamic velocity calculations
- Progressive speed increase
- Angle reflection based on paddle hit position
- Collision detection for:
  - Paddles
  - Window boundaries
  - Score zones

### Graphics
- Semi-transparent paddles (60% opacity)
- Custom background support
- Smooth animations (60 FPS limit)
- Dynamic window scaling

## Installation & Setup

1. Install SFML:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libsfml-dev

   # MacOS
   brew install sfml

   # Windows
   # Download SFML from https://www.sfml-dev.org/download.php
   ```

2. Place required files:
   - Put `game_bg.png` in the game directory
   - Put `Watanabe.ttf` in the game directory

3. Compile the game:
   ```bash
   g++ -c main.cpp
   g++ main.o -o pong -lsfml-graphics -lsfml-window -lsfml-system -pthread
   ```

4. Run the game:
   ```bash
   ./pong
   ```

## Game Configuration

The game window size can be modified by adjusting:
```cpp
#define WindowHeight 550
#define WindowWidth 900
```

## Technical Notes

- Uses SFML's RenderWindow for game display
- Implements multi-threading for smooth gameplay
- Uses custom collision detection algorithms
- Features dynamic speed scaling based on window size
- Implements paddle-based ball direction control
- Uses alpha channels for transparency effects

## Known Issues

- Keep font and background image in the same directory as the executable
- Window size adjustments require recompilation

## Performance

- Runs at 60 FPS
- Uses minimal CPU resources
- Smooth animations through multi-threading
