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

RECT r,messages,errors1,errors2;
HDC hdc;
PAINTSTRUCT ps;
HBRUSH background,font_brush;

char movingError[80]="Stop! Moving Error!";
char Autopilot[80]="Autopilot Mode";
char status2[80]="Stop! Cargo compartment is full! Wait for additional cargo module";

roboticTrolley*my_rt=NULL;


dron*my_dron;

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



field*my_field;

int collisionCheck(field*my_field,dron*my_dron){
    if(my_dron->head->x%4==0&&my_dron->head->y>5&&my_dron->head->y<25){
        printf("If1 X=%d,Y=%d\n",my_dron->head->x,my_dron->head->y);
        return 1;
    }else{
        if(my_dron->head->y>5&&my_dron->head->y<25&&my_dron->head->pos==1){
            printf("If2 X=%d,Y=%d\n",my_dron->head->x,my_dron->head->y);
            return 1;
        }
    }
    printf("Else X=%d,Y=%d\n",my_dron->head->x,my_dron->head->y);
    return 0;
}

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

            messages.bottom=3*scale;
            messages.top=2*scale;
            messages.left=0;
            messages.right=34*scale;

            errors1.bottom=scale;
            errors1.top=0;
            errors1.left=0;
            errors1.right=34*scale;

            errors2.bottom=2*scale;
            errors2.top=scale;
            errors2.left=0;
            errors2.right=34*scale;

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


            if(collisionCheck(my_field,my_dron)==1){
                SetBkMode(hdc, TRANSPARENT);
                DrawText(hdc,movingError,strlen(movingError),&errors1,DT_CENTER);
            }
            if(my_dron->head->status==2){
                SetBkMode(hdc, TRANSPARENT);
                DrawText(hdc,status2,strlen(status2),&errors2,DT_CENTER);
            }
            if(control==1){
                SetBkMode(hdc, TRANSPARENT);
                DrawText(hdc,Autopilot,strlen(Autopilot),&messages,DT_CENTER);
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
                    if(control==0){
                        traj_ptr=0;
                        //traj_ptrToStart();
                    }
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
