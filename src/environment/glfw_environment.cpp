
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
#include "math\vector2d.h"

static GLuint gVAO;
static GLuint gVBO;
static GLuint program;
static GLuint program_gui;
static int g_mouse_x = 0, g_mouse_y = 0;
static GLuint currentProgram;

#define USE_PROGRAM(x) do{ glUseProgram(x); currentProgram = x;}while(0)

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
"    gl_Position =  camera*model*vec4(vert, 1.0);\n"
"}";

static char vertexShader_gui[] = 
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
"    gl_Position =  projection*model*vec4(vert, 1.0);\n"
"}";

static char fragmentShader[] = "#version 150\n"
	"uniform sampler2D tex;\n"
	"uniform vec4 debugcolor\n;"
	"uniform int debug;\n"
	"out vec4 finalColor;\n"
	"in vec2 fragTexCoord;\n"
	"void main() {\n"
    "//note: the texture function was called texture2D in older versions of GLSL\n"
	"if(debug == 0)\n"
	"finalColor = texture(tex, fragTexCoord);\n"
	"else\n"
	"finalColor = texture(tex, fragTexCoord)+debugcolor;"
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

static void glfwSetCamera_ext(char* attribute, float x, float y, float z)
{
	glm::mat4 camerap = glm::perspective<float>(z, 800.0f/600, 0.01, 10.0);
    //camera *= orientation;
    //camera = glm::translate(camera, -glm::vec3(x,y,0));

	glm::mat4 camera = glm::lookAt(glm::vec3(x,y,5), glm::vec3(x,y,0), glm::vec3(0,1,0));

	camera = camerap*camera;
	ShaderSetUniform(ShaderGetUniform(currentProgram,attribute), 1, glm::value_ptr(camera));
}

static void glfwSetCamera(float x, float y, float z)
{
	glfwSetCamera_ext("camera", x, y, z);
}

static void mouseCallback(int x, int y)
{
	g_mouse_x = x;
	g_mouse_y = y;
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

	shaders[0] = ShaderCreateFromStr(GL_VERTEX_SHADER, vertexShader_gui);
	shaders[1] = ShaderCreateFromStr(GL_FRAGMENT_SHADER, fragmentShader);

	program_gui = ShaderCreateProgram(shaders, 2);

	USE_PROGRAM(program);

	error = glGetError();
	if(error != GL_NO_ERROR)
		printf( "OpenGL Error %d: %s\n", error, (const char*)glewGetErrorString(error));

	/*glm::mat4 camera = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
	ShaderSetUniform(ShaderGetUniform(program,"camera"), 1, glm::value_ptr(camera));
	*/
	glfwSetCamera( 0, 0, 1);

	glm::mat4 projection = glm::perspective<float>(60.0, 800.0f/600, 0.01, 10.0);
	ShaderSetUniform(ShaderGetUniform(currentProgram,"projection"), 1,  glm::value_ptr(projection));
	
	glm::mat4 rotation = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0,1,0));
	ShaderSetUniform(ShaderGetUniform(currentProgram,"model"),1, glm::value_ptr(rotation));

	ShaderSetUniformInt(ShaderGetUniform(currentProgram,"debug"), 1);
	ShaderSetUniform4f(ShaderGetUniform(currentProgram,"debugcolor"), 0.0, 0, 0, 0.0f);

	USE_PROGRAM(0);
	USE_PROGRAM(program_gui);

	glfwSetCamera_ext( "projection", 0,0, 60);
	rotation = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0,1,0));
	ShaderSetUniform(ShaderGetUniform(currentProgram,"model"),1, glm::value_ptr(rotation));

	ShaderSetUniformInt(ShaderGetUniform(currentProgram,"debug"), 1);
	ShaderSetUniform4f(ShaderGetUniform(currentProgram,"debugcolor"), 0.0, 0, 0, 0.0f);

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
	glVertexAttribPointer(ShaderGetAttrib(program, "vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);

	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(ShaderGetAttrib(program,"vertTexCoord"));
	glVertexAttribPointer(ShaderGetAttrib(program,"vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
    
		// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(ShaderGetAttrib(program_gui, "vert"));
	glVertexAttribPointer(ShaderGetAttrib(program_gui, "vert"), 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), NULL);

	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(ShaderGetAttrib(program_gui,"vertTexCoord"));
	glVertexAttribPointer(ShaderGetAttrib(program_gui,"vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
    

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	error = glGetError();
	if(error != GL_NO_ERROR)
		printf( "OpenGL Error %d: %s\n", error, (const char*)glewGetErrorString(error));

		glfwSetMousePos(800/2, 600/2);
		glfwDisable(GLFW_MOUSE_CURSOR);
}

static int glfw_getkey(int key)
{
	if(glfwGetKey(key))
		return 1;
	else 
	return 0;
}

static void glfwTransform(Vector2d_t *position, Vector2d_t *size, double *rotation)
{
	glm::mat4 transf = glm::mat4();

	transf = glm::translate(transf, glm::vec3(position->x, position->y,0));	
	transf = glm::rotate(transf,  (float)(*rotation), glm::vec3(0,0,1));
	transf = glm::scale(transf, glm::vec3(size->x, size->y,0));

	ShaderSetUniform(ShaderGetUniform(currentProgram,"model"), 1, glm::value_ptr(transf));
}
static void glfw_render_begin(void)
{
	GLenum err;
	glClearColor(0.2, 0.2, 0.2, 1); // black
				glClear(GL_COLOR_BUFFER_BIT);

	USE_PROGRAM(program);
	glBindVertexArray(gVAO);
}

static void glfw_render_gui_begin(void)
{
	USE_PROGRAM(program_gui);
	glBindVertexArray(gVAO);
}


static void glfw_render_end(void)
{
	glBindVertexArray(0);
	USE_PROGRAM(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glfwSwapBuffers();
}

static void glfwDraw(void)
{
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

static void glfwDrawDebug(float v0, float v1, float v2)
{
	ShaderSetUniform4f(ShaderGetUniform(currentProgram,"debugcolor"), v0, v1, v2, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

static void glfw_update(float dt_) 
{
	glfwGetWindowParam(GLFW_OPENED);	
}

static double glfw_get_time(void)
{
	return glfwGetTime();
}

void glfw_mouseGetDelta(int* dx, int* dy)
{
	glfwGetMousePos(dx, dy);
	glfwSetMousePos(800/2, 600/2);
}

int glfw_mouseGetButtons(int button)
{
	return glfwGetMouseButton(button);
}

extern "C" void GLFW_Environment_Init(Environment_t *env)
{
	env->init = glfw_init;
	env->getkey = glfw_getkey;
	env->newwindow = glfw_newwindow;
	env->update = glfw_update;
	env->render_begin = glfw_render_begin;
	env->render_end = glfw_render_end;
	env->render_gui_begin = glfw_render_gui_begin;
	env->transform = glfwTransform;
	env->draw = glfwDraw;
	env->drawDebug = glfwDrawDebug;
	env->setCamera = glfwSetCamera;
	env->getTime = glfw_get_time;
	env->mouseGetDelta = glfw_mouseGetDelta;
	env->mouseGetButton = glfw_mouseGetButtons;
}