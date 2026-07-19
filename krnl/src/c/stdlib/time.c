void sleep(int ticks) { // 1000 ticks = 1ms
    for (int i = 0; i < ticks; i++) { 
        asm volatile ("nop"); 
    } 
}