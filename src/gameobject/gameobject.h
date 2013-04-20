#ifndef __GAME_OBJECT_H
#define __GAME_OBJECT_H 

#include <stdint.h>
#include "components\components.h"
#include "attributes\attributes.h"
#include "utils\list.h"

typedef struct GameObject_t
{
	struct list_head attribute_list;
	struct list_head component_list;
}GameObject_t;

void GameObject_AddComponent(struct GameObject_t* obj_, struct Component_t* comp_, char* name);
void GameObject_AddAttribute(struct GameObject_t* obj_, struct Attribute_t* attr_, char* name);

void GameObject_Update(struct GameObject_t* obj_);

struct Component_t* GameObject_QueryComponent(struct GameObject_t* obj_, char* name_);
struct Attribute_t* GameObject_QueryAttribute(struct GameObject_t* obj_, char* name_);

#endif