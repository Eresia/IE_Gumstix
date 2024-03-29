#include "network/client.h"

void diep(char *s)
{
	perror(s);
	exit(1);
}

/* diep(), #includes and #defines like in the server */

void launch_client()
{

	struct timeval start,checkpoint;
	long long diff;

	struct sockaddr_in si_other;
	int s, slen=sizeof(si_other);
	char buf[BUFLEN];
	int i;
	int id = 0;
	int lost = 0;

	ObjectInformation informations;

	Vector3 average;

	double beginTime;

	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
	diep("socket");

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	sprintf(buf, "%02x %0x", 0, NB_LOOP);
	if (sendto(s, buf, BUFLEN, 0, (const struct sockaddr *) &si_other, slen)==-1){
		diep("sendto()");
	}

	/*if(recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t*) &slen) == -1){
		diep("recvfrom()");
	}*/

	printf("Reception : %s\n", buf);

	i = 0;

	average = calibrate(1000);
	printf("End of Calibration\n");
	informations.acceleration = getZeroVector3();
	informations.speed = getZeroVector3();
	informations.position = getZeroVector3();
	gettimeofday(&informations.lastTime, NULL);

	beginTime = informations.lastTime.tv_sec;
	beginTime += ((double) informations.lastTime.tv_usec) / 1000000;

	/* on récupère le temps avant de rentrer dans la boucle*/
	gettimeofday(&start, 0);

	while(i < NB_LOOP) {
		calculatePosition(average, &informations);

		/*mesure du temps écoulé depuis le dernier passage ici- attention section critique possible- */
		gettimeofday(&checkpoint, 0);
		diff=(checkpoint.tv_sec-start.tv_sec) * 1000000L + (checkpoint.tv_usec-start.tv_usec);
		if (diff < TASK_PERIOD ){ /* On cadence l'execution toutes les TASK_PERIOD*/

		}
		else {
			gettimeofday(&start, 0); /* On réinitialise le compteur */
			//printf("temps écoulé=%lld usec\n",diff);
			if (diff > TASK_PERIOD + TASK_DEADLINE){
				printf ("***echeance manquée, id : %d\n", id); /* si la condition temps réelle n'est pas respectée */
				lost++;
			}
			else {  /*si la condition temps réel est respectée*/
				/*envoi des informations*/

				double time = informations.lastTime.tv_sec;
				time += ((double) informations.lastTime.tv_usec) / 1000000;

				sprintf(buf, "%02x %x", 1, id);
				sprintf(buf, "%s %lf %lf %lf", buf, informations.position.x, informations.position.y, informations.position.z);
				sprintf(buf, "%s %lf %lf %lf", buf, informations.speed.x, informations.speed.y, informations.speed.z);
				sprintf(buf, "%s %lf %lf %lf", buf, informations.acceleration.x, informations.acceleration.y, informations.acceleration.z);
				sprintf(buf, "%s %lf", buf, time - beginTime);

				if (sendto(s, buf, BUFLEN, 0, (const struct sockaddr *) &si_other, slen)==-1){
					diep("sendto()");
				}

				id++;
			}
			#ifndef IS_GUMSTIX
				incrementSimulatedIterator();
			#endif
			i++;
		}
	}

	int end = 0;

	do{
		int result;
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = STOP_TIMEOUT * 1000;
		fd_set readfs;
		FD_ZERO(&readfs);
		FD_SET(s, &readfs);

		sprintf(buf, "%02x %x", 2, lost);
		if (sendto(s, buf, BUFLEN, 0, (const struct sockaddr *) &si_other, slen)==-1){
			diep("sendto()");
		}

		if((result = select(s + 1, &readfs, NULL, NULL, (struct timeval*) &timeout)) < 0)
		{
			diep("select()");
		}


		if(FD_ISSET(s, &readfs)){
			if(recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t*) &slen) == -1){
				diep("recvfrom()");
			}
			end = 1;
		}

	}while(!end);

	printf("Reception : %s\n", buf);

	printf("Lost : %d\n", lost);

	close(s);
}
