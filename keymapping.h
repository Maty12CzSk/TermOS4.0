#ifndef KEYMAPPING_H
#define KEYMAPPING_H

extern const char kbdmap[128];
extern const char kbdmap_shift[128];

// Arrow key scancodes after 0xE0 prefix
#define SC_LEFT_ARROW   0x4B
#define SC_RIGHT_ARROW  0x4D
#define SC_UP_ARROW     0x48
#define SC_DOWN_ARROW   0x50

#endif