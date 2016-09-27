#include <cstdio>
#include <iostream>
#include <sys/stat.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "initShaders.h"

using namespace::std;

/* ************************************************************************* */
/*                                                                           */
/* ************************************************************************* */

// Create a NULL-terminated string by reading the provided file
static GLchar *readShaderSource(const GLchar *shaderFile) {
	FILE *fp = fopen(shaderFile, "r");

	if (fp == NULL) {
		return NULL;
	}

	struct stat st;
	stat(shaderFile, &st);

	GLint c, d = 0;
	GLchar *buf = new GLchar[st.st_size + 1];

	while ((c = fgetc(fp)) != EOF){
		buf[d] = c;
		++d;
	}

	buf[d] = '\0';

	fclose(fp);

	return buf;
}

/* ************************************************************************* */
/*                                                                           */
/* ************************************************************************* */

// Create a GLSL program object from vertex and fragment shader files
GLuint InitShader(const GLchar *vShaderFile, const GLchar *fShaderFile) {
	tShader shaders[2] = { { vShaderFile, GL_VERTEX_SHADER, NULL },
	{ fShaderFile, GL_FRAGMENT_SHADER, NULL } };

	GLuint program = glCreateProgram();

	for (GLint i = 0; i < 2; ++i) {
		Shader &s = shaders[i];
		s.source = readShaderSource(s.filename);
		if (shaders[i].source == NULL) {
			std::cerr << "Failed to read " << s.filename << std::endl;
			exit(EXIT_FAILURE);
		}

		GLuint shader = glCreateShader(s.type);
		glShaderSource(shader, 1, (const GLchar **)&s.source, NULL);
		glCompileShader(shader);

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			std::cerr << s.filename << " failed to compile:" << std::endl;
			GLint logSize;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			GLchar *logMsg = new GLchar[logSize];
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			std::cerr << logMsg << std::endl;
			delete[] logMsg;

			exit(EXIT_FAILURE);
		}

		delete[] s.source;

		glAttachShader(program, shader);
	}

	/* link  and error check */
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked) {
		std::cerr << "Shader program failed to link" << std::endl;
		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		GLchar *logMsg = new GLchar[logSize];
		glGetProgramInfoLog(program, logSize, NULL, logMsg);
		std::cerr << logMsg << std::endl;
		delete[] logMsg;

		exit(EXIT_FAILURE);
	}

	/* use program object */
	//glUseProgram(program);

	return program;
}