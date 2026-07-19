/* Header for PS/2 keyboard driver */
#ifndef PULSEOS_KEYBDRV_H
#define PULSEOS_KEYBDRV_H

#ifdef __cplusplus
extern "C" {
#endif

void kbd_init(void);
void kbd_poll(void);
int kbd_available(void);
int kbd_getchar(void);

#ifdef __cplusplus
}
#endif

#endif /* PULSEOS_KEYBDRV_H */
