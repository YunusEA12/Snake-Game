#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <ctime>

const int WIDTH = 20;
const int HEIGHT = 20;
const int SQUARE_SIZE = 20; // Größe jedes Quadrats

// Snake position and direction
int snakeX = WIDTH / 2;
int snakeY = HEIGHT / 2;
int snakeDirectionX = 1;
int snakeDirectionY = 0;

// Snake body
const int MAX_BODY_LENGTH = 100;
int snakeBodyX[MAX_BODY_LENGTH];
int snakeBodyY[MAX_BODY_LENGTH];
int snakeLength = 1;

// Initialize random seed
void initRandom() {
    std::srand(std::time(0));
}

// Generate new random position for the red square, avoiding collisions with snake
void generateRandomPosition(int &x, int &y) {
    do {
        x = std::rand() % WIDTH;
        y = std::rand() % HEIGHT;
    } while (x == snakeX && y == snakeY);
}


// Check if the given position is within the boundaries of the game field
bool isValidPosition(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

// Save the player's score and name to the leaderboard file
void saveScore(const std::string& name, int score) {
    std::ofstream leaderboard("leaderboard.txt", std::ios::app);
    if (leaderboard.is_open()) {
        leaderboard << name << " " << score << std::endl;
        leaderboard.close();
    } else {
        std::cerr << "Fehler beim Öffnen der Bestenliste!" << std::endl;
    }
}

// Load the leaderboard from the file and return as a vector of pairs (name, score)
std::vector<std::pair<std::string, int>> loadLeaderboard() {
    std::ifstream leaderboard("leaderboard.txt");
    std::vector<std::pair<std::string, int>> scores;
    std::string name;
    int score;
    while (leaderboard >> name >> score) {
        scores.push_back({name, score});
    }
    leaderboard.close();
    return scores;
}

// Display the leaderboard
void displayLeaderboard(const std::vector<std::pair<std::string, int>>& scores) {
    std::cout << "Leaderboard:" << std::endl;
    for (size_t i = 0; i < scores.size(); ++i) {
        std::cout << i + 1 << ". " << scores[i].first << ": " << scores[i].second << std::endl;
    }
}

int main(int argc, char *argv[]) {
    // Initialize the random seed
    initRandom();

    // Initial position of the red square
    int randomX, randomY;
    generateRandomPosition(randomX, randomY);

    // Initialize the SDL library
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Fehler: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH * SQUARE_SIZE, HEIGHT * SQUARE_SIZE, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Fehler beim Erstellen des Fensters: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create the renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Fehler beim Erstellen des Renderers: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Main loop
    bool running = true;
    bool gameover = false;
    int score = 0;
    SDL_Event event;

    // Hintergrund löschen und Kästchen zeichnen (außerhalb der Schleife)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw the red square before the main loop
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect randomRect = {randomX * SQUARE_SIZE, randomY * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
    SDL_RenderFillRect(renderer, &randomRect);

    // Draw the snake
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int i = 0; i < snakeLength; i++) {
        SDL_Rect snakeRect = {snakeBodyX[i] * SQUARE_SIZE, snakeBodyY[i] * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
        SDL_RenderFillRect(renderer, &snakeRect);
    }

    SDL_RenderPresent(renderer);

    // Main loop
    while (running) {
        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && !gameover) {
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        if (snakeDirectionY == 0) { // Prevent reversing direction
                            snakeDirectionX = 0;
                            snakeDirectionY = -1;
                        }
                        break;
                    case SDLK_s:
                        if (snakeDirectionY == 0) { // Prevent reversing direction
                            snakeDirectionX = 0;
                            snakeDirectionY = 1;
                        }
                        break;
                    case SDLK_a:
                        if (snakeDirectionX == 0) { // Prevent reversing direction
                            snakeDirectionX = -1;
                            snakeDirectionY = 0;
                        }
                        break;
                    case SDLK_d:
                        if (snakeDirectionX == 0) { // Prevent reversing direction
                            snakeDirectionX = 1;
                            snakeDirectionY = 0;
                        }
                        break;
                }
            }
        }

        // Draw the red square before updating the window
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect randomRect = {randomX * SQUARE_SIZE, randomY * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
        SDL_RenderFillRect(renderer, &randomRect);

        // Clear the background before drawing the snake
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the snake
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int i = 0; i < snakeLength; i++) {
            SDL_Rect snakeRect = {snakeBodyX[i] * SQUARE_SIZE, snakeBodyY[i] * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            SDL_RenderFillRect(renderer, &snakeRect);
        }

        // Check for game over condition
        if (!isValidPosition(snakeX, snakeY)) {
            gameover = true;
        }
        for (int i = 1; i < snakeLength; i++) {
            if (snakeX == snakeBodyX[i] && snakeY == snakeBodyY[i]) {
                gameover = true;
                break;
            }
        }

        // Update snake position
        if (!gameover) {
            // Save the current head position
            int prevSnakeX = snakeX;
            int prevSnakeY = snakeY;

            // Move the snake
            snakeX += snakeDirectionX;
            snakeY += snakeDirectionY;

            // Check if the snake eats the red square
            if (snakeX == randomX && snakeY == randomY) {
                // Increase snake length
                if (snakeLength < MAX_BODY_LENGTH) {
                    snakeLength++;
                }
                // Increase score
                score++;

                // Generate new random position for the red square
                generateRandomPosition(randomX, randomY);
            }

            // Update snake body positions (optimized)
            for (int i = snakeLength - 1; i > 0; i--) {
                snakeBodyX[i] = snakeBodyX[i - 1];
                snakeBodyY[i] = snakeBodyY[i - 1];
            }
            // Update the head position
            snakeBodyX[0] = prevSnakeX;
            snakeBodyY[0] = prevSnakeY;
        }

        // Clear the background
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the red square
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &randomRect);

        // Draw the snake
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int i = 0; i < snakeLength; i++) {
            SDL_Rect snakeRect = {snakeBodyX[i] * SQUARE_SIZE, snakeBodyY[i] * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE};
            SDL_RenderFillRect(renderer, &snakeRect);
        }

        // Update the window
        SDL_RenderPresent(renderer);

        // Delay to control the frame rate
        SDL_Delay(100);

        // Game over
        if (gameover) {
            // Ask for player's name
            std::string playerName;
            std::cout << "Game Over! Your score: " << score << std::endl;
            std::cout << "Enter your name: ";
            std::cin >> playerName;

            // Save the score and player's name to leaderboard
            saveScore(playerName, score);

            // Display leaderboard
            std::vector<std::pair<std::string, int>> leaderboard = loadLeaderboard();
            std::sort(leaderboard.begin(), leaderboard.end(), [](const auto& a, const auto& b) {
                return a.second > b.second; // Sort in descending order of scores
            });
            std::cout << "Leaderboard:" << std::endl;
            for (size_t i = 0; i < leaderboard.size() && i < 3; ++i) {
                std::cout << i + 1 << ". " << leaderboard[i].first << ": " << leaderboard[i].second << std::endl;
            }

            // End the game loop
            running = false;
        }
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
