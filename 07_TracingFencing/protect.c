#include <string.h>
#include <libgen.h>
#include <fcntl.h>
#include <unistd.h>

int
remove(char *pathname) {
    char *name = basename(pathname);
    if (!strstr(name, "PROTECT")) {
        return unlink(pathname);
    }
    return 0;
}
