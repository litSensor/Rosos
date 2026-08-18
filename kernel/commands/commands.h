#ifndef COMMANDS_H
#define COMMANDS_H

struct command_entry {
    const char *name;
    void (*func)(const char *args);
};

#define MAX_COMMANDS 30

extern struct command_entry command_table[];
extern int command_count;

// Вспомогательные функции терминала
void putchar_vga(char c);
void print(const char *s);
int  streq(const char *a, const char *b);
unsigned int rand_int(int max);
void scroll_screen(void);

// Автодополнение
void tab_complete(void);   // <-- добавь эту строку

#endif
