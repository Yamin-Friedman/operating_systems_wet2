
#ifndef WET2_ACCOUNT_H
#define WET2_ACCOUNT_H

#include <cstring>
#include <pthread.h>
#include <string>
#include <iostream>


//**************************************************************************************
// class name: Account
// Description: Accounts stored in the bank. Each account has a mutex lock and a conditional variable
// to implement a reader writer lock on the account.
//**************************************************************************************
class Account {
private:
	int id_;
	std::string password_;
	int balance_;
	bool VIP_;
public:
	//**************************************************************************************
	// function name: Account
	// Description: Constructor for an Account
	// Parameters: int id - The id of the Account
	// Parameters: string password - The password for the account
	// Paremeters: int balance - The initial balance of the Account
	// Paremeters: bool VIP - if the Account is a VIP or not
	// Returns: void
	//**************************************************************************************
	Account(int id, std::string password, int balance, bool VIP): id_(id),balance_(balance),VIP_(VIP){
		password_ = password;
		if (pthread_mutex_init(&wrl, NULL) != 0)
		{
			std::cerr << "\n mutex init failed\n" ;
			return;
		}
		if (pthread_cond_init(&c, NULL) != 0)
		{
			std::cerr << "\n cond init failed\n" ;
			return;
		}
		read_count = 0;
		write_flag = false;
	}

	//**************************************************************************************
	// function name: ~Account
	// Description: Destructor for an Account
	// Returns: void
	//**************************************************************************************
	~Account(){
		pthread_mutex_destroy(&wrl);
		pthread_cond_destroy(&c);
	};

	//**************************************************************************************
	// function name: get_ID
	// Description: Gets the id of the account
	// Returns: int
	//**************************************************************************************
	int get_ID() const {
		return id_;
	}

	//**************************************************************************************
	// function name: get_balance
	// Description: Gets the balance of the account
	// Returns: int
	//**************************************************************************************
	int get_balance() const {
		return balance_;
	}

	//**************************************************************************************
	// function name: get_password
	// Description: Gets the password of the account
	// Returns: string
	//**************************************************************************************
	std::string get_password() const {
		return password_;
	}

	//**************************************************************************************
	// function name: check_password
	// Description: Checks if password is correct
	// Parameters: string password - The password to check
	// Returns: bool
	//**************************************************************************************
	bool check_password(std::string password) const {
		if(password == password_)
			return true;
		return false;
	}

	//**************************************************************************************
	// function name: is_VIP
	// Description: Checks if the account is a VIP
	// Returns: bool
	//**************************************************************************************
	bool is_VIP() const {
		return VIP_;
	}

	//**************************************************************************************
	// function name: change_VIP_status
	// Description: Changes the VIP status of the account
	// Parameters: bool is_VIP - The state to change to
	// Returns: void
	//**************************************************************************************
	void change_VIP_status(bool is_VIP){
		VIP_ = is_VIP;
	}

	//**************************************************************************************
	// function name: operator+=
	// Description: Adds money to the account
	// Parameters: int &rhs - The amount to add to the account
	// Returns: Account&
	//**************************************************************************************
	Account &operator+=(int &rhs){
		balance_ += rhs;
		return *this;
	}

	//**************************************************************************************
	// function name: operator-=
	// Description: Subtracts money from the account. throws an error if the account would go into minus
	// Parameters: int &rhs - The amount to remove from the account
	// Returns: Account&
	//**************************************************************************************
	Account &operator-=(int &rhs){
		if(rhs > balance_){
			throw rhs;
		}else{
			balance_ -= rhs;
		}
		return *this;
	}

	pthread_mutex_t wrl;
	pthread_cond_t c;
	int read_count;
	bool write_flag;
};


#endif //WET2_ACCOUNT_H
