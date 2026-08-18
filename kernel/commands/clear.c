#include "commands.h"

#define VGA_MEMORY ((char*)0xb8000)
static char *video = VGA_MEMORY;
extern int cursor;   // будет объявлен в kernel.c

void cmd_clear(const char *args) {
    for (int i = 0; i < 80 * 25; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = 0x0F;
    }
    cursor = 0;
}
