#include "ver.c"
#include <stdbool.h>

extern int sysPanic(char* cause);

extern int print(char* str);

bool devtools_unlocked = false;

int strcmp_custom(char* str1, char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return 0;
        }
        i++;
    }
    if (str1[i] != str2[i]) {
        return 0;
    }
    return 1;
}

void terminal(char* input) {
    if (strcmp_custom(input, "ver")) {
        ver();
    }
    else if (strcmp_custom(input, "exit")) {
        asm volatile ("hlt");
    }
    else if (strcmp_custom(input, "unlock-devtools")) {
        print("Developer Tools Unlocked");
        devtools_unlocked = true;
    }
    else if (strcmp_custom(input, "dev.crashsys")) {
        if (devtools_unlocked) {
            print("System Crash Triggered");
            sysPanic("dev.crashsys");
        }
    }
}
