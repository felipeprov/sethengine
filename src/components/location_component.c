#include "components.h"
#include "math\vector2d.h"
#include <stdlib.h>
#include <stdio.h>
#include "service\message_service.h"
#include "string.h"

typedef struct LocationData_t
{
	Vector2d_t* position;
	Vector2d_t* size;
	double*		rotation;
	double		dt;
}LocationData_t;

static void locationComponentInit(Component_t* comp_)
{
	Attribute_t* att = GameObject_QueryAttribute(comp_->parent, "position");
	LocationData_t* data = (LocationData_t*)malloc(sizeof(LocationData_t));

	att = GameObject_QueryAttribute(comp_->parent, "position");
	if(att == 0)
	{
		att = Attribute_NewVector(0,0);
		GameObject_AddAttribute(comp_->parent,att ,"position");
	}
	data->position = Attribute_ToVector2d(att);


	att = GameObject_QueryAttribute(comp_->parent, "size");
	if(att == 0)
	{
		att = Attribute_NewVector(1,1);
		GameObject_AddAttribute(comp_->parent, att ,"size");
	}
	data->size = Attribute_ToVector2d(att);
	
	att = GameObject_QueryAttribute(comp_->parent, "rotation");
	if(att == 0)
	{
		att = Attribute_NewDouble(0);
		GameObject_AddAttribute(comp_->parent, att ,"rotation");
	}
	data->rotation = Attribute_ToDouble(att);

	comp_->data = data;

	message_service_register_listenner(comp_, "move");
}

static void locationComponentDeinit(Component_t* comp_)
{
}

static void locationOnUpdate(struct Component_t* comp_, double dt)
{
	LocationData_t* data = (LocationData_t*)comp_->data;
	data->dt = dt;
}

static void locationOnMsg(struct Component_t* comp_, char* msg_name, void* msg)
{
	int value = (int)msg;
	LocationData_t* data = (LocationData_t*)comp_->data;

	if(strcmp("move", msg_name) == 0)
	{
		if(value == 2)
		{
			data->position->x -= 10 * data->dt;
		}

		if(value == 3)
		{
			data->position->x+= 10 * data->dt;
		}

		if(value == 4)
		{
			data->position->y+= 10 * data->dt;
		}

		if(value == 5)
		{
			data->position->y-= 10 * data->dt;
		}
	}
}

Component_t* CreateLocationComponent(void)
{
	Component_t* comp = (Component_t*)malloc(sizeof(Component_t));

	comp->init = locationComponentInit;
	comp->deinit = locationComponentDeinit;
	comp->onUpdate = locationOnUpdate;
	comp->onMsg =  locationOnMsg;
	comp->onRefresh = 0;
	return comp;
}
