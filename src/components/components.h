#ifndef __COMPONENTS_H
#define __COMPONENTS_H

#include <stdint.h>
#include "gameobject/gameobject.h"

typedef struct Component_t
{
	void (*init)(struct Component_t* comp_);
	void (*deinit)(struct Component_t* comp_);

	void (*onUpdate)(struct Component_t* comp_, double dt);
	void (*onMsg)(struct Component_t* comp_, char* msg_name, void* msg);
	void (*onRefresh)(struct Component_t* comp_);

	struct GameObject_t* parent;
	void* data;
}Component_t;

typedef struct Component_t* (*createComponentFunction)(void);

#ifdef __cplusplus
extern "C" {  
#endif  


void Component_SystemInit(void);
void Component_Free(struct Component_t* comp_);
void Component_Copy(struct Component_t* dest_, struct Component_t *orig_);
void Component_Update(struct Component_t* comp_, double dt);
void Component_Init(struct Component_t* comp_);
void Component_Refresh(struct Component_t* comp_);

void Component_RegisterNew(char* name_, createComponentFunction func_);
struct Component_t* Component_New(struct GameObject_t* parent, char* name_);


#ifdef __cplusplus
}
#endif

#endif