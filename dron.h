#ifndef DRON_H_INCLUDED
#define DRON_H_INCLUDED

#include <windows.h>
#define MAX_X 34
#define MAX_Y 30
#define scale 20

typedef struct wheel{
    int x;
    int y;
    double ang;
    int width;
    int height;
    HBRUSH color;
}wheel;

typedef struct roboticTrolley{
    int x;
    int y;
    int width;
    int height;
    HBRUSH color;
    HBRUSH color2;
    struct wheel*wheels;
    int status;
    int dir;
    int value;
    int pos;
}roboticTrolley;

typedef struct dron{
    struct roboticTrolley*head;
    int size;
    int dir;
    int value;
    struct roboticTrolley**tail;
}dron;

int trajectory[4];
int traj_ptr;
int traj_flag;
int control;

roboticTrolley*initTrolley(int x,int y);

dron*initDron();

dron* autoPilot(dron*my_dron);

roboticTrolley*  turnHorysontal(roboticTrolley* my_rt);

roboticTrolley*  turnVertical(roboticTrolley* my_rt);

roboticTrolley* moveLeft(roboticTrolley*my_rt);

roboticTrolley* moveRight(roboticTrolley*my_rt);

roboticTrolley* moveUp(roboticTrolley*my_rt);

roboticTrolley* moveDown(roboticTrolley*my_rt);

roboticTrolley* autoMove(roboticTrolley*my_rt,dron*my_dron);

dron*moveTail(dron*my_dron);

dron*dronLeft(dron*my_dron);

dron*dronRight(dron*my_dron);

dron*dronUp(dron*my_dron);

dron*dronDown(dron*my_dron);

#endif // DRON_H_INCLUDED
