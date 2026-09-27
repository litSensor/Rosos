# Rosos — Educational Mini Operating System

This project is a **starting skeleton** for those who want to understand how operating systems work and build their own from scratch.

Everything is already set up to get you started: GRUB bootloader is configured, the folder structure is well-organised, and a simple terminal with a few basic commands is working. All you have to do is explore the code and expand it with your own features.

## What's inside
- **Bootloader**: GRUB configuration to load the kernel.
- **Structure**: Clean directory layout (kernel, drivers, libs, etc.).
- **Terminal**: A working command line with several demo commands. See [Commands](#commands).

## Who is this for
For beginners in systems programming. If you've always wanted to dig into the internals of an OS but didn't know where to begin — take this code, study it, rewrite it, and create your very own operating system.

> **Notes**:
> 1. This is not a production-ready OS. It's a learning material and a launchpad for your own ideas.
> 2. This project is intended for Ubuntu only.

## Commands

| Command | Description |
| --- | --- |
| `banner` | Displays the Rosos ASCII-art banner. |
| `clear` | Clears the terminal screen. |
| `colors` | Displays the supported terminal colors. |
| `description` | Prints a short description of Rosos. |
| `echo` | Prints the given text back to the terminal. |
| `help` | Lists all available commands. |
| `history` | Shows the history of entered commands. |
| `version` | Shows the current Rosos version. |

## Build and run
./Rosos/run.sh

## License
Apache 2.0 — do whatever you want with the code.
