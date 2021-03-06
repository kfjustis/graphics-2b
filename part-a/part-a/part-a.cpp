// part-a.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection


#include "GL/glew.h"
#include <stdio.h>
#include <stdlib.h>
#include "GLFW/glfw3.h"
#include <iostream>

const int NumPoints = 5000;
const int MaxGaskets = 10;
int i = 2;
int total_points = NumPoints * MaxGaskets;
int current_points = 0;
GLfloat points[NumPoints*MaxGaskets];
GLuint shader_programme;
GLuint vao;
GLuint buffer;
//----------------------------------------------------------------------------
void init(void)
{

	GLuint vert_shader, frag_shader;
	/* GL shader programme object [combined, to link] */

	// Create and initialize a buffer object
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Create a vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// "attribute #0 is created from every 2 variables in the above buffer, of type
	// float (i.e. make me vec2s)"
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// Load shaders and use the resulting shader program
	const char *vertex_shader = "#version 410\n"
		"in vec4 vPosition;"
		"void main () {"
		"	gl_Position = vPosition;"
		"}";
	/* the fragment shader colours each fragment (pixel-sized area of the
	triangle) */
	const char *fragment_shader = "#version 410\n"
		"out vec4 FragColor;"
		"void main () {"
		"	FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );"
		"}";

	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vertex_shader, NULL);
	glCompileShader(vert_shader);
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &fragment_shader, NULL);
	glCompileShader(frag_shader);
	shader_programme = glCreateProgram();
	glAttachShader(shader_programme, frag_shader);
	glAttachShader(shader_programme, vert_shader);
	glLinkProgram(shader_programme);
}

void drawGasket(float x, float y) {
	if (current_points == total_points) {
		// stop drawing when he hit the max allowed points
		return;
	}
	GLfloat vertices[] = { x - 1.0f/4, y - 1.0f/4,  x, y + 1.0f/4,  x + 1.0f/4, y - 1.0f/4 };
	points[0] = x + 0.25;
	points[1] = y - 0.5;
	current_points += NumPoints;
	for (i; i < current_points; i += 2) {
		int j = (int) rand() % 3;   // pick a vertex at random
							  // Compute the point halfway between the selected vertex
							  //  and the previous point
		points[i] = (points[i - 2] + vertices[2 * j]) / 2.0;
		points[i + 1] = (points[i - 1] + vertices[2 * j + 1]) / 2.0;
	}
}

void mymouse(GLFWwindow* window, int button, int action, int mods) {
	if (GLFW_PRESS == action) {
		double x_screen, y_screen;
		float x_world, y_world;
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glfwGetCursorPos(window, &x_screen, &y_screen);

		// transform screen to world coordinates
		x_world = (2.0f * x_screen / width) - 1.0f;
		y_world = 1.0f - (2.0f * y_screen) / height;

		drawGasket(x_world, y_world);

		//glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
		//glGenVertexArrays(1, &vao);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		// "attribute #0 is created from every 2 variables in the above buffer, of type
		// float (i.e. make me vec2s)"
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	}
}


//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	GLFWwindow *window = NULL;
	const GLubyte *renderer;
	const GLubyte *version;
	/* start GL context and O/S window using the GLFW helper library */
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	/* We must specify 3.2 core if on Apple OS X -- other O/S can specify
	anything here. I defined 'APPLE' in the makefile for OS X
	Remove the #ifdef #endif and play around with this - you should be starting
	an explicit version anyway, and some non-Apple drivers will require this too.
	*/
#ifdef APPLE
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	window = glfwCreateWindow(640, 480, "Sierpinski Gasket - kfjmbb", NULL, NULL);

	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	/* start GLEW extension handler */
	glewExperimental = GL_TRUE;
	glewInit();
	renderer = glGetString(GL_RENDERER); /* get renderer string */
	version = glGetString(GL_VERSION);	 /* version as a string */
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	init();

	// mouse callback for re-drawing gasket
	glfwSetMouseButtonCallback(window, mymouse);

	do {
		glClear(GL_COLOR_BUFFER_BIT);     // clear the window
		glUseProgram(shader_programme);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, total_points); // draw the points
												  /* update other events like input handling */
		glfwPollEvents();
		glfwSwapBuffers(window);
	} while (!glfwWindowShouldClose(window));

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;

}

