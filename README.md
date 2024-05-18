```markdown
# Snake Game

This is a simple snake game implemented in C++ using the SDL library.

## Description

This snake game allows the player to control a snake on a grid. The objective is to eat as many red squares (food) as possible without colliding with the walls or the snake's own body. The game ends when the snake collides with a wall or itself.

## Features

- Player-controlled snake movement (up, down, left, right).
- Random generation of red squares (food).
- Collision detection with walls and snake's body.
- Score tracking and leaderboard.

## Dependencies

- SDL2 Library: The game uses the SDL2 library for window creation, rendering, and event handling. Make sure to have SDL2 installed on your system.

## Installation

1. Clone the repository to your local machine:

   ```bash
   git clone https://github.com/your-username/snake-game.git
   ```

2. Compile the game using a C++ compiler. Make sure to link against the SDL2 library:

   ```bash
   g++ -o main main.cpp -lSDL2
   ```

3. Run the compiled executable:

   ```bash
   ./main
   ```

## Usage

- Use the arrow keys (up, down, left, right) to control the snake's movement.
- Eat red squares to increase your score.
- Avoid colliding with walls or the snake's own body.
- When the game ends, enter your name to save your score to the leaderboard.

## Contributing

Contributions are welcome! If you'd like to contribute to this project, please fork the repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
```
