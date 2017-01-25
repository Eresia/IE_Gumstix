#include "network/client.h"

int main(int argc, char** argv){

	#ifndef IS_GUMSTIX
		initializeSimulator(NB_LOOP);
	#endif

	launch_client();

	return 0;

}
