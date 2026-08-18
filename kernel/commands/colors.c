#include "commands.h"

void cmd_colors(const char *args) {
    print("VGA colors: 0-15 (0=black, 15=white)\n");
    print("Standard 16-color palette used in text mode.\n");
}
