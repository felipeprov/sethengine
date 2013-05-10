#include "components.h"
#include "math\vector2d.h"
#include "service\renderService.h"
#include "environment\environment.h"
#include "image\image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "service\message_service.h"

typedef struct PlayerCompData_t
{
	int zoomOut;
	int zooomIn;
	int moveUp;
	int moveDown;
	int moveLeft;
	int moveRight;
}PlayerCompData_t;

static void init(Component_t* comp_)
{
	PlayerCompData_t* data = (PlayerCompData_t*)malloc(sizeof(PlayerCompData_t));
	comp_->data = data;
	memset(data, 0, sizeof(PlayerCompData_t));
}

static void deinit(Component_t* comp_)
{
}

static void onUpdate(struct Component_t* comp_, double dt)
{
	PlayerCompData_t* data = (PlayerCompData_t*)(comp_->data);
	
	if(EnvironmentKeyIsPressed('Q'))	data->zoomOut = 1;
	if(EnvironmentKeyIsReleased('Q'))	data->zoomOut = 0;

	if(EnvironmentKeyIsPressed('E'))	data->zooomIn = 1;
	if(EnvironmentKeyIsReleased('E'))	data->zooomIn = 0;
	
	if(EnvironmentKeyIsPressed('W'))	data->moveUp = 1;
	if(EnvironmentKeyIsReleased('W'))	data->moveUp = 0;

	if(EnvironmentKeyIsPressed('S'))	data->moveDown = 1;
	if(EnvironmentKeyIsReleased('S'))	data->moveDown = 0;

	if(EnvironmentKeyIsPressed('A'))	data->moveLeft = 1;
	if(EnvironmentKeyIsReleased('A'))	data->moveLeft = 0;

	if(EnvironmentKeyIsPressed('D'))	data->moveRight = 1;
	if(EnvironmentKeyIsReleased('D'))	data->moveRight = 0;

	if(data->zoomOut)
	{
		message_service_send_msg(comp_->parent,"move", (void*)0);
	}

	if(data->zooomIn)
	{
		message_service_send_msg(comp_->parent,"move", (void*)1);
	}

	if(data->moveUp)
	{
		message_service_send_msg(comp_->parent,"move", (void*)4);
	}

	if(data->moveDown)
	{
		message_service_send_msg(comp_->parent,"move", (void*)5);
	}

	if(data->moveLeft)
	{
		message_service_send_msg(comp_->parent,"move", (void*)2);
	}

	if(data->moveRight)
	{
		message_service_send_msg(comp_->parent,"move", (void*)3);
	}
}

static void onMsg(struct Component_t* comp_, char* msg_name, void* msg)
{
}

Component_t* CreatePlayerComponent(void)
{
	Component_t* comp = (Component_t*)malloc(sizeof(Component_t));

	comp->init = init;
	comp->deinit = deinit;
	comp->onUpdate = onUpdate;
	comp->onMsg =  onMsg;
	comp->onRefresh = 0;
	return comp;
}
