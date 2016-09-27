#ifndef MY_DATA_STRUCTURES
#define MY_DATA_STRUCTURES 1

#include <vector>
#include <GL/gl.h>

using namespace std;

typedef struct {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} Point;

typedef struct {
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
} Color;

typedef struct {
	Point pto;
	Point normal;
	GLint ID;
	GLint valencia;
	Color color;
} Vertice;

typedef struct {
	GLint ID;
	GLint indV[3];
	Point normal;
} Face;

typedef struct {
	GLint ID;
	vector<Vertice *> vPoint;
	vector<Face *> vFace;
} Object;

typedef struct {
	GLfloat *vPoint;
	GLfloat *vNormal;
	GLfloat *vColor;
	GLfloat *vTextCoord;
	GLuint *vFace;
} ObjectVA;

struct MyVertex {
	GLfloat x, y, z;    // Vertex
	GLfloat nx, ny, nz; // Normal
	GLfloat s0, t0;     // Texcoord0
};

#endif
