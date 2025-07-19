#include <stdio.h>
#include <string.h>

// Style macros
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define ITALIC      "\033[3m"
#define UNDERLINE   "\033[4m"
#define BLINK       "\033[5m"
#define REVERSE     "\033[7m"
#define HIDDEN      "\033[8m"
#define STRIKE      "\033[9m"

// Foreground color macros
#define FG_BLACK    "\033[30m"
#define FG_RED      "\033[31m"
#define FG_GREEN    "\033[32m"
#define FG_YELLOW   "\033[33m"
#define FG_BLUE     "\033[34m"
#define FG_MAGENTA  "\033[35m"
#define FG_CYAN     "\033[36m"
#define FG_WHITE    "\033[37m"

// Bright foreground colors
#define FG_BBLACK   "\033[90m"
#define FG_BRED     "\033[91m"
#define FG_BGREEN   "\033[92m"
#define FG_BYELLOW  "\033[93m"
#define FG_BBLUE    "\033[94m"
#define FG_BMAGENTA "\033[95m"
#define FG_BCYAN    "\033[96m"
#define FG_BWHITE   "\033[97m"

// Background colors
#define BG_BLACK    "\033[40m"
#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"
#define BG_MAGENTA  "\033[45m"
#define BG_CYAN     "\033[46m"
#define BG_WHITE    "\033[47m"

// Bright background colors
#define BG_BBLACK    "\033[100m"
#define BG_BRED      "\033[101m"
#define BG_BGREEN    "\033[102m"
#define BG_BYELLOW   "\033[103m"
#define BG_BBLUE     "\033[104m"
#define BG_BMAGENTA  "\033[105m"
#define BG_BCYAN     "\033[106m"
#define BG_BWHITE    "\033[107m"

/* code to check  all styling , coloring in c language 
// Print with custom color and style
void print_colored(const char *text, const char *style, const char *fg, const char *bg) {
    printf("%s%s%s%s%s\n", style, fg, bg, text, RESET);
}

int main() {
    printf("=== TEXT STYLES ===\n");
    print_colored("Bold Text", BOLD, FG_WHITE, "");
    print_colored("Dim Text", DIM, FG_WHITE, "");
    print_colored("Italic Text", ITALIC, FG_WHITE, "");
    print_colored("Underlined Text", UNDERLINE, FG_WHITE, "");
    print_colored("Blinking Text", BLINK, FG_WHITE, "");
    print_colored("Reverse Text", REVERSE, FG_WHITE, "");
    print_colored("Hidden Text (invisible)", HIDDEN, FG_WHITE, "");
    print_colored("Strikethrough Text", STRIKE, FG_WHITE, "");

    printf("\n=== FOREGROUND COLORS ===\n");
    print_colored("Black", "", FG_BLACK, "");
    print_colored("Red", "", FG_RED, "");
    print_colored("Green", "", FG_GREEN, "");
    print_colored("Yellow", "", FG_YELLOW, "");
    print_colored("Blue", "", FG_BLUE, "");
    print_colored("Magenta", "", FG_MAGENTA, "");
    print_colored("Cyan", "", FG_CYAN, "");
    print_colored("White", "", FG_WHITE, "");

    printf("\n=== BRIGHT FOREGROUND COLORS ===\n");
    print_colored("Bright Black (Gray)", "", FG_BBLACK, "");
    print_colored("Bright Red", "", FG_BRED, "");
    print_colored("Bright Green", "", FG_BGREEN, "");
    print_colored("Bright Yellow", "", FG_BYELLOW, "");
    print_colored("Bright Blue", "", FG_BBLUE, "");
    print_colored("Bright Magenta", "", FG_BMAGENTA, "");
    print_colored("Bright Cyan", "", FG_BCYAN, "");
    print_colored("Bright White", "", FG_BWHITE, "");

    printf("\n=== BACKGROUND COLORS ===\n");
    print_colored("Background Black", "", FG_WHITE, BG_BLACK);
    print_colored("Background Red", "", FG_WHITE, BG_RED);
    print_colored("Background Green", "", FG_WHITE, BG_GREEN);
    print_colored("Background Yellow", "", FG_BLACK, BG_YELLOW);
    print_colored("Background Blue", "", FG_WHITE, BG_BLUE);
    print_colored("Background Magenta", "", FG_WHITE, BG_MAGENTA);
    print_colored("Background Cyan", "", FG_BLACK, BG_CYAN);
    print_colored("Background White", "", FG_BLACK, BG_WHITE);

    printf("\n=== BRIGHT BACKGROUND COLORS ===\n");
    print_colored("Bright BG Black", "", FG_WHITE, BG_BBLACK);
    print_colored("Bright BG Red", "", FG_WHITE, BG_BRED);
    print_colored("Bright BG Green", "", FG_BLACK, BG_BGREEN);
    print_colored("Bright BG Yellow", "", FG_BLACK, BG_BYELLOW);
    print_colored("Bright BG Blue", "", FG_WHITE, BG_BBLUE);
    print_colored("Bright BG Magenta", "", FG_WHITE, BG_BMAGENTA);
    print_colored("Bright BG Cyan", "", FG_BLACK, BG_BCYAN);
    print_colored("Bright BG White", "", FG_BLACK, BG_BWHITE);

    return 0;
}
*/