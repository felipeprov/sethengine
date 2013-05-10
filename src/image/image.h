#ifndef __IMAGE_H
#define __IMAGE_H

#include <GL/glew.h>

typedef struct Image_t
{
	GLubyte* texture;
	int width;
	int hight;
	int hasAlpha;
	GLuint  texture_handler;
}Image_t;

#ifdef __cplusplus
extern "C" {  
#endif  

int ImageLoad_PNG(Image_t** image, char* name);
void ImageSetTexture(Image_t* img);

#ifdef __cplusplus
}
#endif

#endif