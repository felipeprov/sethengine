#ifndef __GAME_H
#define __GAME_H

#include "environment\environment.h"
#include "scenes\scene.h"

#ifdef __cplusplus
extern "C" {  
#endif  

void GameInit(Environment_t* env);
int  GameLoop(void);

void GameAddScene(Scene_t* scene);

#ifdef __cplusplus
}
#endif

#endif
