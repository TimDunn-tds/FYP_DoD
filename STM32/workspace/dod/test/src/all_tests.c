#include "unity_fixture.h"

static void RunAllTests(void)
{
    // RTOS Mock Tests
    RUN_TEST_GROUP(RTOSthreads);
    RUN_TEST_GROUP(RTOStimers);
    RUN_TEST_GROUP(RTOSevents);

    // Lab 3
    RUN_TEST_GROUP(RGB_LED);
    RUN_TEST_GROUP(Button);
    RUN_TEST_GROUP(Encoder);
    RUN_TEST_GROUP(Potentiometer);
    RUN_TEST_GROUP(Light);
    RUN_TEST_GROUP(RTOSDimmer);
    
    // Lab 4
    RUN_TEST_GROUP(CmdLineBuffer);
    RUN_TEST_GROUP(IOSpy);
    RUN_TEST_GROUP(CmdParse);
    RUN_TEST_GROUP(CmdProcess);
    RUN_TEST_GROUP(CmdPot);
    RUN_TEST_GROUP(CmdLight);
    RUN_TEST_GROUP(CmdDimmer);
    RUN_TEST_GROUP(CmdHeartbeat);
    RUN_TEST_GROUP(SinTable);

    // Lab 5
    // RUN_TEST_GROUP(Ctrl);

    // Lab 6
    // RUN_TEST_GROUP(Observer);
    RUN_TEST_GROUP(MPC);

    // Lab 8
    RUN_TEST_GROUP(GivensQR);
}

int main(int argc, const char * argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}