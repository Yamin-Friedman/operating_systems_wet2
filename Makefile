CPPFLAGS=-g -pthread
LDFLAGS=-g
RM=rm -f



bank: bank_server.o
	g++ $(LDFLAGS) -o bank

bank_server.o: bank_server.cpp

clean:
    $(RM) bank *.o
