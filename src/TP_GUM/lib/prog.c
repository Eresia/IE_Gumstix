#include "protocol.h"
#include "gnuplot_i.h"

void plot(){
	gnuplot_ctrl *h;
	h = gnuplot_init();
	gnuplot_cmd(h, "set xlabel 'temps en secondes'");
	gnuplot_cmd(h, "set ylabel 'position'");
	gnuplot_cmd (h, "plot '../data/simu' using 1:2 title 'x' with linespoints");
	gnuplot_cmd(h, "replot '../data/simu' using 1:3 title 'y' with linespoints");
	gnuplot_cmd(h,"replot '../data/simu' using 1:4 title 'z' with linespoints");

}

void splot(){
	
	gnuplot_ctrl *h;
	gnuplot_init();
	gnuplot_cmd(h, "set xlabel 'temps en secondes'");
        gnuplot_cmd(h, "set ylabel 'position'");
	gnuplot_cmd(h, "splot '../data/simu' using 4:3:2 with linespoints");
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
        int s, i, slen=sizeof(si_other);
        char buf[BUFLEN];
	Protocol protocol;
	int count=0;
	int messageCount=1;
	Statistic stats;
	int errorPacket=0;
	csvFile = NULL;
	char* ligneCsv = malloc(MAXPAYLOAD*sizeof(char));
	csvFile = fopen("../data/simu", "w");

        if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
        	diep("socket");

        memset((char *) &si_me, 0, sizeof(si_me));
        si_me.sin_family = AF_INET;
        si_me.sin_port = htons(PORT);
        si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s, &si_me, sizeof(si_me))==-1)
              diep("bind");

        while (1) {

        	if (recvfrom(s, buf, BUFLEN, 0, &si_other, &slen)==-1)
	        	diep("recvfrom()");
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
			if (sendto(s, "03", BUFLEN, 0, &si_other, slen)==-1)
		 		diep("sendto()");

		}

		else if(strcmp(protocol.msgid,"01")==0){
			printf("message de packet\n");
			PacketMessage packetMessage;
			packetMessage = parse_PacketMessage(protocol.payload);


			strcpy(packetMessage.msgid,protocol.msgid);

			printf("message parsé : %s %s %s %s %s %s %s %s %s\n", packetMessage.x, packetMessage.y, packetMessage.z, packetMessage.vx, packetMessage.vy, packetMessage.vz, packetMessage.ax, packetMessage.vy, packetMessage.vz);
			ligneCsv = memset(ligneCsv, '\0', 16*sizeof(char));

			/*concatenation des différentes variable à sérialiser*/

			char* packetMessageId = malloc(16*sizeof(char));
			char* x = malloc(16*sizeof(char));
			char* y = malloc(16*sizeof(char));
			char* z = malloc(16*sizeof(char));
			
			int packetMessageId1;
			double x1;
			double y1;
			double z1;
			
			packetMessageId1 = (int) strtol(packetMessage.packetId, NULL,16);
			x1 = atof(packetMessage.x);
			y1 = atof(packetMessage.y);
                        z1 = atof(packetMessage.z);

			sprintf(packetMessageId, "%d", packetMessageId1);
			sprintf(x, "%lf", x1);
                        sprintf(y, "%lf", y1);
                        sprintf(z, "%lf", z1);

			strcat(ligneCsv, packetMessageId);
			strcat(ligneCsv, " ");
			strcat(ligneCsv, x);
			strcat(ligneCsv, " ");          
			strcat(ligneCsv, y);          
			strcat(ligneCsv, " ");          
			strcat(ligneCsv, z);          
			strcat(ligneCsv, "\n");		

			/*ecriture sur le fichier csv*/
			fputs(ligneCsv, csvFile);	

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
			if (sendto(s, "04", BUFLEN, 0, &si_other, slen)==-1)
                        	diep("sendto()");

			stats.nbPacketsRecu = messageCount -1;
			stats.nbPacketsError = (int) strtol(endMessage.echeance, NULL, 16);
			stats.nbPacketsPerdu = stats.nbPacketsTotal - stats.nbPacketsRecu - stats.nbPacketsError;;
	
	
			/*affichage des statistiques*/
			affiche_Statistique(stats);
			fclose(csvFile);
			plot();
			splot();
		}

	}
	close(s);
}

int main(){

	pthread_t threadClient;

	if(pthread_create(&threadClient, NULL, client, NULL) == -1){
		perror("pthread_create");
		return EXIT_FAILURE;
	}
	pthread_join(threadClient,NULL);
	

	return 0;
}
