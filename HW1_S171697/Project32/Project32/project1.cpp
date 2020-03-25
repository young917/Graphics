#include "Project.h"
#include "Shaders/LoadShaders.h"

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0

#define FLOWER 0
#define HOUSE 1
#define SWORD 2
#define CAKE 3

GLfloat axes[6][2];
GLfloat axes_color[3] = { 0.0f, 0.0f, 0.0f };
GLuint VBO_axes, VAO_axes;

void prepare_axes(void) { // Draw axes in their MC.
	axes[0][0] = -win_width / 2.0f; axes[0][1] = win_height / 4.0f;
	axes[1][0] = win_width / 2.0f; axes[1][1] = win_height / 4.0f;
	axes[2][0] = -win_width / 2.0f; axes[2][1] = 0.0f;
	axes[3][0] = win_width / 2.0f; axes[3][1] = 0.0f;
	axes[4][0] = -win_width / 2.0f; axes[4][1] = -win_height / 4.0f;
	axes[5][0] = win_width / 2.0f; axes[5][1] = -win_height / 4.0f;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_axes);
	glBindVertexArray(VAO_axes);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void update_axes(void) {
	axes[0][0] = -win_width / 2.0f; axes[0][1] = win_height / 4.0f;
	axes[1][0] = win_width / 2.0f; axes[1][1] = win_height / 4.0f;
	axes[2][0] = -win_width / 2.0f; axes[2][1] = 0.0f;
	axes[3][0] = win_width / 2.0f; axes[3][1] = 0.0f;
	axes[4][0] = -win_width / 2.0f; axes[4][1] = -win_height / 4.0f;
	axes[5][0] = win_width / 2.0f; axes[5][1] = -win_height / 4.0f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO_axes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes), axes, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void draw_axes(void) {
	glUniform3fv(loc_primitive_color, 1, axes_color);
	glBindVertexArray(VAO_axes);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);
}

void cleanup_axes(void) {
	glDeleteVertexArrays(1, &VAO_axes);
	glDeleteBuffers(1, &VBO_axes);
}


//Flower
#define TOP_LEAF 0
#define MIDDLE 1
#define STEM 2
#define BOT_LEAF 3

GLfloat top_leaf[5][2] = { { 0.0,0.0 },{ 1.0,2.0 },{ 1.0,4.0 },{ -1.0,4.0 },{ -1.0,2.0 } };
GLfloat left_leaf[4][2] = { { 0.0,0.0 },{ -2.0,1.0 },{ -4.0,0.0 },{ -2.0,-1.0 } };
GLfloat down_leaf[3][2] = { { 0.0, 0.0 },{ -1.0,-4.0 },{ 1.0,-4.0 } };
GLfloat right_leaf[6][2] = { { 0.0,0.0 },{ 2.0,1.0 },{ 3.0,1.0 },{ 5.0,0.0 },{ 3.0,-1.0 },{ 2.0,-1.0 } };
GLfloat center_rec[4][2] = { { -0.5,0.5 },{ -0.5,-0.5 },{ 0.5,-0.5 },{ 0.5, 0.5 } };
GLfloat stem[4][2] = { { 0.0,-4.0 },{ 0.5,-4.0 },{ 0.5,-8.0 },{ 0.0,-8.0 } };
GLfloat bot_right_leaf[4][2] = { { 0.5,-8.0 },{ 1.5,-7.5 },{ 4.0,-6.0 },{ 4.5,-5.0 } };
GLfloat bot_left_leaf[4][2] = { { 0.0,-8.0 },{ -1.0,-7.5 },{ -3.5,-6.0 },{ -4.0,-5.0 } };
GLfloat flower_color[4][3] = {
	{ 240 / 255.0f, 230 / 255.0f, 100 / 255.0f }, //top_leaf
{ 200 / 255.0f, 100 / 255.0f, 100 / 255.0f }, //middle
{ 124 / 255.0f, 142 / 255.0f,   50 / 255.0f },  //stem
{ 65 / 255.0f,  83 / 255.0f, 37 / 255.0f },  //bot_leaf	
};
GLuint VBO_flower, VAO_flower;

int flower_clock = 0;
float flower_s_factor = 1.0f;

void prepare_flower() {
	GLsizeiptr buffer_size = sizeof(top_leaf) + sizeof(left_leaf) + sizeof(down_leaf) + sizeof(right_leaf)
		+ sizeof(center_rec) + sizeof(stem) + sizeof(bot_right_leaf) + sizeof(bot_left_leaf);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_flower);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_flower);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(top_leaf), top_leaf);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(top_leaf), sizeof(left_leaf), left_leaf);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(top_leaf) + sizeof(left_leaf), sizeof(down_leaf), down_leaf);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(top_leaf) + sizeof(left_leaf) + sizeof(down_leaf), sizeof(right_leaf), right_leaf);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(top_leaf) + sizeof(left_leaf) + sizeof(down_leaf) + sizeof(right_leaf), sizeof(center_rec), center_rec);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(top_leaf) + sizeof(left_leaf) + sizeof(down_leaf) + sizeof(right_leaf) + sizeof(center_rec),
		sizeof(stem), stem);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(top_leaf) + sizeof(left_leaf) + sizeof(down_leaf) + sizeof(right_leaf) + sizeof(center_rec) + sizeof(stem), sizeof(bot_left_leaf), bot_left_leaf);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(top_leaf) + sizeof(left_leaf) + sizeof(down_leaf) + sizeof(right_leaf) + sizeof(center_rec) + sizeof(stem) + sizeof(bot_left_leaf), sizeof(bot_right_leaf), bot_right_leaf);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_flower);
	glBindVertexArray(VAO_flower);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_flower);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	flower.size_x = 8.0f;
	flower.size_y = 8.0f;
	flower.centerx = -3.0f * (win_width) / 8.0f;
	flower.centery = win_height / 4.0f;
}

void draw_flower() { // Draw airplane in its MC.
	glBindVertexArray(VAO_flower);

	glUniform3fv(loc_primitive_color, 1, flower_color[TOP_LEAF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 5);

	glUniform3fv(loc_primitive_color, 1, flower_color[TOP_LEAF]);
	glDrawArrays(GL_TRIANGLE_FAN, 5, 4);

	glUniform3fv(loc_primitive_color, 1, flower_color[TOP_LEAF]);
	glDrawArrays(GL_TRIANGLE_FAN, 9, 3);

	glUniform3fv(loc_primitive_color, 1, flower_color[TOP_LEAF]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 6);

	glUniform3fv(loc_primitive_color, 1, flower_color[MIDDLE]);
	glDrawArrays(GL_TRIANGLE_FAN, 18, 4);

	glUniform3fv(loc_primitive_color, 1, flower_color[STEM]);
	glDrawArrays(GL_TRIANGLE_FAN, 22, 4);

	glUniform3fv(loc_primitive_color, 1, flower_color[BOT_LEAF]);
	glDrawArrays(GL_TRIANGLE_FAN, 26, 4);

	glUniform3fv(loc_primitive_color, 1, flower_color[BOT_LEAF]);
	glDrawArrays(GL_TRIANGLE_FAN, 30, 4);

	glBindVertexArray(0);
}
void cleanup_flower() {
	glDeleteVertexArrays(1, &VAO_flower);
	glDeleteBuffers(1, &VBO_flower);
}



// airplane
#define AIRPLANE_BIG_WING 0
#define AIRPLANE_SMALL_WING 1
#define AIRPLANE_BODY 2
#define AIRPLANE_BACK 3
#define AIRPLANE_SIDEWINDER1 4
#define AIRPLANE_SIDEWINDER2 5
#define AIRPLANE_CENTER 6
GLfloat big_wing[6][2] = { { 0.0, 0.0 },{ -20.0, 15.0 },{ -20.0, 20.0 },{ 0.0, 23.0 },{ 20.0, 20.0 },{ 20.0, 15.0 } };
GLfloat small_wing[6][2] = { { 0.0, -18.0 },{ -11.0, -12.0 },{ -12.0, -7.0 },{ 0.0, -10.0 },{ 12.0, -7.0 },{ 11.0, -12.0 } };
GLfloat body[5][2] = { { 0.0, -25.0 },{ -6.0, 0.0 },{ -6.0, 22.0 },{ 6.0, 22.0 },{ 6.0, 0.0 } };
GLfloat back[5][2] = { { 0.0, 25.0 },{ -7.0, 24.0 },{ -7.0, 21.0 },{ 7.0, 21.0 },{ 7.0, 24.0 } };
GLfloat sidewinder1[5][2] = { { -20.0, 10.0 },{ -18.0, 3.0 },{ -16.0, 10.0 },{ -18.0, 20.0 },{ -20.0, 20.0 } };
GLfloat sidewinder2[5][2] = { { 20.0, 10.0 },{ 18.0, 3.0 },{ 16.0, 10.0 },{ 18.0, 20.0 },{ 20.0, 20.0 } };
GLfloat center[1][2] = { { 0.0, 0.0 } };
GLfloat airplane_color[7][3] = {
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // big_wing
{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // small_wing
{ 111 / 255.0f,  85 / 255.0f, 157 / 255.0f },  // body
{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // back
{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder1
{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder2
{ 255 / 255.0f,   0 / 255.0f,   0 / 255.0f }   // center
};

GLuint VBO_airplane, VAO_airplane;

int airplane_clock = 0;
float airplane_s_factor = 1.0f;

void prepare_airplane() {
	GLsizeiptr buffer_size = sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1) + sizeof(sidewinder2) + sizeof(center);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(big_wing), big_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing), sizeof(small_wing), small_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing), sizeof(body), body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body), sizeof(back), back);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back),
		sizeof(sidewinder1), sidewinder1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1), sizeof(sidewinder2), sidewinder2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing) + sizeof(small_wing) + sizeof(body) + sizeof(back)
		+ sizeof(sidewinder1) + sizeof(sidewinder2), sizeof(center), center);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_airplane);
	glBindVertexArray(VAO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_airplane() { // Draw airplane in its MC.
	glBindVertexArray(VAO_airplane);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BIG_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SMALL_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BACK]);
	glDrawArrays(GL_TRIANGLE_FAN, 17, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER1]);
	glDrawArrays(GL_TRIANGLE_FAN, 22, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER2]);
	glDrawArrays(GL_TRIANGLE_FAN, 27, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_CENTER]);
	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 32, 1);
	glPointSize(1.0);
	glBindVertexArray(0);
}
void cleanup_airplane() {
	glDeleteVertexArrays(1, &VAO_airplane);
	glDeleteBuffers(1, &VBO_airplane);
}

//house
#define HOUSE_ROOF 0
#define HOUSE_BODY 1
#define HOUSE_CHIMNEY 2
#define HOUSE_DOOR 3
#define HOUSE_WINDOW 4

GLfloat roof[3][2] = { { -12.0, 0.0 },{ 0.0, 12.0 },{ 12.0, 0.0 } };
GLfloat house_body[4][2] = { { -12.0, -14.0 },{ -12.0, 0.0 },{ 12.0, 0.0 },{ 12.0, -14.0 } };
GLfloat chimney[4][2] = { { 6.0, 6.0 },{ 6.0, 14.0 },{ 10.0, 14.0 },{ 10.0, 2.0 } };
GLfloat door[4][2] = { { -8.0, -14.0 },{ -8.0, -8.0 },{ -4.0, -8.0 },{ -4.0, -14.0 } };
GLfloat window[4][2] = { { 4.0, -6.0 },{ 4.0, -2.0 },{ 8.0, -2.0 },{ 8.0, -6.0 } };

GLfloat house_color[5][3] = {
	{ 200 / 255.0f, 39 / 255.0f, 42 / 255.0f },
{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 233 / 255.0f, 113 / 255.0f, 23 / 255.0f },
{ 44 / 255.0f, 180 / 255.0f, 49 / 255.0f }
};

GLuint VBO_house, VAO_house;
void prepare_house() {
	GLsizeiptr buffer_size = sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door)
		+ sizeof(window);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(roof), roof);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof), sizeof(house_body), house_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body), sizeof(chimney), chimney);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney), sizeof(door), door);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof) + sizeof(house_body) + sizeof(chimney) + sizeof(door),
		sizeof(window), window);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_house);
	glBindVertexArray(VAO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	house.size_x = house.size_y = 3.0f;
	house.centerx = -win_width / 2.0f;
}

void draw_house() {
	glBindVertexArray(VAO_house);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_ROOF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 3, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_CHIMNEY]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_DOOR]);
	glDrawArrays(GL_TRIANGLE_FAN, 11, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glBindVertexArray(0);
}
void cleanup_house() {
	glDeleteVertexArrays(1, &VAO_house);
	glDeleteBuffers(1, &VBO_house);
}

//car
#define CAR_BODY 0
#define CAR_FRAME 1
#define CAR_WINDOW 2
#define CAR_LEFT_LIGHT 3
#define CAR_RIGHT_LIGHT 4
#define CAR_LEFT_WHEEL 5
#define CAR_RIGHT_WHEEL 6

GLfloat car_body[4][2] = { { -16.0, -8.0 },{ -16.0, 0.0 },{ 16.0, 0.0 },{ 16.0, -8.0 } };
GLfloat car_frame[4][2] = { { -10.0, 0.0 },{ -10.0, 10.0 },{ 10.0, 10.0 },{ 10.0, 0.0 } };
GLfloat car_window[4][2] = { { -8.0, 0.0 },{ -8.0, 8.0 },{ 8.0, 8.0 },{ 8.0, 0.0 } };
GLfloat car_left_light[4][2] = { { -9.0, -6.0 },{ -10.0, -5.0 },{ -9.0, -4.0 },{ -8.0, -5.0 } };
GLfloat car_right_light[4][2] = { { 9.0, -6.0 },{ 8.0, -5.0 },{ 9.0, -4.0 },{ 10.0, -5.0 } };
GLfloat car_left_wheel[4][2] = { { -10.0, -12.0 },{ -10.0, -8.0 },{ -6.0, -8.0 },{ -6.0, -12.0 } };
GLfloat car_right_wheel[4][2] = { { 6.0, -12.0 },{ 6.0, -8.0 },{ 10.0, -8.0 },{ 10.0, -12.0 } };

GLfloat car_color[7][3] = {
	{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
{ 0 / 255.0f, 149 / 255.0f, 159 / 255.0f },
{ 216 / 255.0f, 208 / 255.0f, 174 / 255.0f },
{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f },
{ 21 / 255.0f, 30 / 255.0f, 26 / 255.0f }
};

GLuint VBO_car, VAO_car;
void prepare_car() {
	GLsizeiptr buffer_size = sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light) + sizeof(car_left_wheel) + sizeof(car_right_wheel);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car_body), car_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body), sizeof(car_frame), car_frame);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame), sizeof(car_window), car_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window), sizeof(car_left_light), car_left_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light),
		sizeof(car_right_light), car_right_light);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light), sizeof(car_left_wheel), car_left_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car_body) + sizeof(car_frame) + sizeof(car_window) + sizeof(car_left_light)
		+ sizeof(car_right_light) + sizeof(car_left_wheel), sizeof(car_right_wheel), car_right_wheel);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car);
	glBindVertexArray(VAO_car);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	car.size_x = 4.5f;
	car.size_y = 4.5f;
	car.x = win_width * (7.0f / 16.0f);
	car.y = win_height / 4.0f;
}

void draw_car() {
	glBindVertexArray(VAO_car);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_FRAME]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_LIGHT]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_LEFT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);

	glUniform3fv(loc_primitive_color, 1, car_color[CAR_RIGHT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 4);

	glBindVertexArray(0);
}
void cleanup_car() {
	glDeleteVertexArrays(1, &VAO_car);
	glDeleteBuffers(1, &VBO_car);
}

// cake
#define CAKE_FIRE 0
#define CAKE_CANDLE 1
#define CAKE_BODY 2
#define CAKE_BOTTOM 3
#define CAKE_DECORATE 4

GLfloat cake_fire[4][2] = { { -0.5, 14.0 },{ -0.5, 13.0 },{ 0.5, 13.0 },{ 0.5, 14.0 } };
GLfloat cake_candle[4][2] = { { -1.0, 8.0 } ,{ -1.0, 13.0 },{ 1.0, 13.0 },{ 1.0, 8.0 } };
GLfloat cake_body[4][2] = { { 8.0, 5.0 },{ -8.0, 5.0 } ,{ -8.0, 8.0 },{ 8.0, 8.0 } };
GLfloat cake_bottom[4][2] = { { -10.0, 1.0 },{ -10.0, 5.0 },{ 10.0, 5.0 },{ 10.0, 1.0 } };
GLfloat cake_decorate[4][2] = { { -10.0, 0.0 },{ -10.0, 1.0 },{ 10.0, 1.0 },{ 10.0, 0.0 } };

GLfloat cake_color[5][3] = {
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 204 / 255.0f, 0 / 255.0f },
{ 255 / 255.0f, 102 / 255.0f, 255 / 255.0f },
{ 255 / 255.0f, 102 / 255.0f, 255 / 255.0f },
{ 102 / 255.0f, 51 / 255.0f, 0 / 255.0f }
};

GLuint VBO_cake, VAO_cake;

void prepare_cake() {
	int size = sizeof(cake_fire);
	GLsizeiptr buffer_size = sizeof(cake_fire) * 5;

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cake);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cake);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, size, cake_fire);
	glBufferSubData(GL_ARRAY_BUFFER, size, size, cake_candle);
	glBufferSubData(GL_ARRAY_BUFFER, size * 2, size, cake_body);
	glBufferSubData(GL_ARRAY_BUFFER, size * 3, size, cake_bottom);
	glBufferSubData(GL_ARRAY_BUFFER, size * 4, size, cake_decorate);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cake);
	glBindVertexArray(VAO_cake);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cake);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	cake.r = 1.0f;
	cake.theta = 90.0f;
	cake.size_x = 3.0f;
	cake.size_y = 3.0f;
	cake.centery = win_height / 4.0f;
}

void draw_cake() {
	glBindVertexArray(VAO_cake);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_FIRE]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_CANDLE]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_BOTTOM]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cake_color[CAKE_DECORATE]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glBindVertexArray(0);
}
void cleanup_cake() {
	glDeleteVertexArrays(1, &VAO_cake);
	glDeleteBuffers(1, &VBO_cake);
}

// sword
#define SWORD_BODY 0
#define SWORD_BODY2 1
#define SWORD_HEAD 2
#define SWORD_HEAD2 3
#define SWORD_IN 4
#define SWORD_DOWN 5
#define SWORD_BODY_IN 6

GLfloat sword_body[4][2] = { { -6.0, 0.0 },{ -6.0, -4.0 },{ 6.0, -4.0 },{ 6.0, 0.0 } };
GLfloat sword_body2[4][2] = { { -2.0, -4.0 },{ -2.0, -6.0 } ,{ 2.0, -6.0 },{ 2.0, -4.0 } };
GLfloat sword_head[4][2] = { { -2.0, 0.0 },{ -2.0, 16.0 } ,{ 2.0, 16.0 },{ 2.0, 0.0 } };
GLfloat sword_head2[3][2] = { { -2.0, 16.0 },{ 0.0, 19.46 } ,{ 2.0, 16.0 } };
GLfloat sword_in[4][2] = { { -0.3, 0.7 },{ -0.3, 15.3 } ,{ 0.3, 15.3 },{ 0.3, 0.7 } };
GLfloat sword_down[4][2] = { { -2.0, -6.0 } ,{ 2.0, -6.0 },{ 4.0, -8.0 },{ -4.0, -8.0 } };
GLfloat sword_body_in[4][2] = { { 0.0, -1.0 } ,{ 1.0, -2.732 },{ 0.0, -4.464 },{ -1.0, -2.732 } };

GLfloat sword_color[7][3] = {
	{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 155 / 255.0f, 155 / 255.0f, 155 / 255.0f },
{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
{ 139 / 255.0f, 69 / 255.0f, 19 / 255.0f },
{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f }
};

GLuint VBO_sword, VAO_sword;

void prepare_sword() {
	GLsizeiptr buffer_size = sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down) + sizeof(sword_body_in);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sword_body), sword_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body), sizeof(sword_body2), sword_body2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2), sizeof(sword_head), sword_head);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head), sizeof(sword_head2), sword_head2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2), sizeof(sword_in), sword_in);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in), sizeof(sword_down), sword_down);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sword_body) + sizeof(sword_body2) + sizeof(sword_head) + sizeof(sword_head2) + sizeof(sword_in) + sizeof(sword_down), sizeof(sword_body_in), sword_body_in);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_sword);
	glBindVertexArray(VAO_sword);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sword);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	sword.size_x = 3.0f;
	sword.size_y = 3.0f;
	sword.centerx = -win_width / 2.0f;
	sword.centery = -win_height / 4.0f;

}

void draw_sword() {
	glBindVertexArray(VAO_sword);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY2]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_HEAD2]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 3);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_DOWN]);
	glDrawArrays(GL_TRIANGLE_FAN, 19, 4);

	glUniform3fv(loc_primitive_color, 1, sword_color[SWORD_BODY_IN]);
	glDrawArrays(GL_TRIANGLE_FAN, 23, 4);

	glBindVertexArray(0);
}
void cleanup_sword() {
	glDeleteVertexArrays(1, &VAO_sword);
	glDeleteBuffers(1, &VBO_sword);
}




void display(void) {
	glm::mat4 ModelMatrix;
	glm::mat4 tmpMatrix;

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(r, g, b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	//axes
	ModelMatrix = glm::mat4(1.0f);
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_axes();

	//flower
	tmpMatrix = glm::rotate(glm::mat4(1.0f), flower.rotate*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	tmpMatrix = glm::scale(tmpMatrix, glm::vec3(flower.size_x, flower.size_y, 1.0f));
	float static_x = (cos(flower.rotate*TO_RADIAN)*(flower.centerx + flower.x) + sin(flower.rotate*TO_RADIAN)*(flower.centery + flower.y)) / flower.size_x;
	float static_y = -(sin(flower.rotate*TO_RADIAN)*(flower.centerx + flower.x) - cos(flower.rotate*TO_RADIAN)*(flower.centery + flower.y)) / flower.size_x;
	for (float move = -win_width; move < win_width; move += (win_width / 3.0f)) {
		float new_x = (flower.centerx + flower.x + move);
		if (new_x<(-win_width / 2.0f) || new_x>(win_width / 2.0f))
			continue;
		new_x = static_x + cos(flower.rotate*TO_RADIAN)*move / flower.size_x;
		float new_y = static_y - sin(flower.rotate*TO_RADIAN)*move / flower.size_y;
		ModelMatrix = glm::translate(tmpMatrix, glm::vec3(new_x, new_y, 0.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_flower();
	}
	
	//cake
	tmpMatrix = glm::rotate(glm::mat4(1.0f), cake.rotate*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	tmpMatrix = glm::scale(tmpMatrix, glm::vec3(cake.size_x, cake.size_y, 1.0f));
	static_x = (cos(cake.rotate*TO_RADIAN)*(cake.centerx + cake.x) + sin(cake.rotate*TO_RADIAN)*(cake.centery + cake.y - win_height/8.0f )) / cake.size_x;
	static_y = (-sin(cake.rotate*TO_RADIAN)*(cake.centerx + cake.x) + cos(cake.rotate*TO_RADIAN)*(cake.centery + cake.y - win_height/8.0f)) / cake.size_x;
	for (float k = 1.0f ; k<= 2.0f;k+=0.5f){
		float new_x = k * static_x;
		float new_y = k * static_y;
		ModelMatrix = glm::translate(tmpMatrix, glm::vec3(new_x, new_y, 0.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_cake();
	}

	//car
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(car.x, car.y, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(car.size_x, car.size_y, 1.0f));
	ModelMatrix = glm::rotate(ModelMatrix, car.rotate*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_car();

	//house
	tmpMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(house.size_x, house.size_y, 1.0f));
	for (float move = -win_width; move < win_width; move += (win_width / 3.0f)) {
		float new_x = (house.centerx + house.x + move);
		float new_y = (house.centery + house.y) / house.size_y;
		if (new_x<(-win_width / 2.0f) || new_x>(win_width / 2.0f))
			continue;
		new_x /= house.size_x;
		ModelMatrix = glm::translate(tmpMatrix, glm::vec3(new_x, new_y, 0.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_house();
	}

	// sword
	tmpMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(sword.size_x, sword.size_y, 1.0f));
	for (float move = -win_width; move < win_width; move += (win_width / 3.0f)) {
		float new_x = (sword.centerx + sword.x + move);
		float new_y = (sword.centery + sword.y) / sword.size_y;
		if (new_x<(-win_width / 2.0f) || new_x>(win_width / 2.0f))
			continue;
		new_x /= sword.size_x;
		ModelMatrix = glm::translate(tmpMatrix, glm::vec3(new_x,new_y, 0.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_sword();
	}

	//airplane
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(airplane.x, airplane.y, 0.0f));
	ModelMatrix = glm::rotate(ModelMatrix, airplane.rotate*TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(airplane.size_x, airplane.size_y, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_airplane();

	glFlush();
}

void crush_car() {
	float distance;
	float static_dist;
	//flower
	static_dist = (flower.y + flower.centery - car.y) * (flower.y + flower.centery - car.y);
	for (float move = -win_width; move < win_width; move += (win_width / 3.0f)) {
		float new_x = (flower.centerx + flower.x + move);
		if (new_x<(-win_width / 2.0f) || new_x>(win_width / 2.0f))
			continue;
		distance = static_dist + (new_x - car.x) * (new_x - car.x);
		distance = sqrt(distance);

		if (distance < 15.0f*car.size_x) {
			g *= 0.8f;
			b *= 0.8f;
			car.size_x -= 0.5f;
			car.size_y -= 0.5f;
			car.rotate += 10.0f;
		}
	}

	//house
	static_dist = (house.y + house.centery - car.y) * (house.y + house.centery - car.y);
	for (float move = -win_width; move < win_width; move += (win_width / 3.0f)) {
		float new_x = (house.centerx + house.x + move);
		if (new_x<(-win_width / 2.0f) || new_x>(win_width / 2.0f))
			continue;
		distance = static_dist + (new_x - car.x) * (new_x - car.x);
		distance = sqrt(distance);

		if (distance < 15.0f*car.size_x) {
			r *= 0.8f;
			b *= 0.8f;
			car.size_x -= 0.5f;
			car.size_y -= 0.5f;
			car.rotate += 10.0f;
		}
	}

	//sword
	static_dist = (sword.y + sword.centery - car.y) * (sword.y + sword.centery - car.y);
	for (float move = -win_width; move < win_width; move += (win_width / 3.0f)) {
		float new_x = (sword.centerx + sword.x + move);
		if (new_x<(-win_width / 2.0f) || new_x>(win_width / 2.0f))
			continue;
		distance  = static_dist + (new_x - car.x) * (new_x - car.x);
		distance = sqrt(distance);

		if (distance < 15.0f*car.size_x) {
			r *= 0.8f;
			g *= 0.8f;
			car.size_x -= 0.5f;
			car.size_y -= 0.5f;
			car.rotate += 10.0f;
		}
	}

	if (r <= 0.001f || g <= 0.001f || b <= 0.001f) {
		r = g = b = 1.0f;
	}
	if (car.size_x < 2.5f) {
		car.size_x = car.size_y = 4.5f;
	}
}

void flower_motion(int value) {
	float inc_x = 5.0f;
	float dtheta = 2.0f;
	float new_x, new_y;
	float dx, dy;

	flower.centerx += inc_x;
	if (flower.centerx > (win_width / 2.0f)) {
		flower.centerx = -(win_width / 2.0f);
	}
	flower.theta = (flower.theta + dtheta);
	if (flower.theta == 360.0f) {
		flower.theta = 0.0f;
	}
	new_x = 100.0f * cos(2 * flower.theta*TO_RADIAN)*cos(flower.theta*TO_RADIAN);
	new_y = 100.0f * cos(2 * flower.theta*TO_RADIAN)*sin(flower.theta*TO_RADIAN);
	dx = new_x - flower.x;
	dy = new_y - flower.y;
	if (dx > 0) {
		flower.rotate = 90.0f + atan(dy / dx)*(1 / TO_RADIAN);
	}
	else if (dx < 0) {
		flower.rotate = -90.0f + atan(dy / dx)*(1 / TO_RADIAN);
	}
	else if (dy > 0) {
		flower.rotate = 180.0f;
	}
	else
		flower.rotate = 0.0f;
	flower.x = new_x;
	flower.y = new_y;
	glutPostRedisplay();
	glutTimerFunc(50, flower_motion, FLOWER);
}
void house_motion(int value) {
	static float chg = 1;
	float d = win_height / 320.0f;

	house.y += chg * d;
	house.x += d;
	if (house.x >= win_width)
		house.x = 0;
	if (house.y >= (win_height / 8.0f))
		chg = -1;
	else if (house.y <= -(win_height / 8.0f))
		chg = 1;
	else if (house.y == 0.0) {
		house.size_y = -1.0f * house.size_y;
		house.centery = -house.centery;
	}
	crush_car();
	glutPostRedisplay();
	if (chg > 0)// slow
		glutTimerFunc(10, house_motion, HOUSE);
	else// fast
		glutTimerFunc(30, house_motion, HOUSE);
}
void sword_motion(int value) {
	static float chg = 1.0f;
	float dx = win_width / 320.f;
	float dtheta = 5.0f;

	sword.theta += 5.0f;
	sword.x += dx;
	if (sword.x > win_width)
		sword.x = 0;
	sword.size_y = chg * (3.0f + 2 * sin(sword.theta *TO_RADIAN));
	if (sword.theta == 180.f) {
		chg *= -1;
		sword.theta = 0.0f;
	}
	crush_car();
	glutPostRedisplay();
	glutTimerFunc(50, sword_motion, SWORD);
}
void cake_motion(int value) {
	static int increase = 0;
	float dpath = win_height / 8.0f;
	float dtheta = 3.0f;
	float new_x, new_y;
	float dx, dy;

	cake.theta += dtheta;
	new_x = (dpath)*(cake.r - sin(cake.theta*TO_RADIAN))*cos(cake.theta*TO_RADIAN);
	new_y = (dpath)*(cake.r - sin(cake.theta*TO_RADIAN))*sin(cake.theta*TO_RADIAN);
	dx = new_x - cake.x;
	dy = new_y - cake.y;
	if (dx > 0.0)
		cake.rotate = 90.0f + atan(dy / dx)*(1 / TO_RADIAN);
	else if (dx < 0)
		cake.rotate = -90.0f + atan(dy / dx)*(1 / TO_RADIAN);
	else if (dy > 0.0)
		cake.rotate = 180.0f;
	else
		cake.rotate = 0.0f;
	cake.x = new_x;
	cake.y = new_y;

	if (cake.theta == 450.0f) {
		cake.theta = 90.0f;
	}
	glutPostRedisplay();
	glutTimerFunc(40, cake_motion, CAKE);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r': {
		r = g = b = 1.0f;
		glutPostRedisplay();
		break;
	}
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	}
}

void special(int key, int x, int y) {
	float SENSITIVITY = 5.0;
	bool display = false;

	switch (key) {
	case GLUT_KEY_LEFT:
		car.x -= SENSITIVITY;
		display = true;
		break;
	case GLUT_KEY_RIGHT:
		car.x += SENSITIVITY;
		display = true;
		break;
	case GLUT_KEY_DOWN:
		if (car.y > (-win_height / 4.0f)) {
			car.y -= win_height / 4.0f;
			display = true;
		}
		break;
	case GLUT_KEY_UP:
		if (car.y < (win_height / 4.0f)) {
			car.y += (win_height / 4.0f);
			display = true;
		}
		break;
	}
	if (display) {
		crush_car();
		glutPostRedisplay();
	}
}

void mouse(int button, int state, int x, int y) {
	float sensitive = 2.0f;
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		float dx = airplane.x - (x - win_width / 2.0f);
		float dy = airplane.y -( win_height / 2.0f - y );
		if (-sensitive <= dx && dx <= sensitive && -sensitive <= dy && dy <= sensitive) {
			//비행기를 누르면 비행기의 크기가 커진다. 3배를 넘어가면 다시 1배로
			float dsize = 1.2f;
			if (airplane.size_x >= 3.0f) {
				airplane.size_x = 1.0f;
				airplane.size_y = 1.0f;
			}
			else {
				airplane.size_x *= dsize;
				airplane.size_y *= dsize;
			}
			glutPostRedisplay();
		}
		leftbuttonpressed = 1;
	}
	else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP))
		leftbuttonpressed = 0;
}

void mouse_motion(int x, int y) {
	static int delay = 0;
	float dr = 30.0f;
	if (leftbuttonpressed) {
		if (delay == 3) {
			airplane.x = x - win_width / 2.0f;
			airplane.y = win_height / 2.0f - y;
			airplane.rotate = airplane.rotate + dr;
			if (airplane.rotate > 360.0f)
				airplane.rotate -= 360.0f;
			delay = 0;
		}
		glutPostRedisplay();
		delay++;
	}
}

void reshape(int width, int height) {
	win_width = (float)width, win_height = (float)height;

	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0,
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	//adjust model center
	update_axes();
	flower.centery = win_height / 4.0f;
	house.centerx = -win_width / 2.0f;
	sword.centerx = -win_width / 2.0f;
	sword.centery = -win_height / 4.0f;
	glutPostRedisplay();
}

void cleanup(void) {

	cleanup_flower();
	cleanup_axes();
	cleanup_flower();
	cleanup_airplane();
	cleanup_house();
	cleanup_car();
	cleanup_cake();
	cleanup_sword();
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(mouse_motion);
	glutReshapeFunc(reshape);
	glutCloseFunc(cleanup);
	glutTimerFunc(0, flower_motion, FLOWER);
	glutTimerFunc(0, house_motion, HOUSE);
	glutTimerFunc(0, sword_motion, SWORD);
	glutTimerFunc(0, cake_motion, CAKE);
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
	glEnable(GL_MULTISAMPLE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glClearColor(44 / 255.0f, 180 / 255.0f, 49 / 255.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	prepare_axes();
	prepare_flower();
	prepare_airplane();
	prepare_house();
	prepare_car();
	prepare_cake();
	prepare_sword();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program();
	initialize_OpenGL();
	prepare_scene();
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

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded by 20171697 \n");
	fprintf(stdout, "      of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 2
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 Project 1";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used\n\n      'ESC' : quit\n      'direction key' : move car\n      'r' : make background white\n",
		"    - Mouse used: move airplane"
	};

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(1200 * 0.95, 800 * 0.95);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);
	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop();
}