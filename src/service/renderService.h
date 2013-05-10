#ifndef __RENDER_SERVICE_H
#define __RENDER_SERVICE_H

#include "gameobject\gameobject.h"
typedef void (*renderCallback)(void* arg);

void render_service_init(void);
void render_service_add_listenner_gui(GameObject_t *obj, renderCallback cb, void* arg);
void render_service_add_listenner(GameObject_t *obj, renderCallback cb, void* arg);
void render_all(void);
void render_gui(void);
void render_service_sort(void);

#endif