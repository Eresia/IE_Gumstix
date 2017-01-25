#include "server/server.h"

void gnu_accel(){
	gnuplot_ctrl *h;
	h = gnuplot_init();
	gnuplot_cmd(h, "set xlabel 'temps en secondes'");
	gnuplot_cmd(h, "set ylabel 'acceleration'");
	gnuplot_cmd(h, "set terminal svg");
	gnuplot_cmd(h, "set output 'gnuplot/acceleration.svg'");
	gnuplot_cmd (h, "plot 'gnuplot/acceleration' using 1:2 title 'x' with linespoints");
	gnuplot_cmd(h, "replot 'gnuplot/acceleration' using 1:3 title 'y' with linespoints");
	gnuplot_cmd(h,"replot 'gnuplot/acceleration' using 1:4 title 'z' with linespoints");

}

void gnu_speed(){
	gnuplot_ctrl *h;
	h = gnuplot_init();
	gnuplot_cmd(h, "set xlabel 'temps en secondes'");
	gnuplot_cmd(h, "set ylabel 'vitesse'");
	gnuplot_cmd(h, "set terminal svg");
	gnuplot_cmd(h, "set output 'gnuplot/vitesse.svg'");
	gnuplot_cmd (h, "plot 'gnuplot/speed' using 1:2 title 'x' with linespoints");
	gnuplot_cmd(h, "replot 'gnuplot/speed' using 1:3 title 'y' with linespoints");
	gnuplot_cmd(h,"replot 'gnuplot/speed' using 1:4 title 'z' with linespoints");

}

void gnu_position(){

	gnuplot_ctrl *h;
	h = gnuplot_init();
	gnuplot_cmd(h, "set xlabel 'x'");
	gnuplot_cmd(h, "set ylabel 'y'");
	gnuplot_cmd(h, "set ylabel 'z'");
	gnuplot_cmd(h, "set terminal svg");
	gnuplot_cmd(h, "set output 'gnuplot/position.svg'");
	gnuplot_cmd(h, "splot 'gnuplot/position' using 2:3:4 with linespoints");
}

/*
* Fonction qui affiche les stastistiques
*/

void affiche_Statistique(Statistic stats){
	/*print*/

	printf("-----------Statistiques-------------\n");
	printf("nombre de paquets envoyé %d\n", stats.nbPacketsTotal);
	printf("Total de paquets recu : %d\n", stats.nbPacketsRecu);
	printf("Nombre de paquets pas recu : %d\n", stats.nbPacketsPerdu);
	printf("Nombre d'échéances manqué : %d\n", stats.nbPacketsError);


}


/*
* Fonctions qui parsent les données reçu
*/



void diep(char *s)
{
	perror(s);
	exit(1);
}

void* client(void* args){

	struct sockaddr_in si_me, si_other;
	int s, slen=sizeof(si_other);
	char buf[BUFLEN];
	Protocol protocol;
	int count=0;
	int messageCount=1;
	Statistic stats;
	int errorPacket=0;

	FILE* positionCsv = NULL;
	FILE* speedCsv = NULL;
	FILE* accelCsv = NULL;

	char* linePosition = malloc(MAXPAYLOAD*sizeof(char));
	char* lineSpeed = malloc(MAXPAYLOAD*sizeof(char));
	char* lineAccel = malloc(MAXPAYLOAD*sizeof(char));

	positionCsv = fopen("gnuplot/position", "w");
	speedCsv = fopen("gnuplot/speed", "w");
	accelCsv = fopen("gnuplot/acceleration", "w");

	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
		diep("socket");
	}

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s, (const struct sockaddr * ) &si_me, sizeof(si_me))==-1)
	{
		diep("bind");
	}

	while (1) {

		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr * ) &si_other, (socklen_t *) &slen)==-1){
			diep("recvfrom()");
		}

		printf("Received packet from %s:%d\nData: %s\n\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);

		/*traitement*/
		protocol = parseMessage(buf);
		if(strcmp(protocol.msgid,"00")==0){

			/*reception du premier message*/
			printf("message de début\n");
			StartMessage startMessage;
			startMessage = parse_StartMessage(protocol.payload);
			strcpy(startMessage.msgid, protocol.msgid);

			stats.nbPacketsTotal = (int) strtol(startMessage.packetCount,NULL, 16);

			/*envoi d'un accusé de récéption*/
			if (sendto(s, "03", BUFLEN, 0, (const struct sockaddr * ) &si_other, slen)==-1){
				diep("sendto()");
			}


		}

		else if(strcmp(protocol.msgid,"01")==0){
			printf("message de packet\n");
			PacketMessage packetMessage;
			packetMessage = parse_PacketMessage(protocol.payload);


			strcpy(packetMessage.msgid,protocol.msgid);

			printf("message parsé : %s %s %s %s %s %s %s %s %s\n", packetMessage.x, packetMessage.y, packetMessage.z, packetMessage.vx, packetMessage.vy, packetMessage.vz, packetMessage.ax, packetMessage.vy, packetMessage.vz);
			linePosition = memset(linePosition, '\0', 16*sizeof(char));
			lineSpeed = memset(lineSpeed, '\0', 16*sizeof(char));
			lineAccel = memset(lineAccel, '\0', 16*sizeof(char));

			/*concatenation des différentes variable à sérialiser*/

			char* packetMessageId = malloc(16*sizeof(char));

			char* x = malloc(16*sizeof(char));
			char* y = malloc(16*sizeof(char));
			char* z = malloc(16*sizeof(char));

			char* vx = malloc(16*sizeof(char));
			char* vy = malloc(16*sizeof(char));
			char* vz = malloc(16*sizeof(char));

			char* ax = malloc(16*sizeof(char));
			char* ay = malloc(16*sizeof(char));
			char* az = malloc(16*sizeof(char));

			int packetMessageId1;
			double x1, y1, z1;
			double vx1, vy1, vz1;
			double ax1, ay1, az1;

			packetMessageId1 = (int) strtol(packetMessage.packetId, NULL,16);
			x1 = atof(packetMessage.x);
			y1 = atof(packetMessage.y);
			z1 = atof(packetMessage.z);

			vx1 = atof(packetMessage.vx);
			vy1 = atof(packetMessage.vy);
			vz1 = atof(packetMessage.vz);

			ax1 = atof(packetMessage.ax);
			ay1 = atof(packetMessage.ay);
			az1 = atof(packetMessage.az);

			sprintf(packetMessageId, "%d", packetMessageId1);
			sprintf(x, "%lf", x1);
			sprintf(y, "%lf", y1);
			sprintf(z, "%lf", z1);

			sprintf(vx, "%lf", vx1);
			sprintf(vy, "%lf", vy1);
			sprintf(vz, "%lf", vz1);

			sprintf(ax, "%lf", ax1);
			sprintf(ay, "%lf", ay1);
			sprintf(az, "%lf", az1);

			strcat(linePosition, packetMessageId);
			strcat(linePosition, " ");
			strcat(linePosition, x);
			strcat(linePosition, " ");
			strcat(linePosition, y);
			strcat(linePosition, " ");
			strcat(linePosition, z);
			strcat(linePosition, "\n");

			strcat(lineSpeed, packetMessageId);
			strcat(lineSpeed, " ");
			strcat(lineSpeed, vx);
			strcat(lineSpeed, " ");
			strcat(lineSpeed, vy);
			strcat(lineSpeed, " ");
			strcat(lineSpeed, vz);
			strcat(lineSpeed, "\n");

			strcat(lineAccel, packetMessageId);
			strcat(lineAccel, " ");
			strcat(lineAccel, ax);
			strcat(lineAccel, " ");
			strcat(lineAccel, ay);
			strcat(lineAccel, " ");
			strcat(lineAccel, az);
			strcat(lineAccel, "\n");

			/*ecriture sur le fichier csv*/
			fputs(linePosition, positionCsv);
			fputs(lineSpeed, speedCsv);
			fputs(lineAccel, accelCsv);

			int packetId = (int)strtol(packetMessage.packetId,NULL,16);

			if(packetId != count){
				/* Un packet à eu une échéance manquée*/
				printf("packet id %d count %d\n", packetId,count);
				errorPacket++;
				count = count + (packetId - count);
			}

			count++;
			messageCount ++;

		}

		else if(strcmp(protocol.msgid,"02")==0){

			/*traitement du message de fin*/
			printf("message de fin\n");
			EndMessage endMessage = parse_EndMessage(protocol.payload);

			/*envoi d'un accusé de récéption*/
			if (sendto(s, "04", BUFLEN, 0, (const struct sockaddr * ) &si_other, slen)==-1){
				diep("sendto()");
			}

			stats.nbPacketsRecu = messageCount -1;
			stats.nbPacketsError = (int) strtol(endMessage.echeance, NULL, 16);
			stats.nbPacketsPerdu = stats.nbPacketsTotal - stats.nbPacketsRecu - stats.nbPacketsError;;


			/*affichage des statistiques*/
			affiche_Statistique(stats);
			fclose(positionCsv);
			fclose(speedCsv);
			fclose(accelCsv);
			gnu_accel();
			gnu_speed();
			gnu_position();
		}

	}
	close(s);
}
