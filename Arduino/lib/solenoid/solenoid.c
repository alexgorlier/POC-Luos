#include "solenoid.h"
#include "Arduino.h"

static void Solenoid_MsgHandler(service_t *service, msg_t *msg);

/* Creation of the service and initialization of the PIN dedicated to the solenoid */
void Solenoid_Init(void)
{
    pinMode(8, OUTPUT);
    revision_t revision = {{{1, 0, 0}}};
    Luos_CreateService(Solenoid_MsgHandler, STATE_TYPE, "solenoid", revision);

}

void Solenoid_Loop(void) 
{
    ;
}

/* Activate or Deactivate the solenoid depending on the Message received */
static void Solenoid_MsgHandler(service_t *service, msg_t *msg)
{
    if (msg->header.cmd == IO_STATE)
    {

        if (msg->data[0] == true)
        {
            digitalWrite(8,true);
        }
        if (msg->data[0] == false)
        {
            digitalWrite(8,false);
        }
    }
}