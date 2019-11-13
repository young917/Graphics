/*********************************  START: camera *********************************/
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, lookAt, perspective, etc.

typedef struct _Camera {
	glm::vec3 pos;
	glm::vec3 prp, vrp, vup;
	glm::vec3 uaxis, vaxis, naxis;
	float theta, alpha;
	float fovy, aspect_ratio, near_c, far_c, zoom_factor;

} Camera;

typedef struct {
	int x, y, w, h;
} VIEWPORT;

enum _CameraType { CAMERA_WM, CAMERA_WS, CAMERA_DRIVER, CAMERA_TIGER } camera_type;
enum _CameraFlag{ SwitchOn, ViewChange, ProjectionChange } CameraFlag;
enum _CamDir { Uaxis, Vaxis, Naxis } CamDir;
#define CAMERA_NUM 4

glm::mat4 ViewProjectionMatrix[CAMERA_NUM], ViewMatrix[CAMERA_NUM], ProjectionMatrix[CAMERA_NUM];
glm::mat4 ModelMatrix_To_DRIVER; // computed only once in initialize_camera()
glm::mat4 ModelMatrix_TO_EYE;
Camera camera[CAMERA_NUM];
VIEWPORT viewport[CAMERA_NUM];
int Camera_Flag[CAMERA_NUM][3];

//temp
void print_mat4(const char *string, glm::mat4 M) {
	fprintf(stdout, "\n***** %s ******\n", string);
	for (int i = 0; i < 4; i++)
		fprintf(stdout, "*** COL[%d] (%f, %f, %f, %f)\n", i, M[i].x, M[i].y, M[i].z, M[i].w);
	fprintf(stdout, "**************\n\n");
}

#define radius 200.0f
void initialize_camera(void) {
	int i;

	camera[CAMERA_WM].prp = glm::vec3( radius, 0.0f, 0.0f);
	camera[CAMERA_WM].vrp = glm::vec3(0.0f, 0.0f, 0.0f);
	camera[CAMERA_WM].vup = glm::vec3(0.0f, 0.0f, 1.0f);
	camera[CAMERA_WM].theta = camera[CAMERA_WM].alpha = 0.0f;
	ViewMatrix[CAMERA_WM] = glm::lookAt(camera[CAMERA_WM].prp, camera[CAMERA_WM].vrp, camera[CAMERA_WM].vup);
	camera[CAMERA_WM].vup = glm::vec3(ViewMatrix[CAMERA_WM][0].y, ViewMatrix[CAMERA_WM][1].y, ViewMatrix[CAMERA_WM][2].y); // in this example code, make vup always equal to the v direction.
	camera[CAMERA_WM].fovy = 45.0f;
	camera[CAMERA_WM].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[CAMERA_WM].near_c = 5.0f;
	camera[CAMERA_WM].far_c = 10000.0f;
	camera[CAMERA_WM].zoom_factor = 1.0f;

	camera[CAMERA_WS].prp = glm::vec3(0.0f, 100.0f, 0.0f);
	camera[CAMERA_WS].vrp = glm::vec3(0.0f, 0.0f, 0.0f);
	camera[CAMERA_WS].vup = glm::vec3(0.0f, 0.0f, 1.0f);
	ViewMatrix[CAMERA_WS] = glm::lookAt(camera[CAMERA_WS].prp, camera[CAMERA_WS].vrp, camera[CAMERA_WS].vup);
	camera[CAMERA_WS].uaxis = glm::vec3(ViewMatrix[CAMERA_WS][0].x, ViewMatrix[CAMERA_WS][1].x, ViewMatrix[CAMERA_WS][2].x);
	camera[CAMERA_WS].vaxis = glm::vec3(ViewMatrix[CAMERA_WS][0].y, ViewMatrix[CAMERA_WS][1].y, ViewMatrix[CAMERA_WS][2].y);
	camera[CAMERA_WS].naxis = glm::vec3(ViewMatrix[CAMERA_WS][0].z, ViewMatrix[CAMERA_WS][1].z, ViewMatrix[CAMERA_WS][2].z);
	camera[CAMERA_WS].fovy = 30.0f;
	camera[CAMERA_WS].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[CAMERA_WS].near_c = 5.0f;
	camera[CAMERA_WS].far_c = 10000.0f;
	camera[CAMERA_WS].zoom_factor = 1.0f;

	ModelMatrix_To_DRIVER = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 2.5f, 0.5f));
	ModelMatrix_To_DRIVER = glm::rotate(ModelMatrix_To_DRIVER, TO_RADIAN*(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	ViewMatrix[CAMERA_DRIVER] = glm::mat4(1.0f);
	camera[CAMERA_DRIVER].fovy = 30.0f;
	camera[CAMERA_DRIVER].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[CAMERA_DRIVER].near_c = 5.0f;
	camera[CAMERA_DRIVER].far_c = 10000.0f;
	camera[CAMERA_DRIVER].zoom_factor = 1.0f;

	ModelMatrix_TO_EYE = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -88.0f, 62.0f));
	ModelMatrix_TO_EYE = glm::rotate(ModelMatrix_TO_EYE, TO_RADIAN*180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix_TO_EYE = glm::rotate(ModelMatrix_TO_EYE, TO_RADIAN*90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	ViewMatrix[CAMERA_TIGER] = glm::mat4(1.0f);
	camera[CAMERA_TIGER].fovy = 30.0f;
	camera[CAMERA_TIGER].aspect_ratio = 1.0f; // will be set when the viewing window popped up.
	camera[CAMERA_TIGER].near_c = 1.0f;
	camera[CAMERA_TIGER].far_c = 200.0f;
	camera[CAMERA_TIGER].zoom_factor = 1.0f;

	for (i = 0; i < 4; i++) {
		Camera_Flag[i][SwitchOn] = false;
		Camera_Flag[i][ViewChange] = true;
		Camera_Flag[i][ProjectionChange] = true;
	}
	Camera_Flag[CAMERA_WM][SwitchOn] = true;
}

#define CAM_MOVE_SPEED 0.002f
void renew_cam_position(int CAMERA_TYPE, float del, int direction) {
	switch (direction) {
	case Uaxis:
		camera[CAMERA_TYPE].prp += CAM_MOVE_SPEED * del *camera[CAMERA_TYPE].uaxis;
		camera[CAMERA_TYPE].vrp += CAM_MOVE_SPEED * del *camera[CAMERA_TYPE].uaxis;
		break;
	case Vaxis:
		camera[CAMERA_TYPE].prp += CAM_MOVE_SPEED * del * camera[CAMERA_TYPE].vaxis;
		camera[CAMERA_TYPE].vrp += CAM_MOVE_SPEED * del * camera[CAMERA_TYPE].vaxis;
		break;
	case Naxis:
		camera[CAMERA_TYPE].prp += CAM_MOVE_SPEED * del * camera[CAMERA_TYPE].naxis;
		break;
	}
	Camera_Flag[CAMERA_TYPE][ViewChange] = true;
}

void move_around_sphere() {
	float rad_theta = TO_RADIAN * camera[CAMERA_WM].theta;
	float rad_alpha = TO_RADIAN * camera[CAMERA_WM].alpha;
	float vup_size,tmp;
	glm::vec3 vup;

	camera[CAMERA_WM].prp = glm::vec3(radius * cos(rad_alpha) * cos(rad_theta),
		radius * cos(rad_alpha) * sin(rad_theta), radius * sin(rad_alpha) );

	// set vup
	vup = glm::vec3(0.0f, 0.0f, radius*sin(rad_alpha)) - camera[CAMERA_WM].prp;
	if (camera[CAMERA_WM].alpha == 0.0f)
		vup = glm::vec3(0.0f, 0.0f, 1.0f);
	else if (camera[CAMERA_WM].alpha == 90.0f)
		vup = glm::vec3(-cos(rad_theta), -sin(rad_theta), 0.0f);
	else if (camera[CAMERA_WM].alpha > 90.0f && camera[CAMERA_WM].alpha < 180.0f)
		vup = -vup;
	else if (camera[CAMERA_WM].alpha == 180.0f)
		vup = glm::vec3(0.0f, 0.0f, -1.0f);
	else if (camera[CAMERA_WM].alpha == 270.0f)
		vup = glm::vec3(cos(rad_theta), sin(rad_theta) , 0.0f);
	else if (camera[CAMERA_WM].alpha > 270.0f && camera[CAMERA_WM].alpha < 360.0f )
		vup = -vup;
	vup_size = sqrt(vup[0] * vup[0] + vup[1] * vup[1] + vup[2] * vup[2]);
	camera[CAMERA_WM].vup = glm::vec3(vup[0]/vup_size,vup[1]/vup_size,vup[2]/vup_size);
	Camera_Flag[CAMERA_WM][ViewChange] = true;
}

#define CAM_RSPEED 8.0f
void renew_cam_orientation_rotation_around_v_axis(int CAMERA_TYPE,int angle) {
	glm::mat4 RotationMatrix;
	glm::vec4 tmp;
	//printf("before change vrp:	%f	%f	%f\n", camera[CAMERA_TYPE].vrp[0], camera[CAMERA_TYPE].vrp[1], camera[CAMERA_TYPE].vrp[2]);
	//printf("before change prp:	%f	%f	%f\n", camera[CAMERA_TYPE].prp[0], camera[CAMERA_TYPE].prp[1], camera[CAMERA_TYPE].prp[2]);
	RotationMatrix = glm::translate(glm::mat4(1.0), camera[CAMERA_TYPE].prp);
	RotationMatrix = glm::translate(glm::mat4(1.0), camera[CAMERA_TYPE].prp);
	RotationMatrix = glm::rotate(RotationMatrix, CAM_RSPEED*TO_RADIAN*angle, glm::vec3(0.0f,0.0f,1.0f));
	RotationMatrix = glm::translate(RotationMatrix, -camera[CAMERA_TYPE].prp);
	tmp = RotationMatrix * glm::vec4(camera[CAMERA_TYPE].vrp[0], camera[CAMERA_TYPE].vrp[1], camera[CAMERA_TYPE].vrp[2], 1.0f);
	camera[CAMERA_TYPE].vrp[0] = tmp[0];
	camera[CAMERA_TYPE].vrp[1] = tmp[1];
	camera[CAMERA_TYPE].vrp[2] = tmp[2];
	//printf("after change vrp:	%f	%f	%f\n", camera[CAMERA_TYPE].vrp[0], camera[CAMERA_TYPE].vrp[1], camera[CAMERA_TYPE].vrp[2]);
	//printf("after change prp:	%f	%f	%f\n", camera[CAMERA_TYPE].prp[0], camera[CAMERA_TYPE].prp[1], camera[CAMERA_TYPE].prp[2]);
	Camera_Flag[CAMERA_TYPE][ViewChange] = true;
}

#define CAM_ZOOM_STEP 0.002f
#define CAM_MAX_ZOOM_IN_FACTOR	0.25f
#define CAM_MAX_ZOOM_OUT_FACTOR	2.50f
void renew_cam_zoom(int CAMERA_TYPE, int del) {
	camera[CAMERA_TYPE].zoom_factor += (float)(del)* CAM_ZOOM_STEP;
	if (camera[CAMERA_TYPE].zoom_factor < CAM_MAX_ZOOM_IN_FACTOR)
		camera[CAMERA_TYPE].zoom_factor = CAM_MAX_ZOOM_IN_FACTOR;
	if (camera[CAMERA_TYPE].zoom_factor > CAM_MAX_ZOOM_OUT_FACTOR)
		camera[CAMERA_TYPE].zoom_factor = CAM_MAX_ZOOM_OUT_FACTOR;
	Camera_Flag[CAMERA_TYPE][ProjectionChange] = true;
}
/*********************************  END: camera *********************************/