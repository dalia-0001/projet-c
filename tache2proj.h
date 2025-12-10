#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// COLORS
#define C_GREEN "\033[1;32m"
#define C_RED   "\033[1;31m"
#define C_CYAN  "\033[1;36m"
#define C_YELLOW "\033[1;33m"
#define RESET   "\033[0m"

#define CODE_LEN 4

// Generate Secret Code
void generer_code_secret(int code[]) {
    for (int i = 0; i < CODE_LEN; i++) {
        code[i] = rand() % 10; // 0-9
    }
    // Optional: Print for debugging (cheat mode)
    // printf("DEBUG: %d%d%d%d\n", code[0], code[1], code[2], code[3]);
}

// Check logic (Your original logic, slightly cleaned)
void verifier_mastermind(int secret[], int essai[], int *bien, int *mal) {
    int freq_secret[10] = {0};
    int freq_essai[10] = {0};
    
    *bien = 0;
    *mal = 0;
    
    // 1. Count "Well Placed" (Bulls)
    for (int i = 0; i < CODE_LEN; i++) {
        if (essai[i] == secret[i]) {
            (*bien)++;
        } else {
            // Only count frequencies for MISMATCHED numbers
            freq_secret[secret[i]]++;
            freq_essai[essai[i]]++;
        }
    }

    // 2. Count "Misplaced" (Cows) using frequencies
    for (int i = 0; i < 10; i++) {
        *mal += (freq_secret[i] < freq_essai[i]) ? freq_secret[i] : freq_essai[i];
    }
}

void play_mastermind(char* username, int* score) {
    srand(time(NULL));
    int secret[CODE_LEN];
    int guess[CODE_LEN];
    int attempts = 0;
    const int MAX_ATTEMPTS = 12;
    
    generer_code_secret(secret);

    printf("\n" C_CYAN "   :: MASTERMIND ALGORITHM ::" RESET "\n");
    
    printf("   [Commands]: 'R' to Reset | 'Q' to Quit\n\n");
    printf("   CRACK THE CODE: %d digits (0-9).\n", CODE_LEN);
    printf("   Max Attempts: %d\n\n", MAX_ATTEMPTS);

    while (attempts < MAX_ATTEMPTS) {
        attempts++;
        printf("   Attempt " C_YELLOW "#%d" RESET " > Enter code (e.g. 1234): ", attempts);

        // Safe Input Reading
        char buffer[50];
        if (fgets(buffer, 50, stdin) == NULL) continue;
        
        // Check for "Give Up"
        if (buffer[0] == 'q' || buffer[0] == 'Q') {
            printf(C_RED "   >> GIVING UP. The code was: " RESET);
            for(int i=0; i<CODE_LEN; i++) printf("%d", secret[i]);
            printf("\n\n");
            return;
        }

        // Parse string "1234" into int array [1,2,3,4]
        int valid_input = 1;
        int count = 0;
        for (int i = 0; buffer[i] != '\0' && buffer[i] != '\n'; i++) {
            if (!isdigit(buffer[i])) continue; // skip spaces/garbage
            if (count < CODE_LEN) {
                guess[count++] = buffer[i] - '0'; // Convert char '1' to int 1
            }
        }

        if (count != CODE_LEN) {
            printf(C_RED "   >> Invalid input! Please enter exactly %d digits." RESET "\n", CODE_LEN);
            attempts--; // Don't count this attempt
            continue;
        }

        // Verification
        int bien = 0, mal = 0;
        verifier_mastermind(secret, guess, &bien, &mal);

        // Feedback Display
        if (bien == CODE_LEN) {
            printf("\n" C_GREEN "   >> ACCESS GRANTED! CODE CRACKED." RESET "\n");
            
            // Calculate Score (Faster = More Points)
            // Base 50 pts - (attempts * 2)
            int points = 60 - (attempts * 3);
            if (points < 10) points = 10; // Min points
            
            printf("   Attempts: %d | Score: +%d\n", attempts, points);
            *score += points;
            
            printf("   (Press Enter to continue)");
            getchar();
            return;
        } else {
            // Hacker-style feedback
            printf("   Result: " C_GREEN "%d Well Placed" RESET " | " C_RED "%d Misplaced" RESET "\n", bien, mal);
        }
    }

    // Loss Condition
    printf("\n" C_RED "   >> SYSTEM LOCKDOWN. OUT OF ATTEMPTS." RESET "\n");
    printf("   The secret code was: " C_CYAN);
    for(int i=0; i<CODE_LEN; i++) printf("%d", secret[i]);
    printf(RESET "\n   (Press Enter)");
    getchar();
}
