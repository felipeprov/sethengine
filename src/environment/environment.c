
#include "environment\environment.h"
#include <stdio.h>
#include "glfw_environment.h"

static Environment_t g_env;
static int g_keys_old[260];
static int g_keys[260];

void EnvironmentInit(void)
{
	GLFW_Environment_Init(&g_env);	

	g_env.init();
}

void EnvironmentNewWindow(char* name, int height, int width)
{
	g_env.newwindow(name, height, width);
}

int EnvironmentGetKey(int key)
{
	return g_keys[key];
}

void EnvironmentUpdate(void)
{
	int i=0;
	g_env.update(0);
	for(i=0; i<260; i++)
	{
		g_keys_old[i] = g_keys[i];
		g_keys[i] = g_env.getkey(i);
	}
}

int EnvironmentKeyIsPressed(int key)
{
	return (g_keys[key] == 1) && (g_keys_old[key] == 0);
}

int EnvironmentKeyIsReleased(int key)
{
	return (g_keys[key] == 0) && (g_keys_old[key] == 1);
}

void EnvironmentRenderBegin(void)
{
	g_env.render_begin();
}

void EnvironmentRenderEnd(void)
{
	g_env.render_end();
}

void EnvironmentRenderGuiBegin(void)
{
	g_env.render_gui_begin();
}

void EnvironmentTransform(Vector2d_t *translation, Vector2d_t *scale, double *rotation)
{
	g_env.transform(translation, scale, rotation);
}

void EnvironmentDraw(void)
{
	g_env.draw();
}

void EnvironmentSetCamera(float x, float y, float z)
{
	g_env.setCamera(x, y, z);

	//printf("camara position %f %f %f\n",x, y, z);
}

double EnvironmentGetTime(void)
{
	return g_env.getTime();
}

void EnvironmentGetMouse(int *x, int *y)
{
	g_env.mouseGetDelta(x, y);
}

int EnvironmentGetMouseButton(int button)
{
	return g_env.mouseGetButton(button);
}

void EnvironmentDrawDebug(float v0, float v1, float v2)
{
	g_env.drawDebug(v0, v1, v2);
}