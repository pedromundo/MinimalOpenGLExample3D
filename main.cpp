//OpenGL Stuff
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

//My includes
#include "myDataStructures.h"
#include "initShaders.h"

GLvoid reshape(GLint x, GLint y);

GLboolean g_bExitESC = false;

//Shader Program Handle
GLuint basicShader;
//Window Dimensions
GLuint wWidth = 640, wHeight = 480;
//Data Dimensions
GLuint dWidth = 640, dHeight = 480, nvertices = dWidth * dHeight;
//Handlers for the VBO and FBOs
GLuint VertexArrayIDs[1], vertexbuffers[2];
//MVP Matrices
glm::mat4 Projection, View, Model;

//Using std::vector because ffs no one wants to work with arrays in 2016
std::vector<Point>* points = new std::vector<Point>();
std::vector<Color>* colors = new std::vector<Color>();


GLvoid shaderPlumbing(){
	//Point size 1 looks like shit
	glPointSize(2);

	//MVP matrix
	glm::mat4 MVP = Projection * Model * View;
	GLuint MVPId = glGetUniformLocation(basicShader, "MVP");
	glUniformMatrix4fv(MVPId, 1, GL_FALSE, glm::value_ptr(MVP));

	//position data
	glBindVertexArray(VertexArrayIDs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[0]);
	glEnableVertexAttribArray(glGetAttribLocation(basicShader, "aPosition"));
	glVertexAttribPointer(glGetAttribLocation(basicShader, "aPosition"), 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	//color data
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[1]);
	glEnableVertexAttribArray(glGetAttribLocation(basicShader, "aColor"));
	glVertexAttribPointer(glGetAttribLocation(basicShader, "aColor"), 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
}

GLvoid display(GLvoid){
	glClearColor(0.3f, 0.3f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	shaderPlumbing();

	glDrawArrays(GL_POINTS, 0, (GLsizei)nvertices);
	glutSwapBuffers();
	glutPostRedisplay();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLvoid initShaders() {
	basicShader = InitShader("basicShader.vert", "basicShader.frag");
	glUseProgram(basicShader);
}

GLvoid keyboard(GLubyte key, GLint x, GLint y)
{
	switch (key)
	{
	case 27:
		g_bExitESC = true;
#if defined (__APPLE__) || defined(MACOSX)
		exit(EXIT_SUCCESS);
#else
		glutDestroyWindow(glutGetWindow());
		return;
#endif
		break;
	default:
		break;
	}
}

GLvoid reshape(GLint x, GLint y)
{
	wWidth = x;
	wHeight = y;
	glViewport(0, 0, x, y);
	glutPostRedisplay();
}

GLint initGL(GLint *argc, GLchar **argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(wWidth, wHeight);
	glutCreateWindow("OpenGL 2D Viewer Scaffold");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glewInit();
	return 1;
}

inline GLfloat interpolate(const GLfloat a, const GLfloat b, const GLfloat coefficient)
{
	return a + coefficient * (b - a);
}

GLvoid initBufferData(){
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat)*nvertices, points->data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat)*nvertices, colors->data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLint main(GLint argc, GLchar **argv)
{
	//Setting up our MVP Matrices

	Model = glm::mat4(1.0f);
	View = glm::lookAt(
		glm::vec3(3, 3, 3), // Camera is at (3,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	Projection = glm::perspective(glm::radians(45.0f), (float)wWidth / (float)wHeight, 0.1f, 100.0f);

	//Setting up the point positions and colors for our screen
	for (GLuint i = 0; i < dHeight; i++)
	{
		for (GLuint j = 0; j < dWidth; j++)
		{
			points->push_back({ interpolate(-1.0f, 1.0f, j / (GLfloat)(dWidth - 1)), interpolate(1.0f, -1.0f, i / (GLfloat)(dHeight - 1)), 1.0f });
			colors->push_back({ interpolate(-1.0f, 1.0f, j / (GLfloat)(dWidth - 1)), interpolate(1.0f, -1.0f, i / (GLfloat)(dHeight - 1)), 1.0f, 1.0f });
		}
	}

#if defined(__linux__)
	setenv("DISPLAY", ":0", 0);
#endif	

	if (false == initGL(&argc, argv))
	{
		return EXIT_FAILURE;
	}

	glGenVertexArrays(1, VertexArrayIDs);
	glGenBuffers(2, vertexbuffers);

	initShaders();
	initBufferData();

	glutMainLoop();

	return EXIT_SUCCESS;
}