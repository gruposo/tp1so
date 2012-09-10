CFLAGS= -lpthread -g -Wall
TARGET1 = parser
TARGET2 = Stack
TARGET3 = serializer
TARGET4 = nodeADT
TARGET5 = $(IPC)
TARGET6 = executor
TARGET7 = library
TARGET8 = serverGeneric
TARGET9 = clientGeneric

all: $(TARGET8) $(TARGET9) 

$(TARGET8):	$(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET6).c $(TARGET7).c $(TARGET8).c
	gcc -o $(TARGET8) $(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET6).c $(TARGET7).c $(TARGET8).c $(CFLAGS)

$(TARGET9):	$(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET6).c $(TARGET7).c $(TARGET9).c
	gcc -o $(TARGET9) $(TARGET1).c $(TARGET2).c $(TARGET3).c $(TARGET4).c $(TARGET5).c $(TARGET6).c $(TARGET7).c $(TARGET9).c $(CFLAGS)

clean:
	\rm -f clientGeneric
	\rm -f serverGeneric
