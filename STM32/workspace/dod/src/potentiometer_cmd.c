#include <inttypes.h>
#include <stdio.h>
#include "potentiometer.h"
#include "potentiometer_cmd.h"

void pot_cmd(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("Potentiometer ADC value is %" PRIu16 "\n", pot_get_value());
    }
    else
    {
        printf("%s: invalid argument \"%s\", syntax is: %s\n", argv[0], argv[1], argv[0]);
    }
}