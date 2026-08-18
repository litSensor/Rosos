#include "commands.h"

extern char history[20][256];
extern int history_count;

void cmd_history(const char *args) {
    for (int i = 0; i < history_count; i++) {
        print(history[i]);
        print("\n");
    }
}
