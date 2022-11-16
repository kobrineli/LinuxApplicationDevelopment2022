#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include "rhash.h"

ssize_t
invite(char **restrict lineptr, size_t *restrict n, FILE *restrict stream) {
    if (*lineptr) {
        free(*lineptr);
        *lineptr = NULL;
    }
#ifndef READLINE
    printf("> ");
    int res = 0;
    if ((res = getline(lineptr, n, stream)) == -1) {
        free(*lineptr);
        return res;
    }
    (*lineptr)[strlen(*lineptr) - 1] = 0;
    return res;
#else
    printf("$ ");
    char *res = readline(NULL);
    if (!res) {
        return -1;
    }
    if (!res[0]) {
        printf("\n");
    }
    *lineptr = res;
    return 0;
#endif
}

unsigned
get_hash_type(const char *hash_name) {
    if (!strcasecmp(hash_name, "md5")) {
        return RHASH_MD5;
    } else if (!strcasecmp(hash_name, "sha1")) {
        return RHASH_SHA1;
    } else  {
        return RHASH_TTH;
    }
}

int
main() {
    char *line = NULL;
    size_t sz = 0;
    char delim = ' ';

    unsigned char digest[64];
    char output[130];
    rhash_library_init();

    while (invite(&line, &sz, stdin) != -1) {
        char *hash_name = strtok(line, &delim);
        if (!hash_name) {
            continue;
        }

        char *arg = strtok(NULL, &delim);
        if (!arg) {
            fprintf(stderr, "Error: the second argument is not specified.\n");
            continue;
        }

        if (strcasecmp(hash_name, "md5") && strcasecmp(hash_name, "sha1") &&
                strcasecmp(hash_name, "tth")) {
            fprintf(stderr, "Error: unrecognized hash function.\n");
            continue;
        }

        unsigned hash_type = get_hash_type(hash_name);
        unsigned out_type = islower(hash_name[0]) ? RHPR_BASE64 : RHPR_HEX;
        if (arg[0] == '"') {
            int res = rhash_msg(hash_type, &arg[1], strlen(&arg[1]), digest);
            if (res < 0) {
                fprintf(stderr, "LibRHash error: %s\n", strerror(errno));
                continue;
            }
        } else {
            int res = rhash_file(hash_type, arg, digest);
            if (res < 0) {
                fprintf(stderr, "LibRHash error: %s %s\n", arg,
                        strerror(errno));
                continue;
            }
        }

        rhash_print_bytes(output, digest, rhash_get_digest_size(hash_type),
                out_type);

        printf("%s\n", output);

        free(line);
        line = NULL;
    }

    if (errno) {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }
    return 0;
}
