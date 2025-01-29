#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

void initialize_editor() {
    initscr();              // Initialize the screen
    cbreak();               // Disable line buffering
    noecho();               // Don't display input characters
    curs_set(0);            // Hide the cursor
    keypad(stdscr, TRUE);   // Enable special keys like arrows
    refresh();
}

void cleanup_editor() {
    endwin();               // End ncurses mode
}

int main() {
    initialize_editor();
    printw("Welcome to Vaultedit! Press 'q' to quit.\n");
    refresh();
    getch(); // Wait for the user to press a key
    cleanup_editor();
    return 0;
}

