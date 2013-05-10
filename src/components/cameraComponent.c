#include "components.h"
#include "math\vector2d.h"
#include "environment\environment.h"
#include "service\renderService.h"
#include "image\image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct CameraCompData_t
{
	Vector2d_t* position;
	double* rotation;
	double z;

	GameObject_t* target;
	Vector2d_t* target_position;
	double dt;
}CameraCompData_t;

static void cameraupdate(void* arg)
{
	Component_t* comp_  =  (Component_t*) arg;
	CameraCompData_t* data = (CameraCompData_t*)comp_->data;

	EnvironmentSetCamera(data->position->x,data->position->y,data->z);
}

static void init(Component_t* comp_)
{
	Attribute_t* att = GameObject_QueryAttribute(comp_->parent, "position");
	CameraCompData_t* data = (CameraCompData_t*)malloc(sizeof(CameraCompData_t));
	memset(data, 0, sizeof(CameraCompData_t));

	att = GameObject_QueryAttribute(comp_->parent, "position");
	if(att == 0)
	{
		att = Attribute_NewVector(0,0);
		GameObject_AddAttribute(comp_->parent,att ,"position");
	}
	data->position = Attribute_ToVector2d(att);

	att = GameObject_QueryAttribute(comp_->parent, "rotation");
	if(att == 0)
	{
		att = Attribute_NewDouble(0);
		GameObject_AddAttribute(comp_->parent, att ,"rotation");
	}
	data->rotation = Attribute_ToDouble(att);

	att = GameObject_QueryAttribute(comp_->parent, "target");
	if(att == 0)
	{
		att = Attribute_NewPointer(0);
		GameObject_AddAttribute(comp_->parent, att ,"target");
	}
	data->target = (GameObject_t*)Attribute_ToPointer(att);

	att = GameObject_QueryAttribute(comp_->parent, "layer");
	if(att == 0)
	{
		att = Attribute_NewInteger(255);
		GameObject_AddAttribute(comp_->parent, att ,"layer");
	}

	comp_->data = data;

	render_service_add_listenner(comp_->parent, cameraupdate, comp_);

	data->z = 60;
}

static void deinit(Component_t* comp_)
{
}

static void onUpdate(struct Component_t* comp_, double dt)
{
	CameraCompData_t* data = (CameraCompData_t*)comp_->data;
	data->dt = dt;

	if(data->target_position)
	{
		if(data->target_position->y > data->position->y+2)
		{
			data->position->y += 10 * dt;
		}

		if(data->target_position->y < data->position->y-2)
		{
			data->position->y -= 10 * dt;
		}
	}
}

static void onMsg(struct Component_t* comp_, char* msg_name, void* msg)
{
	int value = (int)msg;
	CameraCompData_t* data = (CameraCompData_t*)comp_->data;

	if(strcmp("move", msg_name) == 0)
	{
		if(value == 0)
			data->z += 20 * data->dt;

		if(value == 1)
			data->z -= 20 * data->dt;
	}
}

static void onRefresh(struct Component_t* comp_)
{
	Attribute_t* att;
	CameraCompData_t* data = (CameraCompData_t*)comp_->data;

	att = GameObject_QueryAttribute(comp_->parent, "target");
	data->target = (GameObject_t*)Attribute_ToPointer(att);

	att= GameObject_QueryAttribute(data->target, "position");
	data->target_position = Attribute_ToVector2d(att);
}

Component_t* CreateCameraComponent(void)
{
	Component_t* comp = (Component_t*)malloc(sizeof(Component_t));

	comp->init = init;
	comp->deinit = deinit;
	comp->onUpdate = onUpdate;
	comp->onMsg =  onMsg;
	comp->onRefresh = onRefresh;
	return comp;
}
