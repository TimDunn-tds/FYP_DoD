#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include "reference.h"

static float _time = 0.0f;
static float man_ref = 0.0f;

void reference_init(void)
{
    _time = 0.0f;
    // man_ref = 0.0f;
}

float reference_update(void)
{
    // Generate step function reference
    /* if  0 < t < 10  : ref = 0
     * if 10 < t < 20  : ref = 0.5
     * if 20 < t < inf : ref = 0 */

    float reference = 0.0f;
    
    if (_time < 10.0f)
    {
        reference = 0.0f;
    }
    else if (_time >= 10.0f && _time < 13.0f)
    {
        reference = 0.2f;
    }
    else
    {
        reference = 0.0f;
    }
    // printf("time = %.2f\n", _time);
    _time += 0.01f;

    // return reference;
    // printf("                            man ref %f\n", man_ref);
    return man_ref;
}

void manual_reference(float ref)
{
    // printf("                                             Setting manual reference to %f\n", ref);
    man_ref = ref;
    return; 
}

// void man_ref_set(float ref)
// {
//     printf("                Setting manual refernce to %f")
// }