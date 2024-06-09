#include "dron.h"


roboticTrolley*initTrolley(int x,int y){
    int height=2,width=2;
    roboticTrolley*my_rt=(roboticTrolley*)malloc(sizeof(roboticTrolley));
    my_rt->height=height;
    my_rt->width=width;
    my_rt->x=x;//20;
    my_rt->y=y;//2;
    my_rt->color=CreateSolidBrush(RGB(80,80,80));
    my_rt->color2=CreateSolidBrush(RGB(255,100,0));
    my_rt->wheels=(wheel*)malloc(sizeof(wheel)*4);
    my_rt->dir=5;
    my_rt->value=0;
    my_rt->status=1;
    my_rt->pos=1;
    my_rt->wheels[0].x=my_rt->x*scale+2;
    my_rt->wheels[0].y=my_rt->y*scale-8;
    my_rt->wheels[0].width=16;
    my_rt->wheels[0].height=8;
    my_rt->wheels[0].ang=0;
    my_rt->wheels[0].color=CreateSolidBrush(RGB(0,0,0));

    my_rt->wheels[1].x=(my_rt->x+1)*scale+2;
    my_rt->wheels[1].y=my_rt->y*scale-8;
    my_rt->wheels[1].width=16;
    my_rt->wheels[1].height=8;
    my_rt->wheels[1].ang=0;
    my_rt->wheels[1].color=CreateSolidBrush(RGB(0,0,0));

    my_rt->wheels[2].x=my_rt->x*scale+2;
    my_rt->wheels[2].y=(my_rt->y+2)*scale;
    my_rt->wheels[2].width=16;
    my_rt->wheels[2].height=8;
    my_rt->wheels[2].ang=0;
    my_rt->wheels[2].color=CreateSolidBrush(RGB(0,0,0));

    my_rt->wheels[3].x=(my_rt->x+1)*scale+2;
    my_rt->wheels[3].y=(my_rt->y+2)*scale;
    my_rt->wheels[3].width=16;
    my_rt->wheels[3].height=8;
    my_rt->wheels[3].ang=0;
    my_rt->wheels[3].color=CreateSolidBrush(RGB(0,0,0));
    return my_rt;
}


dron*initDron(){
    trajectory[0]=2;
    trajectory[1]=10;
    trajectory[2]=18;
    trajectory[3]=26;
    traj_ptr=0;
    traj_flag=1;
    control=0;

    dron*my_dron=(dron*)malloc(sizeof(dron));
    my_dron->head=initTrolley(20,2);
    my_dron->size=0;
    int value=0;
    my_dron->tail=(roboticTrolley**)malloc(sizeof(roboticTrolley*)*10);
    return my_dron;
}


dron* autoPilot(dron*my_dron){//start->1 // end->2

    //printf("X=%d,Y=%d,Status=%d,Value=%d,Dir=%d\n",my_dron->head->x,my_dron->head->y,my_dron->head->status,my_dron->head->value,my_dron->head->dir);

    if(my_dron->head->dir!=5){
        if((my_dron->head->x!=trajectory[traj_ptr])&&traj_flag==1){
            if(my_dron->head->x>trajectory[traj_ptr]){
                my_dron=dronLeft(my_dron);
            }else{
                my_dron=dronRight(my_dron);
            }
        }else{
            if((my_dron->head->y<25)&&traj_flag==1){
                my_dron=dronDown(my_dron);
            }else{
                traj_flag=2;
                if(my_dron->head->x<trajectory[traj_ptr]+4){
                    my_dron=dronRight(my_dron);
                }else{
                    if(my_dron->head->y>3){
                        my_dron=dronUp(my_dron);
                    }else{
                        if(my_dron->head->x<30){
                            traj_ptr++;
                            traj_flag=1;
                        }
                    }
                }
            }
        }
    }else{
        if(my_dron->head->status==1){
            my_dron->head->dir=6;
        }
    }

    return my_dron;
}



roboticTrolley*  turnHorysontal(roboticTrolley* my_rt){
    for(int i=0;i<4;i++){
        my_rt->wheels[i].width=16;
        my_rt->wheels[i].height=8;
    }
    my_rt->wheels[0].x=my_rt->x*scale+2;
    my_rt->wheels[0].y=my_rt->y*scale-8;

    my_rt->wheels[1].x=(my_rt->x+1)*scale+2;
    my_rt->wheels[1].y=my_rt->y*scale-8;

    my_rt->wheels[2].x=my_rt->x*scale+2;
    my_rt->wheels[2].y=(my_rt->y+2)*scale;

    my_rt->wheels[3].x=(my_rt->x+1)*scale+2;
    my_rt->wheels[3].y=(my_rt->y+2)*scale;
    return my_rt;
}

roboticTrolley*  turnVertical(roboticTrolley* my_rt){
    for(int i=0;i<4;i++){
        my_rt->wheels[i].width=8;
        my_rt->wheels[i].height=16;
    }
    my_rt->wheels[0].x=my_rt->x*scale-8;
    my_rt->wheels[0].y=(my_rt->y+1)*scale+2;

    my_rt->wheels[1].x=my_rt->x*scale-8;
    my_rt->wheels[1].y=my_rt->y*scale+2;

    my_rt->wheels[2].x=(my_rt->x+2)*scale;
    my_rt->wheels[2].y=(my_rt->y+1)*scale+2;

    my_rt->wheels[3].x=(my_rt->x+2)*scale;
    my_rt->wheels[3].y=my_rt->y*scale+2;
    return my_rt;
}

roboticTrolley* moveLeft(roboticTrolley*my_rt){//¬лево
    //printf("Left");
    if(my_rt->x>0){
        my_rt->x = my_rt->x - 2;
        my_rt=turnHorysontal(my_rt);
        my_rt->pos=1;
    }
    return my_rt;
}

roboticTrolley* moveRight(roboticTrolley*my_rt){//¬право
	if (my_rt->x < (MAX_X-1)){
		my_rt->x = my_rt->x + 2;
		my_rt=turnHorysontal(my_rt);
		my_rt->pos=1;
    }
    return my_rt;
}

roboticTrolley* moveUp(roboticTrolley*my_rt){//¬верх
	if (my_rt->y > 0){
		my_rt->y = my_rt->y - 2;
		my_rt=turnVertical(my_rt);
		my_rt->pos=2;
    }
    return my_rt;
}

roboticTrolley* moveDown(roboticTrolley*my_rt){//¬низ
	if (my_rt->y < (MAX_Y-1)){
		my_rt->y = my_rt->y + 2;
		my_rt=turnVertical(my_rt);
		my_rt->pos=2;
    }
    return my_rt;
}

roboticTrolley* autoMove(roboticTrolley*my_rt,dron*my_dron){
    if(my_rt->x>my_dron->head->x){
        my_rt=moveLeft(my_rt);
    }else{
        if(my_rt->y+2<my_dron->head->y){
            my_rt=moveDown(my_rt);
        }else{
            my_dron->tail[my_dron->size]=my_rt;
            my_dron->size++;
            my_rt=NULL;
            my_dron->head->status=1;
        }
    }
    return my_rt;
}

dron*moveTail(dron*my_dron){
    if(my_dron->size>0){
            for (int i = my_dron->size - 1; i > 0; i--){
            my_dron->tail[i]->x = my_dron->tail[i-1]->x;
            my_dron->tail[i]->y = my_dron->tail[i-1]->y;
            my_dron->tail[i]->pos = my_dron->tail[i-1]->pos;
        }
        my_dron->tail[0]->x = my_dron->head->x;
        my_dron->tail[0]->y = my_dron->head->y;
        my_dron->tail[0]->pos = my_dron->head->pos;
        for (int i = my_dron->size - 1; i >= 0; i--){
            switch(my_dron->tail[i]->pos){
                case 1:my_dron->tail[i]=turnHorysontal(my_dron->tail[i]);break;
                case 2:my_dron->tail[i]=turnVertical(my_dron->tail[i]);break;
            }
        }
    }

	return my_dron;
}

dron*dronLeft(dron*my_dron){
    my_dron=moveTail(my_dron);
    my_dron->head=moveLeft(my_dron->head);
    return my_dron;
}

dron*dronRight(dron*my_dron){
    my_dron=moveTail(my_dron);
    my_dron->head=moveRight(my_dron->head);
    return my_dron;
}

dron*dronUp(dron*my_dron){
    my_dron=moveTail(my_dron);
    my_dron->head=moveUp(my_dron->head);
    return my_dron;
}

dron*dronDown(dron*my_dron){
    my_dron=moveTail(my_dron);
    my_dron->head=moveDown(my_dron->head);
    return my_dron;
}
