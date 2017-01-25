#ifndef SERVER_GUMSTIX
#define SERVER_GUMSTIX

#include "lib/protocol.h"
#include "lib/gnuplot_i.h"

void gnu_accel();
void gnu_speed();
void gnu_position();

void affiche_Statistique(Statistic stats);
void diep(char *s);
void* client(void* args);

#endif
