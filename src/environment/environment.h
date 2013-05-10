#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

#include "math\vector2d.h"
typedef struct Environment_t
{
	void (*init)(void);
	void (*newwindow)(char* name, int height, int width);
	int (*getkey)(int keys);
	void (*render_begin)(void);
	void (*render_end)(void);
	void (*render_gui_begin)(void);
	void (*update)(float dt_);
	void (*transform)(Vector2d_t *position, Vector2d_t *size, double *rotation);
	void (*draw)(void);
	void (*drawDebug)(float v0, float v1, float v2);
	void (*setCamera)(float x, float y, float z);
	double (*getTime)(void);
	void	(*mouseGetDelta)(int *dx, int *dy);
	int	 (*mouseGetButton)(int button);
	//set transform
}Environment_t;

#ifdef __cplusplus
extern "C" {  
#endif  

void EnvironmentInit(void);
void EnvironmentNewWindow(char* name, int height, int width);
int EnvironmentGetKey(int key);
int EnvironmentKeyIsPressed(int key);
int EnvironmentKeyIsReleased(int key);
void EnvironmentUpdate(void);
void EnvironmentRenderBegin(void);
void EnvironmentRenderGuiBegin(void);
void EnvironmentRenderEnd(void);
void EnvironmentTransform(Vector2d_t *translation, Vector2d_t *scale, double *rotation);
void EnvironmentDraw(void);
void EnvironmentDrawDebug(float v0, float v1, float v2);
double EnvironmentGetTime(void);
void EnvironmentSetCamera(float x, float y, float z);
void EnvironmentGetMouse(int *x, int *y);
int EnvironmentGetMouseButton(int button);

#ifdef __cplusplus
}
#endif

#endif
