#########
#
# fonctionsTCP
#
#########

# For Solaris
#LD_FLAGS = -lsocket -lnsl

# For Linux
LD_FLAGS =

all: client 

fonctionsSocket.o: fonctionsTCP.c fonctionsTCP.h
	gcc -c -o fonctionsTCP.o fonctionsTCP.c

client: client.c fonctionsTCP.o protocol.h
	gcc client.c -o client fonctionsTCP.o  $(LD_FLAGS)

clean:
	rm *~ ; rm -i \#* ; rm *.o; \
        rm client ; rm serveur
