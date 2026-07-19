extern int print(char* str);
extern void panic_clear(void);
extern void newline(void);


int sysPanic(char* cause) { // Panic mode if somthing goes wrong
    panic_clear();
    print("pulseOS has crashed.");
    newline();
    print("The System is in an unuseable state and needs to be rebooted");
    print("If this message keeps showing");
    print("please repair or reinstall your System");
    newline();
    print("Error Code:");
    print(cause);
    newline();
    print("/ pulseOS 0.25 (Build: OSB-252.dev/krnl 0.95-dev)");
    __asm__ ( "hlt" );

    return 0;
}