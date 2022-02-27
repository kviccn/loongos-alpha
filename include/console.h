#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#define COLUMNS 80
#define ROWS 25
#define VIDEO 0xB8000

void cls(void);

void cprintf(const char *fmt, ...);

#endif
