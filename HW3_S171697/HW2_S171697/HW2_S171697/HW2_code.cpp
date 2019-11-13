#define _CRT_SECURE_NO_WARNINGS

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shaders/LoadShaders.h"

#include "Camera.h"
#include "Geometry.h"

GLuint h_ShaderProgram; // handle to shader program

// #include glm/*.hpp only if necessary
// #include <glm/glm.hpp> 
#include <glm/gtc/matrix_inverse.hpp> //inverse, affineInverse, etc.

glm::mat4 ModelViewProjectionMatrix;

glm::mat4 ModelMatrix_CAR_BODY, ModelMatrix_CAR_WHEEL, ModelMatrix_CAR_NUT, ModelMatrix_CAR_DRIVER;
glm::mat4 ModelMatrix_COW;
glm::mat4 ModelMatrix_TEAPOT;
glm::mat4 ModelMatrix_TIGER;
glm::mat4 ModelMatrix_HUMAN;

typedef struct {
	float x, y, z;
	float angle;
	float size;
} Object_Location;

Object_Location Tiger_loc;
Object_Location Bus_loc;
Object_Location Bus_start;
Object_Location Pot_loc;
float initial_velocity;
float ver_velocity;
float initial_theta;
glm::vec3 direction;
Object_Location Human_loc;
Object_Location Tank_loc;
Object_Location Cow_loc;
float rotation_angle_car = 0.0f;
float angle_per_rotate = 1.0f;
bool wheel_rotate_flag = false;

#define rad 1.7f
#define ww 1.0f
float wheel_rot_angle() {
	static float angle = 0.0f;
	static float dist;

	if (wheel_rotate_flag == false)
		return angle;
	wheel_rotate_flag = false;
	dist = 20.0f *(3.141592) * angle_per_rotate / 360.0f;
	angle += 180.0f * dist / (3.141592 * rad);
	if (angle >= 360.0f)
		angle -= 360.0f;
	return angle;
}

void draw_wheel_and_nut(int CAMERA_TYPE) {
	// angle is used in Hierarchical_Car_Correct later
	int i;

	glUniform3f(loc_primitive_color, 1.000f, 0.552f, 0.094f); // color name: orange
	draw_geom_obj(GEOM_OBJ_ID_CAR_WHEEL); // draw wheel

	for (i = 0; i < 5; i++) {

		ModelMatrix_CAR_NUT = glm::rotate(ModelMatrix_CAR_WHEEL, TO_RADIAN*72.0f*i, glm::vec3(0.0f, 0.0f, 1.0f));
		ModelMatrix_CAR_NUT = glm::translate(ModelMatrix_CAR_NUT, glm::vec3(rad - 0.5f, 0.0f, ww));
		ModelViewProjectionMatrix = ViewProjectionMatrix[CAMERA_TYPE] * ModelMatrix_CAR_NUT;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);

		glUniform3f(loc_primitive_color, 0.627f, 0.101f, 0.325f); // color name: Light Purple
		draw_geom_obj(GEOM_OBJ_ID_CAR_NUT); // draw i-th nut
	}
}

void draw_car_dummy(int CAMERA_TYPE) {
	float angle;

	glUniform3f(loc_primitive_color, 0.000f, 0.725f, 0.471f); // color name: light green
	draw_geom_obj(GEOM_OBJ_ID_CAR_BODY); // draw body
	/*
	glLineWidth(2.0f);
	draw_axes(); // draw MC axes of body
	glLineWidth(1.0f);

	ModelMatrix_CAR_DRIVER = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.0f, 0.5f, 2.5f));
	ModelMatrix_CAR_DRIVER = glm::rotate(ModelMatrix_CAR_DRIVER, TO_RADIAN*90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[CAMERA_TYPE] * ModelMatrix_CAR_DRIVER;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(5.0f);
	draw_axes(); // draw camera frame at driver seat
	glLineWidth(1.0f);*/

	angle = wheel_rot_angle();
	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.9f, -3.5f, 4.5f));
	if (angle_per_rotate > 0.0) {
		ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, 9.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, angle*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[CAMERA_TYPE] * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut(CAMERA_TYPE);  // draw wheel 0

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(3.9f, -3.5f, 4.5f));
	if (angle_per_rotate < 0.0) {
		ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, -9.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, angle*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[CAMERA_TYPE] * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut(CAMERA_TYPE);  // draw wheel 1

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(-3.9f, -3.5f, -4.5f));
	if (angle_per_rotate > 0.0) {
		ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, 9.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, angle*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[CAMERA_TYPE] * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut(CAMERA_TYPE);  // draw wheel 2

	ModelMatrix_CAR_WHEEL = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(3.9f, -3.5f, -4.5f));
	if (angle_per_rotate < 0.0) {
		ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, -9.0f*TO_RADIAN, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	ModelMatrix_CAR_WHEEL = glm::rotate(ModelMatrix_CAR_WHEEL, angle*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix_CAR_WHEEL = glm::scale(ModelMatrix_CAR_WHEEL, glm::vec3(1.0f, 1.0f, -1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix[CAMERA_TYPE] * ModelMatrix_CAR_WHEEL;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_wheel_and_nut(CAMERA_TYPE);  // draw wheel 3
}

void set_ViewMat(int CAMERA_TYPE) {//by camera position or by object position
	glm::mat4 Matrix_CAMERA_inverse;

	switch (CAMERA_TYPE) {
	case CAMERA_WM:
	case CAMERA_WS:
		ViewMatrix[CAMERA_TYPE] = glm::lookAt(camera[CAMERA_TYPE].prp, camera[CAMERA_TYPE].vrp, camera[CAMERA_TYPE].vup);
		break;
	case CAMERA_TIGER:
		Matrix_CAMERA_inverse = ModelMatrix_TIGER * ModelMatrix_TO_EYE;
		ViewMatrix[CAMERA_TIGER] = glm::affineInverse(Matrix_CAMERA_inverse);
		break;
	case CAMERA_DRIVER:
		Matrix_CAMERA_inverse = ModelMatrix_CAR_BODY * ModelMatrix_To_DRIVER;
		ViewMatrix[CAMERA_DRIVER] = glm::affineInverse(Matrix_CAMERA_inverse);
		break;
	}
}

void set_ViewProjectionMatrix(int CAMERA_TYPE) {
	bool setting = false;

	if (Camera_Flag[CAMERA_TYPE][ViewChange]) {
		set_ViewMat(CAMERA_TYPE);
		Camera_Flag[CAMERA_TYPE][ViewChange] = false;
		setting = true;
	}
	if (Camera_Flag[CAMERA_TYPE][ProjectionChange]) {
		ProjectionMatrix[CAMERA_TYPE] = glm::perspective(camera[CAMERA_TYPE].zoom_factor * camera[CAMERA_TYPE].fovy*TO_RADIAN, camera[CAMERA_TYPE].aspect_ratio,
			camera[CAMERA_TYPE].near_c, camera[CAMERA_TYPE].far_c);
		Camera_Flag[CAMERA_TYPE][ProjectionChange] = false;
		setting = true;
	}
	if (setting) {
		ViewProjectionMatrix[CAMERA_TYPE] = ProjectionMatrix[CAMERA_TYPE] * ViewMatrix[CAMERA_TYPE];
	}
}

/*********************************  START: callbacks *********************************/
int tiger_frame = 0;
int human_frame = 0;

void display_at_camera(int CAMERA_TYPE) {

	glm::vec3 axis;
	float angle;

	// set model matrix
	//  				tiger
	ModelMatrix_TIGER = glm::translate(glm::mat4(1.0f), glm::vec3(Tiger_loc.x, Tiger_loc.y, 0.0f));
	ModelMatrix_TIGER = glm::rotate(ModelMatrix_TIGER, Tiger_loc.angle *TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix_TIGER = glm::rotate(ModelMatrix_TIGER, 90.0f *TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix_TIGER = glm::scale(ModelMatrix_TIGER, glm::vec3(0.2f, 0.2f, 0.2f));
	
	//					car
	ModelMatrix_CAR_BODY = glm::rotate(glm::mat4(1.0f), rotation_angle_car, glm::vec3(0.0f, 0.0f, 1.0f));// move circle
	ModelMatrix_CAR_BODY = glm::translate(ModelMatrix_CAR_BODY, glm::vec3(20.0f, 0.0f, 10.0f));
	ModelMatrix_CAR_BODY = glm::rotate(ModelMatrix_CAR_BODY, -90.0f*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix_CAR_BODY = glm::rotate(ModelMatrix_CAR_BODY, 90.0f*TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix_CAR_BODY = glm::scale(ModelMatrix_CAR_BODY, glm::vec3(2.0f, 2.0f, 2.0f));

	// set view-projection matrix
	glViewport(viewport[CAMERA_TYPE].x, viewport[CAMERA_TYPE].y, viewport[CAMERA_TYPE].w, viewport[CAMERA_TYPE].h);
	set_ViewProjectionMatrix(CAMERA_TYPE);

	//draw slope
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[CAMERA_TYPE], glm::vec3(-90.0f, 50.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale( ModelViewProjectionMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw_slope();

	//  draw the floor
	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix[CAMERA_TYPE], glm::vec3(100.0f, 100.0f, 100.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	draw_square();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// draw axes
	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix[CAMERA_TYPE], glm::vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(5.0f);
	draw_axes();// world frame
	glLineWidth(1.0f);

	// draw teapot
	axis = glm::vec3(Pot_loc.y, -Pot_loc.x, 0.0f);
	angle = atan(Pot_loc.y / Pot_loc.x) / TO_RADIAN;
	if (Pot_loc.x > 0)
		angle += 180.0f;
	angle *= TO_RADIAN;
	//				draw axis;
	ModelMatrix_TEAPOT = glm::translate(ViewProjectionMatrix[CAMERA_TYPE], glm::vec3(Pot_loc.x, Pot_loc.y, Pot_loc.z));
	ModelMatrix_TEAPOT = glm::rotate(ModelMatrix_TEAPOT, Pot_loc.angle , axis);

	ModelViewProjectionMatrix = glm::rotate(ModelMatrix_TEAPOT, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(10.0f, 10.0f, 10.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(5.0f);
	draw_axes();
	glLineWidth(1.0f);

	ModelViewProjectionMatrix = glm::translate(ModelMatrix_TEAPOT, glm::vec3(0.0f, 0.0f, 7.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.278f, 0.639f, 0.835f);
	draw_geom_obj(GEOM_OBJ_ID_TEAPOT);

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[CAMERA_TYPE], glm::vec3(Bus_loc.x, Bus_loc.y, Bus_loc.z));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, Bus_loc.angle, glm::vec3(1.0f, 0.0f, 0.0f));

	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(2.0f, 2.0f, 2.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, 90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.26f, 0.43f, 0.71f);
	draw_geom_obj(GEOM_OBJ_ID_BUS);

	// draw human and tank, cow;
	ModelMatrix_HUMAN = glm::translate(ViewProjectionMatrix[CAMERA_TYPE], glm::vec3(Human_loc.x, Human_loc.y, Human_loc.z));
	ModelViewProjectionMatrix = glm::rotate(ModelMatrix_HUMAN, Human_loc.angle * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(35.0f, 35.0f, 35.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, -90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.467f, 0.275f, 0.467f);
	draw_geom_obj(GEOM_OBJ_ID_HUMAN + human_frame);

	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[CAMERA_TYPE], glm::vec3(Tank_loc.x, Tank_loc.y, Tank_loc.z));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	ModelViewProjectionMatrix = glm::translate(ModelViewProjectionMatrix, glm::vec3(0.0f, -14.0f, 0.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.514f, 0.573f, 0.031f);
	draw_geom_obj(GEOM_OBJ_ID_TANK);
	
	ModelViewProjectionMatrix = glm::translate(ViewProjectionMatrix[CAMERA_TYPE], glm::vec3(Cow_loc.x, Cow_loc.y, Cow_loc.z));
	ModelViewProjectionMatrix = glm::translate(ModelViewProjectionMatrix, glm::vec3(0.0f, 0.0f, 10.0f));
	ModelViewProjectionMatrix = glm::rotate(ModelViewProjectionMatrix, 90.0f * TO_RADIAN, glm::vec3(1.0f, 0.0f, 0.0f));
	ModelViewProjectionMatrix = glm::scale(ModelViewProjectionMatrix, glm::vec3(30.0f, 30.0f, 30.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.584f, 0.318f, 0.03f);
	draw_geom_obj(GEOM_OBJ_ID_COW);
	/*
	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix[CAMERA_TYPE], glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.99f, 0.388f, 0.447f);
	draw_geom_obj(GEOM_OBJ_ID_TIGER + tiger_frame);
	
	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix[CAMERA_TYPE], glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_car_dummy(CAMERA_TYPE);*/
	
	// draw tiger
	ModelViewProjectionMatrix = ViewProjectionMatrix[CAMERA_TYPE] * ModelMatrix_TIGER;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glUniform3f(loc_primitive_color, 0.99f, 0.388f, 0.447f);
	draw_geom_obj(GEOM_OBJ_ID_TIGER + tiger_frame);
	
	// draw car
	ModelViewProjectionMatrix = ViewProjectionMatrix[CAMERA_TYPE] * ModelMatrix_CAR_BODY;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_car_dummy(CAMERA_TYPE);
	
	/*
	ModelViewProjectionMatrix = glm::scale(ViewProjectionMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	glLineWidth(2.0f);
	draw_axes();// world frame
	glLineWidth(1.0f);

	ModelViewProjectionMatrix = ViewProjectionMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_path();// put path in world

	if (flag_draw_world_objects)
		draw_objects_in_world();
		*/
}

void display(void) {
	int type;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (type = 0; type < CAMERA_NUM; type++) {
		if (Camera_Flag[type][SwitchOn])
			display_at_camera(type);
	}
	glutSwapBuffers();
}

bool moving_flag = false;

void slope_motion(int not_use) {
	static int during = 0;
	float dist;
	float rate;

	if (moving_flag == false) {// stop motion
		during = 0;
		glutPostRedisplay();
		return;
	}
	else if (Bus_loc.y < -100.0f || (during == 0 && Bus_loc.z == 0.0f) ){
		Bus_loc.x = Bus_start.x;
		Bus_loc.y = Bus_start.y;
		Bus_loc.z = Bus_start.z;
		during = 0;
		Bus_loc.angle = Bus_start.angle;
		glutPostRedisplay();
	}
	else if(Bus_loc.z == 0.0f) {// straight motion
		rate = 9.8f * sin(Bus_start.angle) * during;
		Bus_loc.angle = 0.0f;
		Bus_loc.y -= rate;
		glutPostRedisplay();
		glutTimerFunc(200, slope_motion, 0);
	}
	else {
		during += 1;
		dist = 0.5f * 9.8f * sin(Bus_start.angle) * during * during;
		Bus_loc.y -= dist * cos(Bus_start.angle);
		Bus_loc.z -= dist * sin(Bus_start.angle);
		if (Bus_loc.z <= 0.0f)
			Bus_loc.z = 0.0f;
		glutPostRedisplay();
		glutTimerFunc(200, slope_motion, 0);
	}
}

void arc_motion(int time) {
	float hor_dist, ver_dist, total_dist;
	float tmp_angle;
	glm::vec3 tmp;

	ver_velocity -= 9.8f;
	hor_dist = initial_velocity * cos(initial_theta * TO_RADIAN);
	ver_dist = ver_velocity;
	total_dist = sqrt(hor_dist * hor_dist + ver_dist * ver_dist);
	tmp_angle = Pot_loc.angle + (total_dist / 10.0f);
	tmp_angle /= TO_RADIAN;
	if (tmp_angle > 360.0f)
		tmp_angle -= 360.0f;
	Pot_loc.angle = tmp_angle * TO_RADIAN;
	tmp = hor_dist * direction;
	Pot_loc.x += tmp[0];
	Pot_loc.y += tmp[1];
	Pot_loc.z += ver_dist;

//	printf("x	:	%f	y	:	%f	z	:	%f\n", Pot_loc.x, Pot_loc.y, Pot_loc.z);

	if (Pot_loc.z < 0.0f) {
		if (Pot_loc.x <= -100.0f)
			Pot_loc.x = -100.0f;
		else if (Pot_loc.x >= 100.0f)
			Pot_loc.x = 100.0f;
		if (Pot_loc.y <= -100.0f)
			Pot_loc.y = -100.0f;
		else if (Pot_loc.y >= 100.0f)
			Pot_loc.y = 100.0f;
		Pot_loc.z = 0.0f;
		Pot_loc.angle = 0.0f;
		initial_theta = 0.0f;
		glutPostRedisplay();
		return;
	}
	else {
		glutPostRedisplay();
		glutTimerFunc(300, arc_motion, time + 1);
	}
}

void tiger_scene(int theta) {
	static int a = 3;
	static int size = 15;
	static int move_flag = 0;// -1: -1씩 X축에서 움직이기, 0: 별모양대로 움직이기, 1: +1씩 x축에서 움직이기
	float prevx, prevy;
	float r, delx, dely;

	if (move_flag != 0) {
		Tiger_loc.x += move_flag * 1.0f;

		if (move_flag == 1) {
			Tiger_loc.angle = 0;
			if (Tiger_loc.x >= (size*4.0f))
				move_flag = 0;
		}
		else {
			Tiger_loc.angle = 180;
			if (Tiger_loc.x <= (size*4.0f))
				move_flag = 0;
		}
		Camera_Flag[CAMERA_TIGER][ViewChange] = true;
		glutPostRedisplay();
		tiger_frame = (tiger_frame + 1) % N_TIGER_FRAMES;
		glutTimerFunc(150, tiger_scene, theta);
	}
	else {
		prevx = Tiger_loc.x;
		prevy = Tiger_loc.y;
		r = size * (cos(5 * theta*TO_RADIAN) + a);
		Tiger_loc.x = r * cos(theta*TO_RADIAN);
		Tiger_loc.y = r * sin(theta*TO_RADIAN);
		delx = Tiger_loc.x - prevx;
		dely = Tiger_loc.y - prevy;
		Tiger_loc.angle = atan(dely / delx) / TO_RADIAN;
		if (delx < 0)
			Tiger_loc.angle += 180;
		Camera_Flag[CAMERA_TIGER][ViewChange] = true;
		glutPostRedisplay();
		tiger_frame = (tiger_frame + 1) % N_TIGER_FRAMES;
		if ((theta + 1) == 360) {
			if (size == 20) {
				size = 15;
				move_flag = -1;
			}
			else {
				size = 20;
				move_flag = 1;
			}
		}
		glutTimerFunc(150, tiger_scene, (theta + 1) % 360);
	}
}

void car_scene(int timestamp_scene) {
	rotation_angle_car = (timestamp_scene % 360)*TO_RADIAN;
	Camera_Flag[CAMERA_DRIVER][ViewChange] = true;
	wheel_rotate_flag = true;
	glutPostRedisplay();
	if(angle_per_rotate >0)
		glutTimerFunc(50, car_scene, (timestamp_scene + 1) % INT_MAX);
	else
		glutTimerFunc(50, car_scene, (timestamp_scene - 1) % INT_MAX);
}

int dir, move;
//dir 0: x, dir 1: y

void cow_scene(int not_use) {
	static float velocity = 7.0f;
	static float accel = -0.7f;

	if (velocity <= 0.0f) {
		velocity = 7.0f;
		return;
	}

	if (dir == 0) {// x axis
		if (move < 0) {// minus direction
			Cow_loc.x -= velocity;
		}
		else {// plus direction
			Cow_loc.x += velocity;
		}
	}
	else {// y axis
		if (move < 0) {// minus direction
			Cow_loc.y -= velocity;
		}
		else {// plus direction
			Cow_loc.y += velocity;
		}
	}
	
	if (Cow_loc.x < -100.0f) {
		Cow_loc.x = -100.0f;
		glutPostRedisplay();
		return;
	}
	else if (Cow_loc.x > 100.0f) {
		Cow_loc.x = 100.0f;
		glutPostRedisplay();
		return;
	}
	else if (Cow_loc.y < -100.0f) {
		Cow_loc.y = -100.0f;
		glutPostRedisplay();
		return;
	}
	else if (Cow_loc.y > 100.0f) {
		Cow_loc.y = 100.0f;
		glutPostRedisplay();
		return;
	}

	velocity += accel;

	glutPostRedisplay();
	glutTimerFunc(200, cow_scene, 0);
}

void tank_scene(int not_use) {
	static float velocity = 5.0f;
	static float accel = -1.4f;

	if (velocity <= 0.0f) {
		velocity = 5.0f;
		return;
	}

	if (dir == 0) {// x axis
		if (move < 0) {// minus direction
			Tank_loc.x -= velocity;
		}
		else {// plus direction
			Tank_loc.x += velocity;
		}
	}
	else {// y axis
		if (move < 0) {// minus direction
			Tank_loc.y -= velocity;
		}
		else {// plus direction
			Tank_loc.y += velocity;
		}
	}

	if (Tank_loc.x < -100.0f) {
		Tank_loc.x = -100.0f;
		glutPostRedisplay();
		return;
	}
	else if (Tank_loc.x > 100.0f) {
		Tank_loc.x = 100.0f;
		glutPostRedisplay();
		return;
	}
	else if (Tank_loc.y < -100.0f) {
		Tank_loc.y = -100.0f;
		glutPostRedisplay();
		return;
	}
	else if (Tank_loc.y > 100.0f) {
		Tank_loc.y = 100.0f;
		glutPostRedisplay();
		return;
	}
	velocity += accel;

	glutPostRedisplay();
	glutTimerFunc(200, tank_scene, 0);
}

int static_friction_cow = 5;
int static_friction_tank = 10;

#define human_move 4.0f

void detect_near_object() {
	int meet_flag = false;
	static int flag = 0;

	float delcx, delcy;
	float deltx, delty;

	static int until_power_for_cow = 0;
	static int until_power_for_tank = 0;

	delcx = -Human_loc.x + Cow_loc.x;
	delcy = -Human_loc.y + Cow_loc.y;
	deltx = -Human_loc.x + Tank_loc.x;
	delty = -Human_loc.y + Tank_loc.y;

	if (dir == 0) {
		if (move < 0) {
			if (delcx >= -10.0f && delcx <= 0.0f && delcy >= -5.0f && delcy <= 5.0f) {// flag : 1
				until_power_for_tank = 0;
				if(flag == 1)
					until_power_for_cow++;
				else {
					flag = 1;
					until_power_for_cow = 1;
				}
				if (until_power_for_cow >= static_friction_cow) {
					until_power_for_cow = 0;
					glutTimerFunc(50, cow_scene, 0);
				}
			}
			else if (deltx >= -15.0f && deltx <= 0.0f && delty >= -20.0f && delty <= 20.0f) {// flag : 1
				until_power_for_cow = 0;
				if(flag == 1)
					until_power_for_tank++;
				else {
					flag = 1;
					until_power_for_tank = 1;
				}
				if (until_power_for_tank >= static_friction_tank) {
					until_power_for_tank = 0;
					glutTimerFunc(50, tank_scene, 0);
				}
			}
			else {
				Human_loc.x += move;
				Human_loc.angle = -90.0f;
				human_frame = (human_frame + 1) % N_HUMAN_FRAMES;
				glutPostRedisplay();
			}
		}
		else {
			if (delcx <= 10.0f && delcx >= 0.0f && delcy >= -5.0f && delcy <= 5.0f) {// flag : 2
				until_power_for_tank = 0;
				if(flag == 2)
					until_power_for_cow++;
				else {
					flag = 2;
					until_power_for_cow = 1;
				}
				if (until_power_for_cow >= static_friction_cow) {
					until_power_for_cow = 0;
					glutTimerFunc(50, cow_scene, 0);
				}
			}
			else if (deltx <= 15.0f && deltx >= 0.0f && delty >= -20.0f && delty <= 20.0f) {// flag : 2
				until_power_for_cow = 0;
				if(flag == 2)
					until_power_for_tank++;
				else {
					flag = 2;
					until_power_for_tank = 1;
				}
				if (until_power_for_tank >= static_friction_tank) {
					until_power_for_tank = 0;
					glutTimerFunc(50, tank_scene, 0);
				}
			}
			else {
				Human_loc.x += move;
				Human_loc.angle = 90.0f;
				human_frame = (human_frame + 1) % N_HUMAN_FRAMES;
				glutPostRedisplay();
			}
		}
	}
	else {
		if (move < 0) {
			if (delcy >= -5.0f && delcy <= 0.0f && delcx >= -10.0f && delcx <= 10.0f) {// flag : 3
				until_power_for_tank = 0;
				if(flag == 3)
					until_power_for_cow++;
				else {
					flag = 3;
					until_power_for_cow = 1;
				}
				if (until_power_for_cow >= static_friction_cow) {
					until_power_for_cow = 0;
					glutTimerFunc(50, cow_scene, 0);
				}
			}
			else if (delty >= -20.0f && delty <= 0.0f && deltx >= -15.0f && deltx <= 15.0f) {// flag : 3
				until_power_for_cow = 0;
				if(flag == 3)
					until_power_for_tank++;
				else {
					flag = 3;
					until_power_for_tank = 1;
				}
				if (until_power_for_tank >= static_friction_tank) {
					until_power_for_tank = 0;
					glutTimerFunc(50, tank_scene, 0);
				}
			}
			else {
				Human_loc.y += move;
				Human_loc.angle = 0.0f;
				human_frame = (human_frame + 1) % N_HUMAN_FRAMES;
				glutPostRedisplay();
			}
		}
		else {
			if (delcy <= 5.0f && delcy >= 0.0f && delcx >= -10.0f && delcx <= 10.0f) {// flag : 4
				until_power_for_tank = 0;
				if(flag == 4)
					until_power_for_cow++;
				else {
					flag = 4;
					until_power_for_cow = 1;
				}
				if (until_power_for_cow >= static_friction_cow) {
					until_power_for_cow = 0;
					glutTimerFunc(50, cow_scene, 0);
				}
			}
			else if (delty <= 20.0f && delty >= 0.0f && deltx >= -15.0f && deltx <= 15.0f) {// flag : 4
				until_power_for_cow = 0;
				if(flag == 4)
					until_power_for_tank++;
				else {
					flag = 4;
					until_power_for_tank = 1;
				}
				if (until_power_for_tank >= static_friction_tank) {
					until_power_for_tank = 0;
					glutTimerFunc(50, tank_scene, 0);
				}
			}
			else {
				Human_loc.y += move;
				Human_loc.angle = 180.0f;
				human_frame = (human_frame + 1) % N_HUMAN_FRAMES;
				glutPostRedisplay();
			}
		}
	}
	/*
	//cow
	delcx = Human_loc.x - Cow_loc.x;
	delcy = Human_loc.y - Cow_loc.y;
	if ((delx >= -10.0f) && (delx <= 10.0f) && (dely >= -5.0f) && (dely <= 5.0f)) {
		if (dir == 0)
			Cow_loc.x += move;
		else
			Cow_loc.y += move;
	}
	//tank: 
	deltx = Human_loc.x - Tank_loc.x;
	delty = Human_loc.y - Tank_loc.y;
	if ((delx >= -2.0f) && (delx <= 2.0f) && (dely >= -4.0f) && (dely <= 4.0f)) {
		if (dir == 0)
			Tank_loc.x += move;
		else
			Tank_loc.y += move;
	}
	*/
}
unsigned int leftbutton_pressed;
int prevx, prevy;

void motion(int x, int y) {
	float del_x, del_y;

	if (leftbutton_pressed == 0)
		return;
	if (GLUT_ACTIVE_SHIFT == glutGetModifiers()) {// zoom in&out CAMERA_WM (Main Camera)
		del_x = ( x - prevx );
		renew_cam_zoom(CAMERA_WM, del_x*0.3f);
		glutPostRedisplay();
	}
	else if(Camera_Flag[CAMERA_WS][SwitchOn] == 1){// move CAMERA_WS (Sub Camera)
		del_x = (float)(x - prevx);
		del_y = (float)(prevy - y);
		renew_cam_position(CAMERA_WS, del_x, Uaxis);
		renew_cam_position(CAMERA_WS, del_y, Vaxis);
		glutPostRedisplay();
	}
}

void mousepress(int button, int state, int x, int y) {

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		prevx = x, prevy = y;
		leftbutton_pressed = 1;
		glutPostRedisplay();
	}
	else {
		leftbutton_pressed = 0;
		glutPostRedisplay();
	}
}

#define pot_move 2.0f

void keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 'i':// World Sub Camera Switch
		Camera_Flag[CAMERA_WS][SwitchOn] = (Camera_Flag[CAMERA_WS][SwitchOn] + 1) % 2;
		glutPostRedisplay();
		break;
	case 'o':// Tiger Camera Switch
		Camera_Flag[CAMERA_TIGER][SwitchOn] = (Camera_Flag[CAMERA_TIGER][SwitchOn] + 1) % 2;
		glutPostRedisplay();
		break;
	case 'p':// Driver Camera Switch
		Camera_Flag[CAMERA_DRIVER][SwitchOn] = (Camera_Flag[CAMERA_DRIVER][SwitchOn] + 1) % 2;
		glutPostRedisplay();
		break;
	//World Sub Camera control
	case 'a':// World Sub Camera rotate left
		if (Camera_Flag[CAMERA_WS][SwitchOn] == 0)
			break;
		renew_cam_orientation_rotation_around_v_axis(CAMERA_WS, 1);
		glutPostRedisplay();
		break;
	case 'd':// World Sub Camera rotate right
		if (Camera_Flag[CAMERA_WS][SwitchOn] == 0)
			break;
		renew_cam_orientation_rotation_around_v_axis(CAMERA_WS, -1);
		glutPostRedisplay();
		break;
	case 'w':// World Sub Camera move front
		if (Camera_Flag[CAMERA_WS][SwitchOn] == 0)
			break;
		renew_cam_position(CAMERA_WS, -150.0f, Naxis);
		glutPostRedisplay();
		break;
	case 's':// World Sub Camera move back
		if (Camera_Flag[CAMERA_WS][SwitchOn] == 0)
			break;
		renew_cam_position(CAMERA_WS, 150.0f, Naxis);
		glutPostRedisplay();
		break;
	case 'z':// World Sub Camera zoom in
		if (Camera_Flag[CAMERA_WS][SwitchOn] == 0)
			break;
		renew_cam_zoom(CAMERA_WS, -1);
		glutPostRedisplay();
		break;
	case 'x':// World Sub Camera zoom out
		if (Camera_Flag[CAMERA_WS][SwitchOn] == 0)
			break;
		renew_cam_zoom(CAMERA_WS, 1);
		glutPostRedisplay();
		break;
	// Bus move control
	case 'r':// bus start moving
		moving_flag = true;
		glutTimerFunc(100, slope_motion, 0);
		break;
	case 't':// bus stop moving
		moving_flag = false;
		break;
	// pot moving control
	case 'c':
		if (Pot_loc.x >= -98.0f)
			Pot_loc.x -= pot_move;
		glutPostRedisplay();
		break;
	case 'v':
		if (Pot_loc.y >= -98.0f)
			Pot_loc.y -= pot_move;
		glutPostRedisplay();
		break;
	case 'b':
		if (Pot_loc.x <= 98.0f)
			Pot_loc.x += pot_move;
		glutPostRedisplay();
		break;
	case 'f': 
		if (Pot_loc.y <= 98.0f)
			Pot_loc.y += pot_move;
		glutPostRedisplay();
		break;
	case 'g':
		if (initial_theta <= 88.0f) {
			initial_theta += pot_move;
			Pot_loc.angle -= (initial_theta * TO_RADIAN);
		}
		glutPostRedisplay();
		break;
	case 'h':
		ver_velocity = initial_velocity = 30.0f;
		direction = normalize(glm::vec3(-Pot_loc.x, -Pot_loc.y, 0.0f));
		glutTimerFunc(100, arc_motion, 0);
		break;
	case 'y':
		ver_velocity = initial_velocity = 40.0f;
		direction = normalize(glm::vec3(-Pot_loc.x, -Pot_loc.y, 0.0f));
		glutTimerFunc(100, arc_motion, 0);
		break;
	// moving human
	case 'j':
		if (Human_loc.y <= 98.0f) {
			dir = 1;
			move = human_move;
			detect_near_object();
		}
		break;
	case 'm':
		if (Human_loc.y >= -98.0f) {
			dir = 1;
			move = -human_move;
			detect_near_object();
		}
		break;
	case 'n':
		if (Human_loc.x >= -98.0f) {
			dir = 0;
			move = -human_move;
			detect_near_object();
			
		}
		break;
	case 'k':
		if (Human_loc.x <= 98.0f) {
			dir = 0;
			move = human_move;
			detect_near_object();
		}
		break;
	// change car rotation direction
	case 'q':
		angle_per_rotate *= -1.0f;
		glutPostRedisplay();
		break;
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	}
}

#define CAM_SPEED 3.0f
void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		camera[CAMERA_WM].alpha += CAM_SPEED;
		if (camera[CAMERA_WM].alpha > 360)
			camera[CAMERA_WM].alpha -= 360;
		move_around_sphere();
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		camera[CAMERA_WM].alpha -= CAM_SPEED;
		if (camera[CAMERA_WM].alpha < 0)
			camera[CAMERA_WM].alpha += 360;
		move_around_sphere();
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		camera[CAMERA_WM].theta += CAM_SPEED;
		if (camera[CAMERA_WM].theta >= 360)
			camera[CAMERA_WM].theta -= 360;
		move_around_sphere();
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		camera[CAMERA_WM].theta -= CAM_SPEED;
		if (camera[CAMERA_WM].theta < 0)
			camera[CAMERA_WM].theta += 360;
		move_around_sphere();
		glutPostRedisplay();
		break;
	}
}

void reshape(int width, int height) {//change viewport -> projectionMatrix -> ViewProjectionMatrix

	viewport[CAMERA_WM].x = viewport[CAMERA_WM].y = 0;
	viewport[CAMERA_WM].w = (int)(width); viewport[CAMERA_WM].h = (int)(0.70f*height);
	camera[CAMERA_WM].aspect_ratio = (float)width / (0.70f * height);
	Camera_Flag[CAMERA_WM][ProjectionChange] = true;

	viewport[CAMERA_WS].x = 0; viewport[CAMERA_WS].y = (int)(0.70f*height);
	viewport[CAMERA_WS].w = (int)(0.30f*width); viewport[CAMERA_WS].h = (int)(0.30*height);
	camera[CAMERA_WS].aspect_ratio = (float)width / height;
	Camera_Flag[CAMERA_WS][ProjectionChange] = true;

	viewport[CAMERA_TIGER].x = (int)(0.35f*width); viewport[CAMERA_TIGER].y = (int)(0.70f*height);
	viewport[CAMERA_TIGER].w = (int)(0.30f*width); viewport[CAMERA_TIGER].h = (int)(0.30*height);
	camera[CAMERA_TIGER].aspect_ratio = (float)width / height;
	Camera_Flag[CAMERA_TIGER][ProjectionChange] = true;

	viewport[CAMERA_DRIVER].x = (int)(0.70f*width); viewport[CAMERA_DRIVER].y = (int)(0.70f*height);
	viewport[CAMERA_DRIVER].w = (int)(0.30f*width); viewport[CAMERA_DRIVER].h = (int)(0.30*height);
	camera[CAMERA_DRIVER].aspect_ratio = (float)width / height;
	Camera_Flag[CAMERA_DRIVER][ProjectionChange] = true;

	glutPostRedisplay();
}

void cleanup(void) {
	int i;
	free_axes();
	//free_path();
	free_square();
	free_slope();
	free_geom_obj(GEOM_OBJ_ID_CAR_BODY);
	free_geom_obj(GEOM_OBJ_ID_CAR_WHEEL);
	free_geom_obj(GEOM_OBJ_ID_CAR_NUT);
	free_geom_obj(GEOM_OBJ_ID_CAR_BODY);
	free_geom_obj(GEOM_OBJ_ID_COW);
	free_geom_obj(GEOM_OBJ_ID_TEAPOT);
	for(i=0;i<N_TIGER_FRAMES;i++)
		free_geom_obj(GEOM_OBJ_ID_TIGER+i);
	for (i = 0; i<N_HUMAN_FRAMES; i++)
		free_geom_obj(GEOM_OBJ_ID_HUMAN + i);
	free_geom_obj(GEOM_OBJ_ID_BUS);
	free_geom_obj(GEOM_OBJ_ID_TANK);
}
/*********************************  END: callbacks *********************************/

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousepress);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutTimerFunc(50, tiger_scene, 0);
	glutTimerFunc(100, car_scene, 0);
	glutTimerFunc(100, slope_motion, 0);
	glutTimerFunc(100, arc_motion, 0);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	int i;

	for (i = 0; i < CAMERA_NUM; i++) {
		ProjectionMatrix[i] = glm::mat4(1.0f);
		ViewProjectionMatrix[i] = glm::mat4(1.0f);
	}

	ModelMatrix_CAR_BODY = glm::mat4(1.0f);
	ModelMatrix_CAR_WHEEL = glm::mat4(1.0f);
	ModelMatrix_CAR_NUT = glm::mat4(1.0f);
	ModelMatrix_CAR_DRIVER = glm::mat4(1.0f);
	ModelMatrix_COW = glm::mat4(1.0f);
	ModelMatrix_TEAPOT = glm::mat4(1.0f);
	ModelMatrix_TIGER = glm::mat4(1.0f);

	Tiger_loc.x = Tiger_loc.y = Tiger_loc.z = Tiger_loc.angle = 0.0f;

	Bus_start.angle = Bus_loc.angle = atan(0.5f);
	Bus_loc.y = Bus_start.y = 100.0f - (20.0f * cos(Bus_start.angle));
	Bus_loc.z =	Bus_start.z = 50.0f - (20.0f * sin(Bus_start.angle));
	Bus_loc.x = Bus_start.x = -90.0f;

	Pot_loc.x = Pot_loc.y = 20.0f;
	Pot_loc.z = 0.0f;
	Pot_loc.angle = 0.0f;
	initial_theta = initial_velocity = 0.0f;

	Cow_loc.x = -30.0f;
	Cow_loc.y = -70.0f;
	Cow_loc.z = 0.0f;

	Tank_loc.x = 55.0f;
	Tank_loc.y = -20.0f;
	Tank_loc.z = 0.0f;

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
}

void prepare_scene(void) {
	int i;
	char filename[512];
	prepare_axes(); 
	//prepare_path();
	prepare_square();
	prepare_slope();
	sprintf(filename, "Data/Triangles/car_body_triangles_v.txt");
	prepare_geom_obj(GEOM_OBJ_ID_CAR_BODY, filename, GEOM_OBJ_TYPE_V);
	sprintf(filename, "Data/Triangles/car_wheel_triangles_v.txt");
	prepare_geom_obj(GEOM_OBJ_ID_CAR_WHEEL, filename, GEOM_OBJ_TYPE_V);
	sprintf(filename, "Data/Triangles/car_nut_triangles_v.txt");
	prepare_geom_obj(GEOM_OBJ_ID_CAR_NUT, filename, GEOM_OBJ_TYPE_V);
	sprintf(filename, "Data/Triangles/cow_triangles_v.txt");
	prepare_geom_obj(GEOM_OBJ_ID_COW, filename, GEOM_OBJ_TYPE_V);
	sprintf(filename, "Data/Triangles/teapot_triangles_v.txt");
	prepare_geom_obj(GEOM_OBJ_ID_TEAPOT, filename, GEOM_OBJ_TYPE_V);
	for (i = 0; i < N_TIGER_FRAMES; i++) {
		sprintf(filename, "Data/Triangles/Tiger_%d%d_triangles_vnt.geom", i / 10, i % 10);
		prepare_geom_obj(GEOM_OBJ_ID_TIGER+i, filename, GEOM_OBJ_TYPE_VNT);
	}
	for (i = 0; i < N_HUMAN_FRAMES; i++) {
		sprintf(filename, "Data/Triangles/ben/ben_vn%d%d.geom", i / 10, i % 10);
		prepare_geom_obj(GEOM_OBJ_ID_HUMAN + i, filename, GEOM_OBJ_TYPE_VNT);
	}
	sprintf(filename, "Data/Triangles/bus_vnt.geom");
	prepare_geom_obj(GEOM_OBJ_ID_BUS, filename, GEOM_OBJ_TYPE_VNT);
	sprintf(filename, "Data/Triangles/tank_vnt.geom");
	prepare_geom_obj(GEOM_OBJ_ID_TANK, filename, GEOM_OBJ_TYPE_VNT);
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
	initialize_camera();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

	error = glewInit();
	if (error != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void print_message(const char * m) {
	fprintf(stdout, "%s\n\n", m);
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    Project of CSE4170 course\n");
	fprintf(stdout, "      in Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 9
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 HW3_20171697";
	char messages[N_MESSAGE_LINES][256] = { "    - Keys used:	Car: 'q'",
											"			Bus: 'r', 't'",
											"			Pot: 'c', 'b', 'f', 'v', 'g', 'h' ,'y'",
											"			Human: 'n', 'k', 'j', 'm'",
											"			Subcamera: 'i'",
											"			Tiger Eye: 'o'",
											"			Driver Eye: 'p'",
											"			Quit: 'ESC'\n",
											"    - Mouse used: L-Click and move (with SHIFT)" };

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}