#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include "BTServer.h"
#include <ros/ros.h>

BTServer::BTServer(){
	//char addr[18] = "B8:27:EB:5F:84:D0";
	char addr[18] = "00:00:00:00:00:00";
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	if(s < 0)
	   printf("Error creating socket: %s \n", strerror(errno));
	
	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_channel = (uint8_t) 1;	
	str2ba(addr, &loc_addr.rc_bdaddr);
	
	if(bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr)) < 0)
		printf("Error Binding: %s",strerror(errno));
	else
		;//printf("Binding completed starting listening mode.\n");
	
	if(listen(s, 1) < 0)
		printf("Error Listening: %s",strerror(errno));
	else
		;//printf("wating for cleint to accept.\n");

	opt = sizeof(rem_addr);
}

void BTServer::startServer(){
	client = accept(s, (struct sockaddr *)&rem_addr, &opt);
	ba2str( &rem_addr.rc_bdaddr, buf );
	printf("accepted connection from %s\n", buf);
	connected = 1;
	rThread = std::thread(&BTServer::Read,this);
}

void BTServer::Close(){
	if(ros::ok())
		rThread.join();
	close(client);
	close(s);
	connected = 0;
	printf("Connection to client closed\n");
}


void BTServer::Read(){
	while(true){
		memset(buf, 0, sizeof(buf));
		bytes_read = read(client, buf, sizeof(buf));
		if( bytes_read < 0 ) //failed to read
			break;
		printf("received [%s]\n", buf);
	}
	connected = 0;
}

void BTServer::Write(char * data,int length){
	int status = write(client, data, length);
	if( status < 0 ) {
		printf("Failed to write\n");
		connected = 0;
	}
}


/*
template <class WriteTemp>
void BTServer::Write(WriteTemp input,char type){
	char * output = malloc(sizeof(WriteTemp)+1);
	output[0] = type;
	output[1] = input;
	int status = write(client,output,sizeof(WriteTemp)+1);
	if(status < 0){
		printf("Failed to write\n");
		connected = 0;
	}
}
*/
int BTServer::isConnected(){
	return connected;
}
