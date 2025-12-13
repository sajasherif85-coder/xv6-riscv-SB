// BOOT_EPOCH is normally defined at build time via Makefile:
//    -DBOOT_EPOCH=<timestamp>
// If it's not defined (e.g., for editor/intellisense), define a dummy value.
#ifndef BOOT_EPOCH
#define BOOT_EPOCH 0
#endif
