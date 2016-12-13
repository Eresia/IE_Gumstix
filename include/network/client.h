#ifndef CLIENT_H
#define CLIENT_H

	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <stdio.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <unistd.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <sys/time.h>
	#include <string.h>

	#include "i2c/i2cget.h"

	#define BUFLEN 512
	#define PORT 9930


	#define TASK_PERIOD 50000 /* période de la tâche en microsecondes*/
	#define TASK_DEADLINE 18 /* depassement autorisé*/

	#define NB_LOOP 50
	#define STOP_TIMEOUT 500

	#define SRV_IP "192.168.2.1"
	//#define SRV_IP "127.0.0.1"

	void launch_client();

#endif
