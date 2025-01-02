#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <direct.h>
#include <windows.h>

#define MAX_ROWS 20
#define MAX_COLS 20
#define WALL '#'
#define FLOOR ' '
#define GOAL '.'
#define BOX '$'
#define PLAYER '@'
#define BOX_ON_GOAL '*'
#define PLAYER_ON_GOAL '+'

const char *levels[] = {"level1.txt", "level2.txt", "level3.txt" ,"level4.txt","level5.txt"};
const int num_levels = 5;

char map_grid[MAX_ROWS][MAX_COLS];
int rows = 0, cols = 0;
int player_x = 0, player_y = 0;

int load_level(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open level file: %s\n", filename);
        perror("Error");
        return 0;
    }
    char line[MAX_COLS + 2];
    rows = 0;
    cols = 0;
    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        strcpy(map_grid[rows], line);
        if (len > cols) cols = len;
        for (int i = 0; i < len; i++) {
            if (map_grid[rows][i] == PLAYER || map_grid[rows][i] == PLAYER_ON_GOAL) {
                player_y = rows;
                player_x = i;
            }
        }
        rows++;
        if (rows >= MAX_ROWS) break;
    }
    fclose(file);
    return 1;
}

void display_map() {
    system("cls");
    for (int i = 0; i < rows; i++) {
        printf("%s\n", map_grid[i]);
    }
}

int is_completed() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map_grid[i][j] == BOX) return 0;
        }
    }
    return 1;
}

void move_player(int dx, int dy) {
    int new_x = player_x + dx;
    int new_y = player_y + dy;
    if (new_y < 0 || new_y >= rows || new_x < 0 || new_x >= cols) return;
    char dest = map_grid[new_y][new_x];
    if (dest == WALL) return;
    if (dest == BOX || dest == BOX_ON_GOAL) {
        int box_new_x = new_x + dx;
        int box_new_y = new_y + dy;
        if (box_new_y < 0 || box_new_y >= rows || box_new_x < 0 || box_new_x >= cols) return;
        char box_dest = map_grid[box_new_y][box_new_x];
        if (box_dest == FLOOR || box_dest == GOAL) {
            if (box_dest == FLOOR) {
                map_grid[box_new_y][box_new_x] = BOX;
            } else if (box_dest == GOAL) {
                map_grid[box_new_y][box_new_x] = BOX_ON_GOAL;
            }
            if (dest == BOX) {
                map_grid[new_y][new_x] = FLOOR;
            } else if (dest == BOX_ON_GOAL) {
                map_grid[new_y][new_x] = GOAL;
            }
        } else {
            return;
        }
    }
    if (map_grid[player_y][player_x] == PLAYER) {
        map_grid[player_y][player_x] = FLOOR;
    } else if (map_grid[player_y][player_x] == PLAYER_ON_GOAL) {
        map_grid[player_y][player_x] = GOAL;
    }
    if (map_grid[new_y][new_x] == FLOOR) {
        map_grid[new_y][new_x] = PLAYER;
    } else if (map_grid[new_y][new_x] == GOAL) {
        map_grid[new_y][new_x] = PLAYER_ON_GOAL;
    }
    player_x = new_x;
    player_y = new_y;
}

int display_menu() {
    system("cls");
    printf("Sokoban Game\n");
    printf("Select a level:\n");
    for (int i = 0; i < num_levels; i++) {
        printf("%d. %s\n", i + 1, levels[i]);
    }
    printf("0. Quit\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);
    while (getchar() != '\n');
    if (choice == 0) {
        return -1;
    }
    if (choice < 1 || choice > num_levels) {
        return -2;
    }
    return choice - 1;
}

int main() {
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
    while (1) {
        int selected = display_menu();
        if (selected == -1) {
            printf("Exiting game.\n");
            break;
        }
        if (selected == -2) {
            printf("Invalid choice. Press any key to continue.\n");
            _getch();
            continue;
        }
        if (!load_level(levels[selected])) {
            printf("Failed to load level. Press any key to continue.\n");
            _getch();
            continue;
        }
        while (1) {
            display_map();
            if (is_completed()) {
                printf("Level completed!\n");
                Sleep(1000);
                break;
            }
            printf("Use WASD to move, Q to quit to menu.\n");
            char command = 0;
            if (_kbhit()) {
                command = _getch();
                if (command == 224) {
                    command = _getch();
                    switch (command) {
                        case 72:
                            move_player(0, -1);
                            break;
                        case 80:
                            move_player(0, 1);
                            break;
                        case 75:
                            move_player(-1, 0);
                            break;
                        case 77:
                            move_player(1, 0);
                            break;
                        default:
                            break;
                    }
                } else {
                    switch (command) {
                        case 'w':
                        case 'W':
                            move_player(0, -1);
                            break;
                        case 's':
                        case 'S':
                            move_player(0, 1);
                            break;
                        case 'a':
                        case 'A':
                            move_player(-1, 0);
                            break;
                        case 'd':
                        case 'D':
                            move_player(1, 0);
                            break;
                        case 'q':
                        case 'Q':
                            printf("Returning to menu.\n");
                            Sleep(500);
                            goto level_end;
                        default:
                            break;
                    }
                }
            }
            Sleep(50);
        }
        level_end:;
    }
    return 0;
}
