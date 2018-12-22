CXXFLAGS=-g -std=c++11 -Wall -pthread
LDFLAGS=-g -pthread
RM=rm -f
SRCS=bank_server.cpp Bank.cpp ATM.cpp Account.cpp
OBJS=$(subst .cpp,.o,$(SRCS))
LDLIBS=$(shell root-config --libs -pthread)
CXX=g++



bank: $(OBJS)
	$(CXX) $(LDFLAGS) -o bank $(OBJS) $(LDLIBS)

bank_server.o: bank_server.cpp Bank.h ATM.h Account.h

Bank.o: Bank.cpp Account.h

ATM.o: ATM.cpp Bank.h

Account.o: Account.cpp

clean:
	$(RM) bank *.o


