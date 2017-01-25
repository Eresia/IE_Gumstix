#include "server/server.h"

int main(){

	pthread_t threadClient;

	if(pthread_create(&threadClient, NULL, client, NULL) == -1){
		perror("pthread_create");
		return EXIT_FAILURE;
	}
	pthread_join(threadClient,NULL);


	return 0;
}
