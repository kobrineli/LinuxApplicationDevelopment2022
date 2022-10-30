#include <stdio.h>
#include <stdlib.h>

void
help() {
    printf("range usage:\n");
    printf("./range N -- print numbers from 0 to N - 1\n");
    printf("./range M N -- print numbers from M to N - 1\n");
    printf("./range M N S -- print numbers M, M + S, M + 2S to N - 1\n");
}

void
range(long limit, long init, long step) {
    for (long i = init; i < limit; i += step) {
        printf("%ld\n", i);
    }
}

int
main(int argc, char **argv) {
    switch (argc) {
        case 1:
        {
            help();
            break;
        }
        case 2:
        {
            range(strtol(argv[1], 0, 10), 0, 1);
            break;
        }
        case 3:
        {
            range(strtol(argv[2], 0, 10), strtol(argv[1], 0, 10), 1);
            break;
        }
        case 4:
        {
            range(strtol(argv[2], 0, 10), strtol(argv[1], 0, 10),
                    strtol(argv[3], 0, 10));
            break;
        }
    }

    return 0;
}
