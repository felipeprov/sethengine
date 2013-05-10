#include "scene.h"
#include <stdlib.h>
#include "utils\list.h"
#include "gameobject\gameobject.h"

typedef struct GameObjectNode_t
{
	GameObject_t* gameobject;
	struct list_head next;
}GameObjectNode_t;

Scene_t* SceneNew(char* name, int order)
{
	Scene_t* scene = (Scene_t*)malloc(sizeof(Scene_t));
	scene->name = name;
	scene->state = SceneState_Loading;
	scene->order = order;

	INIT_LIST_HEAD(&scene->game_objects);

	return scene;
}

void Scene_Init(Scene_t* scene)
{
}

void Scene_Update(Scene_t* scene, float dt)
{
	GameObjectNode_t* go;
	struct list_head* p;

	list_for_each(p, &scene->game_objects)
	{
		go = list_entry(p, GameObjectNode_t, next);
		GameObject_Update(go->gameobject,dt);
	}
}

void Scene_AddGameObject(Scene_t* scene, GameObject_t* go)
{
	GameObjectNode_t* node = (GameObjectNode_t*)malloc(sizeof(GameObjectNode_t));
	node->gameobject = go;
	list_add_tail(&node->next, &scene->game_objects);
}