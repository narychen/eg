
#include "EgDispatch.h"
#include "client.hpp"
#include "login_server.hpp"

int main(int argc, char* argv[])
{
    if (argc == 2) {
        if (strcmp(argv[1], "-v") == 0) {
    		printf("Build: %s %s\n", __DATE__, __TIME__);
    		return 0;
        } else if (strcmp(argv[1], "login") == 0) {
            cout << "login server start..." << endl;
        } else if (strcmp(argv[1], "client") == 0) {
        	eg::Client cli;
        	cli.run();
        }
        
    	eg::EgDispatch::Instance()->StartLoop();
    	
	} else {
	    printf("Using %s login|msg|msfs|file|route\n", argv[0]);
	}
	
	return 0;
}