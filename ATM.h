

#ifndef WET2_ATM_H
#define WET2_ATM_H

#include "Bank.h"
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <unistd.h>

using namespace std;


class ATM {
public:
	ATM(int id,Bank *bank):id_(id),bank_(bank){}
	~ATM(){};

	void open_account(int id, string password, int init_balance);
	void make_VIP(int id, string password);
	void deposit(int id,string password, int amount);
	void withdrawl(int id, string password, int amount);
	void check_balance(int id, string password);
	void transfer_money(int id, string password, int target_id, int amount);

	fstream input_file;
private:
	int id_;
	Bank *bank_;
};


#endif //WET2_ATM_H
