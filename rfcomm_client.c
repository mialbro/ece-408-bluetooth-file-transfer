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

void sendFile(int socket) {
  int imageSize = 0, bytes = 0, status = 0;
  char sendBuffer[1024], fileName[100], message[100];

  printf("Enter a file to send to server: ");
  scanf("%s", fileName);
  printf("Enter a message to send to the server: ");
  scanf("%s", message);


  FILE *image = fopen(fileName, "r");	// Open image to send
  // Unable to find file
  if (image == NULL) {
    printf("Could not find file\n");
    return;
  }

  fseek(image, 0, SEEK_END);		// Go to the end of the file
  imageSize = ftell(image);		// Get the image size
  fseek(image, 0, SEEK_SET);		// Go back to the beginning of the file
 
  do {
    status = write(socket, (void *)message, sizeof(message));	// Send a message
  } while (status < 0);

  do {
    status = write(socket, (void *)fileName, sizeof(fileName)); // Send fileName
  } while (status < 0);

  do {
    status = write(socket, (void *)&imageSize, sizeof(int));	// Send image size
  } while (status < 0);

  // SEND FILE
  while (!feof(image)) {
    bytes = fread(sendBuffer, 1, sizeof(sendBuffer) - 1, image);	// Read bytes from file to array

    do {
      status = write(socket, &sendBuffer, bytes); // Send bytes from array to socket (client)
    } while (status < 0);

    memset(sendBuffer, 0, sizeof(sendBuffer));	// Clear buffer array
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
