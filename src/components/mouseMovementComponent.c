#include "components.h"
#include "math\vector2d.h"
#include "environment\environment.h"
#include "service\renderService.h"
#include "image\image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "service\message_service.h"

typedef struct MouseCompData_t
{
	Vector2d_t* position;
	Vector2d_t* abs_position;
	Vector2d_t* d_moviment;
	Vector2d_t* camera_pos;
	int mousebuttons[3];
}MouseCompData_t;

static void init(Component_t* comp_)
{
	GameObject_t* obj_camera;
	Attribute_t* att_camera_pos;
	Attribute_t* att = GameObject_QueryAttribute(comp_->parent, "position");
	MouseCompData_t* data = (MouseCompData_t*)malloc(sizeof(MouseCompData_t));
	memset(data, 0 , sizeof(MouseCompData_t));

	if(att == 0)
	{
		att = Attribute_NewVector(0,0);
		GameObject_AddAttribute(comp_->parent,att ,"position");
	}
	data->position = Attribute_ToVector2d(att);

	att = GameObject_QueryAttribute(comp_->parent, "abs_position");
	if(att == 0)
	{
		att = Attribute_NewVector(0,0);
		GameObject_AddAttribute(comp_->parent,att ,"abs_position");
	}
	data->abs_position = Attribute_ToVector2d(att);

	att = GameObject_QueryAttribute(comp_->parent, "target");
	if(att != 0)
	{
		obj_camera = (GameObject_t*)Attribute_ToPointer(att);
		att_camera_pos = GameObject_QueryAttribute(obj_camera, "position");
		comp_->data = Attribute_ToVector2d(att_camera_pos);
	}

	comp_->data = data;
}

static void deinit(Component_t* comp_)
{
}

static void onUpdate(struct Component_t* comp_, double dt)
{
	int x, y;
	int i;
	double horizontalAngle, verticalAngle;
	MouseCompData_t* data = (MouseCompData_t*)comp_->data;

	EnvironmentGetMouse(&x, &y);

	
	horizontalAngle = 10 * dt * float(800/2 - x );
	verticalAngle   = 10 * dt * float( 600/2 - y );

	data->position->x -= horizontalAngle;
	data->position->y += verticalAngle;



	if(data->abs_position)
	{
		data->abs_position->x += x;
		data->abs_position->y += y;
	}

	if(x != 400)
	{
		printf("mouse %d %d\n", x, y);
		printf("mouse pos %f %f\n", data->position->x, data->position->y);
		printf("mouse abs %f %f\n", data->abs_position->x, data->abs_position->y);
	}

	for(i = 0; i < 3; i++)
	{
		int mouse = EnvironmentGetMouseButton(i);
		if((data->mousebuttons[i] != mouse) && mouse)
		{
			message_service_send_msg(comp_->parent, "mouse_button",(void*) i);
		}
		data->mousebuttons[i] = mouse;
	}
}

static void onMsg(struct Component_t* comp_, char* msg_name, void* msg)
{
	int value = (int)msg;
	MouseCompData_t* data = (MouseCompData_t*)comp_->data;

}

static void onRefresh(struct Component_t* comp_)
{
	Attribute_t* att, *att_camera_pos;
	GameObject_t* obj_camera;
	MouseCompData_t* data = (MouseCompData_t*)comp_->data;

	att = GameObject_QueryAttribute(comp_->parent, "target");
	if(att != 0)
	{
		obj_camera = (GameObject_t*)Attribute_ToPointer(att);
		att_camera_pos = GameObject_QueryAttribute(obj_camera, "position");
		data->camera_pos = Attribute_ToVector2d(att_camera_pos);
	}
}

Component_t* CreateMouseMovementComponent(void)
{
	Component_t* comp = (Component_t*)malloc(sizeof(Component_t));

	comp->init = init;
	comp->deinit = deinit;
	comp->onUpdate = onUpdate;
	comp->onMsg =  onMsg;
	comp->onRefresh = onRefresh;
	return comp;
}
