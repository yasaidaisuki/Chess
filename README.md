# Chess Engine

A feature-rich chess game supporting multiple variants (standard chess, atomic chess) with progressive difficulty AI opponents, built with modern C++17 and optimized for real-time rendering.

## Features

- **Multiple Game Variants** – Standard chess and atomic chess (captures explode pieces)
- **AI Opponents** – Three difficulty levels using minimax with alpha-beta pruning
- **Real-Time Rendering** – Custom differential update algorithm achieving 2.25x performance improvement
- **Thread-Safe Game Engine** – Concurrent AI move calculation without blocking UI
- **Object-Oriented Architecture** – Observer and Factory design patterns for maintainability

## Architecture
<img width="1426" height="721" alt="image" src="https://github.com/user-attachments/assets/41fc1cf6-ab0c-4f60-ab0a-8dcb6167e7a2" />

## Technologies

- **Language:** C++17 (smart pointers, atomic operations)
- **Display:** X11 Graphics System
- **Concurrency:** Atomic variables, thread-safe move calculation
- **Design Patterns:** Observer, Factory

## Pictures

<img width="686" height="665" alt="image" src="https://github.com/user-attachments/assets/e144c3e7-667e-4ed8-8e8d-a2307753d2ae" />
<img width="141" height="170" alt="image" src="https://github.com/user-attachments/assets/76b6a7d1-16d1-49af-bc9d-57d93a5a854f" />



## Building & Running

### Prerequisites
```bash
sudo apt-get install libx11-dev g++ make
```

### Build
```bash
make clean
make
```

### Run
```bash
./chess
```

### Usage

Terminal-Based. Run the game for further instructions.

## AI Difficulty Levels

- **Easy** - Randomized Logic
- **Medium** - Some logic, capable of castling, taking
- **Hard** - Stronger logic, capable of castling, and finding checkmates

## Game Variants

### Standard Chess
Traditional chess rules with standard win conditions.

### Atomic Chess
When a piece captures, both the capturing piece and target explode, removing all adjacent pieces (except pawns). King cannot be in check after capture.

## Design Patterns

- **Observer Pattern** – Game state updates notify renderer and AI engine
- **Factory Pattern \& Decorator Pattern** – Centralized piece creation with variant-specific rules
- **Strategy Pattern** – Swappable AI difficulty implementations

## Performance Optimizations

| Metric | Before | After | Improvement |
|--------|--------|-------|-------------|
| Render Time | 3.5s | 1.95s | 55.7% reduction |
| Move re-render Time | 3.5s| 0.1s | 97.1% reduction |  

The differential update algorithm only redraw changed board squares, minimizing graphics buffer writes even during heavy AI computation.

In order to do this, we decided to keep a Deep Copy of the Chess board, keeping a previous board state. By comparing the previous board with the current board, skipping over where the grid is the same, and only re-rendering changes in the board

## Collaboration

This project was developed in collaboration with [Kevin Lau](https://github.com/kevin-klau).
