#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

#define MAX_BUFFER 1024  // Max characters for the text buffer

enum Mode {
    EDIT_MODE,
    VIEW_MODE,
    AI_MODE
};

enum Mode current_mode = EDIT_MODE;  // Default mode
char buffer[MAX_BUFFER] = {0};       // Holds text content
int cursor_pos = 0;                  // Cursor position in buffer

void draw_status_bar() {
    move(LINES - 1, 0);
    clrtoeol();
    attron(A_REVERSE);
    if (current_mode == EDIT_MODE)
        printw(" EDIT MODE ");
    else if (current_mode == VIEW_MODE)
        printw(" VIEW MODE ");
    else if (current_mode == AI_MODE)
        printw(" AI MODE ");
    attroff(A_REVERSE);
}

void initialize_editor() {
    initscr();
    cbreak();
    noecho();
    curs_set(1);  // Show cursor in edit mode
    keypad(stdscr, TRUE);
    refresh();
}

void cleanup_editor() {
    endwin();
}

void render_editor() {
    clear();
    mvprintw(0, 0, "%s", buffer); // Display the text buffer
    draw_status_bar();
    refresh();
}

void handle_input(int ch) {
    if (current_mode == EDIT_MODE) {
        if (ch == KEY_BACKSPACE || ch == 127) { // Handle backspace
            if (cursor_pos > 0) {
                buffer[--cursor_pos] = '\0';
            }
        } else if (cursor_pos < MAX_BUFFER - 1) {
            buffer[cursor_pos++] = ch;
            buffer[cursor_pos] = '\0';
        }
    }
}

int main() {
    initialize_editor();
    render_editor();

    int ch;
    while ((ch = getch()) != 'q') {  // Press 'q' to quit
        switch (ch) {
            case 1:  // ^A (Ctrl+A) -> AI Mode
                current_mode = AI_MODE;
                break;
            case 5:  // ^E (Ctrl+E) -> Edit Mode
                current_mode = EDIT_MODE;
                curs_set(1);  // Show cursor
                break;
            case 22: // ^V (Ctrl+V) -> View Mode
                current_mode = VIEW_MODE;
                curs_set(0);  // Hide cursor
                break;
            default:
                handle_input(ch);
                break;
        }
        render_editor();
    }

    cleanup_editor();
    return 0;
}
