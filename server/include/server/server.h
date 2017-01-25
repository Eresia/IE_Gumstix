#ifndef SERVER_GUMSTIX
#define SERVER_GUMSTIX

#include "lib/protocol.h"
#include "lib/gnuplot_i.h"
#include "vector3.h"

#define POSITION_SIMU "gnuplot/position"
#define SPEED_SIMU "gnuplot/speed"
#define ACCEL_SIMU "gnuplot/acceleration"

#define POSITION_RESULT "gnuplot/position.svg"
#define SPEED_RESULT "gnuplot/speed.svg"
#define ACCEL_RESULT "gnuplot/acceleration.svg"

void gnu_graph(Vector3 min, Vector3 max, char* simuFile, char* resultFile);

void affiche_Statistique(Statistic stats);
void diep(char *s);
void* client(void* args);

#endif
