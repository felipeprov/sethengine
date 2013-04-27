#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

typedef struct Environment_t
{
	void (*init)(void);
	void (*newwindow)(char* name, int height, int width);
	char (*getkey)(char keys);
	void (*render_begin)(void);
	void (*render_end)(void);
	void (*update)(float dt_);

	//set transform
}Environment_t;

#ifdef __cplusplus
extern "C" {  
#endif  

void EnvironmentInit(void);
void EnvironmentNewWindow(char* name, int height, int width);
char EnvironmentGetKey(char key);
void EnvironmentUpdate(void);
void EnvironmentRenderBegin(void);
void EnvironmentRenderEnd(void);

#ifdef __cplusplus
}
#endif

#endif
