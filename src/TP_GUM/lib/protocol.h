#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "gnuplot_i.h"

#define BUFLEN 512
#define PORT 9930
#define MAXLENGTH 150
#define MAXPAYLOAD 150

FILE* csvFile;

/*
* receive UDP message
*/


/*
* Communication protocol
*
*	protocol : [msgid] [Payload]
*
* 	- msgid : identifiant du message
*	- Payload
*
*
*	case msgid : 0x00 -> debut du message
*	Payload : [nombre de packets à envoyer]
*
*	case msgid : 0x01 -> envoi de packet
*	Payload : [id_packet] [packetx] [packety] [packetz]
*
*	case msgid : 0x02 -> fin d'envoi de packet
*	Payload : []
*
*	case msgid : 0x03 -> accusé de récéption début de séquence d'envoi
*	payload : []
*
*	case msgid : 0x04 -> accusé de récéption fin de séquence d'envoi
*	payload : []
*
*/

/**
 * \struct Protocol
 * \brief structure contenant le msgid du protocol et sa payLoad non parsée
 */


typedef struct protocol Protocol;
struct protocol{
	char *msgid;
	char *payload;
};

/**
 * \struct StartMessage
 * \brief Structure contenant l'intégralité des données d'un message de type StartMessage 
 */


typedef struct startMessage StartMessage;
struct startMessage{
	char *msgid;
	char *packetCount;
};

/**
 * \struct PacketMessage
 * \brief Structure contenant l'intégralité des données d'un message de type PacketMessage
 */


typedef struct packetMessage PacketMessage;
struct packetMessage{
	char *msgid;
	char *packetId;
	char *x;
	char *y;
	char *z;

	char *vx;
	char *vy;
	char *vz;

	char *ax;
	char *ay;
	char *az;

	char *time;
};

/**
 * \struct EndMessage
 * \brief Structure contenant l'intégralité des données d'un message de type EndMessage
 */


typedef struct endMessage EndMessage;
struct endMessage{

	char *msgid;
	char *echeance;
};

/**
 * \struct Statistic
 * \brief Structure contenant les statistiques d'envoi de paquets UDP
 */


typedef struct statistic Statistic;
struct statistic{

	int nbPacketsRecu;
	int nbPacketsTotal;
	int nbPacketsPerdu;
	int nbPacketsError;
};

StartMessage parse_StartMessage(char* message);
EndMessage parse_EndMessage(char* message);
PacketMessage parse_PacketMessage(char* message);
Protocol parseMessage(char *message);
