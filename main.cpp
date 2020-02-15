// OpenGL Stuff
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// My includes
#include "drawing_primitives.h"
#include "init_shaders.h"

// Shader Program Handle
GLuint shader_program_handle;
// Window Dimensions
GLuint window_width = 640, window_height = 480;
// Handles for the VBO and FBOs
GLuint vertex_buffer_handle[2];
// MVP Matrices
glm::mat4 projection, view, model;

GLfloat deg = 45.0f;

std::vector<Point> points;
std::vector<Color> colors;

GLvoid ShaderPlumbing() {
  glPointSize(3);

  model = glm::rotate(glm::identity<glm::mat4>(), glm::radians(++deg),
                      glm::vec3(0, 1, 0));

  // MVP matrix
  glm::mat4 MVP = projection * view * model;
  GLuint mvp_handle = glGetUniformLocation(shader_program_handle, "MVP");
  glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, glm::value_ptr(MVP));

  // position data
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_handle[0]);
  glEnableVertexAttribArray(
      glGetAttribLocation(shader_program_handle, "aPosition"));
  glVertexAttribPointer(glGetAttribLocation(shader_program_handle, "aPosition"),
                        3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);

  // color data
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_handle[1]);
  glEnableVertexAttribArray(
      glGetAttribLocation(shader_program_handle, "aColor"));
  glVertexAttribPointer(glGetAttribLocation(shader_program_handle, "aColor"), 4,
                        GL_FLOAT, GL_FALSE, 0, (GLvoid *)0);
}

GLvoid Display(GLvoid) {
  glClearColor(0.3f, 0.3f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_POINT_SPRITE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  ShaderPlumbing();

  glDrawArrays(GL_POINTS, 0, points.size());

  glutSwapBuffers();
  glutPostRedisplay();
}

GLvoid InitShaders() {
  shader_program_handle = InitShader("basic_shader.vert", "basic_shader.frag");
  glUseProgram(shader_program_handle);
}

GLvoid HandleKeypress(GLubyte key, GLint x, GLint y) {
  switch (key) {
    case 27:
      glutDestroyWindow(glutGetWindow());
      exit(1);
      return;
      break;
    default:
      break;
  }
}

GLvoid HandleWindowReshape(GLint x, GLint y) {
  window_width = x;
  window_height = y;
  glViewport(0, 0, x, y);
  glutPostRedisplay();
}

GLint InitGL(GLint *argc, GLchar **argv) {
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(window_width, window_height);
  glutCreateWindow("OpenGL 3D Viewer Scaffold");
  glutDisplayFunc(Display);
  glutKeyboardFunc(HandleKeypress);
  glutReshapeFunc(HandleWindowReshape);

  // Needed on my old mac, hopefully does break everything everywhere else.
  glewExperimental = GL_TRUE;
  glewInit();

  return 1;
}

GLvoid InitBuffers() {
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_handle[0]);
  glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * points.size(),
               points.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_handle[1]);
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat) * colors.size(),
               colors.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLint main(GLint argc, GLchar **argv) {
  // Setting up our MVP Matrices
  model = glm::rotate(glm::identity<glm::mat4>(), glm::radians(45.0f),
                      glm::vec3(0, 1, 0));
  view = glm::lookAt(
      glm::vec3(3, 1.5, 3),  // Camera position in World Space
      glm::vec3(0, 0, 0),    // and looks at the origin
      glm::vec3(0, 1, 0)     // head is up (set to 0,-1,0 to look upside-down)
  );
  projection =
      glm::perspective(glm::radians(45.0f),
                       (float)window_width / (float)window_height, 0.1f, 10.0f);

  // Setting up the point positions and colors for our screen
  for (GLfloat x = 0.0f; x <= 1.0f; x += 0.1f) {
    for (GLfloat y = 0.0f; y <= 1.0f; y += 0.1f) {
      for (GLfloat z = 0.0f; z <= 1.0f; z += 0.1f) {
        points.push_back({x - 0.5f, y - 0.5f, z - 0.5f});
        colors.push_back({x, y, z, 1.0f});
      }
    }
  }

  if (InitGL(&argc, argv) == false) {
    return EXIT_FAILURE;
  }

  glGenBuffers(2, vertex_buffer_handle);

  InitShaders();
  InitBuffers();

  glutMainLoop();

  return EXIT_SUCCESS;
}