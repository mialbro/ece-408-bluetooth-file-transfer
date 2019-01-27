OOPTS = -Wall -Wextra -g -std=c99

client : da.o cda.o stack.o rfcomm_client.o
		gcc $(OOPTS) da.o cda.o stack.o rfcomm_client.o -o client -lbluetooth

server : da.o cda.o stack.o rfcomm_server.o
		gcc $(OOPTS) da.o cda.o stack.o rfcomm_server.o -o server -lbluetooth

rfcomm_client.o : rfcomm_client.c
		gcc $(OOPTS) -c rfcomm_client.c
		
rfcomm_server.o : rfcomm_server.c
		gcc $(OOPTS) -c rfcomm_server.c

da.o : da.c da.h
	gcc $(OOPTS) -c da.c


stack.o : stack.c stack.h
	gcc $(OOPTS) -c stack.c


clean	:
	rm -f da.o cda.o stack.o rfcomm_client.o rfcomm_server.o ./server ./client
