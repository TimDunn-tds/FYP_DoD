#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "stm32f4xx_hal.h"

#include "control_allocation.h"
#include "dc_motor.h"
#include "cmsis_os2.h"
#include "encoder.h"

// a1
static float _a1 =
{
	#include "a1.def"
};

// a2
static float _a2 =
{
	#include "a2.def"
};

// a3
static float _a3 =
{
	#include "a3.def"
};

// a4
static float _a4 =
{
	#include "a4.def"
};

// a5
static float _a5 =
{
	#include "a5.def"
};

// a6
static float _a6 =
{
	#include "a6.def"
};

// B1
static float _B1 =
{
	#include "B1.def"
};

// B2
static float _B2 =
{
	#include "B2.def"
};

// B3
static float _B3 =
{
	#include "B3.def"
};

// B4
static float _B4 =
{
	#include "B4.def"
};

// B5
static float _B5 =
{
	#include "B5.def"
};

// B6
static float _B6 =
{
	#include "B6.def"
};

// Ra
static float _Ra =
{
	#include "Ra.def"
};

// Kw
static float _Kw =
{
	#include "Kw.def"
};

// N
static float _N =
{
	#include "N.def"
};

// p1
static float _p1 =
{
	#include "p1.def"
};

// p2
static float _p2 =
{
	#include "p2.def"
};

// p3
static float _p3 =
{
	#include "p3.def"
};



#define 	TIMER_TICKS 	5
#define     UMAX_SOFT       5.0f



static osTimerId_t timer_id = NULL;
osStatus_t status;
uint32_t arg_in;

float _heavi_p(float x);
float _heavi_m(float x);
float _sign1(float x);
float _sign2(float x);


static uint8_t _is_running = 0;
static uint8_t _is_init = 0;

static int32_t _count = 0;
static float _voltage = 0.0f;
static float _position = 0.0f;
static float _last_position = 0.0f;
static float _speed = 0.0f;
static float _current = 0.0f;
static float _time;
static float _wd = 0.0f;
static float _Vd = 0.0f;
static float _err_sum = 0.0f;



void ctrl_allc_cmd(int argc, char *argv[])
{
	if (argc == 2)
	{

	    if (!strcmp(argv[1], "start"))
	    {
	    	// On first call, create the timer
		   	if (timer_id == NULL)
		    {
		        timer_id = osTimerNew(ctrl_allc_task, osTimerPeriodic, &arg_in, NULL);
		    }

		    // If the timer isn't running, start the timer.
		    if (osTimerIsRunning(timer_id) == 0)
		    {
		        status = osTimerStart(timer_id,TIMER_TICKS);
		        if (status != osOK)
		        {
		            printf("Timer could not be started.\n");
		        }
		    }
		    printf("controller started\n");
		    encoder_set_count(0);
	    }

	    if (!strcmp(argv[1], "stop"))
	    {
	        // DONE: stop the timer
	        osTimerStop(timer_id);
	        // turn off motor
	        dc_motor_set(0.0f);
		    printf("controller stopped\n");

	    }
	}

	if (argc == 3)
	{
		_wd = atof(argv[2]);
	}
}

void ctrl_allc_task(void *argument)
{
	UNUSED(argument);
	float err = 0.0f;

    _position = dc_motor_get_position();

    // remember to conver timer ticks to seconds
    _speed = (_position - _last_position)/((float)TIMER_TICKS/1000.0f);

    // Error in speed
    err = _wd - _speed;
    _err_sum = _err_sum + err;

    // PI the Voltage
    _Vd = 3.5f*err + 0.7f*_err_sum;

    // Limit voltage
    if (_Vd>UMAX_SOFT)
    {
        _Vd = UMAX_SOFT;
    }
    else if (_Vd<-UMAX_SOFT)
    {
        _Vd = -UMAX_SOFT;
    }

    dc_motor_set(_Vd);
    // printf("Speed: %3.2f, V: %3.2f, wd: %3.2f, errsum: %3.2f\n", _speed, _Vd, _wd, _err_sum);
    // printf("%3.2f, %3.2f, %3.2f, %3.2f, %3.2f\n", _speed/_N, _Vd, Tm, tauF, _tauHat);

    // _time += TIMER_TICKS/1000.0f;
    _last_position = _position;

}


float _heavi_p(float x)
{
	return 1.0f/(1.0f + exp(-2.0f*50.0f*x));
}

float _heavi_m(float x)
{
	return 1.0f/(1.0f + exp(-2.0f*50.0f*-x));
}

float _sign1(float x)
{
	if (x > 0) return 1.0f;
	return 0.0f;
}

float _sign2(float x)
{
	if (x < 0) return 1.0f;
	return 0.0f;
}
