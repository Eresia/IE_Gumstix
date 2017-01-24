#include "lib/protocol.h"

/**
 * \fn StartMessage parse_StartMessage(char* message)
 * \brief Fonction de parsing des messages de type StartMessage
 *
 * \param message contenu du message non parsé
 * \return le message parsé dans la structure StartMessage
 */


StartMessage parse_StartMessage(char* message){

	/*declaration des variables et structures*/
	StartMessage startMessage;
	startMessage.msgid = malloc(MAXLENGTH*sizeof(*(startMessage.msgid)));
	startMessage.packetCount = malloc(MAXLENGTH*sizeof(*(startMessage.packetCount)));
	char* packetCount= malloc(MAXLENGTH * sizeof(*packetCount));
	int i=0;
	int j=0;

	/*parcour de la payload*/

	for (i=0;i<strlen(message);i++){
		packetCount[j] = message[i];
		j++;
	}

	/*copie des variables dans la structure*/
	strcpy(startMessage.packetCount, packetCount);

	return startMessage;
}

/**
 * \fn EndMessage parse_EndMessage(char* message)
 * \brief Fonction de parsing des message de type EndMessage
 *
 * \param message contenu du message non parsé
 * \return le message parsé dans la structure EndMessage
 */


EndMessage parse_EndMessage(char* message){

        /*declaration des variables et structures*/
        EndMessage endMessage;
        endMessage.msgid = malloc(MAXLENGTH*sizeof(*(endMessage.msgid)));
        endMessage.echeance = malloc(MAXLENGTH*sizeof(*(endMessage.echeance)));
        char* echeance= malloc(MAXLENGTH * sizeof(*echeance));
        int i=0;
        int j=0;

        /*parcour de la payload*/
        for (i=0;i<strlen(message);i++){
                echeance[j] = message[i];
                j++;
        }

        /*copie des variables dans la structure*/
        strcpy(endMessage.echeance, echeance);

        return endMessage;
}

/**
 * \fn PacketMessage parse_PacketMessage(char* message)
 * \brief Fonction de parsing des message de type PacketMessage
 *
 * \param message contenu du message non parsé
 * \return le message parsé dans la structure PacketMessage
 */

PacketMessage parse_PacketMessage(char* message){


	/*déclaration des variables et structures*/

        PacketMessage packetMessage;
        packetMessage.msgid = malloc(MAXLENGTH*sizeof(*(packetMessage.msgid)));
        packetMessage.packetId = malloc(MAXLENGTH*sizeof(*(packetMessage.packetId)));
        packetMessage.x = malloc(MAXLENGTH*sizeof(*(packetMessage.x)));
        packetMessage.y = malloc(MAXLENGTH*sizeof(*(packetMessage.y)));
        packetMessage.z = malloc(MAXLENGTH*sizeof(*(packetMessage.z)));
	packetMessage.vx = malloc(MAXLENGTH*sizeof(*(packetMessage.vx)));
	packetMessage.vy = malloc(MAXLENGTH*sizeof(*(packetMessage.vy)));
	packetMessage.vz = malloc(MAXLENGTH*sizeof(*(packetMessage.vz)));
	packetMessage.ax = malloc(MAXLENGTH*sizeof(*(packetMessage.ax)));
        packetMessage.ay = malloc(MAXLENGTH*sizeof(*(packetMessage.ay)));
        packetMessage.az = malloc(MAXLENGTH*sizeof(*(packetMessage.az)));
	packetMessage.time = malloc(MAXLENGTH*sizeof(char));
        char* packetId= malloc(MAXLENGTH * sizeof(*packetId));
        char* x= malloc(MAXLENGTH * sizeof(*x));
        char* y= malloc(MAXLENGTH * sizeof(*y));
        char* z= malloc(MAXLENGTH * sizeof(*z));

        char* vx= malloc(MAXLENGTH * sizeof(*vx));
        char* vy= malloc(MAXLENGTH * sizeof(*vy));
        char* vz= malloc(MAXLENGTH * sizeof(*vz));

        char* ax= malloc(MAXLENGTH * sizeof(*ax));
        char* ay= malloc(MAXLENGTH * sizeof(*ay));
        char* az= malloc(MAXLENGTH * sizeof(*az));

	char* time = malloc(MAXLENGTH * sizeof(char));


        int i=0;
        int j=0;
	int state = 1;

	/*
	* Parcours de la payload
	* Etat 1 : variable 1 de la structure
	* Etat ... : variable ... de la structure
	* Etat 4 : variable 4 de la structure
	*/

        for (i=0;i<strlen(message);i++){

		if(state==1){
			if(message[i]!=' '){
				packetId[j] = message[i];
                		j++;
			}
			else{
				state = 2;
				i++;
				j=0;
			}
		}

		if(state==2){
			if(message[i]!=' '){
				x[j] = message[i];
				j++;
			}
			else{
				state = 3;
				i++;
				j=0;
			}
		}

		if(state==3){
			if(message[i]!=' '){
				y[j] = message[i];
				j++;
			}
			else{
				state=4;
				i++;
				j=0;
			}
		}

		if(state==4){
			if(message[i]!=' '){
				z[j] = message[i];
				j++;
			}
			else{
				state=5;
				i++;
				j=0;
			}
		}

		if(state==5){
                        if(message[i]!=' '){
                                vx[j] = message[i];
                                j++;
                        }
                        else{
                                state=6;
                                i++;
                                j=0;
                        }
                }

		if(state==6){
                        if(message[i]!=' '){
                                vy[j] = message[i];
                                j++;
                        }
                        else{
                                state=7;
                                i++;
                                j=0;
                        }
                }

		if(state==7){
                       if(message[i]!=' '){
                               vz[j] = message[i];
                               j++;
                       }
                       else{
                               state=8;
                               i++;
                               j=0;
                       }
                }

		if(state==8){
                        if(message[i]!=' '){
                                ax[j] = message[i];
                                j++;
                        }
                        else{
                                state=9;
                                i++;
                                j=0;
                        }
                }

		if(state==9){
                        if(message[i]!=' '){
                                ay[j] = message[i];
                                j++;
                        }
                        else{
                                state=10;
                                i++;
                                j=0;
                        }
                }

		if(state==10){
                        if(message[i]!=' '){
                                az[j] = message[i];
                                j++;
                        }
                        else{
                                state=11;
                                i++;
                                j=0;
                        }
                }


		if(state==11){
                        if(message[i]!=' '){
                                time[j] = message[i];
                                j++;
                        }
                        else{
                                state=12;
                                i++;
                                j=0;
                        }
                }


        }

	/*copie des variables dans la structure*/
        strcpy(packetMessage.packetId, packetId);
        strcpy(packetMessage.x, x);
        strcpy(packetMessage.y, y);
        strcpy(packetMessage.z, z);
        strcpy(packetMessage.vx, vx);
        strcpy(packetMessage.vy, vy);
        strcpy(packetMessage.vz, vz);
        strcpy(packetMessage.ax, ax);
        strcpy(packetMessage.ay, ay);
        strcpy(packetMessage.az, az);

        return packetMessage;

}

/**
 * \fn Protocol parseMessage(char* message)
 * \brief Fonction de parsing du protocol
 *
 * \param message contenu du message à parser
 * \return Le protocol parsé avec le msgid et la Payload
 */


Protocol parseMessage(char *message){

	/*Declaration des variable*/

	int state=1;
	Protocol protocol;
	protocol.msgid = malloc(MAXLENGTH*sizeof(char));
	protocol.payload = malloc(MAXPAYLOAD*sizeof(char));
	/*
	 * Case state = 1 : msgid
	 * case state = 2 : Payload
	 */

	char* buf=malloc(MAXLENGTH*sizeof(char));
	char* payload=malloc(MAXPAYLOAD*sizeof(char));
	int j=0;
	int i=0;

	/*parcour du message*/
	for(i=0; i<strlen(message);i++){
		/*case msgid*/
		if(state == 1){
			if(message[i] != ' '){
				buf[j] = message[i];
				j++;
			}

			else{
				i++;
				j=0;
				state=2;
			}
		}

		/*case Payload*/
		if(state == 2){
			payload[j] = message[i];
			j++;
		}
	}

	/*copie des éléments du message dans la structure protocol*/
	strcpy(protocol.payload, payload);
	strcpy(protocol.msgid, buf);

	return protocol;
}
