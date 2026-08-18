#include <stdint.h>
#include "drivers/keyboard/keyboard.h"
#include "commands/commands.h"

/* Multiboot 1 header */
#define MULTIBOOT_MAGIC   0x1BADB002
#define MULTIBOOT_FLAGS   0x00000003
#define MULTIBOOT_CHECKSUM -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

struct multiboot_header {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
    uint32_t header_addr;
    uint32_t load_addr;
    uint32_t load_end_addr;
    uint32_t bss_end_addr;
    uint32_t entry_addr;
} __attribute__((packed));

void kmain(void);

__attribute__((section(".multiboot")))
struct multiboot_header mb_header = {
    .magic          = MULTIBOOT_MAGIC,
    .flags          = MULTIBOOT_FLAGS,
    .checksum       = MULTIBOOT_CHECKSUM,
    .header_addr    = (uint32_t)&mb_header,
    .load_addr      = (uint32_t)&mb_header,
    .load_end_addr  = 0,
    .bss_end_addr   = 0,
    .entry_addr     = (uint32_t)&kmain
};

/* VGA-текстовый буфер */
#define VGA_MEMORY ((char*)0xb8000)
static char *video = VGA_MEMORY;
int cursor = 0;

static unsigned char current_attr = 0x0F;   // ← добавь эту строку

/* Буфер командной строки */
static char cmd_buf[256];
static int cmd_len = 0;

/* История */
#define HISTORY_SIZE 20
char history[HISTORY_SIZE][256];
int history_count = 0;

/* Функции текстового терминала */
void scroll_screen(void) {
    for (int i = 0; i < 80 * 24; i++) {
        video[i * 2] = video[(i + 80) * 2];
        video[i * 2 + 1] = video[(i + 80) * 2 + 1];
    }
    for (int i = 0; i < 80; i++) {
        int pos = 24 * 80 + i;
        video[pos * 2] = ' ';
        video[pos * 2 + 1] = 0x0F;
    }
    cursor = 24 * 80;
}

void putchar_vga(char c) {
    if (c == '\n') {
        cursor = (cursor / 80 + 1) * 80;
        if (cursor >= 80 * 25) scroll_screen();
        return;
    }
    video[cursor * 2] = c;
    video[cursor * 2 + 1] = current_attr;   // ← замени 0x0F на current_attr
    cursor++;
    if (cursor >= 80 * 25) scroll_screen();
}

void print(const char *s) {
    while (*s) putchar_vga(*s++);
}

int streq(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a == *b;
}

/* Автодополнение по Tab */
void tab_complete(void) {
    int matches[MAX_COMMANDS];
    int match_count = 0;
    for (int i = 0; i < command_count; i++) {
        int ok = 1;
        for (int j = 0; j < cmd_len; j++) {
            if (command_table[i].name[j] != cmd_buf[j]) {
                ok = 0;
                break;
            }
        }
        if (ok) matches[match_count++] = i;
    }
    if (match_count == 1) {
        const char *full = command_table[matches[0]].name;
        int full_len = 0;
        while (full[full_len]) full_len++;
        for (int i = cmd_len; i < full_len; i++) {
            putchar_vga(full[i]);
            cmd_buf[cmd_len++] = full[i];
        }
    }
}

/* Выполнение команды */
static void execute_command(void) {
    cmd_buf[cmd_len] = '\0';
    print("\n");

    /* Сохранение в историю */
    if (cmd_len > 0) {
        if (history_count == 0 || !streq(cmd_buf, history[history_count-1])) {
            if (history_count < HISTORY_SIZE) {
                for (int i = 0; i <= cmd_len; i++) history[history_count][i] = cmd_buf[i];
                history_count++;
            } else {
                for (int i = 0; i < HISTORY_SIZE-1; i++)
                    for (int j = 0; j < 256; j++) history[i][j] = history[i+1][j];
                for (int i = 0; i <= cmd_len; i++) history[HISTORY_SIZE-1][i] = cmd_buf[i];
            }
        }
    }

    /* Поиск команды в таблице */
    int found = 0;
    for (int i = 0; i < command_count; i++) {
        if (streq(cmd_buf, command_table[i].name)) {
            command_table[i].func("");
            found = 1;
            break;
        }
    }

    /* Особая обработка echo с аргументами */
    if (!found && cmd_buf[0]=='e' && cmd_buf[1]=='c' && cmd_buf[2]=='h' && cmd_buf[3]=='o' && cmd_buf[4]==' ') {
        for (int i = 0; i < command_count; i++) {
            if (streq(command_table[i].name, "echo")) {
                command_table[i].func(cmd_buf + 5);
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        current_attr = 0x0C;
        print("Unknown: ");
        current_attr = 0x0F;
        print(cmd_buf);
    }

    cmd_len = 0;
    print("\nRosos> ");
}

/* Точка входа */
void kmain(void) {
    /* Очистка экрана */
    for (int i = 0; i < 80 * 25; i++) {
        video[i * 2] = ' ';
        video[i * 2 + 1] = 0x0F;
    }
    cursor = 0;
    
    /* приглашение */
    print("Welcome to Rosos 26.4 (beta version with VGA terminal)!\n");

    current_attr = 0x0A;   // ярко-зелёный текст на чёрном фоне
    print(" ____   ___  ____   ___  ____ \n|  _ \\ / _ \\/ ___| / _ \\/ ___|\n| |_) | | | \\___ \\| | | \\___ \\\n|  _ <| |_| |___) | |_| |___) |\n|_| \\_\\\\___/|____/ \\___/|____/ ");
    current_attr = 0x0F;   // возвращаем обычный белый

    print("\nRosos> ");
    
    /* Главный цикл */
    while (1) {
        int ch = keyboard_getchar();
        if (ch == 0) {
            for (volatile int i = 0; i < 50000; i++) {}
            continue;
        }

        char c = (char)ch;

        if (c == '\t') {
            tab_complete();
            continue;
        }

        if (c == '\n') {
            execute_command();
        } else if (c == '\b') {
            if (cmd_len > 0) {
                cursor--;
                video[cursor * 2] = ' ';
                video[cursor * 2 + 1] = 0x0F;
                cmd_len--;
            }
        } else if (c >= 32 && c <= 126 && cmd_len < 255) {
            putchar_vga(c);
            cmd_buf[cmd_len++] = c;
        }
    }
}
