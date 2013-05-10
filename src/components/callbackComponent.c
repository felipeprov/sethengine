#include "components.h"
#include "math\vector2d.h"
#include "environment\environment.h"
#include "service\renderService.h"
#include "image\image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "service\message_service.h"

typedef void(*callback_f)(void*);
typedef struct CallbackCompData_t
{
	callback_f callback;
}CallbackCompData_t;

static void init(Component_t* comp_)
{
	Attribute_t* att = GameObject_QueryAttribute(comp_->parent, "callback");
	CallbackCompData_t* data = (CallbackCompData_t*)malloc(sizeof(CallbackCompData_t));
	if(att == 0)
	{
		att = Attribute_NewPointer(0);
		GameObject_AddAttribute(comp_->parent,att ,"callback");
	}
	data->callback = (callback_f)Attribute_ToPointer(att);

	comp_->data = data;

	message_service_register_listenner(comp_, "mouse_button");
}

static void deinit(Component_t* comp_)
{
}

static void onUpdate(struct Component_t* comp_, double dt)
{
	int x, y;
	int i;
	double horizontalAngle, verticalAngle;
	CallbackCompData_t* data = (CallbackCompData_t*)comp_->data;
}

static void onMsg(struct Component_t* comp_, char* msg_name, void* msg)
{
	int value = (int)msg;
	CallbackCompData_t* data = (CallbackCompData_t*)comp_->data;
	
	if(strcmp("mouse_button", msg_name) == 0)
	{
		if(data->callback)
		{
			data->callback(comp_->parent);
		}
	}
}

static void onRefresh(struct Component_t* comp_)
{
	Attribute_t* att = GameObject_QueryAttribute(comp_->parent, "callback");
	CallbackCompData_t* data = (CallbackCompData_t*)comp_->data;

	data->callback = (callback_f)Attribute_ToPointer(att);
}

Component_t* CreateCallbackComponent(void)
{
	Component_t* comp = (Component_t*)malloc(sizeof(Component_t));

	comp->init = init;
	comp->deinit = deinit;
	comp->onUpdate = onUpdate;
	comp->onMsg =  onMsg;
	comp->onRefresh = onRefresh;
	return comp;
}
