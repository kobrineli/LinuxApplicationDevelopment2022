#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *
ref_to_capture(char *subst, size_t start, size_t end, const char *str,
               size_t *size, size_t *pos) {
    size_t add_len = end - start;
    subst = realloc(subst, *size + add_len);
    *size += add_len;
    for(int i = 0; i < add_len; ++i) {
        subst[*pos + i] = str[start + i];
    }
    *pos += add_len;
    return subst;
}

char *
process_subst(const char *str, const char *subst, size_t nmatch,
              regmatch_t pmatch[]) {
    char *newsubst = calloc(strlen(subst) + 1, sizeof(*newsubst));
    int backslash = 0;
    size_t cursize = strlen(subst) + 1;
    size_t curpos = 0;
    size_t accum = 0;
    size_t paste_capture = 0;
    for (int i = 0; i < strlen(subst); ++i) {
        if (backslash && !isdigit(subst[i])) {
            if (paste_capture) {
                if (accum >= nmatch) {
                    fprintf(stderr,
                            "There is no %lu capture in regex.\n",
                            accum);
                    free(newsubst);
                    return NULL;
                }
                newsubst = ref_to_capture(newsubst, pmatch[accum].rm_so,
                        pmatch[accum].rm_eo, str, &cursize, &curpos);
                backslash = 0;
                paste_capture = 0;
                accum = 0;
            }
        }
        if (subst[i] == '\\') {
            if (!backslash) {
                backslash = 1;
            } else {
                newsubst[curpos] = '\\';
                ++curpos;
                backslash = 0;
            }
            paste_capture = 0;
            accum = 0;
        } else {
            if (!backslash) {
                newsubst[curpos] = subst[i];
                ++curpos;
            } else if (isdigit(subst[i])) {
                accum = accum * 10 + (subst[i] - '0');
                paste_capture = 1;
            } else {
                newsubst[curpos] = subst[i];
                ++curpos;
            }
        }
    }
    if (paste_capture) {
        if (accum >= nmatch) {
            fprintf(stderr,
                    "There is no %lu capture in regex.\n",
                    accum);
            free(newsubst);
            return NULL;
        }
        newsubst = ref_to_capture(newsubst, pmatch[accum].rm_so,
                pmatch[accum].rm_eo, str, &cursize, &curpos);
    }
    newsubst[curpos] = 0;

    return newsubst;
}

int
main(int argc, char **argv) {
    if (argc != 4) {
        printf("Usage: ./esub regexp substitution string");
        return 0;
    }

    regex_t regex;
    char *regstr = argv[1];
    char *subst = argv[2];
    char *str = argv[3];

    int err;
    char errbuf[1024];
    if ((err = regcomp(&regex, argv[1], REG_EXTENDED))) {
        regerror(err, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Error occured:\n");
        fprintf(stderr, "%s\n", errbuf);
        return 1;
    }

    size_t nmatch = 1;
    for (int i = 0; i < strlen(regstr) - 1; ++i) {
        if (regstr[i] == '(') {
            ++nmatch;
        }
    }

    regmatch_t pmatch[nmatch];
    if (!regexec(&regex, str, nmatch, pmatch, 0)) {
        char *newsubst = process_subst(str, subst, nmatch, pmatch);
        if (newsubst) {
            size_t sz = strlen(str) - pmatch[0].rm_eo + pmatch[0].rm_so +
                strlen(newsubst) + 1;
            char *newstr = calloc(sz, sizeof(*newstr));
            for (int i = 0, k = 0; i < strlen(str); ++i, ++k) {
                if (i == pmatch[0].rm_so) {
                    for (int j = 0; j < strlen(newsubst); ++j) {
                        newstr[i + j] = newsubst[j];
                    }
                    k += strlen(newsubst) - 1;
                    i = pmatch[0].rm_eo - 1;
                    continue;
                }
                newstr[k] = str[i];
            }
            printf("%s\n", newstr);
            free(newsubst);
            free(newstr);
        }
    } else {
        printf("%s\n", str);
    }

    regfree(&regex);
    return 0;
}
