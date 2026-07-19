#include "stdbool.h"

extern int print(char* str);

char* systest() {
    /*
        KRNL needs: 500 MB RAM, 256 MB Storage
    */

    bool testmode = true; // Testmode bypasses the test
    bool error = false; // DO NOT CHANGE!! Sets to true if an error happens
    char* cause = "NULL"; // DO NOT CHANGE!! If an error happens this will have the cause message (UNUSED!!)
    char* errgroup = "NULL"; // DO NOT CHANGE!! If an error happens this will have the group (WARNING, ERROR, FATAL)
    int code = "0000";

    // real stuff:
    if (*errgroup =! "WARNING" && "ERROR" && "FATAL" && "NULL") { // Will flag an error if errgroup is not an valid errgroup type
        // TODO: Make this work
        error = true;
        cause = "errgroup is faulty (sys.c)";
        errgroup = "ERROR";
        code = "0100";

    }

    if(error == true && testmode == false) { // TODO: Display the error message
        // display: [SYSTEST] systest: [errgroup: WARNING, ERROR, FATAL] [cause]

        //some thing like this: char* PRINTTHIS = "[SYSTEST] systest: ", errgroup, cause;

        if ((errgroup =! "WARNING")) {
            return code;
        }

    }
    else if (testmode == true) { // Testmode bypass
        print("[SYSTEST] systest: OK (Testmode is on)");
        return "0";
    }
    else { // Youre good to go!
        print("[SYSTEST] systest: OK");
        return "0";
    }

    return code;
}

