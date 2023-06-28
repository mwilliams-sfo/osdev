
#define TEXT_SCREEN_BASE 0xb8000
#define TEXT_SCREEN_WIDTH 80
#define TEXT_SCREEN_HEIGHT 25

static const char * const msg = "Kernel started";

void kernel_start()
{
	volatile char * const vid_mem = (char *) TEXT_SCREEN_BASE;
	for (int i = 0; i < TEXT_SCREEN_WIDTH * TEXT_SCREEN_HEIGHT;) {
		vid_mem[i++] = ' ';
		vid_mem[i++] = 7;
	}
	for (int i = 0; msg[i]; i++) {
		vid_mem[i * 2] = msg[i];
	}
}
