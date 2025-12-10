#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For sleep()


// MAKE SURE THESE FILENAMES MATCH YOUR ACTUAL FILES
#include "countdown.h" 
#include "tache2proj.h"
#include "leaderboard.h"
#include "ROBOTMAZE.h"
#include "TRIULTIMATE.h"


// --- COLORS ---
#define C_RED     "\033[1;31m"
#define C_YELLOW  "\033[1;33m"
#define C_GREEN   "\033[1;32m"
#define C_CYAN    "\033[1;36m"
#define C_BLUE    "\033[1;34m"
#define C_PURPLE  "\033[1;35m"
#define C_WHITE   "\033[1;37m"
#define C_GRAY    "\033[1;90m"
#define RESET     "\033[0m"
#define BOLD      "\033[1m"

// --- ICONS ---
#define ICON_USER   " "
#define ICON_KEY    " "
#define ICON_MATH   " "
#define ICON_BRAIN  " "
#define ICON_ROBOT  " "
#define ICON_SORT   " "
#define ICON_EXIT   " "
#define ICON_STAR   " "
// --- ICONS ---

#define ICON_HIST  " "

// Global User Variable
char USERNAME[100]; 
// Robust Clear Function
void clear() {
    // ANSI Escape Codes: 
    // \033[H  -> Move cursor to home (top-left)
    // \033[2J -> Clear entire screen
    // \033[3J -> Clear scrollback buffer (optional, good for "fresh" look)
    printf("\033[H\033[2J\033[3J");
}



// --- STEP 1: CAPTURE NAME ---
void perform_login() {
    clear();
    // Header
    printf(C_RED    "    ___                         \n");
    printf(C_YELLOW "   /   |  ________  ____  ____ \n");
    printf(C_GREEN  "  / /| | / ___/ _ \\/ __ \\/ __ \\\n");
    printf(C_CYAN   " / ___ |/ /  /  __/ / / / /_/ /\n");
    printf(C_BLUE   "/_/  |_/_/   \\___/_/ /_/\\__,_/ \n");
    printf(RESET);
    printf("      " C_PURPLE "::" C_WHITE " ALGORITHMIC " C_CYAN "CHALLENGE " C_GREEN "SYSTEM " C_PURPLE "::" RESET "\n\n");
    
    printf("\n\n\n");
    printf(C_BLUE "      :: SYSTEM AUTHENTICATION ::" RESET "\n\n");
    printf(C_CYAN "      Please identify yourself." RESET "\n");
    printf(C_GRAY "      -------------------------" RESET "\n\n");
    
    printf("      " ICON_USER C_GREEN "Username: " RESET);
    
    if (fgets(USERNAME, sizeof(USERNAME), stdin) != NULL) {
        size_t len = strlen(USERNAME);
        if (len > 0 && USERNAME[len-1] == '\n') {
            USERNAME[len-1] = '\0';
        }
    }
    
    if (strlen(USERNAME) < 1) strcpy(USERNAME, "Guest");

    printf("\n");
    printf(C_YELLOW "      >> Verifying identity..." RESET "\n");
    sleep(1); 
    printf(C_GREEN "      >> ACCESS GRANTED. Welcome, %s." RESET "\n", USERNAME);
    sleep(1); 
}

// --- HELPER FOR MENU ITEMS ---
void print_item(char* key, char* icon, char* name, char* desc, char* color) {
    printf(C_GRAY "   │" RESET "  %s[%s]" RESET "  %s%-2s" RESET " %-15s " C_GRAY "%-16s" RESET C_GRAY "│" RESET "\n", 
           color, key, color, icon, name, desc);
}

// --- STEP 2: SHOW DASHBOARD ---
void print_dashboard(int current_score) {
    // 1. Determine Rank based on score
    char *rank_title;
    char *rank_color;

    if (current_score <= 50) {
        rank_title = "Newbie";
        rank_color = C_GRAY;     
    } else if (current_score <= 100) {
        rank_title = "Intermediate";
        rank_color = C_CYAN;     
    } else {
        rank_title = "Expert";
        rank_color = C_RED;      
    }

    // 2. Print Header
    printf(C_RED    "    ___                         \n");
    printf(C_YELLOW "   /   |  ________  ____  ____ \n");
    printf(C_GREEN  "  / /| | / ___/ _ \\/ __ \\/ __ \\\n");
    printf(C_CYAN   " / ___ |/ /  /  __/ / / / /_/ /\n");
    printf(C_BLUE   "/_/  |_/_/   \\___/_/ /_/\\__,_/ \n");
    printf(RESET);
    printf("      " C_PURPLE "::" C_WHITE " ALGORITHMIC " C_CYAN "CHALLENGE " C_GREEN "SYSTEM " C_PURPLE "::" RESET "\n\n");

    // 3. Print User Bar
    printf("   " ICON_STAR C_YELLOW "User: " C_WHITE "%-10s" RESET 
           "   " C_BLUE "Rank: " "%s%s" RESET 
           "   " C_PURPLE "Score: " C_GREEN "%d" RESET "\n\n", 
           USERNAME, rank_color, rank_title, current_score);

    // 4. Print Menu Box
    printf(C_GRAY "   ╭───────────────────────────────────────────────╮" RESET "\n");
    print_item("1", ICON_MATH,  "Countdown",  "Math Solver", C_RED);
    print_item("2", ICON_BRAIN, "Mastermind", "Crack Code",  C_YELLOW);
    print_item("3", ICON_ROBOT, "Robot Path", "Maze Run",    C_GREEN);
    print_item("4", ICON_SORT,  "Quick Sort", "Optimize",    C_CYAN);
    printf(C_GRAY "   │                                               │" RESET "\n");
    print_item("5", ICON_STAR,  "Leaderboard", "Hall of Fame", C_WHITE);
    print_item("6", ICON_HIST,  "My History",  "Past Log",     C_BLUE);
    printf(C_GRAY "   |                                               |" RESET "\n");
    print_item("0", ICON_EXIT,  "Shutdown",   "Exit System", C_PURPLE);
    printf(C_GRAY "   ╰───────────────────────────────────────────────╯" RESET "\n");

    // 5. Prompt
    printf("\n" C_BLUE "   %s@arena" C_WHITE ":" C_CYAN "~$ " RESET, USERNAME);
}

int main() {
    
    perform_login();
     
    
    // 2. NOW calculate the score based on history file
    // 
    int score = get_total_score(USERNAME);
    
    // Debug print (optional, to verify it works)
    // printf("DEBUG: Loaded Score: %d\n", score); sleep(1);

    
    
    reset_user_history(); 

    int choice = -1;
    char input_buffer[50]; 

    while(1) {
        clear();
        print_dashboard(score);
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL) {
            if (sscanf(input_buffer, "%d", &choice) != 1) {
                choice = -1; 
            }
        }

        if (choice == 0) {
            printf("\n" C_RED "   >> SAVING DATA & LOGGING OFF..." RESET "\n");
            break;
        }

        if (choice == 1) {
            printf("\nLaunching Countdown...\n");
            
            int old_score = score; 
            play_countdown_step_by_step(USERNAME, &score);
            
            // LOG THE MISSION IF SCORE INCREASED
            int gained = score - old_score;
            if (gained > 0) {
                log_mission("Countdown", gained);
            }
        }
        
        if (choice == 2) { 
            printf("\nLaunching Mastermind...\n");
            int old_score = score;
            play_mastermind(USERNAME, &score);

            // LOG THE MISSION IF SCORE INCREASED
            int gained = score - old_score;
            if (gained > 0) {
                log_mission("Mastermind", gained);
            }
        }
        
        // In interface.c
if (choice == 3) {
    printf("\nLaunching Robot...\n");
    int old_score = score;
    play_robot_maze(USERNAME, &score); // Call the function from ROBOTMAZE.c
    
    int gained = score - old_score;
    if (gained > 0) {
        log_mission("Robot Path", gained);
    }
}

    
if (choice == 4) {
    printf("\nLaunching Ultimate Sort...\n");
    sleep(1);

    int old_score = score;
    
    // Call the game function
    play_sorting_game(USERNAME, &score);

    // Log if points were won
    int gained = score - old_score;
    if (gained > 0) {
        log_mission("Ultimate Sort", gained);
    }
}

    
        if (choice == 5) {
            show_leaderboard();
        }
        else if (choice == 6) {
                show_user_history(); // New Function
            }
    }
    return 0;
}
