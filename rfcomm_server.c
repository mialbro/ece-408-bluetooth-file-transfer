#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "da.h"

void getFile(int socket, int client) {
  char imageArr[1025];
  int size = 0, bytes = 0, bytesRead = 0;
  FILE *image = fopen("pi2.jpeg", "w");

  do {
	bytes  = read(client, &size, sizeof(int)); // Get the size of the file
  } while (bytes < 0);

	bytes = 0;

	// Read entire file
	while (bytesRead < size) {
	  do {
	    bytes = read(client, imageArr, 1025);  // Read 1024 bits from the client to an arry
	  } while (bytes < 0);

    fwrite(imageArr, 1, bytes, image);  // write the bytes from the array to the new file
	  bytesRead += bytes;
	}
	fclose(image);
}
}

int main() {
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client;
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

    getFile(s, client);

    // close connection
    close(client);
    close(s);
    return 0;
}
