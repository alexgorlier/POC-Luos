#include "App_Barometer.h"

enum // Custom type list
{
    APP_BAROMETER = LUOS_LAST_TYPE
};

service_t *barometer_app;
uint16_t ID_Solenoid = 0;
uint16_t ID_BMP280 = 0;
uint32_t LastAsk;
uint16_t TestTime = 0;
float Altitude;
float _Altitude;
pressure_t _Pressure = 101325;
int Test_Time = 0;

static void App_Barometer_MsgHandler(service_t *service, msg_t *msg);



/* Sends an auto update Message to the service barometer at the end of Luos Detection, and store pressure when received */
static void App_Barometer_MsgHandler(service_t *service, msg_t *msg)
{
    
    if (msg->header.cmd == END_DETECTION)
    {
        search_result_t filter_result;
        RTFilter_Reset(&filter_result);             
                                                    
        RTFilter_Alias(&filter_result, "solenoid");    
        ID_Solenoid = filter_result.result_table[0]->id;

        RTFilter_Reset(&filter_result);             
                                                    
        RTFilter_Alias(&filter_result, "BMP280");    
        ID_BMP280 = filter_result.result_table[0]->id;

        msg_t pub_msg;
        pub_msg.header.cmd         = PARAMETERS;
        pub_msg.header.size        = sizeof(pressure_t);
        pub_msg.header.target      = ID_BMP280;
        pub_msg.header.target_mode = ID;
        memcpy(pub_msg.data, &_Pressure, sizeof(pressure_t));
        Luos_SendMsg(barometer_app, &pub_msg);
        time_luos_t time = TimeOD_TimeFrom_ms(100);
        TimeOD_TimeToMsg(&time, &pub_msg);
        pub_msg.header.cmd = UPDATE_PUB;
        Luos_SendMsg(barometer_app, &pub_msg);
    }

    if (msg->header.cmd == PRESSURE)
    {
        PressureOD_PressureFromMsg( &_Pressure, msg);
        Test_Time = LastAsk - Luos_GetSystick();
        LastAsk = Luos_GetSystick();
    }
    
}

/* Creation of Luso Service, and Detection of other services */
void App_barometer_Init()
{
    revision_t revision = {{{1, 0, 0}}};
    barometer_app        = Luos_CreateService(App_Barometer_MsgHandler, APP_BAROMETER, "App_Barometer", revision);
    Luos_Detect(barometer_app);
    LastAsk = Luos_GetSystick();
}

/* Checks if apogee is reached and sends a Message to teh solenoid service if this is the case */
void App_barometer_Loop()
{
    _Altitude = App_barometer_DetermineAlt(PressureOD_PressureTo_Pa(_Pressure));
    if(_Altitude > 32000)
    {
        App_barometer_Liberation();
    }
}

/* Calculate an altitude from a pressure by comparison with standardized atmosphere */
float App_barometer_DetermineAlt(pressure_t Pressure)
{
    if(Pressure>22632)
    {
        Altitude = ((float)288.15/(float)0.0065)* (1-pow(((Pressure/101325)),(((float)8.314*(float)0.0065))/((float)9.81*(float)0.0289)));
    }
    else if(Pressure>5474)
    {
        Altitude = 11000 - ((((float)8.314*(float)216.65)/((float)9.81*(float)0.0289))*logf((Pressure/(float)22632.06)));
    }
    else if(Pressure>868)
    {
        Altitude = 20000 + (((float)216.65/(float)0.001)*(1 - pow((Pressure/(float)5474.889),(((float)8.314*(float)0.001)/((float)9.81*(float)0.0289)))));
    }
    else
    {
        Altitude = 32000 + (((float)228.65/(float)0.0028)*(1 - pow((Pressure/(float)868.0187),(((float)8.314*(float)0.0028)/((float)9.81*(float)0.0289)))));
    }
    return Altitude;
}

/* Sends a Message to the service Solenoid with the information "True" */
void App_barometer_Liberation()
{
    msg_t pub_msg;
    pub_msg.header.cmd         = IO_STATE;
    pub_msg.header.target_mode = ID;
    pub_msg.header.target      = ID_Solenoid;
    pub_msg.header.size        = sizeof(char);
    pub_msg.data[0]            = true;
    Luos_SendMsg(barometer_app, &pub_msg);   
}







