#include "camera.h"
#include "model.h"
#include "draw.h"
#include <GL/glut.h>
#include <stdio.h>
#include <SOIL/SOIL.h>
#include <math.h>
#define VIEWPORT_RATIO (4.0 / 3.0)
#define VIEWPORT_ASPECT 50.0

#define CAMERA_SPEED 1.0

int mouse_x, mouse_y;
GLfloat spot_direction[] = {0.0f,1.0f,-2.0f};
GLfloat light_color[] = {0.2f, 0.2f, 0.2f, 1.0f};
GLfloat light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat light_diffuse[] = { 0.8f, 0.0f, 0.0f, 1.0f };
GLfloat light_specular[] = { 1.0f, 1.0f, 0.0f, 1.0f };
GLfloat mat_emission[] = {-1.0f,-1.0f,-1.0f,1.0f};
GLfloat sky_emission[] = {0.3f,0.0f,0.0f,1.0f};
GLfloat ground_emission[] = {-0.7f,-0.7f,-0.7f,1.0f};
GLfloat light_position[] = {4.0f, 8.0f, 10000.0f, 0.0f};
GLfloat light_position1[] = {8.0f, 8.0f, 4.0f, 1.0f};
GLfloat light_position2[] = {8.0f, -8.0f, 4.0f, 1.0f};
GLfloat light_position3[] = {23.0f, -8.0f, 4.0f, 1.0f};
GLfloat light_position4[] = {-7.5f, 8.0f, 4.0f, 1.0f};
GLfloat light_position5[] = {-7.5f, 25.0f, 4.0f, 1.0f};
GLfloat light_position6[] = {-23.0f, 25.0f, 4.0f, 1.0f};
GLuint sky_texture_id;
GLuint texture_id;
GLuint ground_texture_id;
int onground = 1;
int jumppeaked = 0;
int sprint = 2;
struct Camera camera;
struct Model model;
struct Model ground;

typedef GLubyte Pixel[3];
struct Action
{
    int move_forward;
    int move_backward;
    int step_left;
    int step_right;
    int jump;
};

struct Action action;
int time;

GLuint load_texture(char* filename)
{
    GLuint texture_name;
    glGenTextures(1, &texture_name);

    int width;
    int height;

    Pixel* image = (Pixel*)SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);

    glBindTexture(GL_TEXTURE_2D, texture_name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, (Pixel*)image);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

/*
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
*/
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    return texture_name;
}

void update_camera_position(struct Camera* camera, double elapsed_time)
{
    double distance;

    distance = elapsed_time * CAMERA_SPEED*sprint;
    if (action.move_forward == TRUE) {
		move_camera_forward(camera, distance);
    }

    if (action.move_backward == TRUE) {
		move_camera_backward(camera, distance);
    }

    if (action.step_left == TRUE) {
	    step_camera_left(camera, distance);
    }

    if (action.step_right == TRUE) {
		step_camera_right(camera, distance);
    }
    if (action.jump == TRUE || camera->position.z != 4.3){
        move_camera_up(camera, distance/sprint);
    }
    if(camera->position.x < -8.82 && camera->position.y > 24.17){
        camera->position.x += 16.12;
        camera->position.y -= 16.37;
    }
    if(camera->position.x >9.1 && camera->position.y < -7.18){
        camera->position.x -= 16.21;
        camera->position.y += 16.38;
    }

}

double calc_elapsed_time()
{
    int current_time;
    double elapsed_time;

    current_time = glutGet(GLUT_ELAPSED_TIME);
    elapsed_time = (double)(current_time - time) / 1000.0;
    time = current_time;

    return elapsed_time;
}
void draw_lightfog(){
    GLfloat fogColor[] = {0.5f, 0.5f, 0.5f, 1};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY,0.5f);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,light_diffuse);
    glLightfv(GL_LIGHT1,GL_POSITION,light_position1);
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,90.0);

    glLightfv(GL_LIGHT2,GL_DIFFUSE,light_diffuse);
    glLightfv(GL_LIGHT2,GL_POSITION,light_position2);
    glLightf(GL_LIGHT2,GL_SPOT_CUTOFF,90.0);
    glLightfv(GL_LIGHT3,GL_DIFFUSE,light_diffuse);
    glLightfv(GL_LIGHT3,GL_POSITION,light_position3);
    glLightf(GL_LIGHT3,GL_SPOT_CUTOFF,90.0);
    glLightfv(GL_LIGHT4,GL_DIFFUSE,light_diffuse);
    glLightfv(GL_LIGHT4,GL_POSITION,light_position4);
    glLightf(GL_LIGHT4,GL_SPOT_CUTOFF,90.0);
    glLightfv(GL_LIGHT5,GL_DIFFUSE,light_diffuse);
    glLightfv(GL_LIGHT5,GL_POSITION,light_position5);
    glLightf(GL_LIGHT5,GL_SPOT_CUTOFF,90.0);
    glLightfv(GL_LIGHT6,GL_DIFFUSE,light_diffuse);
    glLightfv(GL_LIGHT6,GL_POSITION,light_position6);
    glLightf(GL_LIGHT6,GL_SPOT_CUTOFF,90.0);
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spot_direction);
    glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,spot_direction);
    glLightfv(GL_LIGHT3,GL_SPOT_DIRECTION,spot_direction);
    glLightfv(GL_LIGHT4,GL_SPOT_DIRECTION,spot_direction);
    glLightfv(GL_LIGHT5,GL_SPOT_DIRECTION,spot_direction);
    glLightfv(GL_LIGHT6,GL_SPOT_DIRECTION,spot_direction);
    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,light_color);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,light_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_ambient);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission);
    }
void display()
{
    double elapsed_time;
    draw_lightfog();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
    elapsed_time = calc_elapsed_time();
    update_camera_position(&camera, elapsed_time);
	set_view_point(&camera);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_id);
    draw_model(&model);
    glBindTexture(GL_TEXTURE_2D,ground_texture_id);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,ground_emission);
    draw_model(&ground);
    glBindTexture(GL_TEXTURE_2D,sky_texture_id);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,sky_emission);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,light_color);
    draw_skybox();

	glutSwapBuffers();
}
void draw_skybox()
{
    double theta, phi1, phi2;
    double x1, y1, z1;
    double x2, y2, z2;
    double u, v1, v2;

    int n_slices, n_stacks;
    double radius;
    int i, k;

    n_slices = 100;
    n_stacks = 50;
    radius = 50;

    glPushMatrix();

    glScaled(radius, radius, radius);

    glColor3f(1, 1, 1);

	glBegin(GL_TRIANGLE_STRIP);

    for (i = 0; i < n_stacks; ++i) {
        v1 = (double)i / n_stacks;
        v2 = (double)(i + 1) / n_stacks;
        phi1 = v1 * M_PI / 2.0;
        phi2 = v2 * M_PI / 2.0;
        for (k = 0; k <= n_slices; ++k) {
            u = (double)k / n_slices;
            theta = u * 2.0 * M_PI;
            x1 = cos(theta) * cos(phi1);
            y1 = sin(theta) * cos(phi1);
            z1 = sin(phi1);
            x2 = cos(theta) * cos(phi2);
            y2 = sin(theta) * cos(phi2);
            z2 = sin(phi2);
            glTexCoord2d(u, 1.0 - v1);
            glVertex3d(x1, y1, z1);
            glTexCoord2d(u, 1.0 - v2);
            glVertex3d(x2, y2, z2);
        }
    }

    glEnd();

    glPopMatrix();
}
void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport (x, y, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLdouble)width / (GLdouble)height, 0.01, 10000.0);
}

void mouse_handler(int button, int state, int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}

void motion_handler(int x, int y)
{
	double horizontal, vertical;

	horizontal = mouse_x - x;
	vertical = mouse_y - y;

	rotate_camera(&camera, horizontal, vertical);

	mouse_x = x;
	mouse_y = y;

    glutPostRedisplay();
}

void key_handler(int key, int x, int y)
{

	switch (key) {
	case 'w':
        action.move_forward = TRUE;
		break;
	case 's':
        action.move_backward = TRUE;
		break;
	case 'a':
        action.step_left = TRUE;
		break;
	case 'd':
        action.step_right = TRUE;
		break;
    case ' ':
        action.jump = TRUE;
        break;
    case '+':
        if(mat_emission[0] < 1.0){
        mat_emission[0] += 0.01;
        mat_emission[1] += 0.01;
        mat_emission[2] += 0.01;
        sky_emission[0] += 0.01;
        sky_emission[1] += 0.01;
        sky_emission[2] += 0.01;
        ground_emission[0] += 0.01;
        ground_emission[1] += 0.01;
        ground_emission[2] += 0.01;
        }
        break;
    case '-':
        if(mat_emission[0] > -1.0){
        mat_emission[0] -= 0.01;
        mat_emission[1] -= 0.01;
        mat_emission[2] -= 0.01;
        sky_emission[0] -= 0.01;
        sky_emission[1] -= 0.01;
        sky_emission[2] -= 0.01;
        ground_emission[0] -= 0.01;
        ground_emission[1] -= 0.01;
        ground_emission[2] -= 0.01;
        }
        break;
    case 'h':
        if(light_diffuse[0] == 0.8f){
            light_diffuse[0] = 1.0f;
            light_diffuse[1] = 1.0f;
            light_diffuse[2] = 1.0f;
        }
        else{
            light_diffuse[0] = 0.8f;
            light_diffuse[1] = 0.0f;
            light_diffuse[2] = 0.0f;
        }
        break;
    case 'r':
        sprint = 4;
        break;
    case 'f':
        if(glIsEnabled(GL_FOG)){
            glDisable(GL_FOG);
        }
        else{
            glEnable(GL_FOG);
        }
	}
	glutPostRedisplay();
}

void key_up_handler(int key, int x, int y)
{
	switch (key) {
	case 'w':
        action.move_forward = FALSE;
		break;
	case 's':
        action.move_backward = FALSE;
		break;
	case 'a':
        action.step_left = FALSE;
		break;
	case 'd':
        action.step_right = FALSE;
		break;
    case ' ':
        action.jump = FALSE;
        break;
    case 'r':
        sprint = 2;
        break;
	}
	glutPostRedisplay();
}

void idle()
{
    glutPostRedisplay();
}

void initialize()
{


    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.7, 0.9, 1.0, 1.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);
    glEnable(GL_LIGHT6);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_MODELVIEW);

    glShadeModel(GL_SMOOTH);
    texture_id = load_texture("brick-wall-texture.jpg");
    sky_texture_id = load_texture("sky.jpg");
    ground_texture_id = load_texture("ground_texture.jpg");
    glClearDepth(1.0);


}

/**
 * Main function
 */
int main(int argc, char* argv[])
{
    load_model("grafika2.obj", &model);
    load_model("ground.obj", &ground);
    print_bounding_box(&model);
    glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	int window = glutCreateWindow("GLUT Window");
	glutSetWindow(window);

    initialize();


    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key_handler);
    glutKeyboardUpFunc(key_up_handler);
    glutMouseFunc(mouse_handler);
    glutMotionFunc(motion_handler);
    glutIdleFunc(idle);

    init_camera(&camera);

    action.move_forward = FALSE;
    action.move_backward = FALSE;
    action.step_left = FALSE;
    action.step_right = FALSE;
    action.jump = FALSE;

    glutMainLoop();

    return 0;
}

