#include "field.h"

field*initBeds(){
    int height=20,width=2,y=5;
    double p_r=0,p_a=0,r_step=0,g_step=0;
    field*my_field=(field*)malloc(sizeof(field));
    my_field->size=8;
    my_field->color=CreateSolidBrush(RGB(186,109,41));
    my_field->beds=(bed*)malloc(sizeof(bed)*my_field->size);
    for(int i=0;i<my_field->size;i++){
        my_field->beds[i].height=height;
        my_field->beds[i].width=width;
        my_field->beds[i].x=width+i*(2*width);
        my_field->beds[i].y=y;
        my_field->beds[i].color=CreateSolidBrush(RGB(0,109,0));
        my_field->beds[i].size=10;
        my_field->beds[i].pumpkin_bushs=(pumpkin_bush*)malloc(sizeof(pumpkin_bush)*my_field->beds[i].size);
        for(int j=0;j<my_field->beds[i].size;j++){
            my_field->beds[i].pumpkin_bushs[j].size=3+rand()%5;
            my_field->beds[i].pumpkin_bushs[j].x=my_field->beds[i].x+width/2;
            my_field->beds[i].pumpkin_bushs[j].y=y+1+width*j;
            my_field->beds[i].pumpkin_bushs[j].r=10;
            my_field->beds[i].pumpkin_bushs[j].pumpkins=(pumpkin*)malloc(sizeof(pumpkin)*my_field->beds[i].pumpkin_bushs[j].size);
            for(int k=0;k<my_field->beds[i].pumpkin_bushs[j].size;k++){
                p_a=rand()%6;
                p_r=5+rand()%10;
                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].x=my_field->beds[i].pumpkin_bushs[j].x*scale+(int)(cos(p_a)*p_r);
                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].y=my_field->beds[i].pumpkin_bushs[j].y*scale+(int)(sin(p_a)*p_r);
                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].max_r=5+rand()%5;

                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].r=0;
                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].R=0;
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].G=255;
//                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].B=0;
                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].g_rate=((double)(rand()%10))/(double)20;
                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].r_rate=((double)(rand()%5))/(double)5;

                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].size=10;
                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].ripeness=(HBRUSH*)malloc(sizeof(HBRUSH)*my_field->beds[i].pumpkin_bushs[j].pumpkins[k].size);
                r_step=28;
                g_step=16;
                for(int l=0;l<my_field->beds[i].pumpkin_bushs[j].pumpkins[k].size;l++){
                    my_field->beds[i].pumpkin_bushs[j].pumpkins[k].ripeness[l]=CreateSolidBrush(RGB(r_step*l,255-l*g_step,0));
                }
                my_field->beds[i].pumpkin_bushs[j].pumpkins[k].flag=1;
            }
        }
    }
    return my_field;
}
