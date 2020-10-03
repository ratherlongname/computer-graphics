/*
*	CS 461 Computer Graphics Assignment 04
*	 - Left click drag to rotate
*	 - Scroll wheel up/down to zoom in/out
*	 - Right click to switch between FILL and OUTLINE mode
*	
*	Shivam Bansal
*	Roll no 170101063
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GL/glut.h>

#define WINDOW_HEIGHT 1000
#define WINDOW_WIDTH 1000

#define MAX_VERTICES 1000
#define MAX_FACES 10000
#define MAX_OBJ_LINE_LENGTH 128
#define ZOOM_STEP 0.011

// Rotate
double mouse_x = 0.0;
double mouse_y = 0.0;
double rotate_x = 0.0;
double rotate_y = 0.0;

// Zoom
double zoom_level = 1.0;

// Obj file
char file_name[] = "./lowpolybunny.obj";
float v[MAX_VERTICES][3];
int f[MAX_VERTICES][3];
int v_index = 0, f_index = 0;    // Number of vertices, faces

// Colours for vertices
static GLfloat red[3] = {1.0, 0.0, 0.0};
static GLfloat green[3] = {0.0, 1.0, 0.0};
static GLfloat blue[3] = {0.0, 0.0, 1.0};
static bool is_polygon_mode_fill = true; // Store if current mode is FILL or OUTLINE

void read_obj_file(const char file_name[], float v[MAX_VERTICES][3], int f[MAX_FACES][3], int &v_index, int &f_index) {
	FILE* obj_file = fopen(file_name, "r");
	char* buf[MAX_OBJ_LINE_LENGTH];

	while(true) {
		char v_or_f;
		int ret_val = fscanf(obj_file, "%c", &v_or_f);
		
		if(ret_val == EOF) break;
		else if(v_or_f == 'v') {
			fscanf(obj_file, " %f %f %f ", &v[v_index][0], &v[v_index][1], &v[v_index][2]);
			// printf("vertex read: v %f %f %f\n", v[v_index][0], v[v_index][1], v[v_index][2]);
			v_index++;
			
			if(v_index > MAX_VERTICES) {
				printf("Error: Number of vertices in obj file exceeds MAX_VERTICES.\n");
				exit(1);
			}
		}
		else if(v_or_f == 'f') {
			fscanf(obj_file, " %d %d %d ", &f[f_index][0], &f[f_index][1], &f[f_index][2]);
			// printf("face read: f %d %d %d\n", f[f_index][0], f[f_index][1], f[f_index][2]);
			f_index++;

			if(f_index > MAX_FACES) {
				printf("Error: Number of faces in obj file exceeds MAX_FACES.\n");
				exit(1);
			}
		}
		else {
			printf("Error: Unexpected error in reading obj file.\n");
			exit(1);
		}
	}

	return;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Rotate
	glRotatef(rotate_x, 0.0, -1.0, 0.0);
	glRotatef(rotate_y, -1.0, 0.0, 0.0);
	rotate_x = 0.0;    // Reset rotation values after rotating
	rotate_y = 0.0;

	// Zoom
	glScalef(zoom_level, zoom_level, zoom_level);
	zoom_level = 1.0;    // Reset zoom level after zoom

	// Draw all faces
	glBegin(GL_TRIANGLES);
	for(int i = 0 ; i < f_index ; i++) {
		int v_1 = f[i][0];
		int v_2 = f[i][1];
		int v_3 = f[i][2];

		glColor3fv(red); glVertex3fv(v[v_1 - 1]);
		glColor3fv(green); glVertex3fv(v[v_2 - 1]);
		glColor3fv(blue); glVertex3fv(v[v_3 - 1]);
	}

	glEnd();
	glFlush();
}

void mouse_move(int x, int y) {
	// Rotation is difference of last mouse position and current position
	rotate_x = (mouse_x - x) / 10;
	rotate_y = (mouse_y - y) / 10;
	
	// Update mouse position for next callback
	mouse_x = x;
	mouse_y = y;

	glutPostRedisplay();
	return;
}

void mouse_press(int button, int state, int x, int y) {
	switch(button) {
		case GLUT_LEFT_BUTTON:    // Left click
			if(state == GLUT_DOWN) {
				// Start noting mouse position for drag-rotate
				mouse_x = x;
				mouse_y = y;
			}
			break;

		case GLUT_RIGHT_BUTTON:    // Right click
			if(state == GLUT_DOWN) {
				// Switch FILL, OUTLINE modes
				if(is_polygon_mode_fill == true){
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					is_polygon_mode_fill = false;
				}
				else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					is_polygon_mode_fill = true;
				}
				
				glutPostRedisplay();
			}
			break;
		
		case 3:    // Scroll wheel up
			if(state == GLUT_UP) {
				// Set zoom_level greater than 1 (Zoom in)
				zoom_level += ZOOM_STEP;
				glutPostRedisplay();
			}
			break;

		case 4:    // Scroll wheel down
			if(state == GLUT_UP) {
				// Set zoom_level less than 1 (Zoom out)
				zoom_level -= ZOOM_STEP;
				glutPostRedisplay();
			}
			break;
	}

	return;
}

void init() {
	glClearColor(0.25, 0.25, 0.25, 0.0);

	// Adjust model to be in centre of window and a bit larger
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.2, -0.75, 0.0);
	glScalef(4, 4, 4);

	glPolygonMode(GL_FRONT, GL_FILL);

	// Seed rand() for random colors to be generated on left click
	srand(time(0));
	
	// Read OBJ file
	read_obj_file(file_name, v, f, v_index, f_index);

	return;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(250, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("BUNNYYYY!");
	
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouse_press);
	glutMotionFunc(mouse_move);
	glutMainLoop();

	return 0;
}