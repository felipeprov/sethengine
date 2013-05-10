#include "components.h"
#include "math\vector2d.h"
#include "service\renderService.h"
#include "environment\environment.h"
#include "image\image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct RenderCompData_t
{
	Vector2d_t* position;
	Vector2d_t* size;
	double*		rotation;
	char*		imagepath;
	int *		layer;
	int*		is_gui;

	Vector3d_t*  debug_color;
	Image_t*		image;
}RenderCompData_t;

static void render(void* arg)
{
	Component_t* comp_  =  (Component_t*) arg;
	RenderCompData_t* data = (RenderCompData_t*)comp_->data;

	EnvironmentTransform(data->position,data->size, data->rotation);
	ImageSetTexture(data->image);
	//EnvironmentDraw();

	EnvironmentDrawDebug(data->debug_color->x, data->debug_color->y, data->debug_color->y);
}

static void init(Component_t* comp_)
{
	Attribute_t* att = GameObject_QueryAttribute(comp_->parent, "position");
	RenderCompData_t* data = (RenderCompData_t*)malloc(sizeof(RenderCompData_t));

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

	att = GameObject_QueryAttribute(comp_->parent, "is_gui");
	if(att == 0)
	{
		att = Attribute_NewInteger(0);
		GameObject_AddAttribute(comp_->parent, att ,"is_gui");
	}
	data->is_gui = Attribute_ToInteger(att);
	
	att = GameObject_QueryAttribute(comp_->parent, "rotation");
	if(att == 0)
	{
		att = Attribute_NewDouble(0);
		GameObject_AddAttribute(comp_->parent, att ,"rotation");
	}
	data->rotation = Attribute_ToDouble(att);

	att = GameObject_QueryAttribute(comp_->parent, "layer");
	if(att == 0)
	{
		att = Attribute_NewInteger(255);
		GameObject_AddAttribute(comp_->parent, att ,"layer");
	}
	data->layer = Attribute_ToInteger(att);

	att = GameObject_QueryAttribute(comp_->parent, "debug_color");
	if(att == 0)
	{
		data->debug_color = (Vector3d_t*)malloc(sizeof(Vector3d_t));
		memset(data->debug_color, 0, sizeof(Vector3d_t));
		att = Attribute_NewPointer(data->debug_color);
		GameObject_AddAttribute(comp_->parent, att ,"debug_color");
	}

	data->debug_color = (Vector3d_t*)Attribute_ToPointer(att);

	att = GameObject_QueryAttribute(comp_->parent, "imagepath");
	if(att == 0)
	{
		att = Attribute_NewString("C:/Users/Felipe/Desktop/firefox2005-icon-png.png");
		GameObject_AddAttribute(comp_->parent, att ,"imagepath");
	}
	data->imagepath = Attribute_ToString(att);
	ImageLoad_PNG(&data->image, data->imagepath);

	comp_->data = data;

	if(*data->is_gui)
	{
		render_service_add_listenner_gui(comp_->parent, render, comp_);
	}
	else
	{
		render_service_add_listenner(comp_->parent, render, comp_);
	}
}

static void deinit(Component_t* comp_)
{
}

static void onUpdate(struct Component_t* comp_, double dt)
{
}

static void onMsg(struct Component_t* comp_, char* msg_name, void* msg)
{
}

static void onRefresh(struct Component_t* comp_)
{
	RenderCompData_t* data = (RenderCompData_t*)comp_->data;
	Attribute_t* att = GameObject_QueryAttribute(comp_->parent, "imagepath");
	data->imagepath = Attribute_ToString(att);

	ImageLoad_PNG(&data->image, data->imagepath);

	render_service_sort();
}

Component_t* CreateRenderComponent(void)
{
	Component_t* comp = (Component_t*)malloc(sizeof(Component_t));

	comp->init = init;
	comp->deinit = deinit;
	comp->onUpdate = onUpdate;
	comp->onMsg =  onMsg;
	comp->onRefresh = onRefresh;
	return comp;
}
