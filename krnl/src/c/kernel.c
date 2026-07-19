#include "io.c"
#include "boot.c"
#include "osbridge.c"

// TODO: Fix all compiler warnings

int krnlPanic(char* cause);
char* STRETURN;

int krnlPanic(char* cause) { // Panic mode if somthing goes wrong
    panic_clear();
    print("!!! KERNEL PANIC !!!");
    newline();
    print("The System is in an unuseable state and needs to be rebooted");
    print("If this message keeps showing");
    print("please repair or reinstall your System");
    newline();
    print("Error Code:");
    print(cause);
    newline();
    print("/ pulseKrnl 0.95 (KRB-951.dev)");
    __asm__ ( "hlt" );

    return 0;
}


int init() { // initilizer
    terminal_initialize();
    
    print("[KRNL] Terminal ready");
    print("[KRNL] Runnning systest");

    STRETURN = systest();

    if(STRETURN != "0") {
        krnlPanic(STRETURN);
    }

    print("[KRNL] Loading OS");
    loadbridge();
    
    return 0;
}

void syskrnlMain(void) { // main func for the kernel
    init();
    
}
