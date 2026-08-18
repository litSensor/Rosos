#include "commands.h"

/* ─── Прототипы 8 команд ─── */
extern void cmd_help(const char *args);
extern void cmd_clear(const char *args);
extern void cmd_echo(const char *args);
extern void cmd_version(const char *args);
extern void cmd_history(const char *args);
extern void cmd_banner(const char *args);
extern void cmd_colors(const char *args);
extern void cmd_description(const char *args);   // бывшая about

/* ─── Таблица команд ─── */
struct command_entry command_table[] = {
    { "help",        cmd_help        },
    { "clear",       cmd_clear       },
    { "echo",        cmd_echo        },
    { "version",     cmd_version     },
    { "history",     cmd_history     },
    { "banner",      cmd_banner      },
    { "colors",      cmd_colors      },
    { "description", cmd_description },
};

/* ─── Количество команд ─── */
int command_count = sizeof(command_table) / sizeof(command_table[0]);
