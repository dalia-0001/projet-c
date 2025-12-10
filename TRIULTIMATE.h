/*
    TRIULTIMATE.c
    Task 4: Tri Ultime (Ultimate Sort)
    Objective: Sort the array by swapping elements manually.
    Constraints: Limited number of swaps allowed.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> // For sleep()

// --- COLORS DEFINITIONS (Safety Check) ---
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

#define ARRAY_SIZE 6

// Helper: Check if array is sorted in ascending order
int is_sorted_check(int arr[], int n) {
    for(int i = 0; i < n - 1; i++)
        if(arr[i] > arr[i+1])
            return 0;
    return 1;
}

// Helper: Display the array with indices
void print_sort_interface(int arr[], int n) {
    printf("\n");
    printf("  " C_GRAY "Indices: " RESET);
    for(int i = 0; i < n; i++) printf(" %2d  ", i);
    printf("\n");
    
    printf("  " C_GRAY "Values : " RESET);
    for(int i = 0; i < n; i++) {
        // Simple visual hint: if a number is bigger than the next one, color it RED
        if (i < n-1 && arr[i] > arr[i+1])
             printf(C_RED " %2d  " RESET, arr[i]);
        else
             printf(C_CYAN " %2d  " RESET, arr[i]);
    }
    printf("\n\n");
}

// Main Game Function called by interface.c
void play_sorting_game(char* username, int* global_score) {
    int arr[ARRAY_SIZE];
    int moves = 0;
    int max_moves = 15; // Constraint: Sort in 15 swaps or less
    
    // 1. Generate a solvable but shuffled array
    // We pick specific numbers to ensure it looks nice, then shuffle them
    int pool[ARRAY_SIZE] = {10, 5, 99, 42, 25, 50};
    srand(time(NULL));
    
    // Fisher-Yates Shuffle
    for (int i = 0; i < ARRAY_SIZE; i++) {
        int r = rand() % ARRAY_SIZE;
        int temp = pool[i];
        pool[i] = pool[r];
        pool[r] = temp;
    }
    // Copy to game array
    for(int i=0; i<ARRAY_SIZE; i++) arr[i] = pool[i];

    char input[50];
    int idx1, idx2;

    while (moves < max_moves) {
        // Clear screen
        printf("\033[H\033[J");
        
        // Header
        printf(C_PURPLE " :: ULTIMATE SORT CHALLENGE ::" RESET "\n");
        
        printf("   [Commands]: 'R' to Reset | 'Q' to Quit\n\n");
        printf(C_GRAY " Goal: Sort the array using SWAPS." RESET "\n");
        printf(C_YELLOW " Operations Left: %d" RESET "\n", max_moves - moves);

        // Display current state
        print_sort_interface(arr, ARRAY_SIZE);

        // Check if sorted (Win Condition)
        if (is_sorted_check(arr, ARRAY_SIZE)) {
            printf(C_GREEN " >> SORTING COMPLETE! SYSTEM OPTIMIZED." RESET "\n");
            
            // Score Calculation
            // Base score 20 + (5 points per unused move)
            int bonus = (max_moves - moves) * 5;
            int points = 20 + bonus;
            
            printf(C_YELLOW " >> Moves Used: %d\n" RESET, moves);
            printf(C_GREEN " >> Points Awarded: +%d\n" RESET, points);
            
            *global_score += points;
            
            printf("\n(Press Enter to return)");
            while(getchar() != '\n'); getchar();
            return;
        }

        // Input Prompt
        printf(C_BLUE " %s@sort" C_WHITE ":" C_CYAN "~$ " RESET "Swap indices (e.g., 0 1): ", username);
        
        if (fgets(input, sizeof(input), stdin) == NULL) break;

        // Parse Input
        if (sscanf(input, "%d %d", &idx1, &idx2) == 2) {
            // Validate Indices
            if (idx1 >= 0 && idx1 < ARRAY_SIZE && idx2 >= 0 && idx2 < ARRAY_SIZE && idx1 != idx2) {
                // Perform Swap
                int temp = arr[idx1];
                arr[idx1] = arr[idx2];
                arr[idx2] = temp;
                
                moves++;
                printf(C_GRAY " >> Swapping..." RESET "\n");
                sleep(1);
            } else {
                printf(C_RED " >> Invalid indices! Use 0 to 5." RESET "\n");
                sleep(1);
            }
        } else {
            // Quit option
            if (input[0] == 'q' || input[0] == 'Q') return;
            
            printf(C_RED " >> Error: Enter two numbers separated by space." RESET "\n");
            sleep(1);
        }
    }

    // Loss Condition (Ran out of moves)
    printf(C_RED "\n >> MISSION FAILED: Too many operations." RESET "\n");
    sleep(2);
}
