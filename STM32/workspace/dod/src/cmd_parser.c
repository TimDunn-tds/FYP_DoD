#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <inttypes.h> // For PRIxx and SCNxx macros
#include <math.h>
#include "cmd_line_buffer.h"
#include "cmd_parser.h"
#include "adc_cmd.h"


// Type for each command table entry
typedef struct
{
    void (*func)(int argc, char *argv[]);   // Command function pointer
    const char * cmd;                       // Command name
    const char * args;                      // Command arguments syntax
    const char * help;                      // Command description
} CMD_T;

// Forward declaration for built-in help command
static void _help(int, char *[]);

// Modules that provide commands
#include "potentiometer_cmd.h"
#include "encoder_cmd.h"
#include "heartbeat_cmd.h"
#include "dimmer_cmd.h"
#include "light_cmd.h"
#include "sin_table.h"
#include "log_data.h"
#include "motor_cmd.h"
#include "controller_cmd.h"
#include "MPC_cmd.h"
#include "sysID_cmd.h"
#include "mpu6050_cmd.h"
#include "kalman_cmd.h"
#include "stepper_motor.h"
#include "reference_cmd.h"
#include "dc_motor_cmd.h"
#include "dc_motor_task_cmd.h"
#include "cc_cmd.h"
#include "control_allocation.h"

// Command table
static CMD_T cmd_table[] =
{
    {_help,             "help",         "",                                     "Displays this help message"},
    // {stepper_set_speed, "step_speed",   "[<rad/s>]",                            "Attemps to step at [rad/s]"},
    // {stepper_set_dir,   "step_dir",     "[<dir1> <dir2>]",                      "Change direction (0 or 1)"},
    // {stepper_turn_on,   "stepper_on",   "",                                     "Enables stepper PWM generation"},
    // {stepper_turn_off,  "stepper_off",  "",                                     "Disables stepper PWM generatrion"},
    // {pot_cmd,           "pot",          "",                                     "Get potentiometer ADC value"},
    {enc_cmd,           "enc",          "[reset]",                              "Get or reset encoder count"},
    {adc_cmd,           "adc",          "",                                     "Poll ADC for motor voltage"},
    {heartbeat_cmd,     "heartbeat",    "[start|stop]",                         "Get status or start/stop heartbeat task"},
    {dc_motor_task_cmd, "cc_task",      "[start|stop]",                         "Get status or start/stop current control task"},
    // {dimmer_cmd,        "dimmer",       "[start|stop]",                         "Get status or start/stop dimmer task"},
    {light_cmd,         "light",        "[<H> <S> <V>]",                        "Get or set light HSV values"},
    {sin_table_cmd,     "sin",          "[<#> <amp> <freq>]",                   "Remember to match the definition"},
    {log_data_cmd,      "log",          "[pot|enc|mpu|dc|ctrl|reset] <#>",                  "Otherwise memory mad."},
    // {manual_reference_cmd,  "ref",          "[<m/s>]",                               "Set manual reference"},
    // {controller_cmd,    "ctrl",         "[start|stop]",      "Get status or start/stop controller task"},
    {dc_motor_cmd,      "motor",        "[<value>] [supOut]",                            "Get or set motor voltage"},
    {cc_cmd,            "current",      "[<value>] [supOut]",                       "Get or set motor current"},
    {ctrl_allc_cmd,     "ctrl",         "[<start>|<stop>], [<wd>]",         "Start or stop control allocation, or set demanded torque"},
    // {MPC_cmd,           "MPC",          "[start|stop]",      "Start or stop MPC controller"},
    // {sysID_cmd,         "sysID",        "[cart|pend|stop] [<samples> [<amplitude> <frequency>]]", "Perform experiment/collect sysID data"},
    // {MPU6050_cmd,       "MPU",          "",                                       "Retrieve a single measurement from IMU"},
    // {kf_cmd,            "kf",           "run                  Run KF measurement and time update\n        kf reset                Reset KF to initial conditions\n        kf update meas|time     Run KF measurement or time update\n        kf get mean             Print KF mean estimate\n        kf set at|ar|gyro|u <value> Set KF measurement data", ""},
};
enum {CMD_TABLE_SIZE = sizeof(cmd_table)/sizeof(CMD_T)};
enum {CMD_MAX_TOKENS = 5};      // Maximum number of tokens to process (command + arguments)


// Command function definitions

static void _print_chip_pinout(void);

void _help(int argc, char *argv[])
{
    printf(
        "\n"
        "\n"
    );

    _print_chip_pinout();
    
    printf("\n");

    // Describe argument syntax using POSIX.1-2008 convention
    // see http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap12.html
    switch (argc)
    {
    case 1:
        printf(
            "   Command Arguments            Description\n"
            "-------------------------------------------\n"
        );
        for (int i = 0; i < CMD_TABLE_SIZE; i++)
        {
            printf("%10s %-20s %s\n", cmd_table[i].cmd, cmd_table[i].args, cmd_table[i].help);
        }
        // printf("\nFor more information, enter help followed by the command name\n\n");
        break;
    case 2:
        printf("Not yet implemented.\n\n");
        // TODO: Scan command table, and lookup extended help string.
        break;
    default:
        printf("help is expecting zero or one argument.\n\n");
    }
}

void _print_chip_pinout(void)
{
    printf(
        "Pin configuration:\n"
        "\n"
        "       .---------------------------------------.\n"
        " PC10--|  1  2 --PC11              PC9--  1  2 |--PC8\n"
        " PC12--|  3  4 --PD2               PB8--  3  4 |--PC6\n"
        "  VDD--|  5  6 --E5V               PB9--  5  6 |--PC5\n"
        "BOOT0--|  7  8 --GND              AVDD--  7  8 |--U5V\n"
        "   NC--|  9 10 --NC                GND--  9 10 |--NC\n"
        "   NC--| 11 12 --IOREF             PA5-- 11 12 |--PA12\n"
        " PA13--| 13 14 --RESET             PA6-- 13 14 |--PA11\n"
        " PA14--| 15 16 --+3v3              PA7-- 15 16 |--PB12\n"
        " PA15--| 17 18 --+5v               PB6-- 17 18 |--NC\n"
        "  GND--| 19 20 --GND               PC7-- 19 20 |--GND\n"
        "  PB7--| 21 22 --GND               PA9-- 21 22 |--PB2\n"
        " PC13--| 23 24 --VIN               PA8-- 23 24 |--PB1\n"
        " PC14--| 25 26 --NC               PB10-- 25 26 |--PB15\n"
        " PC15--| 27 28 --PA0               PB4-- 27 28 |--PB14\n"
        "  PH0--| 29 30 --PA1               PB5-- 29 30 |--PB13\n"
        "  PH1--| 31 32 --PA4               PB3-- 31 32 |--AGND\n"
        " VBAT--| 33 34 --PB0              PA10-- 33 34 |--PC4\n"
        "  PC2--| 35 36 --PC1               PA2-- 35 36 |--NC\n"
        "  PC3--| 37 38 --PC0               PA3-- 37 38 |--NC\n"
        "       |________                   ____________|\n"
        "                \\_________________/\n"
    );
}

// Command parser and dispatcher

static int _makeargv(char *s, char *argv[], int argvsize);

#ifdef NO_LD_WRAP
void cmd_parse(char *) __asm__("___real_cmd_parse");
#endif

void cmd_parse(char * cmd)
{
    if (cmd == NULL)
    {
        printf("ERROR: Tried to parse NULL command pointer\n");
        return;
    }
    else if (*cmd == '\0') // Empty command string
    {
        return;
    }

    // Tokenise command string
    char *argv[CMD_MAX_TOKENS];
    int argc = _makeargv(cmd, argv, CMD_MAX_TOKENS);

    // Execute corresponding command function
    for (int i = 0; i < CMD_TABLE_SIZE; i++)
    {
        if (strcmp(argv[0], cmd_table[i].cmd) == 0)
        {
            cmd_table[i].func(argc, argv);
            return;
        }
    }

    // Command not found
    printf("Unknown command: \"%s\"\n", argv[0]);
    // if (argc > 1)
    // {
    //     printf("    with arguments:\n");
    //     for (int i = 1; i < argc; i++)
    //     {
    //         printf("        %d: %s\n", i, argv[i]);
    //     }
    // }
    // printf_P(PSTR("\nEnter \"help\" for a list of commands.\n\n"));
}

// Command tokeniser

int _makeargv(char *s, char *argv[], int argvsize)
{
    char *p = s;
    int argc = 0;

    for(int i = 0; i < argvsize; ++i)
    {
        // skip leading whitespace
        while (isspace(*p))
            p++;

        if(*p != '\0')
            argv[argc++] = p;
        else
        {
            argv[argc] = NULL;
            break;
        }

        // scan over arg
        while(*p != '\0' && !isspace(*p))
            p++;

        // terminate arg
        if(*p != '\0' && i < argvsize - 1)
            *p++ = '\0';
    }

    return argc;
}


