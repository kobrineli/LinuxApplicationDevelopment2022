#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libintl.h>
#include <locale.h>

#define _(STRING) gettext(STRING)

int
guess(unsigned low, unsigned high) {
    if (low == high) {
        return high;
    }

    char ans[8];
    unsigned border = (high + low) / 2;

    printf(_("Is your number greater than %u? yes/no\n> "), (high + low) / 2);
    scanf("%s", ans);

    if (!strcmp(ans, _("yes"))) {
        return guess(border + 1, high);
    } else {
        return guess(low, border);
    }
}

int
main() {
    setlocale(LC_ALL, "");
    bindtextdomain("guesser", getenv("PWD"));
    textdomain("guesser");

    printf(_("Think of a number from 1 to 100\n"));
    printf(_("Your number is %u!\n"), guess(1, 100));
    return 0;
}
