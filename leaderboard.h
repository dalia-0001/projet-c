/*
    LEADERBOARD.c
    Task 6: Score System & Ranking
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern char USERNAME[100];

#define HISTORY_FILE "history_log.txt"

// --- COLORS ---
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

typedef struct {
    char name[100];
    int total_score;
} PlayerStats;

// --- SAVE MISSION ---
void log_mission(char* game_name, int points_earned) {
    FILE *f = fopen(HISTORY_FILE, "a");
    if (f) {
        fprintf(f, "%s;%s;%d\n", USERNAME, game_name, points_earned);
        fclose(f);
    }
}

// --- RESET HISTORY ---
// Kept empty to preserve data across runs
void reset_user_history() { }
// --- NEW FUNCTION: LOAD TOTAL SCORE ---
int get_total_score(char* target_user) {
    FILE *f = fopen(HISTORY_FILE, "r");
    if (!f) return 0; // No history file = 0 points

    char line[256], u[100], g[100];
    int p;
    int total = 0;

    // Read every line and sum points for THIS user
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%[^;];%[^;];%d", u, g, &p) == 3) {
            // If the name matches the current user, add points
            if (strcmp(u, target_user) == 0) {
                total += p;
            }
        }
    }
    fclose(f);
    return total;
}


// --- FEATURE 1: SHOW GLOBAL RANKING ---
void show_leaderboard() {
    FILE *f = fopen(HISTORY_FILE, "r");
    printf("\033[H\033[J"); // Clear screen
    
    printf(C_YELLOW "      .:: HALL OF FAME ::." RESET "\n");
    printf(C_GRAY "  ------------------------------" RESET "\n");
    printf("  " C_WHITE "%-4s %-15s %s" RESET "\n", "RK", "USERNAME", "TOTAL");
    printf(C_GRAY "  ------------------------------" RESET "\n");

    if (!f) {
        printf("\n  " C_GRAY "(No data found)" RESET "\n");
        printf("\n  (Press Enter to return)");
        while (getchar() != '\n'); getchar();
        return;
    }

    PlayerStats players[100]; 
    int player_count = 0;
    char line[256], u[100], g[100];
    int p;

    // Aggregate scores
    while (fgets(line, sizeof(line), f)) {
        if (sscanf(line, "%[^;];%[^;];%d", u, g, &p) == 3) {
            int found = 0;
            for(int i=0; i<player_count; i++) {
                if (strcmp(players[i].name, u) == 0) {
                    players[i].total_score += p;
                    found = 1; 
                    break;
                }
            }
            if (!found && player_count < 100) {
                strcpy(players[player_count].name, u);
                players[player_count].total_score = p;
                player_count++;
            }
        }
    }
    fclose(f);

    // Sort
    for (int i = 0; i < player_count - 1; i++) {
        for (int j = 0; j < player_count - i - 1; j++) {
            if (players[j].total_score < players[j+1].total_score) {
                PlayerStats temp = players[j];
                players[j] = players[j+1];
                players[j+1] = temp;
            }
        }
    }

    // Display
    int user_rank = -1;
    for (int i = 0; i < player_count; i++) {
        char* color = C_WHITE;
        if (i == 0) color = C_YELLOW;
        else if (i == 1) color = C_CYAN;
        else if (i == 2) color = C_RED;

        // Highlight Current User
        if (strcmp(players[i].name, USERNAME) == 0) {
            printf(C_BLUE " ->" RESET " %s%-2d %-15s %d pts" RESET "\n", color, i+1, players[i].name, players[i].total_score);
            user_rank = i + 1;
        } else {
            printf("    %s%-2d %-15s %d pts" RESET "\n", color, i+1, players[i].name, players[i].total_score);
        }
    }
    
    if (user_rank != -1) {
        printf("\n  " C_GREEN ">> You are ranked #%d!" RESET "\n", user_rank);
    } else {
        printf("\n  " C_GRAY "(You haven't played yet)" RESET "\n");
    }

    printf(C_GRAY "  ------------------------------" RESET "\n");
    printf("\n  (Press Enter to return)");
    int c; while ((c = getchar()) != '\n' && c != EOF); 
    char buffer[10]; fgets(buffer, sizeof(buffer), stdin);
}

// --- FEATURE 2: SHOW PERSONAL HISTORY ---
void show_user_history() {
    FILE *f = fopen(HISTORY_FILE, "r");
    printf("\033[H\033[J");
    
    printf(C_CYAN "      .:: PERSONAL LOG ::." RESET "\n");
    printf(C_GRAY "  Displaying history for: " C_WHITE "%s" RESET "\n", USERNAME);
    printf(C_GRAY "  ------------------------------" RESET "\n");
    printf("  " C_WHITE "%-15s %s" RESET "\n", "MISSION", "RESULT");
    printf(C_GRAY "  ------------------------------" RESET "\n");

    if (!f) {
        printf("\n  (No history found)\n");
    } else {
        char line[256], u[100], g[100];
        int p;
        int found = 0;
        while (fgets(line, sizeof(line), f)) {
            if (sscanf(line, "%[^;];%[^;];%d", u, g, &p) == 3) {
                if (strcmp(u, USERNAME) == 0) {
                    printf("  %-15s " C_GREEN "+%d pts" RESET "\n", g, p);
                    found = 1;
                }
            }
        }
        if (!found) printf("  (No games played yet)\n");
        fclose(f);
    }
    printf(C_GRAY "  ------------------------------" RESET "\n");
    
    printf("\n  (Press Enter to return)");
    int c; while ((c = getchar()) != '\n' && c != EOF);
    char buffer[10]; fgets(buffer, sizeof(buffer), stdin);
}
