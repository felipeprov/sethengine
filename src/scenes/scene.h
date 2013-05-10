#ifndef __SCENE_H
#define __SCENE_H

#include "utils\list.h"
#include "gameobject\gameobject.h"

enum SceneState_e
{
	SceneState_Loading = 0,
	SceneState_Running,
	SceneState_End
};

typedef struct Scene_t
{
	char* name;
	int order;
	int state;
	struct list_head game_objects;
}Scene_t;

#ifdef __cplusplus
extern "C" {  
#endif  

Scene_t* SceneNew(char* name, int order);
void Scene_Init(Scene_t* scene);
void Scene_Update(Scene_t* scene, float dt);
void Scene_AddGameObject(Scene_t* scene, GameObject_t* go);

#ifdef __cplusplus
}
#endif

#endif