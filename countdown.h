#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// COLORS
#define C_GREEN "\033[1;32m"
#define C_RED   "\033[1;31m"
#define C_CYAN  "\033[1;36m"
#define C_YELLOW "\033[1;33m"
#define RESET   "\033[0m"



// Helper to verify target is possible (by GENERATING it)
int generate_solvable_target(int *nums, int count) {
    // Make a copy of the pool so we don't mess up the real game
    int pool[20];
    memcpy(pool, nums, count * sizeof(int));
    int current_count = count;
    
    // Perform 3 to 5 random valid operations to build a target
    int steps = (rand() % 3) + 3; // 3, 4, or 5 steps
    
    for(int i=0; i<steps; i++) {
        if (current_count < 2) break;
        
        // Pick two random indices
        int idx1 = rand() % current_count;
        int idx2 = rand() % current_count;
        while (idx1 == idx2) idx2 = rand() % current_count;
        
        int val1 = pool[idx1];
        int val2 = pool[idx2];
        
        // Pick a random safe operator
        int op = rand() % 3; // 0=+, 1=-, 2=* (Skip / to avoid decimals)
        int res = 0;
        
        if (op == 0) res = val1 + val2;
        else if (op == 1) res = abs(val1 - val2); // Avoid negatives
        else if (op == 2) res = val1 * val2;
        
        // Remove used numbers (simulate the game flow)
        // (Simplest way: replace one with result, remove other)
        pool[idx1] = res;
        pool[idx2] = pool[current_count - 1]; // Move last element to empty spot
        current_count--;
    }
    
    // The result at pool[idx1] (or any remaining) is our guaranteed target
    // But let's just pick one of the remaining numbers as the target
    return pool[rand() % current_count];
}


// Structure to save the initial state (for RESET)
typedef struct {
    int nums[20];
    int count;
} GameState;

int find_index(int *pool, int size, int value) {
    for (int i = 0; i < size; i++) {
        if (pool[i] == value) return i;
    }
    return -1;
}

void remove_at(int *pool, int *size, int index) {
    for (int i = index; i < *size - 1; i++) {
        pool[i] = pool[i + 1];
    }
    (*size)--;
}

void play_countdown_step_by_step(char* username, int* score) {
    srand(time(NULL));
    
    // 1. DECLARE variables first!
    int available[20]; 
    int count = 6;

    // 2. FILL the array
    for(int i=0; i<6; i++) {
        available[i] = (rand() % 10) + 1;
        if (i == 5) available[i] = 25; 
    }

    // 3. GENERATE target using the filled array
    // Now 'available' actually contains numbers, so the generator works
    int target = generate_solvable_target(available, count);
    
    
    // Save State for RESET
    GameState initial_state;
    memcpy(initial_state.nums, available, sizeof(available));
    initial_state.count = count;

    printf("\n" C_CYAN "   :: COUNTDOWN AUTO-CALC ::" RESET "\n");
    printf("   [Commands]: 'R' to Reset | 'Q' to Quit\n\n");
    printf("   Target: " C_RED "%d" RESET "\n\n", target);


    while (1) {
        // Show Pool
        printf(C_YELLOW "   [POOL]: " RESET);
        for(int i=0; i<count; i++) printf("%d ", available[i]);
        printf("\n");

        printf("   > Enter operation (e.g. 25 * 5) or 'R' to Reset: ");
        
        // Read Input: We read as string first to check for "R"
        int op1, op2;
        char op;
        char input_buffer[50];
        
        // Clean buffer loop (fixes the skipped input bug)
        // fflush(stdin); // Not standard in Linux, using fgets is safer
        
        if (fgets(input_buffer, 50, stdin) == NULL) continue;
        // --- QUIT OPTION ---
        // Check for 'q', 'Q', or "exit"
        if (input_buffer[0] == 'q' || input_buffer[0] == 'Q' || strncmp(input_buffer, "exit", 4) == 0) {
            printf("\n" C_RED "   >> MISSION ABORTED. Returning to base..." RESET "\n");
            
            
            // Break loop to exit function
            break; 
        }

        // Check for RESET
        if (input_buffer[0] == 'R' || input_buffer[0] == 'r') {
            memcpy(available, initial_state.nums, sizeof(initial_state.nums));
            count = initial_state.count;
            printf(C_CYAN "   >> RESETTING NUMBERS..." RESET "\n\n");
            continue;
        }
        
        // Parse: "25 * 5"
        // sscanf parses the string inside input_buffer
        int parsed = sscanf(input_buffer, "%d %c %d", &op1, &op, &op2);
        
        if (parsed != 3) {
            printf(C_RED "   >> Invalid format. Use: 25 * 5" RESET "\n");
            continue;
        }

        // 1. Check Existence
        int idx1 = find_index(available, count, op1);
        if (idx1 == -1) {
            printf(C_RED "   >> Number %d not in pool!" RESET "\n", op1);
            continue;
        }
        
        // Temp remove to find second
        int temp_val = available[idx1];
        available[idx1] = -999; // Mark as used temporarily
        
        int idx2 = find_index(available, count, op2);
        available[idx1] = temp_val; // Restore
        
        if (idx2 == -1) {
            printf(C_RED "   >> Number %d not in pool!" RESET "\n", op2);
            continue;
        }

        // 2. Perform Calculation
        int res = 0;
        int valid = 1;
        switch(op) {
            case '+': res = op1 + op2; break;
            case '-': res = op1 - op2; break;
            case '*': res = op1 * op2; break;
            case '/': 
                if (op2 != 0 && op1 % op2 == 0) res = op1 / op2; 
                else {
                    printf(C_RED "   >> Invalid division (must be exact)!" RESET "\n");
                    valid = 0;
                }
                break;
            default:
                printf(C_RED "   >> Unknown operator." RESET "\n");
                valid = 0;
        }

        if (!valid) continue;

        // 3. Success: Update Pool
        printf(C_GREEN "   >> RESULT: %d" RESET "\n\n", res);
        
        // Remove old numbers (careful with indices shifting)
        if (idx1 > idx2) {
            remove_at(available, &count, idx1);
            remove_at(available, &count, idx2);
        } else {
            remove_at(available, &count, idx2);
            remove_at(available, &count, idx1);
        }
        
        // Add result
        available[count] = res;
        count++;

        // 4. Check Win Condition Immediately
        if (res == target) {
            printf("\n" C_GREEN "   >> TARGET REACHED! YOU WIN!" RESET "\n");
            *score += 50;
            printf("   (Press Enter to return to menu)");
            getchar();
            break; // Exit game loop
        }
    }
}
