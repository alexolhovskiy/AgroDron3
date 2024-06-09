#ifndef FIELD_H_INCLUDED
#define FIELD_H_INCLUDED
#include <windows.h>
#include <time.h>
#include <math.h>

#define MAX_X 34
#define MAX_Y 30
#define scale 20

typedef struct pumpkin{
    int flag;
    int x;
    int y;
    double r;
    int max_r;
    double R;
    double g_rate;
    double r_rate;
    int size;
    HBRUSH*ripeness;
}pumpkin;

typedef struct pumpkin_bush{
    int x;
    int y;
    double r;
    int size;
    struct pumpkin*pumpkins;
}pumpkin_bush;

typedef struct bed{
    int x;
    int y;
    int width;
    int height;
    HBRUSH color;
    int size;
    struct pumpkin_bush*pumpkin_bushs;
}bed;

typedef struct field{
    struct bed*beds;
    int size;
    HBRUSH color;
}field;


field*initBeds();

#endif // FIELD_H_INCLUDED
