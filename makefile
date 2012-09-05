ifndef IPC
	IPC = sockets
endif

CFLAGS= -lpthread -g -Wall
TARGET1 = parser
TARGET2 = Stack
TARGET3 = serializer
TARGET4 = nodeADT
TARGET5 = $(IPC)
TARGET6 = serverGeneric
TARGET7 = clientGeneric
TARGET8 = executor
TARGET9 = clients


all: $(TARGET6) $(TARGET7) $(TARGET9)

$(TARGET6):	$(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET6).c $(TARGET8).c
	gcc -o $(TARGET6) $(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET6).c $(TARGET8).c $(CFLAGS)

$(TARGET7):	$(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET7).c
	gcc -o $(TARGET7) $(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET7).c $(TARGET8).c $(CFLAGS)

$(TARGET9):	$(TARGET9).c
	gcc -o $(TARGET9).c $(CFLAGS)

clean:
	\rm -f clientGeneric
	\rm -f serverGeneric
	\rm -f clients
