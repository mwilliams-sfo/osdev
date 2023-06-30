
#include <stdint.h>
#include "intr.h"

#define TEXT_SCREEN_BASE 0xb8000
#define TEXT_SCREEN_WIDTH 80
#define TEXT_SCREEN_HEIGHT 25

static uint16_t * const vid_mem = (uint16_t *) TEXT_SCREEN_BASE;
static int terminal_row = 0;
static int terminal_col = 0;

void term_init(void) {
	for (int i = 0; i < TEXT_SCREEN_WIDTH * TEXT_SCREEN_HEIGHT; i++) {
		vid_mem[i] = (7 << 8) | ' ';
	}
}

static void tplot(int x, int y, char c, int color) {
	if (x < 0 || TEXT_SCREEN_WIDTH <= x || y < 0 || TEXT_SCREEN_HEIGHT <= y)
		return;
	vid_mem[y * TEXT_SCREEN_WIDTH + x] = (color << 8) | c;
}

static void tputc(char c, int color) {
	if (terminal_row == TEXT_SCREEN_HEIGHT) {
		for (int i = 0; i < (TEXT_SCREEN_HEIGHT - 1) * TEXT_SCREEN_WIDTH; i++) {
			vid_mem[i] = vid_mem[i + TEXT_SCREEN_WIDTH];
		}
		terminal_row--;
	}
	switch (c) {
	case '\r':
		terminal_col = 0;
		return;
	case '\n':
		terminal_col = 0;
		terminal_row++;
		return;
	}
	tplot(terminal_col, terminal_row, c, color);
	terminal_col++;
	if (terminal_col == TEXT_SCREEN_WIDTH) {
		terminal_col = 0;
		terminal_row++;
	}
}

static void tputs(const char * s, int color) {
	for (int i = 0; s[i]; i++) {
		tputc(s[i], color);
	}
}

void print(const char * s) {
	tputs(s, 7);
}
