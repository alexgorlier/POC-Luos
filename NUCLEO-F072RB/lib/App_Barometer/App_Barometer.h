#ifndef __APP_BAROMETER_H__
#define __APP_BAROMETER_H__

#include "math.h"
#include "gpio.h"
#include "luos_engine.h"

void App_barometer_Init();
void App_barometer_Loop();
float App_barometer_DetermineAlt(float Pressure);
void App_barometer_Liberation();
//void App_Barometer_Get_Pressure();

#endif