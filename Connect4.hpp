
#include <iostream>
#include <vector>
#include <climits>
#include <cstdint>
#include <fstream>
#include <sstream>
const int ROWS = 6;
const int COLS = 7;
const int WIN_CONDITION = 4;
const int EMPTY = 0;
const int PLAYER1 = 1;
const int PLAYER2 = 2;

class Connect4 {
private:
    std::vector<std::vector<int>> board;
    int currentPlayer;
    int maxDepth;

public:
    Connect4(int difficulty) : board(ROWS, std::vector<int>(COLS, EMPTY)), currentPlayer(PLAYER1) {
        setMaxDepth(difficulty);
    }

    void saveGame(const std::string& filename) const {
        std::ofstream file(filename);

        if (file.is_open()) {
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    file << board[row][col] << ' ';
                }
                file << '\n';
            }

            file.close();
            std::cout << "Game saved successfully." << std::endl;
        } else {
            std::cerr << "Unable to open the file: " << filename << std::endl;
        }
    }
    void loadGame(const std::string& filename) {
        std::ifstream file(filename);

        if (file.is_open()) {
            // clear
            board = std::vector<std::vector<int>>(ROWS, std::vector<int>(COLS, EMPTY));

            std::string line;
            int row = 0;

            while (std::getline(file, line) && row < ROWS) {
                std::istringstream iss(line);
                int value;

                for (int col = 0; col < COLS && iss >> value; col++) {
                    board[row][col] = value;
                }

                row++;
            }

            file.close();

            // show
            std::cout << "Game loaded successfully." << std::endl;
            printBoard();
        } else {
            std::cerr << "Unable to open the file: " << filename << std::endl;
        }
    }
    void setMaxDepth(int difficulty) {
        switch (difficulty) {
            case 1:
                maxDepth = 3;  // Set an appropriate depth for easy difficulty
                break;
            case 2:
                maxDepth = 5;  // Set an appropriate depth for medium difficulty
                break;
            case 3:
                maxDepth = 10;  // Set an appropriate depth for difficult difficulty
                break;
            default:
                std::cerr << "Invalid choice. Exiting." << std::endl;
                exit(1);
        }
    }

    void printBoard() const {
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                if (board[row][col] == PLAYER1) {
                    std::cout << 'X' << " ";
                } else if (board[row][col] == PLAYER2) {
                    std::cout << 'O' << " ";
                } else {
                    std::cout << "  ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << "*---------------*" << std::endl;
    }

    int evaluatePosition(const std::vector<std::vector<int>>& board, int row, int col, int player) const {
        int score = 0;

        // Evaluate horizontally, vertically, and diagonally
        score += evaluateDirection(board, row, col, 0, 1, player);
        score += evaluateDirection(board, row, col, 1, 0, player);
        score += evaluateDirection(board, row, col, 1, 1, player);
        score += evaluateDirection(board, row, col, -1, 1, player);

        return score;
    }

    int evaluateDirection(const std::vector<std::vector<int>>& board, int row, int col, int rowDir, int colDir, int player) const {
        int consecutivePlayer = 0;  // Number of consecutive player pieces
        int emptySpaces = 0;       // Number of empty spaces in the sequence

        for (int i = 0; i < WIN_CONDITION; i++) {
            int r = row + i * rowDir;
            int c = col + i * colDir;

            if (r >= 0 && r < ROWS && c >= 0 && c < COLS) {
                if (board[r][c] == player) {
                    consecutivePlayer++;
                } else if (board[r][c] == EMPTY) {
                    emptySpaces++;
                }
            }
        }

        // Assign scores based on the number of consecutive pieces and empty spaces
        if (consecutivePlayer == WIN_CONDITION) {
            // Winning move
            return (player == PLAYER1) ? 100 : -100;
        } else if (consecutivePlayer == WIN_CONDITION - 1 && emptySpaces == 1) {
            // Almost winning move
            return (player == PLAYER1) ? 50 : -50;
        } else if (consecutivePlayer == 3 && emptySpaces == 1) {
            // Potential winning move
            return (player == PLAYER1) ? 10 : -10;
        } else {
            // No immediate win or loss
            return 0;
        }
    }

    bool isWinningMove(const std::vector<std::vector<int>>& board, int player, int row, int col) {
        // Check horizontally
        for (int i = 0; i <= COLS - WIN_CONDITION; i++) {
            bool win = true;
            for (int j = 0; j < WIN_CONDITION; j++) {
                if (board[row][i + j] != player) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }

        // Check vertically
        for (int i = 0; i <= ROWS - WIN_CONDITION; i++) {
            bool win = true;
            for (int j = 0; j < WIN_CONDITION; j++) {
                if (board[i + j][col] != player) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }

        // Check diagonally (down-right)
        for (int i = 0; i <= ROWS - WIN_CONDITION; i++) {
            for (int j = 0; j <= COLS - WIN_CONDITION; j++) {
                bool win = true;
                for (int k = 0; k < WIN_CONDITION; k++) {
                    if (board[i + k][j + k] != player) {
                        win = false;
                        break;
                    }
                }
                if (win) return true;
            }
        }

        // Check diagonally (up-right)
        for (int i = WIN_CONDITION - 1; i < ROWS; i++) {
            for (int j = 0; j <= COLS - WIN_CONDITION; j++) {
                bool win = true;
                for (int k = 0; k < WIN_CONDITION; k++) {
                    if (board[i - k][j + k] != player) {
                        win = false;
                        break;
                    }
                }
                if (win) return true;
            }
        }

        return false;
    }

    bool isBoardFull() const {
        for (int col = 0; col < COLS; col++) {
            if (board[0][col] == EMPTY) {
                return false;
            }
        }
        return true;
    }

    int getAvailableRow(const std::vector<std::vector<int>>& board, int col) const {
        for (int row = ROWS - 1; row >= 0; row--) {
            if (board[row][col] == EMPTY) {
                return row;
            }
        }
        return -1;  // Column is full
    }

    int minimax(std::vector<std::vector<int>>& board, int depth, bool maximizingPlayer, int alpha, int beta) {
        int score = evaluateBoard(board);

        if (depth == 0 || score != 0 || isBoardFull()) {
            return score;
        }

        if (maximizingPlayer) {
            int maxScore = INT32_MIN;
            for (int col = 0; col < COLS; col++) {
                if (board[0][col] == EMPTY) {
                    for (int row = ROWS - 1; row >= 0; row--) {
                        if (board[row][col] == EMPTY) {
                            board[row][col] = PLAYER2;
                            int currentScore = minimax(board, depth - 1, false, alpha, beta);
                            board[row][col] = EMPTY;
                            maxScore = std::max(maxScore, currentScore);
                            alpha = std::max(alpha, maxScore);
                            if (beta <= alpha) {
                                break;
                            }
                        }
                    }
                }
            }
            return maxScore;
        } else {
            int minScore = INT32_MAX;
            for (int col = 0; col < COLS; col++) {
                if (board[0][col] == EMPTY) {
                    for (int row = ROWS - 1; row >= 0; row--) {
                        if (board[row][col] == EMPTY) {
                            board[row][col] = PLAYER1;
                            int currentScore = minimax(board, depth - 1, true, alpha, beta);
                            board[row][col] = EMPTY;
                            minScore = std::min(minScore, currentScore);
                            beta = std::min(beta, minScore);
                            if (beta <= alpha) {
                                break;
                            }
                        }
                    }
                }
            }
            return minScore;
        }
    }

    int getAIMove(const std::vector<std::vector<int>>& board, int maxDepth) {
        int bestScore = INT32_MIN;
        int bestMove = -1;

        for (int col = 0; col < COLS; col++) {
            if (board[0][col] == EMPTY) {
                for (int row = ROWS - 1; row >= 0; row--) {
                    if (board[row][col] == EMPTY) {
                        std::vector<std::vector<int>> tempBoard = board;
                        tempBoard[row][col] = PLAYER2;
                        int score = minimax(tempBoard, maxDepth, false, INT32_MIN, INT32_MAX);
                        if (score > bestScore) {
                            bestScore = score;
                            bestMove = col;
                        }
                        break;
                    }
                }
            }
        }

        return bestMove;
    }

    int evaluateBoard(const std::vector<std::vector<int>>& board) {
        int score = 0;
        int rows = board.size();
        int cols = board[0].size();

        // Evaluate rows
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols - 3; col++) {
                int player1Count = 0;
                int player2Count = 0;
                for (int i = 0; i < 4; i++) {
                    if (board[row][col + i] == PLAYER1) {
                        player1Count++;
                    } else if (board[row][col + i] == PLAYER2) {
                        player2Count++;
                    }
                }
                score += player1Count * 10;
                score -= player2Count * 10;
            }
        }

        // Evaluate columns
        for (int col = 0; col < cols; col++) {
            for (int row = 0; row < rows - 3; row++) {
                int player1Count = 0;
                int player2Count = 0;
                for (int i = 0; i < 4; i++) {
                    if (board[row + i][col] == PLAYER1) {
                        player1Count++;
                    } else if (board[row + i][col] == PLAYER2) {
                        player2Count++;
                    }
                }
                score += player1Count * 10;
                score -= player2Count * 10;
            }
        }

        // Evaluate diagonals (down-right)
        for (int row = 0; row < rows - 3; row++) {
            for (int col = 0; col < cols - 3; col++) {
                int player1Count = 0;
                int player2Count = 0;
                for (int i = 0; i < 4; i++) {
                    if (board[row + i][col + i] == PLAYER1) {
                        player1Count++;
                    } else if (board[row + i][col + i] == PLAYER2) {
                        player2Count++;
                    }
                }
                score += player1Count * 10;
                score -= player2Count * 10;
            }
        }

        // Evaluate diagonals (up-right)
        for (int row = 3; row < rows; row++) {
            for (int col = 0; col < cols - 3; col++) {
                int player1Count = 0;
                int player2Count = 0;
                for (int i = 0; i < 4; i++) {
                    if (board[row - i][col + i] == PLAYER1) {
                        player1Count++;
                    } else if (board[row - i][col + i] == PLAYER2) {
                        player2Count++;
                    }
                }
                score += player1Count * 10;
                score -= player2Count * 10;
            }
        }

        // Evaluate vertical patterns
        for (int col = 0; col < cols; col++) {
            for (int row = 0; row < rows - 3; row++) {
                int player1Count = 0;
                int player2Count = 0;
                for (int i = 0; i < 4; i++) {
                    if (board[row + i][col] == PLAYER1) {
                        player1Count++;
                    } else if (board[row + i][col] == PLAYER2) {
                        player2Count++;
                    }
                }
                score += player1Count * 10;
                score -= player2Count * 10;
            }
        }

        return score;
    }



 void playGame() {
        while (true) {
            // Player's turn
            if (currentPlayer == PLAYER1) {
                int col;
                std::cout << "Player 1, enter a column (0-" << COLS - 1 << "): ";
                std::cin >> col;

                // Check if the move is valid
                if (col < 0 || col >= COLS || board[0][col] != EMPTY) {
                    std::cout << "Invalid move! Try again." << std::endl;
                    continue;
                }

                // Make the move
                for (int row = ROWS - 1; row >= 0; row--) {
                    if (board[row][col] == EMPTY) {
                        board[row][col] = PLAYER1;
                        break;
                    }
                }
            }
            // AI's turn
            else {
                std::cout << "AI's turn..." << std::endl;
                int aiMove = getAIMove(board, maxDepth);

                // Make the AI move
                for (int row = ROWS - 1; row >= 0; row--) {
                    if (board[row][aiMove] == EMPTY) {
                        board[row][aiMove] = PLAYER2;
                        break;
                    }
                }
            }

            printBoard();

            // Check for a win
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    if (board[row][col] != EMPTY && isWinningMove(board, board[row][col], row, col)) {
                        std::cout << "Player " << board[row][col] << " wins!" << std::endl;

                        // After the game ends, save the final state to the CSV file
                        saveGame("final_game.csv");

                        return;
                    }
                }
            }

            // Check for a draw
            if (isBoardFull()) {
                std::cout << "It's a draw!" << std::endl;

                // After the game ends in a draw, save the final state to the CSV file
                saveGame("final_game.csv");

                return;
            }

            // Switch players
            currentPlayer = (currentPlayer == PLAYER1) ? PLAYER2 : PLAYER1;

            // Ask the player if they want to save the game
            char saveChoice;
            std::cout << "Do you want to save the game? (y/n): ";
            std::cin >> saveChoice;

            if (saveChoice == 'y' || saveChoice == 'Y') {
                saveGame("intermediate_game.csv");
            }

            // Ask the player if they want to load a game
            char loadChoice;
            std::cout << "Do you want to load a game? (y/n): ";
            std::cin >> loadChoice;

            if (loadChoice == 'y' || loadChoice == 'Y') {
                std::string filename;
                std::cout << "Enter the filename to load: ";
                std::cin >> filename;
                loadGame(filename);
                printBoard();
            }
        }
    }
};

