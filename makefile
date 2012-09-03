ifndef IPC
	IPC = shmem
endif

CFLAGS= -lpthread -g
TARGET1 = parser
TARGET2 = Stack
TARGET3 = serializer
TARGET4 = nodeADT
TARGET5 = $(IPC)
TARGET6 = serverGeneric
TARGET7 = clientGeneric

all: $(TARGET6) $(TARGET7)

$(TARGET6):	$(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET6).c
	gcc -o $(TARGET6) $(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET6).c $(CFLAGS)

$(TARGET7):	$(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET7).c
	gcc -o $(TARGET7) $(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET7).c $(CFLAGS)

clean:
	\rm -f clientGeneric
	\rm -f serverGeneric
