#include "components.h"
#include "math\vector2d.h"
#include "service\renderService.h"
#include "environment\environment.h"
#include "image\image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct RenderGuiCompData_t
{
	int*			isGui;
	Component_t*	base_component;
}RenderGuiCompData_t;

extern Component_t* CreateRenderComponent(void);

static void init(Component_t* comp_)
{
	Attribute_t* att = GameObject_QueryAttribute(comp_->parent, "is_gui");
	RenderGuiCompData_t* data = (RenderGuiCompData_t*)malloc(sizeof(RenderGuiCompData_t));

	if(att == 0)
	{
		att = Attribute_NewInteger(1);
		GameObject_AddAttribute(comp_->parent,att ,"is_gui");
	}

	data->isGui = Attribute_ToInteger(att);

	data->base_component = CreateRenderComponent();
	data->base_component->parent = comp_->parent;

	data->base_component->init(data->base_component);
	
	comp_->data = data;
}

static void deinit(Component_t* comp_)
{
	RenderGuiCompData_t* data = (RenderGuiCompData_t*)comp_->data;
	data->base_component->deinit(data->base_component);
}

static void onUpdate(struct Component_t* comp_, double dt)
{
	RenderGuiCompData_t* data = (RenderGuiCompData_t*)comp_->data;
	data->base_component->onUpdate(data->base_component, dt);
}

static void onMsg(struct Component_t* comp_, char* msg_name, void* msg)
{
	RenderGuiCompData_t* data = (RenderGuiCompData_t*)comp_->data;
	data->base_component->onMsg(data->base_component, msg_name, msg);
}

static void onRefresh(struct Component_t* comp_)
{
	RenderGuiCompData_t* data = (RenderGuiCompData_t*)comp_->data;
	data->base_component->onRefresh(data->base_component);
}

Component_t* CreateRenderGuiComponent(void)
{
	Component_t* comp = (Component_t*)malloc(sizeof(Component_t));

	comp->init = init;
	comp->deinit = deinit;
	comp->onUpdate = onUpdate;
	comp->onMsg =  onMsg;
	comp->onRefresh = onRefresh;
	return comp;
}
