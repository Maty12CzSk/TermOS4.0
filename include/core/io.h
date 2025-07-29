#ifndef IO_H
#define IO_H

#include <stdint.h>

static inline uint8_t inb(uint16_t port) {
    uint8_t r; __asm__ __volatile__ ("inb %1, %0" : "=a"(r) : "Nd"(port)); return r;
}
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}

#endif