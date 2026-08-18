#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

/* Константы Multiboot */
#define MULTIBOOT_MAGIC        0x1BADB002
#define MULTIBOOT_FLAGS        0x00000003  /* выравнивание + информация о памяти */
#define MULTIBOOT_CHECKSUM     -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

/* Структура заголовка */
struct multiboot_header {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
    /* Дополнительные поля (необязательно) */
    uint32_t header_addr;
    uint32_t load_addr;
    uint32_t load_end_addr;
    uint32_t bss_end_addr;
    uint32_t entry_addr;
} __attribute__((packed));

#endif
