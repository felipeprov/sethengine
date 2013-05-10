
#include "Shader.h"
#include "GL/glew.h"
#include <stdlib.h>
#include <stdio.h>

uint32_t ShaderCreateFromFile(GLenum shaderType, char* filename)
{
	return 0;
}

uint32_t ShaderCreateFromStr(GLenum shaderType, char* shaderStr)
{
	GLuint shader = glCreateShader(shaderType);
	const char *strFileData = shaderStr;
	GLint status;

    glShaderSource(shader, 1, &strFileData, 0);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
    {
        GLint infoLogLength;
        GLchar *strInfoLog;
		const char *strShaderType = NULL;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        strInfoLog = (GLchar*)malloc( infoLogLength + 1);

		
        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
        
        switch(shaderType)
        {
        case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
        case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
        case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
        }
        
        printf( "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
        free( strInfoLog);
    }

	return shader;

}

GLuint ShaderCreateProgram(GLuint* shaderList, int size)
{
    GLuint program = glCreateProgram();
    int i =0;
	GLint status;

    for(i = 0; i < size; i++)
    	glAttachShader(program, shaderList[i]);
    
    glLinkProgram(program);
        
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
		GLchar *strInfoLog;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        
        strInfoLog = (GLchar*)malloc( infoLogLength + 1);
        glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
        printf( "Linker failure: %s\n", strInfoLog);
        free(strInfoLog);
    }
    
    for(i = 0; i < size; i++)
        glDetachShader(program, shaderList[i]);

    return program;
}

GLint ShaderGetAttrib(GLint program, const GLchar* attribName) 
{
	GLint attrib;
    if(!attribName)
        printf("attribName was NULL");
    
    attrib = glGetAttribLocation(program, attribName);
    if(attrib == -1)
        printf("Program attribute not found: %d", program);
    
    return attrib;
}

GLint ShaderGetUniform(GLint program, const GLchar* uniformName) 
{
	GLint uniform;
    if(!uniformName)
        printf("uniformName was NULL");
    
    uniform = glGetUniformLocation(program, uniformName);
    if(uniform == -1)
        printf("Program uniform not found:%s",  uniformName);
    
    return uniform;
}


void ShaderSetUniform(GLint location, int size, GLfloat* value)
{
	glUniformMatrix4fv(location,1,0, value);
}

void ShaderSetUniformInt(GLint location, int value)
{
	glUniform1i(location, value);
}

void ShaderSetUniform4f(GLint location, float v0, float v1, float v2, float v3)
{
	glUniform4f(location, v0, v1, v2, v3);
}