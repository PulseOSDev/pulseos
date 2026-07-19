extern void kbd_init(void);
extern void newline();
extern void terminal_writestring(const char* data);
extern int kbd_available(void);
extern int kbd_getchar(void);
extern void invisiblewritestring(char* str);

#include "stdint.h"

// Fixed: properly appends character to string
void addChar(char* s, char c, int* len) {
	if (*len < 254) {  // max 255 including null terminator
		s[*len] = c;
		(*len)++;
		s[*len] = '\0';
	}
}

char* input(char* prefix) {
    static char out[255] = "";
    int len = 0;
    out[0] = '\0'; // Ensure the string is null-terminated

	terminal_writestring(prefix);
	while (1) {
		int c = kbd_getchar();
		
		if (c == '\r' || c == '\n') {
			newline();
            return out;
		} else if (c == 8) { /* backspace */
			if (len > 0) {
				len--;
				out[len] = '\0';
				invisiblewritestring("\b \b");
			}
		} else if (c >= 32 && c <= 126) { /* printable ASCII */
			addChar(out, (char)c, &len);
			char temp[2] = {(char)c, '\0'};
			terminal_writestring(temp);
		}
	}
	return out;
}