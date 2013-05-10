#ifndef __MESSAGE_SERVICE_H
#define __MESSAGE_SERVICE_H

#include "components\components.h"
#include "gameobject\gameobject.h"

void message_service_init(void);

void message_service_register_listenner(Component_t* comp, char* msg_name);
void message_service_send_msg(GameObject_t* dest, char* msg_name, void* msg);

void message_dispatch(void);

#endif
