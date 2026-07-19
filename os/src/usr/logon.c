/* Simple logon test that echoes input lines using the keyboard driver. */

extern void kbd_init(void);
extern int print(char* str);
extern void terminal_writestring(const char* data);
extern int kbd_available(void);
extern int kbd_getchar(void);

char* logon() {
	char buf[80];
	int pos = 0;
	terminal_writestring("Logon: ");
	while (1) {
		terminal_writestring(buf);
		int c = kbd_getchar();
		buf[pos] = '\0';

		if (pos > 0) {
			pos = 0;
		}
		if (c == '\r' || c == '\n') {
			newline();
			print(buf);
			terminal_writestring("Logon: ");
		} else if (c == 8) { /* backspace */
			if (pos > 0) pos--;
			
		} else {
			if (pos < (int)sizeof(buf)-1) buf[pos++] = (char)c;
		}
	}
}
