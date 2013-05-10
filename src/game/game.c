#include "game.h"
#include "scenes\scene.h"
#include "utils\list.h"
#include <stdlib.h>
#include "service\renderService.h"
#include <stdio.h>
#include "service\message_service.h"
#include "service\colision_service.h"

enum GameState_e
{
	GAME_INIT = 0,
	GAME_LOAD,
	GAME_RUNNING,
	GAME_PAUSED,
	GAME_EXIT
};

typedef struct SceneNode_t
{
	Scene_t* scene;
	struct list_head next;
}SceneNode_t;

typedef struct Game_t
{
	int state;
	Environment_t* env;
	struct list_head scenes;
}Game_t;

static Game_t my_game;

static void GameUpdateState(void)
{
	if(EnvironmentGetKey('P'))
	{
		if(my_game.state == GAME_RUNNING)
		{
			my_game.state = GAME_PAUSED;
		}
		else
		{
			my_game.state = GAME_RUNNING;
		}
	}
}
static int GameExit(void)
{
	//gets ESC
	return EnvironmentGetKey(257);
}

static void GameUpdate(float dt)
{
	SceneNode_t* scene_node;
	struct list_head* p;

	colision_service_update(dt);

	list_for_each(p, &my_game.scenes)
	{
		scene_node = list_entry(p, SceneNode_t, next);
		Scene_Update(scene_node->scene,dt);
	}
}

static void GameLoopPaused(float dt)
{
}

static void GameRender(void)
{
	EnvironmentRenderBegin();

	render_all();

	EnvironmentRenderGuiBegin();
	render_gui();

	EnvironmentRenderEnd();
}

static void GameLoopRunning(float dt)
{
	GameUpdate(dt);
}


void GameAddScene(Scene_t* scene)
{
	SceneNode_t* node = (SceneNode_t*)malloc(sizeof(SceneNode_t));
	node->scene = scene;

	list_add_tail(&node->next, &my_game.scenes);
}

static void GameInitComponents(void)
{
	extern Component_t* CreateLocationComponent(void);
	extern Component_t* CreateRenderComponent(void);
	extern Component_t* CreatePlayerComponent(void);
	extern Component_t* CreateCameraComponent(void);
	extern Component_t* CreateMouseMovementComponent(void);
	extern Component_t* CreateLimitComponent(void);
	extern Component_t* CreateCollisionComponent(void);
	extern Component_t* CreateCallbackComponent(void);
	extern Component_t* CreateRenderGuiComponent(void);

	Component_SystemInit();
	Component_RegisterNew("location", CreateLocationComponent);
	Component_RegisterNew("render", CreateRenderComponent);
	Component_RegisterNew("player", CreatePlayerComponent);
	Component_RegisterNew("camera", CreateCameraComponent);
	Component_RegisterNew("mouse", CreateMouseMovementComponent);
	Component_RegisterNew("limit", CreateLimitComponent);
	Component_RegisterNew("collision", CreateCollisionComponent);
	Component_RegisterNew("callback", CreateCallbackComponent);
	Component_RegisterNew("render_gui", CreateRenderGuiComponent);
}

void GameInit(Environment_t* env)
{
	my_game.state = GAME_INIT;
	EnvironmentInit();
	EnvironmentNewWindow("SethEngine", 600, 800);

	INIT_LIST_HEAD(&my_game.scenes);
	GameInitComponents();
	render_service_init();
	message_service_init();
	colision_service_init();
}

int  GameLoop(void)
{
	double last_time = EnvironmentGetTime();
	double current_time;
	double d_time = 0;
	double acc = 0;
	int fps = 0;
	do
	{
		current_time = EnvironmentGetTime();
		d_time = current_time - last_time;

		GameUpdateState();
		message_dispatch();
		EnvironmentUpdate();
		switch(my_game.state)
		{
		case GAME_INIT:
			my_game.state = GAME_RUNNING;
			break;

		case GAME_RUNNING:
			GameLoopRunning(d_time);
			break;

		case GAME_PAUSED:
			GameLoopPaused(d_time);
			break;

		case GAME_EXIT:
			break;
		}
		GameRender();

		acc += d_time;
		fps++;

		if(acc >= 1.0)
		{
			printf("FPS %d \n", fps);
			fps = 0;
			acc = 0.0;
		}

		last_time = current_time;
	}while( GameExit() == 0);

	return 0;
}