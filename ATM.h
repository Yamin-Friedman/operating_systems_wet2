

#ifndef WET2_ATM_H
#define WET2_ATM_H

#include "Bank.h"
#include <iostream>
#include <pthread.h>
#include <fstream>
#include <unistd.h>

using namespace std;

//**************************************************************************************
// class name: ATM
// Description: Each ATM can do actions on the accounts in the bank in parallel by using locks
//**************************************************************************************
class ATM {
public:

	//**************************************************************************************
	// function name: ATM
	// Description: Constructor for an ATM
	// Parameters: int id - The id of the ATM
	// Parameters: Bank *bank - The bank that the ATM is part of
	// Returns: void
	//**************************************************************************************
	ATM(int id,Bank *bank):id_(id),bank_(bank){}

	//**************************************************************************************
	// function name: ~ATM
	// Description: Deconstructor for an ATM
	// Returns: void
	//**************************************************************************************
	~ATM(){};

	//**************************************************************************************
	// function name: open_account
	// Description: Opens a new account in the bank. Locks the bank for writing.
	// Parameters: int id - The id of the account
	// Parameters: string password - The password for the account
	// Paremeters: int init_balance - The initial balance of the Account
	// Returns: void
	//**************************************************************************************
	void open_account(int id, string password, int init_balance);

	//**************************************************************************************
	// function name: make_VIP
	// Description: Makes an account a VIP. Locks the bank for reading and the account for writing.
	// Parameters: int id - The id of the account
	// Parameters: string password - The password for the account
	// Returns: void
	//**************************************************************************************
	void make_VIP(int id, string password);

	//**************************************************************************************
	// function name: deposit
	// Description: Deposits amount into the account. Locks the bank for reading and the account for writing.
	// Parameters: int id - The id of the account
	// Parameters: string password - The password for the account
	// Paremeters: int amount - The amount to deposit
	// Returns: void
	//**************************************************************************************
	void deposit(int id,string password, int amount);

	//**************************************************************************************
	// function name: withdrawl
	// Description: Withdraws amount from the account. Locks the bank for reading and the account for writing.
	// Parameters: int id - The id of the account
	// Parameters: string password - The password for the account
	// Paremeters: int amount - The amount to withdraw
	// Returns: void
	//**************************************************************************************
	void withdrawl(int id, string password, int amount);

	//**************************************************************************************
	// function name: check_balance
	// Description: Checks the balance of the account. Locks the bank for reading and the account for reading.
	// Parameters: int id - The id of the account
	// Parameters: string password - The password for the account
	// Returns: void
	//**************************************************************************************
	void check_balance(int id, string password);

	//**************************************************************************************
	// function name: transfer_money
	// Description: Transfers money from one account to another. Locks the bank for writing to prevent deadlock between
	// accounts and the accounts for writing.
	// Parameters: int id - The id of the account transfering money
	// Parameters: string password - The password for the account
	// Parameters: int id - The id of the account being transfered to
	// Paremeters: int amount - The amount to transfer
	// Returns: void
	//**************************************************************************************
	void transfer_money(int id, string password, int target_id, int amount);

	fstream input_file;
private:
	int id_;
	Bank *bank_;
};


#endif //WET2_ATM_H
