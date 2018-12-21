CPPFLAGS=-g -pthread
CXXFLAGS=-g -std=c++11 -Wall -pedantic
LDFLAGS=-g
RM=rm -f



bank: bank_server.o
	g++ $(LDFLAGS) -o bank

bank_server.o: bank_server.cpp
	g++ $(CXXFLAGS) -c bank_server.cpp 	

clean:
	$(RM) bank *.o
