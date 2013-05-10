#ifndef __COLISION_SERVICE_H
#define __COLISION_SERVICE_H

#include "components\components.h"
#include "gameobject\gameobject.h"

void colision_service_init(void);
void colision_service_register(Component_t* comp, char* filter);
void colision_service_unregister(Component_t* comp);
void colision_service_send_msg_neighbors(Component_t* comp_, char* msg, void* value);
void colision_service_update(double dt);
#endif
