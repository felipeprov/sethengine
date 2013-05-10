#ifndef __GAME_OBJECT_H
#define __GAME_OBJECT_H 

#include <stdint.h>
#include "components\components.h"
#include "attributes\attributes.h"
#include "utils\list.h"


#ifdef __cplusplus
extern "C" {  
#endif  

typedef struct GameObject_t
{
	struct list_head attribute_list;
	struct list_head component_list;
}GameObject_t;


GameObject_t* GameObjectNew(void);
void GameObject_AddComponent(struct GameObject_t* obj_, struct Component_t* comp_, char* name);
void GameObject_AddAttribute(struct GameObject_t* obj_, struct Attribute_t* attr_, char* name);
void GameObject_Update(struct GameObject_t* obj_, double dt);
void GameObject_OnMsg(struct GameObject_t*, char* msg_name, void* msg);
struct Component_t* GameObject_QueryComponent(struct GameObject_t* obj_, char* name_);
struct Attribute_t* GameObject_QueryAttribute(struct GameObject_t* obj_, char* name_);

void GameObject_Refresh(struct GameObject_t* obj_);

void GameObject_SetAttributeInt(struct GameObject_t* obj_, char* att_name, int x);
void GameObject_SetAttributeDouble(struct GameObject_t* obj_, char* att_name, double x);
void GameObject_SetAttributeVector2d(struct GameObject_t* obj_, char* att_name, float x, float y);
void GameObject_SetAttributeString(struct GameObject_t* obj_, char* att_name, char* str);
void GameObject_SetAttributePointer(struct GameObject_t* obj_, char* att_name, void* ptr);

#ifdef __cplusplus
}
#endif

#endif