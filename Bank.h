
#ifndef WET2_BANK_H
#define WET2_BANK_H

#include <map>
#include "Account.h"
#include <pthread.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

//**************************************************************************************
// class name: Bank
// Description: The main bank object. Stores all the accounts and has a mutex lock and a condition variable to implement
// a reader writer lock on the account list.
//**************************************************************************************
class Bank {
public:

	//**************************************************************************************
	// function name: Bank
	// Description: Constructor for a Bank
	// Returns: void
	//**************************************************************************************
	Bank();

	//**************************************************************************************
	// function name: ~Bank
	// Description: Deconstructor for a Bank
	// Returns: void
	//**************************************************************************************
	~Bank();

	//**************************************************************************************
	// function name: get_account
	// Description: Gets an account based on id. Returns NULL if the account does not exist
	// Parameters: int id - The id of the account
	// Returns: Account*
	//**************************************************************************************
	Account *get_account(int id){
		if(account_map.find(id) != account_map.end())
			return account_map[id];
		return NULL;
	}

	//**************************************************************************************
	// function name: insert_account
	// Description: Inserts an account into the bank
	// Parameters: Account *account - A pointer to the account to add
	// Returns: bool
	//**************************************************************************************
	bool insert_account(Account *account);

	//**************************************************************************************
	// function name: print_status
	// Description: Prints the current status of the bank to the screen
	// Returns: void
	//**************************************************************************************
	void print_status();

	//**************************************************************************************
	// function name: charge_commision
	// Description: Charges a commision from all the non-VIP accounts in the bank
	// Returns: void
	//**************************************************************************************
	void charge_commision();


	pthread_mutex_t wrl;
	pthread_cond_t c;
	int read_count;
	bool write_flag;
	fstream output_log;
	bool bank_open; // This is a flag that will be lowered when all the ATMs finish working
private:
	map<int, Account*> account_map;
	int private_balance;
};


#endif //WET2_BANK_H
