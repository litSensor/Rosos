#include "commands.h"

void cmd_echo(const char *args) {
    print(args);   // args указывает на строку после "echo "
}
