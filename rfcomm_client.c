#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <string.h>
#include <bluetooth/rfcomm.h>

#include "da.h"

void displayString(void *string, FILE *fp) {
    fprintf(fp, "%s", (char *)string);
}

void sendFile(int socket) {
	int imageSize = 0, bytes = 0;
	char sendBuffer[10240] = {0};
	
	FILE *image = fopen("space.jpeg", "r");	// Open image to send
  fseek(image, 0, SEEK_END);		// Go to the end of the file
  imageSize = ftell(image);		// Get the image size
  fseek(image, 0, SEEK_SET);		// Go back to the beginning of the file
	
 	write(s, (void *)&imageSize, sizeof(int));	// Send image size
  
	// SEND FILE
  while (feof(image) != NULL) {
		bytes = fread(sendBuffer, 1, sizeof(sendBuffer) - 1, image);	// read file bytes to array
		res = write(s, &sendBuffer, bytes);
		while (res < 0)
			res = write(s, &sendBuffer, bytes);
		
		memset(sendBuffer, 0, sizeof(sendBuffer));	// Clear buffer
  }
	fclose(image);
  return;
}

void connectToServer(char *address) {
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18];
    strcpy(dest, address);
    
    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    if (status == 0) {
			sendFile(s);	
		}
	else {
		perror("Unable To Send Data");
	}
	close(s);
}

int main() {
    connectToServer("B8:27:EB:DC:E7:05"); // Connect to raspberry pi
    return 0;
}
