
#include <stdlib.h>
#include <stdio.h>

#include "scenes/scene.h"
#include "game/game.h"
#include "attributes/attributes.h"

GameObject_t* AddGenericObj(Scene_t* scene, float x, float y);

Scene_t* g_scene;

void callback_btn(void* arg)
{
	GameObject_t* obj = (GameObject_t*)(arg);
	Attribute_t* att = GameObject_QueryAttribute(obj, "position");
	Vector2d_t *position = Attribute_ToVector2d(att);

	AddGenericObj(g_scene, position->x, position->y);
}

GameObject_t* AddMouseObj(Scene_t* scene)
{
	GameObject_t* obj;

	obj = GameObjectNew();
	GameObject_AddComponent(obj,Component_New(obj,"limit"), "limit");
	GameObject_AddComponent(obj,Component_New(obj,"location"), "comp1");
	GameObject_AddComponent(obj,Component_New(obj,"render"), "comp2");
	GameObject_AddComponent(obj,Component_New(obj,"mouse"), "mouse");
	GameObject_AddComponent(obj, Component_New(obj, "collision"), "collision");
	GameObject_AddComponent(obj, Component_New(obj, "callback"), "callback");

	GameObject_SetAttributeString(obj, "imagepath", "D:/2.CurrentProjects/git/sethengine/build/content/images/mouse_cursor.png");
	GameObject_SetAttributeVector2d(obj, "position", 10, 0);
	GameObject_SetAttributeVector2d(obj, "size", 0.5, 0.5);
	GameObject_SetAttributePointer(obj, "callback", callback_btn);
	GameObject_SetAttributeInt(obj, "layer", 1);
	
	GameObject_Refresh(obj);
	Scene_AddGameObject(scene, obj);
	g_scene = scene;

	return obj;
}

GameObject_t* AddGenericObj(Scene_t* scene, float x, float y)
{
	GameObject_t* obj;
	obj = GameObjectNew();
	GameObject_AddComponent(obj,Component_New(obj,"location"), "comp1");
	GameObject_AddComponent(obj,Component_New(obj,"render"), "comp2");
	GameObject_AddComponent(obj, Component_New(obj, "collision"), "collision");

	GameObject_SetAttributeString(obj, "imagepath", "D:/2.CurrentProjects/git/sethengine/build/content/images/firefox2005-icon-png.png");
	GameObject_SetAttributeVector2d(obj, "position", x, y);
	GameObject_SetAttributeVector2d(obj, "size", 0.4, 0.4);
	GameObject_SetAttributeInt(obj, "layer", 2);

	GameObject_Refresh(obj);
	Scene_AddGameObject(scene, obj);

	return obj;
}

GameObject_t* AddGuiObj(Scene_t* scene, float x, float y)
{
	GameObject_t* obj;
	obj = GameObjectNew();
	GameObject_AddComponent(obj,Component_New(obj,"location"), "comp1");
	GameObject_AddComponent(obj,Component_New(obj,"render_gui"), "comp2");
	//GameObject_AddComponent(obj, Component_New(obj, "collision"), "collision");

	GameObject_SetAttributeString(obj, "imagepath", "D:/2.CurrentProjects/git/sethengine/build/content/images/firefox2005-icon-png.png");
	GameObject_SetAttributeVector2d(obj, "position", x, y);
	GameObject_SetAttributeVector2d(obj, "size", 0.5, 0.4);
	GameObject_SetAttributeInt(obj, "layer", 2);

	GameObject_Refresh(obj);
	Scene_AddGameObject(scene, obj);

	return obj;
}

GameObject_t* AddBackgroundcObj(Scene_t* scene)
{
	GameObject_t* obj;
	obj = GameObjectNew();
	GameObject_AddComponent(obj,Component_New(obj,"location"), "comp1");
	GameObject_AddComponent(obj,Component_New(obj,"render"), "comp2");

	GameObject_SetAttributeVector2d(obj, "position", 0, 0);
	GameObject_SetAttributeVector2d(obj, "size", 4, 6);
	GameObject_SetAttributeString(obj, "imagepath", "D:/2.CurrentProjects/git/sethengine/build/content/images/battlefield.png");
	
	GameObject_Refresh(obj);
	Scene_AddGameObject(scene, obj);

	return obj;
}

GameObject_t* AddCameraObj(Scene_t* scene, GameObject_t *target)
{
	GameObject_t* obj;

	obj = GameObjectNew();
	GameObject_AddComponent(obj,Component_New(obj,"limit"), "limit");
	GameObject_AddComponent(obj,Component_New(obj,"location"), "location");
	GameObject_AddComponent(obj,Component_New(obj,"camera"), "camera");
	//GameObject_AddComponent(obj,Component_New(obj,"player"), "control");

	GameObject_SetAttributeVector2d(obj, "limit_up", 1, 3);
	GameObject_SetAttributeVector2d(obj, "limit_bottom", -1, -3);
	GameObject_SetAttributePointer(obj, "target", target);
	GameObject_Refresh(obj);

	Scene_AddGameObject(scene, obj);

	return obj;
}

GameObject_t* UpdateGameObjectWithTarget(GameObject_t* obj, GameObject_t* target)
{
	GameObject_SetAttributePointer(obj, "target", target);
	GameObject_Refresh(obj);

	return obj;
}

int main(int argc, char* argv[])
{
	Scene_t* scene;
	GameObject_t* obj, *target;
	GameObject_t* camera;
	printf("SethEngineC starting\n");

	GameInit(0);

	scene = SceneNew("inicio", 0);
	AddBackgroundcObj(scene);
	obj = AddMouseObj(scene);
	AddGenericObj(scene, 0.5, 0.5);
	AddGenericObj(scene, 0.5, -0.5);
	AddGenericObj(scene, -0.5, 0.5);
	//AddGenericObj(scene, 0, 0);

	AddGuiObj(scene, 2.8, 0.5);
	AddGuiObj(scene, 2.8, 1.5);
	AddGuiObj(scene, 2.8, 2);
	
	camera = AddCameraObj(scene, obj);

	UpdateGameObjectWithTarget(obj, camera);

	GameAddScene(scene);

	return GameLoop();
}