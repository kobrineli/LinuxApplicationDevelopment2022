/*! @mainpage
 *
 *  Usage: ./guesser [OPTIONS]\n
 *  Guess the number with half division method.
 *
 *  Options:\n
 *    -r        Represent numbers in Roman way.\n
 *    --help    Layout help message.
 *
 *  Использование: ./guesser [ОПЦИИ]\n
 *  Угадывание чисел методом половинного деления.
 *
 *  Опции:\n
 *    -r        Представлять числа в римской записи.\n
 *    --help    Вывести сообщение с помощью.\n
 */

#include <argp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libintl.h>
#include <locale.h>

#define _(STRING) gettext(STRING)

//! @file guesser.c

//! Layout the help message
void
help() {
    printf(_("Usage: ./guesser [OPTIONS]\n"));
    printf(_("Guess the number with half division method.\n\n"));
    printf(_("Options:\n"));
    printf(_("  -r        Represent numbers in Roman way.\n"));
    printf(_("  --help    Layout help message.\n\n"));
}

/*! Translate number from Arabic to Roman form.
 *
 *  \param num Number to ber translated.
 *  \return String representation of number Roman form.
 */
const char *
arabic_to_roman(int num) {
    static char *roman_table[] = {"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX",
    "X", "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII", "XIX", "XX",
    "XXI", "XXII", "XXIII", "XXIV", "XXV", "XXVI", "XXVII", "XXVIII", "XXIX", "XXX",
    "XXXI", "XXXII", "XXXIII", "XXXIV", "XXXV", "XXXVI", "XXXVII", "XXXVIII", "XXXIX",
    "XL", "XLI", "XLII", "XLIII", "XLIV", "XLV", "XLVI", "XLVII", "XLVIII", "XLIX",
    "L", "LI", "LII", "LIII", "LIV", "LV", "LVI", "LVII", "LVIII", "LIX", "LX", "LXI",
    "LXII", "LXIII", "LXIV", "LXV", "LXVI", "LXVII", "LXVIII", "LXIX", "LXX", "LXXI",
    "LXXII", "LXXIII", "LXXIV", "LXXV", "LXXVI", "LXXVII", "LXXVIII", "LXXIX", "LXXX",
    "LXXXI", "LXXXII", "LXXXIII", "LXXXIV", "LXXXV", "LXXXVI", "LXXXVII", "LXXXVIII",
    "LXXXIX", "XC", "XCI", "XCII", "XCIII", "XCIV", "XCV", "XCVI", "XCVII", "XCVIII",
    "XCIX", "C" };
    return roman_table[num - 1];
}

/*! Guess the number from the area with half division method.
 *
 *  \param low Lower bound of number.
 *  \param high Higher bound of number.
 *  \return Guessed number.
 */
int
guess(unsigned low, unsigned high, int roman) {
    if (low == high) {
        return high;
    }

    char ans[8];
    unsigned border = (high + low) / 2;

    if (roman) {
        printf(_("Is your number greater than %s? yes/no\n> "),
                arabic_to_roman((high + low) / 2));
    } else {
        printf(_("Is your number greater than %u? yes/no\n> "), (high + low) / 2);
    }
    scanf("%s", ans);

    if (!strcmp(ans, _("yes"))) {
        return guess(border + 1, high, roman);
    } else {
        return guess(low, border, roman);
    }
}

int
main(int argc, char **argv) {

    setlocale(LC_ALL, "");
    bindtextdomain("guesser", getenv("PWD"));
    textdomain("guesser");

    int roman = 0;

    if (argc > 1 && !strcmp(argv[1], "-r")) {
        roman = 1;
    } else if (argc > 1 && !strcmp(argv[1], "--help")) {
        help();
        return 0;
    }
    if (roman) {
        printf(_("Think of a number from %s to %s\n"), arabic_to_roman(1),
                arabic_to_roman(100));
        printf(_("Your number is %s!\n"), arabic_to_roman(guess(1, 100, roman)));
    } else {
        printf(_("Think of a number from 1 to 100\n"));
        printf(_("Your number is %d!\n"), guess(1, 100, roman));
    }
    return 0;
}
