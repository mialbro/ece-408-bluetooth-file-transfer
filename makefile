OOPTS = -Wall -Wextra -g -std=c99

client : rfcomm_client.o
		gcc $(OOPTS) rfcomm_client.o -o client -lbluetooth

server : rfcomm_server.o
		gcc $(OOPTS) rfcomm_server.o -o server -lbluetooth

rfcomm_client.o : rfcomm_client.c
		gcc $(OOPTS) -c rfcomm_client.c
		
rfcomm_server.o : rfcomm_server.c
		gcc $(OOPTS) -c rfcomm_server.c

clean	:
	rm -f rfcomm_client.o rfcomm_server.o ./server ./client
