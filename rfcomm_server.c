#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "da.h"

void getFile(int socket, int client) {
	char imageArr[10241];
	int size = 0, bytes = 0, bytesRead = 0;
	FILE *image = fopen("spacex.jpeg", "w");
	
	// Get the size of the file
	bytes = read(client, &size, sizeof(int));
	while (bytes < 0) 
		bytes = read(client, &size, sizeof(int));
	bytes = 0;
	
	// Read entire file
	while (bytesRead < size) {
		bytes = read(socket, imageArr, 10241);
		while (bytes < 0)
			bytes = read(socket, imageArr, 10241);
		
		fwrite(imageArr, 1, bytes, image);
		bytesRead += bytes;
	}
	fclose(image);
}

void displayString(void *value, FILE *fp) {
    fprintf(fp, "%s", (char *)value); 
}

int main() {
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read, i = 0;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));
    
	sendImage(s, client);

    // close connection
    close(client);
    close(s);
    return 0;
}
