#include <stdint.h>
#include "../time.c"

/* Simple PS/2 keyboard driver (polling) - produces ASCII characters.
   Exposes: kbd_init(), kbd_poll(), kbd_available(), kbd_getchar().
   This is a small, self-contained driver meant for early OS use.
*/

/* Port I/O */
static inline unsigned char inb(unsigned short port) {
	unsigned char ret;
	__asm__ volatile ("inb %1, %0" : "=a" (ret) : "Nd" (port));
	return ret;
}

/* Status port: 0x64, data port: 0x60 */
#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64

/* Ring buffer for translated characters */
#define KBD_BUF_SIZE 256
static volatile unsigned char kbd_buf[KBD_BUF_SIZE];
static volatile unsigned int kbd_head = 0;
static volatile unsigned int kbd_tail = 0;

static int kbd_shift = 0;
static int kbd_caps = 0;

static void kbd_push(unsigned char c) {
	unsigned int next = (kbd_head + 1) & (KBD_BUF_SIZE - 1);
	if (next == kbd_tail) return; /* full, drop */
	kbd_buf[kbd_head] = c;
	kbd_head = next;
}

static int kbd_pop(void) {
	if (kbd_head == kbd_tail) return -1;
	unsigned char c = kbd_buf[kbd_tail];
	kbd_tail = (kbd_tail + 1) & (KBD_BUF_SIZE - 1);
	return c;
}

/* Scancode to ASCII (set 1) - QWERTZ layout mapping for common keys. Index by scancode. */
static const unsigned char scmap[] = {
	0, 27, '1','2','3','4','5','6','7','8','9','0', 0xDF, 0xB4,'\b', /* 0x00 - 0x0e */
	'\t','q','w','e','r','t','z','u','i','o','p', 0xFC, '+','\n',    /* 0x0f - 0x1c */
	0, /* 0x1d (lctrl) */ 'a','s','d','f','g','h','j','k','l', 0xF6, 0xE4, '^', /* 0x1e - 0x29 */
	0, /* 0x2a lshift */ '#','y','x','c','v','b','n','m',',','.','-', 0, /* 0x2b - 0x35 */
	'*', 0, ' ', /* 0x36 - 0x39 */
};

/* Shifted equivalents for number-row and punctuation in QWERTZ layout */
static const unsigned char scmap_shift[] = {
	0, 0, '!','"', 0xA7, '$','%','&','/','(',')','=','?','\b', /* 0x00 - 0x0e */
	'\t','Q','W','E','R','T','Z','U','I','O','P', 0xDC, '*','\n',    /* 0x0f - 0x1c */
	0, 'A','S','D','F','G','H','J','K','L', 0xD6, 0xC4, '`', /* 0x1e - 0x29 */
	0, '\'', 'Y','X','C','V','B','N','M',';',':','_', 0, /* 0x2b - 0x35 */
	'*', 0, ' ', /* 0x36 - 0x39 */
};

/* Helper to translate a make scancode (non-break) to ASCII considering modifiers */
static int scancode_to_ascii(unsigned char sc) {
	if (sc == 0 || sc >= sizeof(scmap)) return -1;
	unsigned char base = scmap[sc];
	if (base == 0) return -1;
	int isalpha = (base >= 'a' && base <= 'z');
	if (kbd_shift) {
		if (sc < sizeof(scmap_shift)) return scmap_shift[sc];
	}
	if (kbd_caps && isalpha) {
		return base - ('a' - 'A');
	}
	return base;
}

/* Public API */
void kbd_init(void) {
	/* Initialize buffer and state */
	kbd_head = kbd_tail = 0;
	kbd_shift = 0;
	kbd_caps = 0;
	/* Flush keyboard output if any */
	while (inb(KBD_STATUS_PORT) & 1) { (void)inb(KBD_DATA_PORT); }
}

/* Poll once: read any available scancodes and translate to ASCII. */
void kbd_poll(void) {
	unsigned char status = inb(KBD_STATUS_PORT);
	if (!(status & 1)) return; /* no data */
	unsigned char sc = inb(KBD_DATA_PORT);

	/* ignore extended (0xE0) sequences for now */
	if (sc == 0xE0) return;

	if (sc & 0x80) {
		/* break code */
		unsigned char make = sc & 0x7F;
		if (make == 0x2A || make == 0x36) kbd_shift = 0;
		/* other releases ignored */
		return;
	}

	/* make code */
	if (sc == 0x2A || sc == 0x36) { kbd_shift = 1; return; }
	if (sc == 0x3A) { kbd_caps = !kbd_caps; return; }

	int ch = scancode_to_ascii(sc);
	if (ch >= 0) kbd_push((unsigned char)ch);
}

int kbd_available(void) {
	return kbd_head != kbd_tail;
}

/* Blocking getchar: polls until a character is available. */
int kbd_getchar(void) {
	int c;
	while ((c = kbd_pop()) == -1) {
		kbd_poll();
	}
	sleep(100000);
	return c;
}

