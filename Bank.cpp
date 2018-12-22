
#include "Bank.h"

Bank::Bank(){
	if (pthread_mutex_init(&wrl, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return;
    }
	if (pthread_cond_init(&c, NULL) != 0)
	 	{
	      printf("\n cond init failed\n");
	      return;
	  }
		read_count = 0;
		write_flag = false;
		private_balance = 0;
		bank_open = true;
}

Bank::~Bank(){
	pthread_mutex_destroy(&wrl);
	pthread_cond_destroy(&c);

	// Might need to be fixed
	map<int, Account*>::iterator it = account_map.begin();
	for(; it != account_map.end(); it++) {
		delete (*it).second;
	}
}

void Bank::print_status(){
	pthread_mutex_lock(&wrl);
	while(&write_flag)
		pthread_cond_wait(&c, &wrl);
	read_count++;
	pthread_mutex_unlock(&wrl);

	printf("\e1;1H\e[2J");
	printf("\033[1;1H");
	cout << "Current Bank Status" << endl;

	map<int, Account*>::iterator it = account_map.begin();
	for(; it != account_map.end(); it++) {
		Account *account = (*it).second;

		pthread_mutex_lock(&account->wrl);
		while(&account->write_flag)
			pthread_cond_wait(&account->c, &account->wrl);
		account->read_count++;
		pthread_mutex_unlock(&account->wrl);

		cout << "Account " << account->get_ID() << ":: Balance – " << account->get_balance() << " $ , Account Password – " << account->get_password() << endl;

		pthread_mutex_lock(&account->wrl);
		account->read_count--;
		if(account->read_count == 0)
			pthread_cond_broadcast(&c);
		pthread_mutex_unlock(&account->wrl);
	}

	cout << "The Bank has " << private_balance << "$" << endl;

	pthread_mutex_lock(&wrl);
	read_count--;
	if(read_count == 0)
		pthread_cond_broadcast(&c);
	pthread_mutex_unlock(&wrl);
}

void Bank::charge_commision(){
	int percent = (rand() %  3) + 2; // get random number between 2 and 4
	cout << percent << endl; //debug print

	pthread_mutex_lock(&wrl);
	while(&write_flag)
		pthread_cond_wait(&c, &wrl);
	read_count++;
	pthread_mutex_unlock(&wrl);

	map<int, Account*>::iterator it = account_map.begin();
	for(; it != account_map.end(); it++) {
		Account *account = (*it).second;

		pthread_mutex_lock(&account->wrl);
		while(&account->write_flag)
			pthread_cond_wait(&account->c, &account->wrl);
		account->write_flag = true;
		while(account->read_count > 0)
			pthread_cond_wait(&account->c, &account->wrl);
		pthread_mutex_unlock(&account->wrl);

		if (!account->is_VIP()){
			int commision = (int)((account->get_balance() * percent )/ 100);
			*account -= commision;
			private_balance += commision;
			output_log << "Bank: commissions of " << percent << " % were charged, the bank gained " << commision << " $ from account " << account->get_ID() << endl;
		}

		pthread_mutex_lock(&account->wrl);
		account->write_flag = false;
		pthread_cond_broadcast(&account->c);
		pthread_mutex_unlock(&account->wrl);
	}

	pthread_mutex_lock(&wrl);
	write_flag = false;
	pthread_cond_broadcast(&c);
	pthread_mutex_unlock(&wrl);
}

bool Bank::insert_account(Account *account){
	int id = account->get_ID();
	if(account_map.find(id) == account_map.end()) {
		account_map[id] = account;
		return true;
	}
	return false;
}
