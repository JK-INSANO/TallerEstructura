
#include <iostream>
#include <vector>
#include <climits>
#include "Connect4.hpp"
int main() {
    int difficulty;

    std::cout << "Welcome to Connect 4!" << std::endl;
    std::cout << "Player 1: X" << std::endl;
    std::cout << "Player 2 (AI): O" << std::endl;

    // Difficulty menu1
    std::cout << "Select difficulty level:" << std::endl;
    std::cout << "1. Easy" << std::endl;
    std::cout << "2. Medium" << std::endl;
    std::cout << "3. Difficult" << std::endl;
    std::cout << "Enter the number corresponding to your choice: ";
    std::cin >> difficulty;

    Connect4 game(difficulty);
    game.playGame();

    return 0;
};
