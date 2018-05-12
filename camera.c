#include "camera.h"

#include "utils.h"

#include <GL/glut.h>
#include <math.h>

int jumped = 0;
int Collided(float x, float y)
{
    if(x<7.3 && y<7.8){
        return 1;
    }
    if(x<9.1 && y>9.2 && x>-7.11){
        return 1;
    }
    if(x>9.1 && y>-7.18){
        return 1;
    }
    if(y<-8.6){
        return 1;
    }
    if(x<-8.82 && y<24.17){
        return 1;
    }
    if(y>25.5){
        return 1;
    }

    return 0;
}
void init_camera(struct Camera* camera)
{
    camera->position.x = 4;
    camera->position.y = 8.4;
    camera->position.z = 4.3;

    camera->pose.x = 0;
    camera->pose.y = 0;
    camera->pose.z = 270.0;
}

void set_view_point(const struct Camera* camera)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(-(camera->pose.x + 90), 1.0, 0, 0);
	glRotatef(-(camera->pose.z - 90), 0, 0, 1.0);
	glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(struct Camera* camera, double horizontal, double vertical)
{
	camera->pose.z += horizontal;
	if(camera->pose.x <= 90.0 || camera->pose.x >= 270.0){
        camera->pose.x += vertical;
	}
	else if(camera->pose.x >= 90.0 && camera->pose.x < 200.0){
        camera->pose.x = 90.0;
	}
	else{
        camera->pose.x = 270.0;
	}


	if (camera->pose.z < 0) {
		camera->pose.z += 360.0;
	}

	if (camera->pose.z > 360.0) {
		camera->pose.z -= 360.0;
	}

	if (camera->pose.x < 0) {
		camera->pose.x += 360.0;
	}

	if (camera->pose.x > 360.0) {
		camera->pose.x -= 360.0;
	}
}
void move_camera_up(struct Camera* camera, double distance)
{
    if(camera->position.z >= 4.3 && jumped == 0){
    camera->position.z += distance;
    if(camera->position.z >= 4.5){
        jumped = 1;
        camera->position.z = 4.5;
    }
    }
    if(jumped == 1){
        camera->position.z -= distance;
        if(camera->position.z <= 4.3){
            jumped = 0;
            camera->position.z = 4.3;
        }
    }

}
void move_camera_forward(struct Camera* camera, double distance)
{
	double angle = degree_to_radian(camera->pose.z);
    if(!Collided(camera->position.x,camera->position.y + sin(angle)*distance )){
    camera->position.y += sin(angle) * distance;
}
    if(!Collided(camera->position.x + cos(angle) * distance ,camera->position.y)){
	camera->position.x += cos(angle) * distance;
    }
}

void move_camera_backward(struct Camera* camera, double distance)
{
	double angle = degree_to_radian(camera->pose.z);
    if(!Collided(camera->position.x,camera->position.y - sin(angle)*distance )){
    camera->position.y -= sin(angle) * distance;
}
    if(!Collided(camera->position.x - cos(angle) * distance ,camera->position.y)){
	camera->position.x -= cos(angle) * distance;
    }
}

void step_camera_left(struct Camera* camera, double distance)
{
	double angle = degree_to_radian(camera->pose.z + 90.0);
    if(!Collided(camera->position.x,camera->position.y + sin(angle)*distance )){
        camera->position.y += sin(angle) * distance;
}
    if(!Collided(camera->position.x + cos(angle) * distance ,camera->position.y)){
	camera->position.x += cos(angle) * distance;
    }
}

void step_camera_right(struct Camera* camera, double distance)
{
	double angle = degree_to_radian(camera->pose.z - 90.0);
    if(!Collided(camera->position.x,camera->position.y + sin(angle)*distance )){
    camera->position.y += sin(angle) * distance;
}
    if(!Collided(camera->position.x + cos(angle) * distance ,camera->position.y)){
	camera->position.x += cos(angle) * distance;
    }
}

