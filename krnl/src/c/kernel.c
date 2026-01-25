#include "io.c"
#include "sys.c"
#include "os.c"
#include "drvmgr.c"

// TODO: Fix all compiler warnings

int krnlPanic(char* cause);
char* STRETURN;

int init() { // initilizer
    terminal_initialize();
    
    print("[KRNL] Terminal ready");
    print("[KRNL] Runnning systest");

    STRETURN = systest();

    if(STRETURN != "0") {
        krnlPanic(STRETURN);
    }

    return 0;
}

void syskrnlMain(void) { // main func for the kernel
    init();
}


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
    print("/ pulseKrnl 0.25 (B251)");
    __asm__ ( "hlt" );

    return 0;
}