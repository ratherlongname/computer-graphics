#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// read .obj file
	float v[502][3];
	int f[1000][3];
	FILE* obj_file = fopen("./lowpolybunny.obj", "r");
	char* buf[128];
	int v_index = 0;
	int f_index = 0;

	while(true) {
		char v_or_f;
		int ret = fscanf(obj_file, "%c", &v_or_f);
		
		if(ret == EOF) break;
		else if(v_or_f == 'v') {
			fscanf(obj_file, " %f %f %f ", &v[v_index][0], &v[v_index][1], &v[v_index][2]);
			v_index++;
			if(v_index > 502) {
				printf("too many vertices!\n");
				break;
			}
			// printf("vertex!\n");
		}
		else if(v_or_f == 'f') {
			fscanf(obj_file, " %d %d %d ", &f[f_index][0], &f[f_index][1], &f[f_index][2]);
			f_index++;
			if(f_index > 1000) {
				printf("too many faces!\n");
				break;
			}
			// printf("face!\n");
		}
	}


	glBegin(GL_TRIANGLES);

	
	// printf("v_index = %d, f_index = %d\n", v_index, f_index);
	// for(int i = 0 ; i < 502 ; i++) {
	// 	printf("v %f, %f, %f\n", v[i][0], v[i][1], v[i][2]);
	// }

	for(int i = 0 ; i < 1000 ; i++) {
		int v_1 = f[i][0];
		int v_2 = f[i][1];
		int v_3 = f[i][2];

		glColor3f(1, 0, 0); glVertex3f(v[v_1 - 1][0], v[v_1 - 1][1], v[v_1 - 1][2]);
		glColor3f(1, 0, 0); glVertex3f(v[v_2 - 1][0], v[v_2 - 1][1], v[v_2 - 1][2]);
		glColor3f(1, 0, 1); glVertex3f(v[v_3 - 1][0], v[v_3 - 1][1], v[v_3 - 1][2]);
	}

	// glColor3f(1, 0, 0); glVertex2f(-0.6, -0.75);
	// glColor3f(0, 1, 0); glVertex2f(0.6, -0.75);
	// glColor3f(0, 0, 1); glVertex2f(0, 0.75);
	glEnd();
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(400, 300);
	glutCreateWindow("Triangle");
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}