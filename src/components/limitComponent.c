#include "components.h"
#include "math\vector2d.h"
#include "environment\environment.h"
#include "service\renderService.h"
#include "image\image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct LimitCompData_t
{
	Vector2d_t* position;
	Vector2d_t* limit_up;
	Vector2d_t* limit_bottom;
}LimitCompData_t;

static void init(Component_t* comp_)
{
	Attribute_t* att = GameObject_QueryAttribute(comp_->parent, "position");
	LimitCompData_t* data = (LimitCompData_t*)malloc(sizeof(LimitCompData_t));
	memset(data, 0, sizeof(LimitCompData_t));

	att = GameObject_QueryAttribute(comp_->parent, "position");
	if(att == 0)
	{
		att = Attribute_NewVector(0,0);
		GameObject_AddAttribute(comp_->parent,att ,"position");
	}
	data->position = Attribute_ToVector2d(att);

	att = GameObject_QueryAttribute(comp_->parent, "limit_up");
	if(att == 0)
	{
		att = Attribute_NewVector(4,6);
		GameObject_AddAttribute(comp_->parent,att ,"limit_up");
	}
	data->limit_up = Attribute_ToVector2d(att);

	att = GameObject_QueryAttribute(comp_->parent, "limit_bottom");
	if(att == 0)
	{
		att = Attribute_NewVector(-4,-6);
		GameObject_AddAttribute(comp_->parent,att ,"limit_bottom");
	}
	data->limit_bottom = Attribute_ToVector2d(att);


	comp_->data = data;
}

static void deinit(Component_t* comp_)
{
}

static void onUpdate(struct Component_t* comp_, double dt)
{
	LimitCompData_t* data = (LimitCompData_t*)comp_->data;

	if(data->position->x > data->limit_up->x)
		data->position->x = data->limit_up->x;

	if(data->position->y > data->limit_up->y)
		data->position->y = data->limit_up->y;

	if(data->position->x < data->limit_bottom->x)
		data->position->x = data->limit_bottom->x;

	if(data->position->y < data->limit_bottom->y)
		data->position->y = data->limit_bottom->y;
}

static void onMsg(struct Component_t* comp_, char* msg_name, void* msg)
{
	int value = (int)msg;
	LimitCompData_t* data = (LimitCompData_t*)comp_->data;
}

static void onRefresh(struct Component_t* comp_)
{
}

Component_t* CreateLimitComponent(void)
{
	Component_t* comp = (Component_t*)malloc(sizeof(Component_t));

	comp->init = init;
	comp->deinit = deinit;
	comp->onUpdate = onUpdate;
	comp->onMsg =  onMsg;
	comp->onRefresh = onRefresh;
	return comp;
}
