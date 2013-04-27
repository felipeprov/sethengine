
#include "GL/glew.h"
#include <GL/glfw.h>
#include <stdio.h>
#include "environment.h"
#include "Windows.h"
#include "shaders\Shader.h"
#include <math.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "image/image.h"

static GLuint gVAO;
static GLuint gVBO;
static GLuint program;

static char vertexShader[] = 
""
"#version 150\n"
"\n"
"\n"
"uniform mat4 camera;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"in vec3 vert;\n"
"in vec2 vertTexCoord;\n"
"out vec2 fragTexCoord;\n"
"\n"
"void main() {\n"
    "// Apply all matrix transformations to vert\n"
	"fragTexCoord = vertTexCoord;\n"
"    gl_Position =  projection*camera*model*vec4(vert, 1.0);\n"
"}";

static char fragmentShader[] = "#version 150\n"
	"uniform sampler2D tex;\n"
	"out vec4 finalColor;\n"
	"in vec2 fragTexCoord;\n"
	"void main() {\n"
    "//note: the texture function was called texture2D in older versions of GLSL\n"
	"finalColor = texture(tex, fragTexCoord);\n"
"}";


static void glfw_init(void)
{
	GLint data[4];
	AllocConsole();
	freopen("CONIN$", "r", stdin); 
	freopen("CONOUT$", "w", stdout); 
	freopen("CONOUT$", "w", stderr);

	if(glfwInit() == 0)
	{
		printf("Glfw init failed\n");
	}
}

static void glfw_newwindow(char* name, int heigth, int width)
{
	int major, minor, rev;
	GLenum error;
	GLuint shaders[2];
	GLint  data[4];

	GLfloat vertexData[] = {
		//  X     Y    Z
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
		1.0f, 1.0f,-1.0f,   1.0f, 1.0f
	};

	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	if(!glfwOpenWindow(width, heigth, 8, 8, 8, 8, 16, 0, GLFW_WINDOW))
		printf("glfwOpenWindow failed. Can your hardware handle OpenGL 3.2?");
    
	if(glewInit() != GLEW_OK)
		printf("glewInit failed");

	glShadeModel(GL_SMOOTH);   
	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);  
	glClearDepth(1.0f);                         // Depth Buffer Setup
			
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);    

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glAlphaFunc( GL_GREATER, 0 );		
	glEnable(GL_POLYGON_SMOOTH);

	// print out some info about the graphics drivers
	printf( "OpenGL version: %s \n", glGetString(GL_VERSION));
	printf( "GLSL version: %s \n",glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("Vendor: %s\n",glGetString(GL_VENDOR));
	printf("Renderer: %s\n",glGetString(GL_RENDERER));


	shaders[0] = ShaderCreateFromStr(GL_VERTEX_SHADER, vertexShader);
	shaders[1] = ShaderCreateFromStr(GL_FRAGMENT_SHADER, fragmentShader);

	program = ShaderCreateProgram(shaders, 2);

	glUseProgram(program);

	error = glGetError();
	if(error != GL_NO_ERROR)
		printf( "OpenGL Error %d: %s\n", error, (const char*)glewGetErrorString(error));

	glm::mat4 camera = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,1,0));
	ShaderSetUniform(ShaderGetUniform(program,"camera"), 1, glm::value_ptr(camera));
	
	glm::mat4 projection = glm::perspective<float>(90.0, 800.0f/600, 0.1, 10.0);
	ShaderSetUniform(ShaderGetUniform(program,"projection"), 1,  glm::value_ptr(projection));
	
	glm::mat4 rotation = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0,1,0));
	ShaderSetUniform(ShaderGetUniform(program,"model"),1, glm::value_ptr(rotation));
	
	glUseProgram(0);

	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	// make and bind the VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);
	// Put the three triangle verticies into the VBO

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(ShaderGetAttrib(program, "vert"));
	//glVertexAttribPointer(ShaderGetAttrib(program, "vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(ShaderGetAttrib(program, "vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);

	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(ShaderGetAttrib(program,"vertTexCoord"));
	glVertexAttribPointer(ShaderGetAttrib(program,"vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
    
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	error = glGetError();
	if(error != GL_NO_ERROR)
		printf( "OpenGL Error %d: %s\n", error, (const char*)glewGetErrorString(error));

	//ImageLoad_PNG(&img,"C:/Users/Felipe/Desktop/firefox2005-icon-png.png");


	//glGetIntegerv(GL_ACTIVE_TEXTURE, data);
	//printf("GL_ACTIVE_TEXTURE: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_BLEND, data);
	//printf("GL_BLEND: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_BLEND_COLOR, data);
	//printf("GL_BLEND_COLOR: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_BLEND_SRC_ALPHA, data);
	//printf("GL_BLEND_SRC_ALPHA: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_BLEND_DST_ALPHA, data);
	//printf("GL_BLEND_DST_ALPHA: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_BLEND_EQUATION_RGB, data);
	//printf("GL_BLEND_EQUATION_RGB: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_BLEND_EQUATION_ALPHA, data);
	//printf("GL_BLEND_EQUATION_ALPHA: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_CURRENT_PROGRAM, data);
	//printf("GL_CURRENT_PROGRAM: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_DEPTH_FUNC, data);
	//printf("GL_DEPTH_FUNC: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_COLOR_CLEAR_VALUE, data);
	//printf("GL_COLOR_CLEAR_VALUE: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_COLOR_WRITEMASK, data);
	//printf("GL_COLOR_WRITEMASK: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_CURRENT_PROGRAM, data);
	//printf("GL_CURRENT_PROGRAM: 0x%08X\n", data[0]);

	//glGetIntegerv(GL_CURRENT_PROGRAM, data);
	//printf("GL_CURRENT_PROGRAM: 0x%08X\n", data[0]);
}

static char glfw_getkey(char key)
{
	if(glfwGetKey(key))
		return 1;
	else 
	return 0;
}

static void glfw_render_begin(void)
{
	GLenum err;
	glClearColor(0.5, 0.5, 0.5, 1); // black
				glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program);
	glBindVertexArray(gVAO);
}

static void glfw_render_end(void)
{
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glfwSwapBuffers();
}

static void glfw_update(float dt_) 
{
	glfwGetWindowParam(GLFW_OPENED);	
}

extern "C" void GLFW_Environment_Init(Environment_t *env)
{
	env->init = glfw_init;
	env->getkey = glfw_getkey;
	env->newwindow = glfw_newwindow;
	env->update = glfw_update;
	env->render_begin = glfw_render_begin;
	env->render_end = glfw_render_end;
}