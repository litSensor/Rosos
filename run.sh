#!/bin/bash
cd "$(dirname "$0")"
make clean && make kernel.elf
qemu-system-i386 -kernel kernel.elf -vga std -m 128
