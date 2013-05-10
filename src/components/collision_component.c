#include "components.h"
#include "math\vector2d.h"
#include "environment\environment.h"
#include "service\renderService.h"
#include "image\image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "service\colision_service.h"
#include "service\message_service.h"

typedef struct CollisionCompData_t
{
	Vector3d_t* debug_color;
	char* collision_group;
	int onColision;
}CollisionCompData_t;

static void init(Component_t* comp_)
{
	CollisionCompData_t* data = (CollisionCompData_t*)malloc(sizeof(CollisionCompData_t));
	Attribute_t* att;

	memset(data, 0, sizeof(CollisionCompData_t));

	att = GameObject_QueryAttribute(comp_->parent, "debug_color");
	if(att == 0)
	{
		data->debug_color = (Vector3d_t*)malloc(sizeof(Vector3d_t));
		memset(data->debug_color, 0, sizeof(Vector3d_t));
		att = Attribute_NewPointer(data->debug_color);
		GameObject_AddAttribute(comp_->parent,att ,"debug_color");
	}
	data->debug_color = (Vector3d_t*)Attribute_ToPointer(att);

	att = GameObject_QueryAttribute(comp_->parent, "collision_group");
	if(att == 0)
	{
		att = Attribute_NewString("ui");
		GameObject_AddAttribute(comp_->parent,att ,"collision_group");
	}
	data->collision_group = Attribute_ToString(att);

	
	colision_service_register(comp_, data->collision_group);
	message_service_register_listenner(comp_, "collision");
	
	comp_->data = data;
}

static void deinit(Component_t* comp_)
{
}

static void onUpdate(struct Component_t* comp_, double dt)
{
	CollisionCompData_t* data = (CollisionCompData_t*)comp_->data;
	
	if(data->onColision)
	{
		data->debug_color->x =  -0.2;
		data->debug_color->y = -0.2;
		data->debug_color->z = -0.2;
		data->onColision = 0;
	}
	else
	{
		data->debug_color->x = 0.0;
		data->debug_color->y = 0.0;
		data->debug_color->z = 0.0;
	}
}

static void onMsg(struct Component_t* comp_, char* msg_name, void* msg)
{
	int value = (int)msg;
	CollisionCompData_t* data = (CollisionCompData_t*)comp_->data;

	if(strcmp("collision", msg_name) == 0)
	{
		data->onColision = 1;
	}
}

static void onRefresh(struct Component_t* comp_)
{
	CollisionCompData_t* data = (CollisionCompData_t*)comp_->data;

	colision_service_unregister(comp_);
	colision_service_register(comp_, data->collision_group);
}

Component_t* CreateCollisionComponent(void)
{
	Component_t* comp = (Component_t*)malloc(sizeof(Component_t));

	comp->init = init;
	comp->deinit = deinit;
	comp->onUpdate = onUpdate;
	comp->onMsg =  onMsg;
	comp->onRefresh = onRefresh;
	return comp;
}
