#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 1000
#define MAX_COLS 256

typedef enum { EDIT_MODE, VIEW_MODE, AI_MODE } Mode;

char buffer[MAX_ROWS][MAX_COLS] = {0}; 
int row = 0, col = 0;
Mode current_mode = EDIT_MODE;
char filename[128] = {0};

void show_welcome_screen();
void show_status_bar();
void show_status(const char *message);
void save_file();
void load_file(const char *fname);
void handle_input(WINDOW *win);
void update_screen(WINDOW *win);
void prompt_filename();

int main(int argc, char *argv[]) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1); 
    refresh();

    if (argc > 1) {
        strncpy(filename, argv[1], sizeof(filename) - 1);
        load_file(filename);
    } else {
        show_welcome_screen();
        getch();
    }

    clear();
    refresh();
    
    WINDOW *win = newwin(LINES - 2, COLS, 0, 0);
    keypad(win, TRUE);

    update_screen(win);
    handle_input(win);

    endwin();
    return 0;
}

void show_welcome_screen() {
    clear();
    mvprintw(LINES / 2 - 2, (COLS - 24) / 2, "Welcome to VaultEdit!");
    mvprintw(LINES / 2 - 1, (COLS - 12) / 2, "Version 0.1");
    mvprintw(LINES / 2, (COLS - 17) / 2, "Created by t2ow");
    mvprintw(LINES / 2 + 1, (COLS - 30) / 2, "VaultEdit is completely open source.");
    mvprintw(LINES / 2 + 3, (COLS - 27) / 2, "Press any key to continue...");
    refresh();
}

void show_status_bar() {
    move(LINES - 1, 0);
    clrtoeol();
    switch (current_mode) {
        case EDIT_MODE:
            attron(A_BOLD);
            printw("[EDIT MODE]");
            attroff(A_BOLD);
            break;
        case VIEW_MODE:
            attron(A_BOLD);
            printw("[VIEW MODE]");
            attroff(A_BOLD);
            break;
        case AI_MODE:
            attron(A_BOLD);
            printw("[AI MODE]");
            attroff(A_BOLD);
            break;
    }
    refresh();
}

void show_status(const char *message) {
    move(LINES - 1, 20);
    clrtoeol();
    mvprintw(LINES - 1, 20, "%s", message);
    refresh();
}

void save_file() {
    if (strlen(filename) == 0) {
        prompt_filename();
    }

    if (strlen(filename) == 0) {
        show_status("Save cancelled.");
        return;
    }

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        show_status("Error: Unable to save file!");
        return;
    }

    for (int i = 0; i < MAX_ROWS && buffer[i][0] != '\0'; i++)
        fprintf(fp, "%s\n", buffer[i]);

    fclose(fp);
    show_status("File saved successfully!");
}

void prompt_filename() {
    echo();
    mvprintw(LINES - 2, 0, "Enter filename: ");
    refresh();
    getnstr(filename, sizeof(filename) - 1);
    noecho();
}

void load_file(const char *fname) {
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        show_status("New file. No previous data.");
        return;
    }

    int r = 0;
    while (fgets(buffer[r], MAX_COLS, fp) && r < MAX_ROWS) {
        buffer[r][strcspn(buffer[r], "\n")] = '\0';
        r++;
    }

    fclose(fp);
}

void update_screen(WINDOW *win) {
    werase(win);
    
    for (int i = 0; i < MAX_ROWS && buffer[i][0] != '\0'; i++)
        mvwprintw(win, i, 0, "%s", buffer[i]);

    wmove(win, row, col);
    wrefresh(win);
    show_status_bar();
}

void handle_input(WINDOW *win) {
    int ch;
    while ((ch = wgetch(win)) != 27) { // ESC to exit
        switch (ch) {
            case 10:  // Enter key
                row = (row + 1 < MAX_ROWS) ? row + 1 : row;
                col = 0;
                break;
            case KEY_BACKSPACE:
            case 127:
                if (col > 0) buffer[row][--col] = '\0';
                break;
            case KEY_LEFT:
                if (col > 0) col--;
                break;
            case KEY_RIGHT:
                if (col < (int)strlen(buffer[row])) col++;
                break;
            case KEY_UP:
                if (row > 0) row--;
                break;
            case KEY_DOWN:
                if (row < MAX_ROWS - 1) row++;
                break;
            case 1:  // ^A - AI Mode
                current_mode = AI_MODE;
                show_status("AI Mode (Not implemented)");
                break;
            case 5:  // ^E - Edit Mode
                current_mode = EDIT_MODE;
                show_status("Edit Mode");
                break;
            case 22: // ^V - View Mode
                current_mode = VIEW_MODE;
                show_status("View Mode (Read-only)");
                break;
            case 19: // ^S - Save File
                save_file();
                break;
            case 24: // ^X - Save and Exit
                if (strlen(filename) == 0) {
                    show_status("No filename! Use ^S to save first.");
                } else {
                    save_file();
                    return;
                }
                break;
            default:
                if (current_mode == EDIT_MODE && col < MAX_COLS - 1) {
                    buffer[row][col++] = ch;
                    buffer[row][col] = '\0';
                }
        }

        update_screen(win);
    }
}