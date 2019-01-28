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

void connectToServer(char *address, DA *);

void connectToServer(char *address, DA *da) {
    struct sockaddr_rc addr = { 0 };
    int s, status;
    char dest[18];
    char message[50];
    strcpy(dest, address);
    
    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    
    FILE *fp = fopen("gettysburg", "r");
    while (fscanf(fp, "%s", message) != EOF) {
        char *newData = malloc(sizeof(message));
        memcpy(newData, message, sizeof(message));
        insertDAback(da, newData);
    }
    
    for (int i = 0; i < sizeDA(da); i++) {
        status = write(s, getDA(da, i), sizeof(getDA(da, i)));
    }

    if ( status < 0 ) 
	perror("Unable To Send Data");

    close(s);
    return;
}

int main() {
    DA *da = newDA();
    debugDA(da, 0);
    setDAdisplay(da, displayString);
    connectToServer("B8:27:EB:DC:E7:05", da); // Connect to raspberry pi
    return 0;
}
