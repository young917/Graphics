#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.

struct model_info {
	float centerx, centery;
	float x, y;
	float r, theta;
	float rotate, size_x, size_y;
	model_info() {
		centerx = centery = x = y = r = theta = rotate = 0.0f;
		size_x = size_y = 1.0f;
	}
};

GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables
GLuint h_ShaderProgram; // handle to shader program
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;


model_info airplane;
model_info house;
model_info cake;
model_info car;
model_info sword;
model_info flower;

float win_width = 1200 * 0.95;
float win_height = 800 * 0.95;
double r = 1.0, g = 1.0, b = 1.0;
int leftbuttonpressed = 0;