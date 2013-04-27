
#include "environment\environment.h"
#include <stdio.h>
#include "glfw_environment.h"

static Environment_t g_env;
static char g_keys[256];

void EnvironmentInit(void)
{
	GLFW_Environment_Init(&g_env);	

	g_env.init();
}

void EnvironmentNewWindow(char* name, int height, int width)
{
	g_env.newwindow(name, height, width);
}

char EnvironmentGetKey(char key)
{
	return g_keys[key];
}

void EnvironmentUpdate(void)
{
	int i=0;
	g_env.update(0);
	for(i=0; i<256; i++)
	{
		g_keys[i] = g_env.getkey(i);
	}
}

void EnvironmentRenderBegin(void)
{
	g_env.render_begin();
}

void EnvironmentRenderEnd(void)
{
	g_env.render_end();
}