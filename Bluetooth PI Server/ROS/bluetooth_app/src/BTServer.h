
#ifndef _BTServer
#define _BTServer

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <thread>
#include <vector>

class BTServer{
private:
	struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
	char buf[1024] = { 0 };
	int s, client, bytes_read, connected;
	socklen_t opt;
	void readThread();
	std::thread rThread;
public:
	BTServer();
	void startServer();
	template <class WriteTemp>
	void Write(WriteTemp input,char type){
	        char * output = (char *) malloc(sizeof(WriteTemp)+1);
	        printf("Package length is  %d",(int)sizeof(WriteTemp)+1);
			output[0] = type;
	        output[1] = input;
	        int status = write(client,output,sizeof(WriteTemp)+1);
	        if(status < 0){
			printf("Failed to write\n");
			connected = 0;
		}
	}
	
	
	void Write(char * data, int length);
	void Read();
	void Close();
	int isConnected();
	
	float lx,ly,az;
};


#endif
