

#include "ATM.h"

#define SEC 1000000

//**************************************************************************************
// function name: open_account
// Description: Opens a new account in the bank. Locks the bank for writing.
// Parameters: int id - The id of the account
// Parameters: string password - The password for the account
// Paremeters: int init_balance - The initial balance of the Account
// Returns: void
//**************************************************************************************
void ATM::open_account(int id, string password, int init_balance){
	pthread_mutex_lock(&bank_->wrl);
	while(bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->write_flag = true;
	while(bank_->read_count > 0)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	pthread_mutex_unlock(&bank_->wrl);

	if(bank_->get_account(id) != NULL){
		bank_->output_log << "Error " << id_ << ": Your transaction failed – account with the same id exists" << endl;
	} else {
		Account *new_account = new Account(id,password,init_balance,false);
		bank_->insert_account(new_account);
		usleep(SEC);
		bank_->output_log << id_ << ": New account id is " << id << " with password " << password << " and initial balance " << init_balance << endl;
	}
	pthread_mutex_lock(&bank_->wrl);
	bank_->write_flag = false;
	pthread_cond_broadcast(&bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}

//**************************************************************************************
// function name: make_VIP
// Description: Makes an account a VIP. Locks the bank for reading and the account for writing.
// Parameters: int id - The id of the account
// Parameters: string password - The password for the account
// Returns: void
//**************************************************************************************
void ATM::make_VIP(int id, string password){
	pthread_mutex_lock(&bank_->wrl);
	while(bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->read_count++;
	pthread_mutex_unlock(&bank_->wrl);

	Account *account = bank_->get_account(id);

	if(account == NULL){
		bank_->output_log << "Error " << id_ << ": Your transaction failed – account id " << id <<" does not exist" << endl;
		pthread_mutex_lock(&bank_->wrl);
		bank_->read_count--;
		if(bank_->read_count == 0)
			pthread_cond_broadcast(&bank_->c);
		pthread_mutex_unlock(&bank_->wrl);

		return;
	}

	pthread_mutex_lock(&account->wrl);
	while(account->write_flag)
		pthread_cond_wait(&account->c, &account->wrl);
	account->write_flag = true;
	while(account->read_count > 0)
		pthread_cond_wait(&account->c, &account->wrl);
	pthread_mutex_unlock(&account->wrl);

	if (!account->check_password(password)) {
		bank_->output_log << "Error " << id_ << ": Your transaction failed – password for account id " << id << " is incorrect" << endl;
	} else {
		account->change_VIP_status(true);
		usleep(SEC);
	}
	pthread_mutex_lock(&account->wrl);
	account->write_flag = false;
	pthread_cond_broadcast(&account->c);
	pthread_mutex_unlock(&account->wrl);

	pthread_mutex_lock(&bank_->wrl);
	bank_->read_count--;
	if(bank_->read_count == 0)
		pthread_cond_broadcast(&bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}

//**************************************************************************************
// function name: deposit
// Description: Deposits amount into the account. Locks the bank for reading and the account for writing.
// Parameters: int id - The id of the account
// Parameters: string password - The password for the account
// Paremeters: int amount - The amount to deposit
// Returns: void
//**************************************************************************************
void ATM::deposit(int id, string password, int amount){
	pthread_mutex_lock(&bank_->wrl);
	while(bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->read_count++;
	pthread_mutex_unlock(&bank_->wrl);

	Account *account = bank_->get_account(id);

	if(account == NULL){
		bank_->output_log << "Error " << id_ << ": Your transaction failed – account id " << id <<" does not exist" << endl;
		pthread_mutex_lock(&bank_->wrl);
		bank_->read_count--;
		if(bank_->read_count == 0)
			pthread_cond_broadcast(&bank_->c);
		pthread_mutex_unlock(&bank_->wrl);

		return;
	}

	pthread_mutex_lock(&account->wrl);
	while(account->write_flag)
		pthread_cond_wait(&account->c, &account->wrl);
	account->write_flag = true;
	while(account->read_count > 0)
		pthread_cond_wait(&account->c, &account->wrl);
	pthread_mutex_unlock(&account->wrl);

	if(!account->check_password(password)){
		bank_->output_log << "Error " << id_ << ": Your transaction failed – password for account id " << id << " is incorrect" << endl;
	} else {
		*account += amount;
		bank_->output_log << id_ << ": Account " << id << " new balance is " << account->get_balance() << " after " << amount << " $ was deposited" << endl;
		usleep(SEC);
	}

	pthread_mutex_lock(&account->wrl);
	account->write_flag = false;
	pthread_cond_broadcast(&account->c);
	pthread_mutex_unlock(&account->wrl);

	pthread_mutex_lock(&bank_->wrl);
	bank_->read_count--;
	if(bank_->read_count == 0)
		pthread_cond_broadcast(&bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}

//**************************************************************************************
// function name: withdrawl
// Description: Withdraws amount from the account. Locks the bank for reading and the account for writing.
// Parameters: int id - The id of the account
// Parameters: string password - The password for the account
// Paremeters: int amount - The amount to withdraw
// Returns: void
//**************************************************************************************
void ATM::withdrawl(int id, string password, int amount){
	pthread_mutex_lock(&bank_->wrl);
	while(bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->read_count++;
	pthread_mutex_unlock(&bank_->wrl);

	Account *account = bank_->get_account(id);

	if(account == NULL){
		bank_->output_log << "Error " << id_ << ": Your transaction failed – account id " << id <<" does not exist" << endl;
		pthread_mutex_lock(&bank_->wrl);
		bank_->read_count--;
		if(bank_->read_count == 0)
			pthread_cond_broadcast(&bank_->c);
		pthread_mutex_unlock(&bank_->wrl);

		return;
	}

	pthread_mutex_lock(&account->wrl);
	while(account->write_flag)
		pthread_cond_wait(&account->c, &account->wrl);
	account->write_flag = true;
	while(account->read_count > 0)
		pthread_cond_wait(&account->c, &account->wrl);
	pthread_mutex_unlock(&account->wrl);

	if (!account->check_password(password)) {
		bank_->output_log << "Error " << id_ << ": Your transaction failed – password for account id " << id << " is incorrect" << endl;
	} else {
		try{
			*account -= amount;
			bank_->output_log << id_ << ": Account " << id << " new balance is " << account->get_balance() << " after " << amount << " $ was withdraw" << endl;
		}
		catch (int){
			bank_->output_log << "Error " << id_ << ": Your transaction failed – account id " << id <<" balance is lower than " << amount << endl;
		}
		usleep(SEC);
	}

	pthread_mutex_lock(&account->wrl);
	account->write_flag = false;
	pthread_cond_broadcast(&account->c);
	pthread_mutex_unlock(&account->wrl);

	pthread_mutex_lock(&bank_->wrl);
	bank_->read_count--;
	if(bank_->read_count == 0)
		pthread_cond_broadcast(&bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}

//**************************************************************************************
// function name: check_balance
// Description: Checks the balance of the account. Locks the bank for reading and the account for reading.
// Parameters: int id - The id of the account
// Parameters: string password - The password for the account
// Returns: void
//**************************************************************************************
void ATM::check_balance(int id, string password){

	pthread_mutex_lock(&bank_->wrl);
	while(bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->read_count++;
	pthread_mutex_unlock(&bank_->wrl);

	Account *account = bank_->get_account(id);

	if(account == NULL){
		bank_->output_log << "Error " << id_ << ": Your transaction failed – account id " << id <<" does not exist" << endl;

		pthread_mutex_lock(&bank_->wrl);
		bank_->read_count--;
		if(bank_->read_count == 0)
			pthread_cond_broadcast(&bank_->c);
		pthread_mutex_unlock(&bank_->wrl);

		return;
	}

	pthread_mutex_lock(&account->wrl);
	while(account->write_flag)
		pthread_cond_wait(&account->c, &account->wrl);
	account->read_count++;
	pthread_mutex_unlock(&account->wrl);

	if (!account->check_password(password)) {
		bank_->output_log << "Error " << id_ << ": Your transaction failed – password for account id " << id << " is incorrect" << endl;
	} else {
		bank_->output_log << id_ << ": Account " << id << " balance is " << account->get_balance() << endl;
		usleep(SEC);
	}

	pthread_mutex_lock(&account->wrl);
	account->read_count--;
	if(account->read_count == 0)
		pthread_cond_broadcast(&account->c);
	pthread_mutex_unlock(&account->wrl);

	pthread_mutex_lock(&bank_->wrl);
	bank_->read_count--;
	if(bank_->read_count == 0)
		pthread_cond_broadcast(&bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}

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
void ATM::transfer_money(int id, string password, int target_id, int amount){

	//Do nothing if they are the same account
	if (id == target_id)
		return;

	pthread_mutex_lock(&bank_->wrl);
	while(bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->write_flag = true;
	while(bank_->read_count > 0)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	pthread_mutex_unlock(&bank_->wrl);

	Account *account = bank_->get_account(id);
	if(account == NULL){
		bank_->output_log << "Error " << id_ << ": Your transaction failed – account id " << id <<" does not exist" << endl;

		pthread_mutex_lock(&bank_->wrl);
		bank_->write_flag = false;
		pthread_cond_broadcast(&bank_->c);
		pthread_mutex_unlock(&bank_->wrl);
		return;
	}

	Account *target_account = bank_->get_account(target_id);
	if (target_account == NULL) {
		bank_->output_log << "Error " << id_ << ": Your transaction failed – account id " << target_id <<" does not exist" << endl;

		pthread_mutex_lock(&bank_->wrl);
		bank_->write_flag = false;
		pthread_cond_broadcast(&bank_->c);
		pthread_mutex_unlock(&bank_->wrl);
		return;
	}

	pthread_mutex_lock(&account->wrl);
	while(account->write_flag)
		pthread_cond_wait(&account->c, &account->wrl);
	account->write_flag = true;
	while(account->read_count > 0)
		pthread_cond_wait(&account->c, &account->wrl);
	pthread_mutex_unlock(&account->wrl);

	pthread_mutex_lock(&target_account->wrl);
	while(target_account->write_flag)
		pthread_cond_wait(&target_account->c, &target_account->wrl);
	target_account->write_flag = true;
	while(target_account->read_count > 0)
		pthread_cond_wait(&target_account->c, &target_account->wrl);
	pthread_mutex_unlock(&target_account->wrl);

	 if (!account->check_password(password)) {
		bank_->output_log << "Error " << id_ << ": Your transaction failed – password for account id " << id << " is incorrect" << endl;
	} else if (account->get_balance() < amount) {
		bank_->output_log << "Error " <<id_ << ": Your transaction failed – account id " << id << " balance is lower than " << amount << endl;
	} else {
		*account -= amount;
		*target_account += amount;
		bank_->output_log << id_ << ">: Transfer " << amount << " from account " << id << " to account " << target_id;
		bank_->output_log << " new account balance is " << account->get_balance() << " new target account balance is " << target_account->get_balance() << endl;
		usleep(SEC);
	}

	pthread_mutex_lock(&target_account->wrl);
	target_account->write_flag = false;
	pthread_cond_broadcast(&target_account->c);
	pthread_mutex_unlock(&target_account->wrl);

	pthread_mutex_lock(&account->wrl);
	account->write_flag = false;
	pthread_cond_broadcast(&account->c);
	pthread_mutex_unlock(&account->wrl);

	pthread_mutex_lock(&bank_->wrl);
	bank_->write_flag = false;
	pthread_cond_broadcast(&bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}
