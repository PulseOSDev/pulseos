#include "sys/drvbridge.c"
#include "sys/drv/keybdrv.c"
#include "sys/syspanic.c"
#include "sys/input.c"
#include "term/terminalbr.c"
#include "usr/logon.c"

extern int print(char* str);

void initbridge() {
	kbd_init();

	while (1) {
		char* baum = input("pulseOS > ");
		terminal(baum);
	}
}