#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define KEY_ESCAPE 27
#define KEY_SPACE 32
#define STRING_CHUNK 128

void
init_screen(WINDOW *win, char *filename) {
    noecho();
    keypad(stdscr, TRUE);
    cbreak();

    curs_set(0);
    printw("File: %s;", filename);
    refresh();

    box(win, 0, 0);
    wrefresh(win);
}

char **
init_page(size_t page_size, FILE *file) {
    // TODO: process longer strings.
    char **page = calloc(page_size, sizeof(*page));
    if (!page) {
        fprintf(stderr, "Error allocating space for page lines.\n");
        return NULL;
    }

    for (size_t i = 0; i < page_size; ++i) {
        page[i] = calloc(STRING_CHUNK, sizeof(**page));
        if (!page[i]) {
            fprintf(stderr, "Error allocating space for line.\n");
            for (size_t j = 0; j < i; ++j) {
                free(page[j]);
            }
            free(page);
            return NULL;
        }
        fgets(page[i], STRING_CHUNK, file);
    }

    return page;
}

int
process_loop(WINDOW *win, char **page, size_t page_size, FILE *file) {
    // TODO: process errors and key right/left pressing.
    unsigned lineoff = 0;
    for (;;) {
        werase(win);
        for (size_t i = 0; i < page_size; ++i) {
            mvwprintw(win, i + 1, 1, "%3u: %s", i + lineoff + 1, page[i]);
        }
        box(win, 0, 0);
        wrefresh(win);

        int ch = getch();
        int end = 0;
        switch(ch) {
            case KEY_ESCAPE:
            {
                end = 1;
                break;
            }
            case KEY_SPACE:
            {
                char *new_line = calloc(STRING_CHUNK, sizeof(*new_line));
                if (!fgets(new_line, STRING_CHUNK, file)) {
                    break;
                }
                ++lineoff;
                free(page[0]);
                for (size_t i = 0; i < page_size - 1; ++i) {
                    page[i] = page[i + 1];
                }
                page[page_size - 1] = new_line;
            }
        }

        if (end) {
            break;
        }
    }

    return 0;
}

int
main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Error: specify the path to the file.\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error opening the file.");
        return 1;
    }

    if (!initscr()) {
        fprintf(stderr, "Error initializing ncurses.\n");
        return 1;
    }

    const unsigned win_lines = LINES - 4;
    const unsigned win_cols = COLS - 4;
    const unsigned win_offlines = 2;
    const unsigned win_offcols = 2;
    WINDOW *win = newwin(win_lines, win_cols, win_offlines, win_offcols);

    init_screen(win, argv[1]);

    const size_t page_size = win_lines - 2;
    char **page = init_page(page_size, file);
    if (!page) {
        return 1;
    }

    process_loop(win, page, page_size, file);

    for (size_t i = 0; i < page_size; ++i) {
        free(page[i]);
    }
    free(page);

    fclose(file);
    endwin();

    return 0;
}
