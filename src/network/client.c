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
	int val1,val2,val3;
	int tmp = 0;
	int i;
	int id = 0;

	val1=val2=val3=4;

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

	if(recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t*) &slen) == -1){
		diep("recvfrom()");
	}

	printf("BUUUUF : %s\n", buf);

	/* on récupère le temps avant de rentrer dans la boucle*/
	gettimeofday(&start, 0);

	for(i = 0; i < NB_LOOP; i++) {


		/*mesure du temps écoulé depuis le dernier passage ici- attention section critique possible- */
		gettimeofday(&checkpoint, 0);
		diff=(checkpoint.tv_sec-start.tv_sec) * 1000000L + (checkpoint.tv_usec-start.tv_usec);
		if (diff < TASK_PERIOD ) ; /* On cadence l'execution toutes les TASK_PERIOD*/
		else {
			gettimeofday(&start, 0); /* On réinitialise le compteur */
			printf("temps écoulé=%lld usec\n",diff);
			if (diff > TASK_PERIOD + TASK_DEADLINE) printf ("***echeance manquée \n"); /* si la condition temps réelle n'est pas respectée */
			else {  /*si la condition temps réel est respectée*/

				/*envoi des informations*/
				sprintf(buf, "%02x %0x %d %d %d\n", 1, id, val1,val2,val3);
				if (sendto(s, buf, BUFLEN, 0, (const struct sockaddr *) &si_other, slen)==-1)
				diep("sendto()");

				/* changement des informations (simul*/
				tmp++;
				if (tmp ==254){
					tmp=0;
				}
				val1=val2=val3=tmp;

				id++;
			}
		}
	}

	int end = 0;

	do{
		int result;
		int timeout = STOP_TIMEOUT;
		fd_set readfs;
		FD_ZERO(&readfs);
		FD_SET(s, &readfs);

		sprintf(buf, "%02x", 2);
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

	printf("BUUUUF : %s\n", buf);

	close(s);
}
