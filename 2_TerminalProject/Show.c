#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
fill_buf(FILE *file, size_t page_size, size_t *sz) {
    char **buf = calloc(page_size, sizeof(*buf));
    if (!buf) {
        fprintf(stderr, "Error allocating space for file buffer.\n");
        return NULL;
    }

    char *s = calloc(STRING_CHUNK, sizeof(*s));
    size_t filesz = 0;
    size_t coef = 1;
    while (fgets(s, STRING_CHUNK, file)) {
        ++filesz;
        if (filesz > coef * page_size) {
            ++coef;
            buf = realloc(buf, coef * page_size * sizeof(buf));
        }

        size_t len = strlen(s);
        while (len >= 2 && s[len - 1] != '\n') {
            s = realloc(s, len + STRING_CHUNK);
            if (!fgets(s + len, STRING_CHUNK, file)) {
                break;
            }
            len = strlen(s);
        }

        buf[filesz - 1] = s;
        s = calloc(STRING_CHUNK, sizeof(*s));
    }
    free(s);

    *sz = filesz > page_size ? filesz : page_size;
    return buf;
}

int
process_loop(WINDOW *win, char **buf, size_t page_size, size_t total_size) {
    // TODO: handle key_right/left pressing.
    unsigned lineoff = 0;
    for (;;) {
        werase(win);
        for (size_t i = 0; i < page_size; ++i) {
            if (i + lineoff < total_size) {
                mvwprintw(win, i + 1, 1, "%3u: %s", i + lineoff + 1,
                        buf[i + lineoff]);
            }
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
            case KEY_DOWN:
            {
                if (lineoff < total_size - page_size) {
                    ++lineoff;
                }
                break;
            }
            case KEY_UP:
            {
                if (lineoff) {
                    --lineoff;
                }
                break;
            }
            case KEY_NPAGE:
            {
                lineoff += page_size;
                if (lineoff >= total_size - page_size) {
                    lineoff = total_size - page_size;
                }
                break;
            }
            case KEY_PPAGE:
            {
                if (lineoff <= page_size) {
                    lineoff = 0;
                } else {
                    lineoff -= page_size;
                }
                break;
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
    size_t total_size = 0;
    char **buf = fill_buf(file, page_size, &total_size);
    if (!buf) {
        return 1;
    }
    fclose(file);

    process_loop(win, buf, page_size, total_size);

    for (size_t i = 0; i < total_size; ++i) {
        free(buf[i]);
    }
    free(buf);

    endwin();

    return 0;
}
