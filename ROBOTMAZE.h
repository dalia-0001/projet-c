/*
    ROBOTMAZE.c
    Task 3: Robot Path (Course de Robots)
    Objective: Navigate a grid to find the exit using N/S/E/O commands.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For sleep()

// --- COLOR DEFINITIONS (Redefined here to ensure availability) ---
#ifndef COLORS_DEFINED
#define COLORS_DEFINED
#define C_RED     "\033[1;31m"
#define C_YELLOW  "\033[1;33m"
#define C_GREEN   "\033[1;32m"
#define C_CYAN    "\033[1;36m"
#define C_BLUE    "\033[1;34m"
#define C_PURPLE  "\033[1;35m"
#define C_WHITE   "\033[1;37m"
#define C_GRAY    "\033[1;90m"
#define RESET     "\033[0m"
#endif

// Grid Dimensions
#define ROWS 10
#define COLS 20

// Symbols
#define CHAR_WALL   '#'
#define CHAR_PATH   ' '
#define CHAR_ROBOT  'R'
#define CHAR_EXIT   'E'
#define CHAR_START  'S'

// Structure for coordinates
typedef struct {
    int r;
    int c;
} Position;

// --- LEVEL DATA ---
// A simple hardcoded maze for the exam project
// 0 = Path, 1 = Wall, 2 = Start, 3 = Exit
int LEVEL_1[ROWS][COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 1, 0, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// Helper: Clear screen (local version to avoid dependency issues)
void rm_clear() {
    printf("\033[H\033[J");
}

// Helper: Display the maze
void print_maze(Position robot_pos) {
    printf("\n");
    printf("    " C_GRAY "--- MISSION: ROBOT PATH ---" RESET "\n");
    
    printf("   [Commands]: 'R' to Reset | 'Q' to Quit\n\n");
    printf("    " C_YELLOW "Goal:" RESET " Guide the Robot (" C_CYAN "R" RESET ") to the Exit (" C_GREEN "E" RESET ")\n");
    printf("    " C_GRAY "Controls:" RESET " N (Up), S (Down), E (Right), O (Left)\n\n");

    for (int i = 0; i < ROWS; i++) {
        printf("    "); // Indent
        for (int j = 0; j < COLS; j++) {
            if (i == robot_pos.r && j == robot_pos.c) {
                printf(C_CYAN "%c" RESET, CHAR_ROBOT);
            } else if (LEVEL_1[i][j] == 1) {
                printf(C_GRAY "%c" RESET, CHAR_WALL);
            } else if (LEVEL_1[i][j] == 3) {
                printf(C_GREEN "%c" RESET, CHAR_EXIT);
            } else if (LEVEL_1[i][j] == 2) {
                printf(C_GRAY "." RESET); // Start point marker
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// --- MAIN GAME FUNCTION ---
void play_robot_maze(char* username, int* global_score) {
    Position robot;
    
    // 1. Initialize Position
    // Find start position (2)
    for(int i=0; i<ROWS; i++) {
        for(int j=0; j<COLS; j++) {
            if (LEVEL_1[i][j] == 2) {
                robot.r = i;
                robot.c = j;
            }
        }
    }

    int moves = 0;
    int max_moves = 50; // Challenge constraint
    int won = 0;
    char input[10];

    // Game Loop
    while(moves < max_moves && !won) {
        rm_clear();
        print_maze(robot);
        printf(C_PURPLE " Moves: " C_WHITE "%d/%d" RESET "\n", moves, max_moves);
        printf(C_BLUE " %s@robot" C_WHITE ":" C_CYAN "~$ " RESET, username);

        if (fgets(input, sizeof(input), stdin) == NULL) break;

        char cmd = input[0];
        // Normalize to uppercase
        if (cmd >= 'a' && cmd <= 'z') cmd -= 32;

        // Quit command
        if (cmd == 'Q') {
            printf(C_RED "\n >> Mission Aborted.\n" RESET);
            sleep(1);
            return;
        }

        // Calculate new position
        int new_r = robot.r;
        int new_c = robot.c;

        if (cmd == 'N') new_r--;      // Nord
        else if (cmd == 'S') new_r++; // Sud
        else if (cmd == 'E') new_c++; // Est
        else if (cmd == 'O') new_c--; // Ouest (West)
        else {
            printf(C_RED " >> Invalid Command! Use N, S, E, O.\n" RESET);
            sleep(1);
            continue;
        }

        // Check Collisions
        if (LEVEL_1[new_r][new_c] == 1) {
            printf(C_RED " >> CRASH! Wall detected.\n" RESET);
            sleep(1);
        } else {
            // Move valid
            robot.r = new_r;
            robot.c = new_c;
            moves++;

            // Check Victory
            if (LEVEL_1[robot.r][robot.c] == 3) {
                won = 1;
            }
        }
    }

    // End Game Handling
    rm_clear();
    print_maze(robot);

    if (won) {
        printf("\n");
        printf(C_GREEN " " "SUCCESS! TARGET REACHED." " " RESET "\n");
        
        // Calculate Score based on efficiency
        // Base 50 pts - (moves taken / 2)
        int points = 50 - (moves / 2);
        if (points < 10) points = 10; // Minimum reward

        printf(C_YELLOW " >> Performance Analysis: %d moves.\n" RESET, moves);
        printf(C_GREEN " >> Points Awarded: +%d\n" RESET, points);
        
        *global_score += points;
    } else {
        printf(C_RED "\n >> MISSION FAILED: Out of moves or energy.\n" RESET);
    }

    printf("\n (Press Enter to return to Dashboard)");
    getchar(); // Wait for user
}
