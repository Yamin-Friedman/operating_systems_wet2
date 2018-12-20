
#ifndef WET2_BANK_H
#define WET2_BANK_H

#include <map>
#include "Account.h"
#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

class Bank {
public:
	Bank();
	~Bank();
	Account *get_account(int id){
		if(account_map.find(id) != account_map.end())
			return account_map[id];
		return NULL;
	}
	bool insert_account(Account *account);
	void print_status();
	void charge_commision();


	pthread_mutex_t wrl;
	pthread_cond_t c;
	int read_count;
	bool write_flag;
	ostream output_log;
	bool bank_open; // This is a flag that will be lowered when all the ATMs finish working
private:
	map<int, Account*> account_map;
	int private_balance;
};


#endif //WET2_BANK_H
