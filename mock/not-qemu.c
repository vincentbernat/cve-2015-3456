#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

void fdc_initialize(void);
void fdc_execute(uint8_t);

int
main()
{
    char *warning = malloc(256); /* Just to get an heap */
    sprintf(warning, "[!] I am PID %d.\n", getpid());
    printf("%s", warning);
    fdc_initialize();
    printf("[!] Please, never execute 0xa.\n");
    while (1) {
        int cmd;
        printf("[?] What command should be executed? ");
        scanf("%10xi\n", &cmd);
        fdc_execute(cmd);
    }
}
