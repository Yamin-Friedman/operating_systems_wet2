

#include "ATM.h"

void ATM::open_account(int id, char password[PASSWORD_LEN + 1], int init_balance){
	pthread_mutex_lock(&bank_->wrl);
	while(&bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->write_flag = true;
	while(bank_->read_count > 0)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	pthread_mutex_unlock(&bank_->wrl);

	if(bank_->get_account(id) != NULL){
		bank->output_log << "Error " << id_ << ": Your transaction failed – account with the same id exists" << endl;
	} else {
		Account *new_account = new Account(id,password,init_balance,false);
		bank_->insert_account(new_account);
		sleep(1);
		bank->output_log << id_ << ": New account id is " << id << " with password " << password << " and initial balance " << init_balance << endl;
	}

	pthread_mutex_lock(&bank_->wrl);
	bank_->write_flag = false;
	pthread_cond_broadcast(bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}

void ATM::make_VIP(int id, char password[PASSWORD_LEN + 1]){
	pthread_mutex_lock(&bank_->wrl);
	while(&bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->read_count++;
	pthread_mutex_unlock(&bank_->wrl);

	Account *account = bank_->get_account(id);

	pthread_mutex_lock(&account->wrl);
	while(&account->write_flag)
		pthread_cond_wait(&account->c, &account->wrl);
	account->write_flag = true;
	while(account->read_count > 0)
		pthread_cond_wait(&account->c, &account->wrl);
	pthread_mutex_unlock(&account->wrl);

	if (account == NULL || !account->check_password(password)) {
		bank->output_log << "Error " << id_ << ": Your transaction failed – password for account id " << id << " is incorrect" << endl;
	} else {
		account->change_VIP_status(true);
		sleep(1);
	}

	pthread_mutex_lock(&account->wrl);
	account->write_flag = false;
	pthread_cond_broadcast(account->c);
	pthread_mutex_unlock(&account->wrl);

	pthread_mutex_lock(&bank_->wrl);
	bank_->read_count--;
	if(bank_->read_count == 0)
		pthread_cond_broadcast(bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}

void ATM::deposit(int id, char password[PASSWORD_LEN + 1], int amount){
	pthread_mutex_lock(&bank_->wrl);
	while(&bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->read_count++;
	pthread_mutex_unlock(&bank_->wrl);

	Account *account = bank_->get_account(id);

	pthread_mutex_lock(&account->wrl);
	while(&account->write_flag)
		pthread_cond_wait(&account->c, &account->wrl);
	account->write_flag = true;
	while(account->read_count > 0)
		pthread_cond_wait(&account->c, &account->wrl);
	pthread_mutex_unlock(&account->wrl);

	if(account == NULL){
		bank->output_log << "Error " << id_ << ": Your transaction failed – account id " << id <<" does not exist" << endl;
	} else if(!account->check_password(password)){
		bank->output_log << "Error " << id_ << ": Your transaction failed – password for account id " << id << " is incorrect" << endl;
	} else {
		*account += amount;
		bank->output_log << id_ << ": Account " << id << " new balance is " << account->get_balance() << " after " << amount << " $ was deposited" << endl;
		sleep(1);
	}

	pthread_mutex_lock(&account->wrl);
	account->write_flag = false;
	pthread_cond_broadcast(account->c);
	pthread_mutex_unlock(&account->wrl);

	pthread_mutex_lock(&bank_->wrl);
	bank_->read_count--;
	if(bank_->read_count == 0)
		pthread_cond_broadcast(bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}

void ATM::withdrawl(int id, char password[PASSWORD_LEN + 1], int amount){
	pthread_mutex_lock(&bank_->wrl);
	while(&bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->read_count++;
	pthread_mutex_unlock(&bank_->wrl);

	Account *account = bank_->get_account(id);

	pthread_mutex_lock(&account->wrl);
	while(&account->write_flag)
		pthread_cond_wait(&account->c, &account->wrl);
	account->write_flag = true;
	while(account->read_count > 0)
		pthread_cond_wait(&account->c, &account->wrl);
	pthread_mutex_unlock(&account->wrl);

	if(account == NULL){
		bank->output_log << "Error " << id_ << ": Your transaction failed – account id " << id <<" does not exist" << endl;
	} else if (!account->check_password(password)) {
		bank->output_log << "Error " << id_ << ": Your transaction failed – password for account id " << id << " is incorrect" << endl;
	} else {
		try{
			*account -= amount;
			bank->output_log << id_ << ": Account " << id << " new balance is " << account->get_balance() << " after " << amount << " $ was withdraw" << endl;
		}
		catch (int){
			bank->output_log << "Error " << id_ << ": Your transaction failed – account id " << id <<" balance is lower than " << amount << endl;
		}
		sleep(1);
	}

	pthread_mutex_lock(&account->wrl);
	account->write_flag = false;
	pthread_cond_broadcast(account->c);
	pthread_mutex_unlock(&account->wrl);

	pthread_mutex_lock(&bank_->wrl);
	bank_->read_count--;
	if(bank_->read_count == 0)
		pthread_cond_broadcast(bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}

void ATM::check_balance(int id, char password[PASSWORD_LEN + 1]){

	pthread_mutex_lock(&bank_->wrl);
	while(&bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->read_count++;
	pthread_mutex_unlock(&bank_->wrl);

	Account *account = bank_->get_account(id);

	pthread_mutex_lock(&account->wrl);
	while(&account->write_flag)
		pthread_cond_wait(&account->c, &account->wrl);
	account->read_count++;
	pthread_mutex_unlock(&account->wrl);

	if(account == NULL){
		bank->output_log << "Error " << id_ << ": Your transaction failed – account id " << id <<" does not exist" << endl;
	} else if (!account->check_password(password)) {
		bank->output_log << "Error " << id_ << ": Your transaction failed – password for account id " << id << " is incorrect" << endl;
	} else {
		bank->output_log << id_ << ": Account " << id << " balance is " << account->get_balance() << endl;
		sleep(1);
	}

	pthread_mutex_lock(&account->wrl);
	account->read_count--;
	if(account->read_count == 0)
		pthread_cond_broadcast(bank_->c);
	pthread_mutex_unlock(&account->wrl);

	pthread_mutex_lock(&bank_->wrl);
	bank_->read_count--;
	if(bank_->read_count == 0)
		pthread_cond_broadcast(bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}

void ATM::transfer_money(int id, char password[PASSWORD_LEN + 1], int target_id, int amount){
	pthread_mutex_lock(&bank_->wrl);
	while(&bank_->write_flag)
		pthread_cond_wait(&bank_->c, &bank_->wrl);
	bank_->read_count++;
	pthread_mutex_unlock(&bank_->wrl);

	Account *account = bank_->get_account(id);

	pthread_mutex_lock(&account->wrl);
	while(&account->write_flag)
		pthread_cond_wait(&account->c, &account->wrl);
	account->write_flag = true;
	while(account->read_count > 0)
		pthread_cond_wait(&account->c, &account->wrl);
	pthread_mutex_unlock(&account->wrl);

	Account *target_account = bank_->get_account(target_id);

	pthread_mutex_lock(&target_account->wrl);
	while(&target_account->write_flag)
		pthread_cond_wait(&target_account->c, &target_account->wrl);
	target_account->write_flag = true;
	while(target_account->read_count > 0)
		pthread_cond_wait(&target_account->c, &target_account->wrl);
	pthread_mutex_unlock(&target_account->wrl);

	if(account == NULL){
		bank->output_log << "Error " << id_ << ": Your transaction failed – account id " << id <<" does not exist" << endl;
	} else if (!account->check_password(password)) {
		bank->output_log << "Error " << id_ << ": Your transaction failed – password for account id " << id << " is incorrect" << endl;
	} else if (target_account == NULL) {
		bank->output_log << "Error " << id_ << ": Your transaction failed – account id " << target_id <<" does not exist" << endl;
	} else if (account->get_balance() < amount) {
		bank->output_log << "Error " <<id_ << ": Your transaction failed – account id " << id << " balance is lower than " << amount << endl;
	} else {
		*account -= amount;
		*target_account += amount;
		bank->output_log << id_ << ">: Transfer " << amount << " from account " << id << " to account " << target_id;
		bank->output_log << " new account balance is " << account->get_balance() << " new target account balance is " << target_account->get_balance() << endl;
		sleep(1);
	}

	pthread_mutex_lock(&target_account->wrl);
	target_account->write_flag = false;
	pthread_cond_broadcast(target_account->c);
	pthread_mutex_unlock(&target_account->wrl);

	pthread_mutex_lock(&account->wrl);
	account->write_flag = false;
	pthread_cond_broadcast(account->c);
	pthread_mutex_unlock(&account->wrl);

	pthread_mutex_lock(&bank_->wrl);
	bank_->read_count--;
	if(bank_->read_count == 0)
		pthread_cond_broadcast(bank_->c);
	pthread_mutex_unlock(&bank_->wrl);
}
