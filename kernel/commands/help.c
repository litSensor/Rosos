#include "commands.h"

void cmd_help(const char *args) {
    print("=== ROSOS commands ===\n");
    for (int i = 0; i < command_count; i++) {
        print(command_table[i].name);
        int len = 0;
        while (command_table[i].name[len]) len++;
        for (int s = len; s < 16; s++) putchar_vga(' ');
        if ((i + 1) % 5 == 0) print("\n");
    }
    if (command_count % 5 != 0) print("\n");
}
