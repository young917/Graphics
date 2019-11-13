/*********************************  START: geometry *********************************/
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0
#define LOC_NORMAL 1
#define LOC_TEXCOORD 2

GLint  loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

GLuint axes_VBO, axes_VAO;
GLfloat axes_vertices[6][3] = {
	{ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }
};
GLfloat axes_color[3][3] = { { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };

void prepare_axes(void) { // Draw coordinate axes.
	// Initialize vertex buffer object.
	glGenBuffers(1, &axes_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axes_vertices), &axes_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &axes_VAO);
	glBindVertexArray(axes_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, axes_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void draw_axes(void) {
	glBindVertexArray(axes_VAO);
	glUniform3fv(loc_primitive_color, 1, axes_color[0]);
	glDrawArrays(GL_LINES, 0, 3);
	glUniform3fv(loc_primitive_color, 1, axes_color[1]);
	glDrawArrays(GL_LINES, 2, 2);
	glUniform3fv(loc_primitive_color, 1, axes_color[2]);
	glDrawArrays(GL_LINES, 4, 2);
	glBindVertexArray(0);
}
void free_axes(void) {
	glDeleteVertexArrays(1, &axes_VAO);
	glDeleteBuffers(1, &axes_VBO);
}

GLuint slope_VBO, slope_VAO;
GLfloat slope_vertices[18][3] = {
{0.1f, -0.5f, 0.0f}, {0.1f, 0.5f, 0.0f}, {0.1f, 0.5f, 0.5f},
{0.1f, 0.5f, 0.5f}, {0.1f, 0.5f, 0.0f}, {-0.1f,0.5f,0.0f},
{-0.1f, 0.5f,0.0f},{-0.1f, 0.5f, 0.5f},{0.1f, 0.5f, 0.5f},
{-0.1f,0.5f, 0.5f},{-0.1f, 0.5f, 0.0f},{-0.1f, -0.5f, 0.0f},
{0.1f,-0.5f, 0.0f}, {0.1f, 0.5f, 0.5f}, {-0.1f,0.5f, 0.5f},
{-0.1f, 0.5f, 0.5f},{-0.1f, -0.5f, 0.0f},{0.1f,-0.5f, 0.0f}
};
GLfloat slope_color[4][3] = { { 0.78f, 0.34f, 0.01f },{ 0.60f, 0.26f, 0.00f },{ 0.78f, 0.34f, 0.01f },{ 1.00f, 0.62f, 0.33f } };

void prepare_slope(void) { // Draw coordinate axes.
						   // Initialize vertex buffer object.
	glGenBuffers(1, &slope_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, slope_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(slope_vertices), &slope_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &slope_VAO);
	glBindVertexArray(slope_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, slope_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void draw_slope(void) {
	glBindVertexArray(slope_VAO);
	glUniform3fv(loc_primitive_color, 1, slope_color[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUniform3fv(loc_primitive_color, 1, slope_color[1]);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glUniform3fv(loc_primitive_color, 1, slope_color[1]);
	glDrawArrays(GL_TRIANGLES, 6, 3);
	glUniform3fv(loc_primitive_color, 1, slope_color[2]);
	glDrawArrays(GL_TRIANGLES, 9, 3);
	glUniform3fv(loc_primitive_color, 1, slope_color[3]);
	glDrawArrays(GL_TRIANGLES, 12, 3);
	glUniform3fv(loc_primitive_color, 1, slope_color[3]);
	glDrawArrays(GL_TRIANGLES, 15, 3);
	glBindVertexArray(0);
}
void free_slope(void) {
	glDeleteVertexArrays(1, &slope_VAO);
	glDeleteBuffers(1, &slope_VBO);
}

GLuint square_VBO, square_VAO;
GLfloat square_vertices[6][3] = {
	{1.0f, 1.0f, 0.0f}, {-1.0f, 1.0f,0.0f}, {1.0f, -1.0f,0.0f},
	{ -1.0f, -1.0f,0.0f },{ -1.0f, 1.0f, 0.0f },{ 1.0f, -1.0f,0.0f }
};
GLfloat square_color[1][3] = { {1.0f, 0.89f, 0.43f} };

void prepare_square() {
	glGenBuffers(1, &square_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, square_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), &square_vertices[0][0], GL_STATIC_DRAW);

	// Initialize vertex array object.
	glGenVertexArrays(1, &square_VAO);
	glBindVertexArray(square_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, square_VBO);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void draw_square(void) {
	glBindVertexArray(square_VAO);
	glUniform3fv(loc_primitive_color, 1, square_color[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUniform3fv(loc_primitive_color, 1, square_color[0]);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glBindVertexArray(0);
}
void free_square(void) {
	glDeleteVertexArrays(1, &square_VAO);
	glDeleteBuffers(1, &square_VBO);
}
/*
GLuint path_VBO, path_VAO;
GLfloat *path_vertices;
int path_n_vertices;

int read_path_file(GLfloat **object, char *filename) {
	int i, n_vertices;
	float *flt_ptr;
	FILE *fp;

	fprintf(stdout, "Reading path from the path file %s...\n", filename);
	fp = fopen(filename, "r");
	if (fp == NULL){
		fprintf(stderr, "Cannot open the path file %s ...", filename);
		return -1;
	}

	fscanf(fp, "%d", &n_vertices);
	*object = (float *)malloc(n_vertices * 3 * sizeof(float));
	if (*object == NULL){
		fprintf(stderr, "Cannot allocate memory for the path file %s ...", filename);
		return -1;
	}

	flt_ptr = *object;
	for (i = 0; i < n_vertices; i++) {
		fscanf(fp, "%f %f %f", flt_ptr, flt_ptr + 1, flt_ptr + 2);
		flt_ptr += 3;
	}
	fclose(fp);

	fprintf(stdout, "Read %d vertices successfully.\n\n", n_vertices);

	return n_vertices;
}

void prepare_path(void) { // Draw path.
	//	return;
	path_n_vertices = read_path_file(&path_vertices, "Data/path.txt");
	printf("%d %f\n", path_n_vertices, path_vertices[(path_n_vertices - 1)]);
	// Initialize vertex array object.
	glGenVertexArrays(1, &path_VAO);
	glBindVertexArray(path_VAO);
	glGenBuffers(1, &path_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, path_VBO);
	glBufferData(GL_ARRAY_BUFFER, path_n_vertices * 3 * sizeof(float), path_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_path(void) {
	glBindVertexArray(path_VAO);
	glUniform3f(loc_primitive_color, 1.000f, 1.000f, 0.000f); // color name: Yellow
	glDrawArrays(GL_LINE_STRIP, 0, path_n_vertices);
}

void free_path(void) {
	glDeleteVertexArrays(1, &path_VAO);
	glDeleteBuffers(1, &path_VBO);
}
*/

#define N_GEOMETRY_OBJECTS 49
#define GEOM_OBJ_ID_CAR_BODY 0
#define GEOM_OBJ_ID_CAR_WHEEL 1
#define GEOM_OBJ_ID_CAR_NUT 2
#define GEOM_OBJ_ID_COW 3
#define GEOM_OBJ_ID_TEAPOT 4
// rb file
#define GEOM_OBJ_ID_TIGER 5
#define N_TIGER_FRAMES 12
#define GEOM_OBJ_ID_HUMAN 17
#define N_HUMAN_FRAMES 30
#define GEOM_OBJ_ID_BUS 47
#define GEOM_OBJ_ID_TANK 48

typedef enum _GEOM_OBJ_TYPE { GEOM_OBJ_TYPE_V = 0, GEOM_OBJ_TYPE_VN, GEOM_OBJ_TYPE_VNT } GEOM_OBJ_TYPE;
// GEOM_OBJ_TYPE_V: (x, y, z)
// GEOM_OBJ_TYPE_VN: (x, y, z, nx, ny, nz)
// GEOM_OBJ_TYPE_VNT: (x, y, z, nx, ny, nz, s, t)
int GEOM_OBJ_ELEMENTS_PER_VERTEX[3] = { 3, 6, 8 };

GLuint geom_obj_VBO[N_GEOMETRY_OBJECTS];
GLuint geom_obj_VAO[N_GEOMETRY_OBJECTS];

int geom_obj_n_triangles[N_GEOMETRY_OBJECTS];
GLfloat *geom_obj_vertices[N_GEOMETRY_OBJECTS];

int read_geometry_bfile(GLfloat **object, int bytes_per_primitive, char *filename) {
	int n_triangles;
	FILE *fp;

	// fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open the object file %s ...", filename);
		return -1;
	}
	fread(&n_triangles, sizeof(int), 1, fp);
	*object = (float *)malloc(n_triangles*bytes_per_primitive);
	if (*object == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}

	fread(*object, bytes_per_primitive, n_triangles, fp);
	// fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);
	fclose(fp);

	return n_triangles;
}

int read_geometry_file(GLfloat **object, char *filename, GEOM_OBJ_TYPE geom_obj_type) {
	int i, n_triangles;
	float *flt_ptr;
	FILE *fp;

	fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Cannot open the geometry file %s ...", filename);
		return -1;
	}

	fscanf(fp, "%d", &n_triangles);
	*object = (float *)malloc(n_triangles * 3 * GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type] * sizeof(float));
	if (*object == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}

	flt_ptr = *object;
	for (i = 0; i < 3 * n_triangles * GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type]; i++)
		fscanf(fp, "%f", flt_ptr++);
	fclose(fp);

	fprintf(stdout, "Read %d primitives successfully.\n\n", n_triangles);

	return n_triangles;
}

void prepare_geom_obj(int geom_obj_ID, char *filename, GEOM_OBJ_TYPE geom_obj_type) {
	int n_bytes_per_vertex;

	n_bytes_per_vertex = GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type] * sizeof(float);//한 vertex마다의 크기
	if(geom_obj_ID >= GEOM_OBJ_ID_TIGER)
		geom_obj_n_triangles[geom_obj_ID] = read_geometry_bfile(&geom_obj_vertices[geom_obj_ID], 3 * n_bytes_per_vertex ,filename);
	else
		geom_obj_n_triangles[geom_obj_ID] = read_geometry_file(&geom_obj_vertices[geom_obj_ID], filename, geom_obj_type);

	// Initialize vertex array object.
	glGenVertexArrays(1, &geom_obj_VAO[geom_obj_ID]);
	glBindVertexArray(geom_obj_VAO[geom_obj_ID]);
	glGenBuffers(1, &geom_obj_VBO[geom_obj_ID]);
	glBindBuffer(GL_ARRAY_BUFFER, geom_obj_VBO[geom_obj_ID]);
	glBufferData(GL_ARRAY_BUFFER, 3 * geom_obj_n_triangles[geom_obj_ID] * n_bytes_per_vertex,
		geom_obj_vertices[geom_obj_ID], GL_STATIC_DRAW);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));//vertex coord fill
	glEnableVertexAttribArray(0);
	if (geom_obj_type >= GEOM_OBJ_TYPE_VN) {
		glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));// normal coord fill
		glEnableVertexAttribArray(1);
	}
	if (geom_obj_type >= GEOM_OBJ_TYPE_VNT) {
		glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));// texture coord fill
		glEnableVertexAttribArray(2);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	free(geom_obj_vertices[geom_obj_ID]);
}

void draw_geom_obj(int geom_obj_ID) {
	if(geom_obj_ID >= GEOM_OBJ_ID_TIGER)
		glFrontFace(GL_CCW);
	glBindVertexArray(geom_obj_VAO[geom_obj_ID]);
	glDrawArrays(GL_TRIANGLES, 0, 3 * geom_obj_n_triangles[geom_obj_ID]);
	glBindVertexArray(0);
}

void free_geom_obj(int geom_obj_ID) {
	glDeleteVertexArrays(1, &geom_obj_VAO[geom_obj_ID]);
	glDeleteBuffers(1, &geom_obj_VBO[geom_obj_ID]);
}
/*
int geom_obj_frag_num[N_GEOMETRY_OBJECTS + N_TIGER_FRAMES - 1];//fragment개수 저장
int *geom_obj_vn_per_frag[N_GEOMETRY_OBJECTS + N_TIGER_FRAMES - 1];//fragment마다 이루고 있는 vertex개수 저장

GEOM_OBJ_TYPE geom_obj_type;
GLfloat **geom_obj_vertices;//fragment마다 이루고 있는 vertex array 저장


int read_geometry_file(int geom_obj_ID, char *filename) {
	int i,j;
	int total_vertex=0;
	char ch;
	float *flt_ptr;
	FILE *fp;

	fprintf(stdout, "Reading geometry from the geometry file %s...\n", filename);
	fp = fopen(filename, "r");
	if (fp == NULL){
		fprintf(stderr, "Cannot open the geometry file %s ...", filename);
		return -1;
	}
	fscanf(fp, "%c%*c", &ch);
	switch (ch) {
	case 'V': geom_obj_type = GEOM_OBJ_TYPE_V; break;
	case 'N': geom_obj_type = GEOM_OBJ_TYPE_VN; break;
	case 'T': geom_obj_type = GEOM_OBJ_TYPE_VNT; break;
	}
	
	fscanf(fp, "%d", &geom_obj_frag_num[geom_obj_ID]);
	geom_obj_vn_per_frag[geom_obj_ID] = (int*)malloc(geom_obj_frag_num[geom_obj_ID] * sizeof(int));
	if (geom_obj_vn_per_frag[geom_obj_ID] == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}
	geom_obj_vertices = (GLfloat**)malloc(geom_obj_frag_num[geom_obj_ID] * sizeof(GLfloat*));
	if (geom_obj_vertices == NULL) {
		fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
		return -1;
	}
	for(i = 0; i< geom_obj_frag_num[geom_obj_ID]; i++){
		fscanf(fp, "%d", &geom_obj_vn_per_frag[geom_obj_ID][i]);
		total_vertex += geom_obj_vn_per_frag[geom_obj_ID][i];
		geom_obj_vertices[i] = (GLfloat*)malloc(geom_obj_vn_per_frag[geom_obj_ID][i] * GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type] * sizeof(GLfloat));
		if (geom_obj_vertices[i] == NULL) {
			fprintf(stderr, "Cannot allocate memory for the geometry file %s ...", filename);
			return -1;
		}

		flt_ptr = geom_obj_vertices[i];
		for (j = 0; j < geom_obj_vn_per_frag[geom_obj_ID][i] * GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type]; j++)
			fscanf(fp, "%f", flt_ptr++);
	}

	fclose(fp);
	return total_vertex;
}

void prepare_geom_obj(int geom_obj_ID, char *filename) {
	int i;
	int until_vn = 0;
	int total_vertex;
	int n_bytes_per_vertex;

	total_vertex = read_geometry_file(geom_obj_ID, filename);
	n_bytes_per_vertex = GEOM_OBJ_ELEMENTS_PER_VERTEX[geom_obj_type] * sizeof(float);
	glGenBuffers(1, &geom_obj_VBO[geom_obj_ID]);
	glBindBuffer(GL_ARRAY_BUFFER, geom_obj_VBO[geom_obj_ID]);
	glBufferData(GL_ARRAY_BUFFER, total_vertex * n_bytes_per_vertex, NULL, GL_STATIC_DRAW);
	for (i = 0; i < geom_obj_frag_num[geom_obj_ID]; i++) {
		glBufferSubData(GL_ARRAY_BUFFER, until_vn * n_bytes_per_vertex, 
			geom_obj_vn_per_frag[geom_obj_ID][i] * n_bytes_per_vertex, geom_obj_vertices[i]);
		until_vn += geom_obj_vn_per_frag[geom_obj_ID][i];
	}
	for (i = 0; i<geom_obj_frag_num[geom_obj_ID]; i++)
		free(geom_obj_vertices[i]);
	free(geom_obj_vertices);

	glGenVertexArrays(1, &geom_obj_VAO[geom_obj_ID]);
	glBindVertexArray(geom_obj_VAO[geom_obj_ID]);
	glBindBuffer(GL_ARRAY_BUFFER, geom_obj_VBO[geom_obj_ID]);
	glVertexAttribPointer(LOC_VERTEX, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(0));//vertex coord fill
	glEnableVertexAttribArray(0);
	if (geom_obj_type >= GEOM_OBJ_TYPE_VN) {
		glVertexAttribPointer(LOC_NORMAL, 3, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(3 * sizeof(float)));// normal coord fill
		glEnableVertexAttribArray(1);
	}
	if (geom_obj_type >= GEOM_OBJ_TYPE_VNT) {
		glVertexAttribPointer(LOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, n_bytes_per_vertex, BUFFER_OFFSET(6 * sizeof(float)));// texture coord fill
		glEnableVertexAttribArray(2);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_geom_obj(int geom_obj_ID) {
	int i;
	int until = 0;
	glBindVertexArray(geom_obj_VAO[geom_obj_ID]);
	for (i = 0; i < geom_obj_frag_num[geom_obj_ID]; i++) {
		glDrawArrays(GL_LINE_LOOP, until, geom_obj_vn_per_frag[geom_obj_ID][i]);
		until += geom_obj_vn_per_frag[geom_obj_ID][i];
	}
	glBindVertexArray(0);
}

void free_geom_obj(int geom_obj_ID) {
	glDeleteVertexArrays(1, &geom_obj_VAO[geom_obj_ID]);
	glDeleteBuffers(1, &geom_obj_VBO[geom_obj_ID]);
	free(geom_obj_vn_per_frag[geom_obj_ID]);
}
*/
/*********************************  END: geometry *********************************/