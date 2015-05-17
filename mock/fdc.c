#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <alloca.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

enum {
    FD_CMD_READ_TRACK = 0x02,
    FD_CMD_SPECIFY = 0x03,
    FD_CMD_SENSE_DRIVE_STATUS = 0x04,
    FD_CMD_WRITE = 0x05,
    FD_CMD_READ = 0x06,
    FD_CMD_RECALIBRATE = 0x07,
    FD_CMD_SENSE_INTERRUPT_STATUS = 0x08,
    FD_CMD_WRITE_DELETED = 0x09,
    FD_CMD_READ_ID = 0x0a,
    FD_CMD_READ_DELETED = 0x0c,
    FD_CMD_FORMAT_TRACK = 0x0d,
    FD_CMD_DUMPREG = 0x0e,
    FD_CMD_SEEK = 0x0f,
    FD_CMD_VERSION = 0x10,
    FD_CMD_SCAN_EQUAL = 0x11,
    FD_CMD_PERPENDICULAR_MODE = 0x12,
    FD_CMD_CONFIGURE = 0x13,
    FD_CMD_LOCK = 0x14,
    FD_CMD_VERIFY = 0x16,
    FD_CMD_POWERDOWN_MODE = 0x17,
    FD_CMD_PART_ID = 0x18,
    FD_CMD_SCAN_LOW_OR_EQUAL = 0x19,
    FD_CMD_SCAN_HIGH_OR_EQUAL = 0x1d,
    FD_CMD_SAVE = 0x2e,
    FD_CMD_OPTION = 0x33,
    FD_CMD_RESTORE = 0x4e,
    FD_CMD_DRIVE_SPECIFICATION_COMMAND = 0x8e,
    FD_CMD_RELATIVE_SEEK_OUT = 0x8f,
    FD_CMD_FORMAT_AND_WRITE = 0xcd,
    FD_CMD_RELATIVE_SEEK_IN = 0xcf,
};

static void
fdctrl_noop() {
    printf("[*] I do nothing.\n");
}

static void
fdctrl_segfault() {
    char *array = alloca(rand()/RAND_MAX * 4);
    sprintf(array, "[!] I really talk too much sir! I shouldn't. Really. No.");
    printf("%s\n", array);
    free(array);
}

static void
fdctrl_unimplemented() {
    printf("%s\n", "[*] I am not implemented");
}

static const struct {
    uint8_t value;
    uint8_t mask;
    const char* name;
    int parameters;
    void (*handler)();
} handlers[] = {
    { FD_CMD_READ, 0x1f, "READ", 8, fdctrl_noop },
    { FD_CMD_WRITE, 0x3f, "WRITE", 8, fdctrl_noop },
    { FD_CMD_SEEK, 0xff, "SEEK", 2, fdctrl_noop },
    { FD_CMD_SENSE_INTERRUPT_STATUS, 0xff, "SENSE INTERRUPT STATUS", 0, fdctrl_noop },
    { FD_CMD_RECALIBRATE, 0xff, "RECALIBRATE", 1, fdctrl_noop },
    { FD_CMD_FORMAT_TRACK, 0xbf, "FORMAT TRACK", 5, fdctrl_noop },
    { FD_CMD_READ_TRACK, 0xbf, "READ TRACK", 8, fdctrl_noop },
    { FD_CMD_RESTORE, 0xff, "RESTORE", 17, fdctrl_noop },
    { FD_CMD_SAVE, 0xff, "SAVE", 0, fdctrl_noop },
    { FD_CMD_READ_DELETED, 0x1f, "READ DELETED DATA", 8, fdctrl_noop },
    { FD_CMD_SCAN_EQUAL, 0x1f, "SCAN EQUAL", 8, fdctrl_noop },
    { FD_CMD_VERIFY, 0x1f, "VERIFY", 8, fdctrl_noop },
    { FD_CMD_SCAN_LOW_OR_EQUAL, 0x1f, "SCAN LOW OR EQUAL", 8, fdctrl_noop },
    { FD_CMD_SCAN_HIGH_OR_EQUAL, 0x1f, "SCAN HIGH OR EQUAL", 8, fdctrl_noop },
    { FD_CMD_WRITE_DELETED, 0x3f, "WRITE DELETED DATA", 8, fdctrl_noop },
    { FD_CMD_READ_ID, 0xbf, "READ ID", 1, fdctrl_segfault },
    { FD_CMD_SPECIFY, 0xff, "SPECIFY", 2, fdctrl_noop },
    { FD_CMD_SENSE_DRIVE_STATUS, 0xff, "SENSE DRIVE STATUS", 1, fdctrl_noop },
    { FD_CMD_PERPENDICULAR_MODE, 0xff, "PERPENDICULAR MODE", 1, fdctrl_noop },
    { FD_CMD_CONFIGURE, 0xff, "CONFIGURE", 3, fdctrl_noop },
    { FD_CMD_POWERDOWN_MODE, 0xff, "POWERDOWN MODE", 2, fdctrl_noop },
    { FD_CMD_OPTION, 0xff, "OPTION", 1, fdctrl_noop },
    { FD_CMD_DRIVE_SPECIFICATION_COMMAND, 0xff, "DRIVE SPECIFICATION COMMAND", 5, fdctrl_noop },
    { FD_CMD_RELATIVE_SEEK_OUT, 0xff, "RELATIVE SEEK OUT", 2, fdctrl_noop },
    { FD_CMD_FORMAT_AND_WRITE, 0xff, "FORMAT AND WRITE", 10, fdctrl_noop },
    { FD_CMD_RELATIVE_SEEK_IN, 0xff, "RELATIVE SEEK IN", 2, fdctrl_noop },
    { FD_CMD_LOCK, 0x7f, "LOCK", 0, fdctrl_noop },
    { FD_CMD_DUMPREG, 0xff, "DUMPREG", 0, fdctrl_noop },
    { FD_CMD_VERSION, 0xff, "VERSION", 0, fdctrl_noop },
    { FD_CMD_PART_ID, 0xff, "PART ID", 0, fdctrl_noop },
    { FD_CMD_WRITE, 0x1f, "WRITE (BeOS)", 8, fdctrl_noop },
    { 0, 0, "unknown", 0, fdctrl_unimplemented }, /* default handler */
};

static uint8_t command_to_handler[256];

static void
fdctrl_realize_common(void)
{
    int i, j;
    static int command_tables_inited = 0;

    /* Fill 'command_to_handler' lookup table */
    if (!command_tables_inited) {
        command_tables_inited = 1;
        for (i = ARRAY_SIZE(handlers) - 1; i >= 0; i--) {
            for (j = 0; j < sizeof(command_to_handler); j++) {
                if ((j & handlers[i].mask) == handlers[i].value) {
                    command_to_handler[j] = i;
                }
            }
        }
    }

    printf("[*] Ready!\n");
}

void
fdc_initialize(void)
{
    fdctrl_realize_common();
}

void
fdc_execute(uint8_t cmd)
{
    int pos = command_to_handler[cmd];
    (*handlers[pos].handler)();
}
