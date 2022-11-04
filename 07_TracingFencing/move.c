#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: ./move infile outfile\n");
        return 1;
    }

    FILE *infile = fopen(argv[1], "r");
    if (!infile) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 2;
    }

    FILE *outfile = fopen(argv[2], "w");
    if (!outfile) {
        fprintf(stderr, "%s\n", strerror(errno));
        if (fclose(infile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        return 3;
    }

    if (fseek(infile, 0, SEEK_END) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        if (fclose(infile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        if (fclose(outfile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        if (remove(argv[2]) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        return 4;
    }

    long fsize = ftell(infile);
    if (fsize == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        if (fclose(infile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        if (fclose(outfile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        if (remove(argv[2]) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        return 5;
    }
    rewind(infile);

    char *content = malloc(fsize);
    if (!content) {
        fprintf(stderr, "%s\n", strerror(errno));
        if (fclose(infile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        if (fclose(outfile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        if (remove(argv[2]) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        return 6;
    }

    fread(content, sizeof(*content), fsize, infile);
    if (ferror(infile)) {
        fprintf(stderr, "%s\n", strerror(errno));
        if (fclose(infile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        if (fclose(outfile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        if (remove(argv[2]) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        free(content);
        return 7;
    }

    fwrite(content, sizeof(*content), fsize, outfile);
    if (ferror(outfile)) {
        fprintf(stderr, "%s\n", strerror(errno));
        if (fclose(infile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        if (fclose(outfile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        if (remove(argv[2]) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        free(content);
        return 8;
    }
    free(content);

    if (fclose(infile) == EOF) {
        fprintf(stderr, "%s\n", strerror(errno));
        if (fclose(outfile) == EOF) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        if (remove(argv[2]) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        return 9;
    }

    if (fclose(outfile) == EOF) {
        fprintf(stderr, "%s\n", strerror(errno));
        if (remove(argv[2]) == -1) {
            fprintf(stderr, "%s\n", strerror(errno));
        }
        return 10;
    }

    if (remove(argv[1]) == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 11;
    }

    return 0;
}
