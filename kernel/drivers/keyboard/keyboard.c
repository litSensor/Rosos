#include "keyboard.h"

/* Порты ввода-вывода */
static inline unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

/* Состояние модификаторов */
static int shift_pressed = 0;
static int caps_lock    = 0;

/* Полные таблицы скан-кодов (58 элементов, US QWERTY) */
static const unsigned char sc_lower[58] = {
    0,0,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    0,'q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,'\\','z','x','c','v','b','n','m',',','.','/',0,
    0,0,0,' '
};

static const unsigned char sc_upper[58] = {
    0,0,'!','@','#','$','%','^','&','*','(',')','_','+','\b',
    0,'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,'A','S','D','F','G','H','J','K','L',':','"','~',
    0,'|','Z','X','C','V','B','N','M','<','>','?',0,
    0,0,0,' '
};

void keyboard_init(void) {
    /* клавиатура всегда готова, дополнительной инициализации не требуется */
}

int keyboard_getchar(void) {
    unsigned char status = inb(0x64);
    if (!(status & 0x01)) return 0;   // нет данных

    unsigned char sc = inb(0x60);

    /* Отпускание клавиш */
    if (sc & 0x80) {
        if (sc == 0xAA || sc == 0xB6) shift_pressed = 0;
        // Caps Lock не обрабатываем на отпускание
        return 0;
    }

    /* Модификаторы (нажатие) */
    switch (sc) {
        case 0x2A: case 0x36: shift_pressed = 1; return 0;
        case 0x3A: caps_lock = !caps_lock;      return 0;
        // Ctrl (0x1D), Alt (0x38) можно добавить при необходимости
    }

    /* Специальные клавиши */
    switch (sc) {
        case 0x48: return KEY_UP;
        case 0x50: return KEY_DOWN;
        case 0x4B: return KEY_LEFT;
        case 0x4D: return KEY_RIGHT;
        case 0x01: return KEY_ESC;
        case 0x3B: return KEY_F1;
        case 0x3C: return KEY_F2;
        case 0x3D: return KEY_F3;
        case 0x3E: return KEY_F4;
        case 0x3F: return KEY_F5;
        case 0x40: return KEY_F6;
        case 0x41: return KEY_F7;
        case 0x42: return KEY_F8;
        case 0x43: return KEY_F9;
        case 0x44: return KEY_F10;
        case 0x57: return KEY_F11;
        case 0x58: return KEY_F12;
        // Можно добавить Print Screen, Scroll Lock, Pause и т.д.
    }

    /* Обычные символы */
    char c = 0;

    /* Пробел – гарантированно работает */
    if (sc == 0x39) {
        c = ' ';
    } else if (sc < 58) {
        c = shift_pressed ? sc_upper[sc] : sc_lower[sc];
        if (caps_lock) {
            if (c >= 'a' && c <= 'z') c ^= 0x20;       // инвертируем регистр
            else if (c >= 'A' && c <= 'Z') c ^= 0x20;
        }
    }

    return (unsigned char)c;
}
