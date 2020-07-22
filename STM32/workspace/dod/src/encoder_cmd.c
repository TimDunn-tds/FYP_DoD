#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "encoder.h"
#include "encoder_cmd.h"

void enc_cmd(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Encoder count is %" PRId32 "\n", encoder_get_count());
    }
    else
    {
        if (strcmp(argv[1], "reset") == 0)
        {
            encoder_set_count(0);
            printf("Encoder count reset to 0\n");
        }
        else
        {
            printf("%s: invalid argument \"%s\", syntax is: %s [reset]\n", argv[0], argv[1], argv[0]);
        }
    }
}