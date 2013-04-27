#ifndef __SHADERS_H
#define __SHADERS_H


#include <stdint.h>
#include <GL/glew.h>

#ifdef __cplusplus
extern "C" {  
#endif  

uint32_t ShaderCreateFromFile(GLenum shaderType, char* filename);
uint32_t ShaderCreateFromStr(GLenum shaderType, char* shader);

GLuint ShaderCreateProgram(GLuint* shaderList, int size);
GLint ShaderGetAttrib(GLint program, const GLchar* attribName) ;
GLint ShaderGetUniform(GLint program, const GLchar* uniformName) ;
void ShaderSetUniform(GLint location, int size, GLfloat* value);


#ifdef __cplusplus
}
#endif
#endif