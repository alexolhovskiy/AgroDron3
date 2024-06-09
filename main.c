#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include "field.h"
#include "dron.h"


#define MAX_X 34
#define MAX_Y 30
#define scale 20

RECT r,messages;
HDC hdc;
PAINTSTRUCT ps;
HBRUSH background,font_brush;

char Autopilot[80]="Autopilot Mode";
char status2[80]="Cargo compartment is full! Wait for additional cargo module";

//
//typedef struct pumpkin{
//    int flag;
//    int x;
//    int y;
//    double r;
//    int max_r;
//    double R;
//    double g_rate;
//    double r_rate;
//    int size;
//    HBRUSH*ripeness;
//}pumpkin;
//
//typedef struct pumpkin_bush{
//    int x;
//    int y;
//    double r;
//    int size;
//    struct pumpkin*pumpkins;
//}pumpkin_bush;
//
//typedef struct bed{
//    int x;
//    int y;
//    int width;
//    int height;
//    HBRUSH color;
//    int size;
//    struct pumpkin_bush*pumpkin_bushs;
//}bed;
//
//typedef struct field{
//    struct bed*beds;
//    int size;
//    HBRUSH color;
//}field;


//typedef struct wheel{
//    int x;
//    int y;
//    double ang;
//    int width;
//    int height;
//    HBRUSH color;
//}wheel;
//
//typedef struct roboticTrolley{
//    int x;
//    int y;
//    int width;
//    int height;
//    HBRUSH color;
//    HBRUSH color2;
//    struct wheel*wheels;
//    int status;
//    int dir;
//    int value;
//    int pos;
//}roboticTrolley;
//
//typedef struct dron{
//    struct roboticTrolley*head;
//    int size;
//    int dir;
//    int value;
//    struct roboticTrolley**tail;
//}dron;

roboticTrolley*my_rt=NULL;
//
//roboticTrolley*initTrolley(int x,int y){
//    int height=2,width=2;
//    roboticTrolley*my_rt=(roboticTrolley*)malloc(sizeof(roboticTrolley));
//    my_rt->height=height;
//    my_rt->width=width;
//    my_rt->x=x;//20;
//    my_rt->y=y;//2;
//    my_rt->color=CreateSolidBrush(RGB(80,80,80));
//    my_rt->color2=CreateSolidBrush(RGB(255,100,0));
//    my_rt->wheels=(wheel*)malloc(sizeof(wheel)*4);
//    my_rt->dir=5;
//    my_rt->value=0;
//    my_rt->status=1;
//    my_rt->pos=1;
//    my_rt->wheels[0].x=my_rt->x*scale+2;
//    my_rt->wheels[0].y=my_rt->y*scale-8;
//    my_rt->wheels[0].width=16;
//    my_rt->wheels[0].height=8;
//    my_rt->wheels[0].ang=0;
//    my_rt->wheels[0].color=CreateSolidBrush(RGB(0,0,0));
//
//    my_rt->wheels[1].x=(my_rt->x+1)*scale+2;
//    my_rt->wheels[1].y=my_rt->y*scale-8;
//    my_rt->wheels[1].width=16;
//    my_rt->wheels[1].height=8;
//    my_rt->wheels[1].ang=0;
//    my_rt->wheels[1].color=CreateSolidBrush(RGB(0,0,0));
//
//    my_rt->wheels[2].x=my_rt->x*scale+2;
//    my_rt->wheels[2].y=(my_rt->y+2)*scale;
//    my_rt->wheels[2].width=16;
//    my_rt->wheels[2].height=8;
//    my_rt->wheels[2].ang=0;
//    my_rt->wheels[2].color=CreateSolidBrush(RGB(0,0,0));
//
//    my_rt->wheels[3].x=(my_rt->x+1)*scale+2;
//    my_rt->wheels[3].y=(my_rt->y+2)*scale;
//    my_rt->wheels[3].width=16;
//    my_rt->wheels[3].height=8;
//    my_rt->wheels[3].ang=0;
//    my_rt->wheels[3].color=CreateSolidBrush(RGB(0,0,0));
//    return my_rt;
//}

dron*my_dron;

//dron*initDron(){
//    dron*my_dron=(dron*)malloc(sizeof(dron));
//    my_dron->head=initTrolley(20,2);
//    my_dron->size=0;
//    int value=0;
//    my_dron->tail=(roboticTrolley**)malloc(sizeof(roboticTrolley*)*10);
//    return my_dron;
//}

int commonValue(dron*my_dron){
    int value=my_dron->head->value;
    for(int o=0;o<my_dron->size;o++){
        value+=my_dron->tail[o]->value;
    }
    return value;
}

void takePumpkin(field*my_field,dron*my_dron){
    for(int i=0;i<my_field->size;i++){
        for(int j=0;j<my_field->beds[i].size;j++){
            for(int k=0;k<my_field->beds[i].pumpkin_bushs[j].size;k++){
                if(my_dron->head->x*scale<my_field->beds[i].pumpkin_bushs[j].pumpkins[k].x&&
                   (my_dron->head->x+my_dron->head->width)*scale>my_field->beds[i].pumpkin_bushs[j].pumpkins[k].x&&
                   my_dron->head->y*scale<my_field->beds[i].pumpkin_bushs[j].pumpkins[k].y&&
                   (my_dron->head->y+my_dron->head->height)*scale>my_field->beds[i].pumpkin_bushs[j].pumpkins[k].y&&
                   my_field->beds[i].pumpkin_bushs[j].pumpkins[k].R>8&&
                   my_dron->head->status!=2){
                    my_field->beds[i].pumpkin_bushs[j].pumpkins[k].flag=0;
                    if(my_dron->value<(my_dron->head->height+my_dron->head->height*my_dron->size)*scale){
                        if(my_dron->head->value<my_dron->head->height*scale){
                            my_dron->head->value++;
                        }else{
                            for(int o=0;o<my_dron->size;o++){
                                if(my_dron->tail[o]->value<my_dron->head->height*scale){
                                    my_dron->tail[o]->value++;
                                }
                            }
                        }
                        my_dron->value=commonValue(my_dron);
                    }else{
                        my_dron->head->status=2;
                        my_dron->head->dir=5;
                        my_rt=initTrolley(MAX_X+8,2);
                    }
                }
            }
        }
    }
}

//roboticTrolley*  turnHorysontal(roboticTrolley* my_rt){
//    for(int i=0;i<4;i++){
//        my_rt->wheels[i].width=16;
//        my_rt->wheels[i].height=8;
//    }
//    my_rt->wheels[0].x=my_rt->x*scale+2;
//    my_rt->wheels[0].y=my_rt->y*scale-8;
//
//    my_rt->wheels[1].x=(my_rt->x+1)*scale+2;
//    my_rt->wheels[1].y=my_rt->y*scale-8;
//
//    my_rt->wheels[2].x=my_rt->x*scale+2;
//    my_rt->wheels[2].y=(my_rt->y+2)*scale;
//
//    my_rt->wheels[3].x=(my_rt->x+1)*scale+2;
//    my_rt->wheels[3].y=(my_rt->y+2)*scale;
//    return my_rt;
//}
//
//roboticTrolley*  turnVertical(roboticTrolley* my_rt){
//    for(int i=0;i<4;i++){
//        my_rt->wheels[i].width=8;
//        my_rt->wheels[i].height=16;
//    }
//    my_rt->wheels[0].x=my_rt->x*scale-8;
//    my_rt->wheels[0].y=(my_rt->y+1)*scale+2;
//
//    my_rt->wheels[1].x=my_rt->x*scale-8;
//    my_rt->wheels[1].y=my_rt->y*scale+2;
//
//    my_rt->wheels[2].x=(my_rt->x+2)*scale;
//    my_rt->wheels[2].y=(my_rt->y+1)*scale+2;
//
//    my_rt->wheels[3].x=(my_rt->x+2)*scale;
//    my_rt->wheels[3].y=my_rt->y*scale+2;
//    return my_rt;
//}
//
//roboticTrolley* moveLeft(roboticTrolley*my_rt){//¬лево
//    //printf("Left");
//    if(my_rt->x>0){
//        my_rt->x = my_rt->x - 2;
//        my_rt=turnHorysontal(my_rt);
//        my_rt->pos=1;
//    }
//    return my_rt;
//}
//
//roboticTrolley* moveRight(roboticTrolley*my_rt){//¬право
//	if (my_rt->x < (MAX_X-1)){
//		my_rt->x = my_rt->x + 2;
//		my_rt=turnHorysontal(my_rt);
//		my_rt->pos=1;
//    }
//    return my_rt;
//}
//
//roboticTrolley* moveUp(roboticTrolley*my_rt){//¬верх
//	if (my_rt->y > 0){
//		my_rt->y = my_rt->y - 2;
//		my_rt=turnVertical(my_rt);
//		my_rt->pos=2;
//    }
//    return my_rt;
//}
//
//roboticTrolley* moveDown(roboticTrolley*my_rt){//¬низ
//	if (my_rt->y < (MAX_Y-1)){
//		my_rt->y = my_rt->y + 2;
//		my_rt=turnVertical(my_rt);
//		my_rt->pos=2;
//    }
//    return my_rt;
//}
//
//roboticTrolley* autoMove(roboticTrolley*my_rt){
//    if(my_rt->x>my_dron->head->x){
//        my_rt=moveLeft(my_rt);
//    }else{
//        if(my_rt->y+2<my_dron->head->y){
//            my_rt=moveDown(my_rt);
//        }else{
//            my_dron->tail[my_dron->size]=my_rt;
//            my_dron->size++;
//            my_rt=NULL;
//            my_dron->head->status=1;
//        }
//    }
//    return my_rt;
//}
//
//dron*moveTail(dron*my_dron){
//    if(my_dron->size>0){
//            for (int i = my_dron->size - 1; i > 0; i--){
//            my_dron->tail[i]->x = my_dron->tail[i-1]->x;
//            my_dron->tail[i]->y = my_dron->tail[i-1]->y;
//            my_dron->tail[i]->pos = my_dron->tail[i-1]->pos;
//        }
//        my_dron->tail[0]->x = my_dron->head->x;
//        my_dron->tail[0]->y = my_dron->head->y;
//        my_dron->tail[0]->pos = my_dron->head->pos;
//        for (int i = my_dron->size - 1; i >= 0; i--){
//            switch(my_dron->tail[i]->pos){
//                case 1:my_dron->tail[i]=turnHorysontal(my_dron->tail[i]);break;
//                case 2:my_dron->tail[i]=turnVertical(my_dron->tail[i]);break;
//            }
//        }
//    }
//
//	return my_dron;
//}
//
//dron*dronLeft(dron*my_dron){
//    my_dron=moveTail(my_dron);
//    my_dron->head=moveLeft(my_dron->head);
//    return my_dron;
//}
//
//dron*dronRight(dron*my_dron){
//    my_dron=moveTail(my_dron);
//    my_dron->head=moveRight(my_dron->head);
//    return my_dron;
//}
//
//dron*dronUp(dron*my_dron){
//    my_dron=moveTail(my_dron);
//    my_dron->head=moveUp(my_dron->head);
//    return my_dron;
//}
//
//dron*dronDown(dron*my_dron){
//    my_dron=moveTail(my_dron);
//    my_dron->head=moveDown(my_dron->head);
//    return my_dron;
//}

field*my_field;

//field*initBeds(){
//    int height=20,width=2,y=5;
//    double p_r=0,p_a=0,r_step=0,g_step=0;
//    field*my_field=(field*)malloc(sizeof(field));
//    my_field->size=8;
//    my_field->color=CreateSolidBrush(RGB(186,109,41));
//    my_field->beds=(bed*)malloc(sizeof(bed)*my_field->size);
//    for(int i=0;i<my_field->size;i++){
//        my_field->beds[i].height=height;
//        my_field->beds[i].width=width;
//        my_field->beds[i].x=width+i*(2*width);
//        my_field->beds[i].y=y;
//        my_field->beds[i].color=CreateSolidBrush(RGB(0,109,0));
//        my_field->beds[i].size=10;
//        my_field->beds[i].pumpkin_bushs=(pumpkin_bush*)malloc(sizeof(pumpkin_bush)*my_field->beds[i].size);
//        for(int j=0;j<my_field->beds[i].size;j++){
//            my_field->beds[i].pumpkin_bushs[j].size=3+rand()%5;
//            my_field->beds[i].pumpkin_bushs[j].x=my_field->beds[i].x+width/2;
//            my_field->beds[i].pumpkin_bushs[j].y=y+1+width*j;
//            my_field->beds[i].pumpkin_bushs[j].r=10;
//            my_field->beds[i].pumpkin_bushs[j].pumpkins=(pumpkin*)malloc(sizeof(pumpkin)*my_field->beds[i].pumpkin_bushs[j].size);
//            for(int k=0;k<my_field->beds[i].pumpkin_bushs[j].size;k++){
//                p_a=rand()%6;
//                p_r=5+rand()%10;
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].x=my_field->beds[i].pumpkin_bushs[j].x*scale+(int)(cos(p_a)*p_r);
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].y=my_field->beds[i].pumpkin_bushs[j].y*scale+(int)(sin(p_a)*p_r);
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].max_r=5+rand()%5;
//
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].r=0;
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].R=0;
////                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].G=255;
////                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].B=0;
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].g_rate=((double)(rand()%10))/(double)20;
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].r_rate=((double)(rand()%5))/(double)5;
//
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].size=10;
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].ripeness=(HBRUSH*)malloc(sizeof(HBRUSH)*my_field->beds[i].pumpkin_bushs[j].pumpkins[k].size);
//                r_step=28;
//                g_step=16;
//                for(int l=0;l<my_field->beds[i].pumpkin_bushs[j].pumpkins[k].size;l++){
//                    my_field->beds[i].pumpkin_bushs[j].pumpkins[k].ripeness[l]=CreateSolidBrush(RGB(r_step*l,255-l*g_step,0));
//                }
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].flag=1;
//            }
//        }
//    }
//    return my_field;
//}

//int trajectory[]={2,10,18,26};
//int traj_ptr=0,traj_flag=1,control=0;
//
//dron* autoPilot(dron*my_dron){//start->1 // end->2
//
//    printf("X=%d,Y=%d,Status=%d,Value=%d,Dir=%d\n",my_dron->head->x,my_dron->head->y,my_dron->head->status,my_dron->head->value,my_dron->head->dir);
//
//    if(my_dron->head->dir!=5){
//        if((my_dron->head->x!=trajectory[traj_ptr])&&traj_flag==1){
//            if(my_dron->head->x>trajectory[traj_ptr]){
//                my_dron=dronLeft(my_dron);
//            }else{
//                my_dron=dronRight(my_dron);
//            }
//        }else{
//            if((my_dron->head->y<25)&&traj_flag==1){
//                my_dron=dronDown(my_dron);
//            }else{
//                traj_flag=2;
//                if(my_dron->head->x<trajectory[traj_ptr]+4){
//                    my_dron=dronRight(my_dron);
//                }else{
//                    if(my_dron->head->y>3){
//                        my_dron=dronUp(my_dron);
//                    }else{
//                        if(my_dron->head->x<30){
//                            traj_ptr++;
//                            traj_flag=1;
//                        }
//                    }
//                }
//            }
//        }
//    }else{
//        if(my_dron->head->status==1){
//            my_dron->head->dir=6;
//        }
//    }
//
//    return my_dron;
//}


LRESULT CALLBACK WndProc(HWND wnd,UINT msg,WPARAM wp,LPARAM lp)
{
    switch(msg)
    {
        case WM_CREATE:
            my_field=initBeds();
            my_dron=initDron();

            r.bottom=(MAX_Y)*scale;
            r.left=0;
            r.right=(MAX_X)*scale;
            r.top=0;

            messages.bottom=2*scale;
            messages.top=scale;
            messages.left=0;
            messages.right=34*scale;

            background=CreateSolidBrush(RGB(0,255,0));
            SetTimer(wnd,0,250,NULL);
            return TRUE;
        case WM_TIMER:
            //cout<<"Timer"<<endl;

            for(int i=0;i<my_field->size;i++){
                for(int j=0;j<my_field->beds[i].size;j++){
                    for(int k=0;k<my_field->beds[i].pumpkin_bushs[j].size;k++){
                        if(my_field->beds[i].pumpkin_bushs[j].pumpkins[k].r<
                           my_field->beds[i].pumpkin_bushs[j].pumpkins[k].max_r){
                            my_field->beds[i].pumpkin_bushs[j].pumpkins[k].r+=my_field->beds[i].pumpkin_bushs[j].pumpkins[k].g_rate;
                        }else{
                            if(my_field->beds[i].pumpkin_bushs[j].pumpkins[k].R<my_field->beds[i].pumpkin_bushs[j].pumpkins[k].size-1){
                                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].R+=my_field->beds[i].pumpkin_bushs[j].pumpkins[k].r_rate;
                                //printf("%f\n",my_field->beds[i].pumpkin_bushs[j].pumpkins[k].R);
                            }
                        }
                    }
                }
            }

            if(control==1){
                my_dron=autoPilot(my_dron);
            }else{
                switch(my_dron->head->dir){
                    case 1:my_dron=dronRight(my_dron);break;
                    case 2:my_dron=dronLeft(my_dron);break;
                    case 3:my_dron=dronDown(my_dron);break;
                    case 4:my_dron=dronUp(my_dron);break;
                    case 5:;break;//Stop
                }
            }


            takePumpkin(my_field,my_dron);

            if(my_dron->head->status==2){
                my_rt=autoMove(my_rt,my_dron);
            }

            InvalidateRect(wnd,&r,FALSE);

            break;
        case WM_PAINT:

            hdc=BeginPaint(wnd,&ps);

            SelectObject(hdc,background);
            Rectangle(hdc,0,0,MAX_X*scale,MAX_Y*scale);

            for(int i=0;i<my_field->size;i++){
                SelectObject(hdc,my_field->color);
                Rectangle(hdc,my_field->beds[i].x*scale,my_field->beds[i].y*scale,
                          (my_field->beds[i].x+my_field->beds[i].width)*scale,
                          (my_field->beds[i].y+my_field->beds[i].height)*scale);
                for(int j=0;j<my_field->beds[i].size;j++){
//                    SelectObject(hdc,my_field->beds[i].color);
//                    Ellipse(hdc,my_field->beds[i].pumpkin_bushs[j].x*scale-my_field->beds[i].pumpkin_bushs[j].r,
//                            my_field->beds[i].pumpkin_bushs[j].y*scale-my_field->beds[i].pumpkin_bushs[j].r,
//                            my_field->beds[i].pumpkin_bushs[j].x*scale+my_field->beds[i].pumpkin_bushs[j].r,
//                            my_field->beds[i].pumpkin_bushs[j].y*scale+my_field->beds[i].pumpkin_bushs[j].r);
                    for(int k=0;k<my_field->beds[i].pumpkin_bushs[j].size;k++){
                        if(my_field->beds[i].pumpkin_bushs[j].pumpkins[k].flag==1){
                            SelectObject(hdc,my_field->beds[i].pumpkin_bushs[j].pumpkins[k].ripeness[(int)(my_field->beds[i].pumpkin_bushs[j].pumpkins[k].R)]);

                            Ellipse(hdc,my_field->beds[i].pumpkin_bushs[j].pumpkins[k].x-
                                   my_field->beds[i].pumpkin_bushs[j].pumpkins[k].r,
                                   my_field->beds[i].pumpkin_bushs[j].pumpkins[k].y-
                                   my_field->beds[i].pumpkin_bushs[j].pumpkins[k].r,
                                   my_field->beds[i].pumpkin_bushs[j].pumpkins[k].x+
                                   my_field->beds[i].pumpkin_bushs[j].pumpkins[k].r,
                                   my_field->beds[i].pumpkin_bushs[j].pumpkins[k].y+
                                   my_field->beds[i].pumpkin_bushs[j].pumpkins[k].r);
                        }
                    }
                }
            }

            SelectObject(hdc,my_dron->head->color);
            Rectangle(hdc,my_dron->head->x*scale,my_dron->head->y*scale,(my_dron->head->x+my_dron->head->width)*scale,(my_dron->head->y+my_dron->head->height)*scale);
            SelectObject(hdc,my_dron->head->color2);
            Rectangle(hdc,my_dron->head->x*scale,my_dron->head->y*scale,(my_dron->head->x+my_dron->head->width)*scale,my_dron->head->y*scale+my_dron->head->value);

            for(int i=0;i<4;i++){
                SelectObject(hdc,my_dron->head->wheels[i].color);
                Rectangle(hdc,my_dron->head->wheels[i].x,
                          my_dron->head->wheels[i].y,
                          my_dron->head->wheels[i].x+my_dron->head->wheels[i].width,
                          my_dron->head->wheels[i].y+my_dron->head->wheels[i].height);
            }

            for(int i=0;i<my_dron->size;i++){
                SelectObject(hdc,my_dron->tail[i]->color);
                Rectangle(hdc,my_dron->tail[i]->x*scale,my_dron->tail[i]->y*scale,(my_dron->tail[i]->x+my_dron->tail[i]->width)*scale,(my_dron->tail[i]->y+my_dron->tail[i]->height)*scale);
                SelectObject(hdc,my_dron->tail[i]->color2);
                Rectangle(hdc,my_dron->tail[i]->x*scale,my_dron->tail[i]->y*scale,(my_dron->tail[i]->x+my_dron->tail[i]->width)*scale,my_dron->tail[i]->y*scale+my_dron->tail[i]->value);

                for(int j=0;j<4;j++){
                    SelectObject(hdc,my_dron->tail[i]->wheels[j].color);
                    Rectangle(hdc,my_dron->tail[i]->wheels[j].x,
                              my_dron->tail[i]->wheels[j].y,
                              my_dron->tail[i]->wheels[j].x+my_dron->tail[i]->wheels[j].width,
                              my_dron->tail[i]->wheels[j].y+my_dron->tail[i]->wheels[j].height);
                }
            }

            if(my_rt!=NULL){
                SelectObject(hdc,my_rt->color);
                Rectangle(hdc,my_rt->x*scale,my_rt->y*scale,(my_rt->x+my_rt->width)*scale,(my_rt->y+my_rt->height)*scale);
                SelectObject(hdc,my_rt->color2);
                Rectangle(hdc,my_rt->x*scale,my_rt->y*scale,(my_rt->x+my_rt->width)*scale,my_rt->y*scale+my_rt->value);

                for(int i=0;i<4;i++){
                    SelectObject(hdc,my_rt->wheels[i].color);
                    Rectangle(hdc,my_rt->wheels[i].x,
                              my_rt->wheels[i].y,
                              my_rt->wheels[i].x+my_rt->wheels[i].width,
                              my_rt->wheels[i].y+my_rt->wheels[i].height);
                }
            }

            if(my_dron->head->dir==6){
                SetBkMode(hdc, TRANSPARENT);
                DrawText(hdc,Autopilot,strlen(Autopilot),&messages,DT_CENTER);
            }
            if(my_dron->head->status==2){
                SetBkMode(hdc, TRANSPARENT);
                DrawText(hdc,status2,strlen(status2),&messages,DT_CENTER);
            }

            EndPaint(wnd,&ps);
            break;
        case WM_CHAR:
            switch((char)wp)
            {
                case 'D':
                case 'd':
                    my_dron->head->dir=1;
                    break;
                case 'A':
                case 'a':
                    my_dron->head->dir=2;
                    break;
                case 'S':
                case 's':
                    my_dron->head->dir=3;
                    break;
                case 'W':
                case 'w':
                    my_dron->head->dir=4;
                    break;
                case 'e':
                case 'E':
                    my_dron->head->dir=5;
                    break;
                case 'f':
                case 'F':
                    control^=1;
                    my_dron->head->dir=6;
                    break;
                default:;
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default: DefWindowProc(wnd,msg,wp,lp);
    }
}


int main()
{
    srand(time(NULL));
    WNDCLASS wnd;
    char*ClassName="WndClass";
    HINSTANCE hinst=GetModuleHandle(NULL);
    HBRUSH hb=CreateSolidBrush(RGB(0,255,0));
    HCURSOR hc=LoadCursor(NULL,IDC_CROSS);
    ZeroMemory(&wnd,sizeof(wnd));
    wnd.hInstance=hinst;
    wnd.lpszClassName=ClassName;
    wnd.lpfnWndProc=WndProc;
    wnd.hbrBackground=hb;
    wnd.hCursor=hc;
    RegisterClass(&wnd);
    HWND hwnd=CreateWindow(ClassName,"AgroDron3",WS_OVERLAPPEDWINDOW,200,50,(MAX_X+1)*scale,(MAX_Y+2)*scale,NULL,NULL,hinst,NULL);

    ShowWindow(hwnd,SW_SHOW);
    MSG msg;
    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
