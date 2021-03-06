
#include "Bank.h"

//**************************************************************************************
// function name: Bank
// Description: Constructor for a Bank
// Returns: void
//**************************************************************************************
Bank::Bank(){
	if (pthread_mutex_init(&wrl, NULL) != 0)
    {
        cerr << "\n mutex init failed\n" ;
        return;
    }
	if (pthread_cond_init(&c, NULL) != 0)
	{
		cerr << "\n cond init failed\n" ;
		return;
	}
	read_count = 0;
	write_flag = false;
	private_balance = 0;
	bank_open = true;

}

//**************************************************************************************
// function name: ~Bank
// Description: Deconstructor for a Bank
// Returns: void
//**************************************************************************************
Bank::~Bank(){
	pthread_mutex_destroy(&wrl);
	pthread_cond_destroy(&c);

	map<int, Account*>::iterator it = account_map.begin();
	for(; it != account_map.end(); it++) {
		delete (*it).second;
	}
}

//**************************************************************************************
// function name: print_status
// Description: Prints the current status of the bank to the screen
// Returns: void
//**************************************************************************************
void Bank::print_status(){
	pthread_mutex_lock(&wrl);
	while(write_flag)
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
		while(account->write_flag)
			pthread_cond_wait(&account->c, &account->wrl);
		account->read_count++;
		pthread_mutex_unlock(&account->wrl);

		cout << "Account " << account->get_ID() << ":: Balance – " << account->get_balance() << " $ , Account Password – " << account->get_password() << endl;

		pthread_mutex_lock(&account->wrl);
		account->read_count--;
		if(account->read_count == 0)
			pthread_cond_broadcast(&account->c);
		pthread_mutex_unlock(&account->wrl);
	}

	cout << "The Bank has " << private_balance << " $" << endl;

	pthread_mutex_lock(&wrl);
	read_count--;
	if(read_count == 0)
		pthread_cond_broadcast(&c);
	pthread_mutex_unlock(&wrl);
}

//**************************************************************************************
// function name: charge_commision
// Description: Charges a commision from all the non-VIP accounts in the bank
// Returns: void
//**************************************************************************************
void Bank::charge_commision(){
	int percent = rand() %  3 + 2; // get random number between 2 and 4

	pthread_mutex_lock(&wrl);
	while(write_flag)
		pthread_cond_wait(&c, &wrl);
	write_flag = true;
	while(read_count > 0)
		pthread_cond_wait(&c, &wrl);
	pthread_mutex_unlock(&wrl);

	map<int, Account*>::iterator it = account_map.begin();
	for(; it != account_map.end(); it++) {
		Account *account = (*it).second;

		pthread_mutex_lock(&account->wrl);
		while(account->write_flag)
			pthread_cond_wait(&account->c, &account->wrl);
		account->write_flag = true;
		while(account->read_count > 0)
			pthread_cond_wait(&account->c, &account->wrl);
		pthread_mutex_unlock(&account->wrl);

		if (!account->is_VIP()){
			int commision = (int)((account->get_balance() * percent )/ 100);
			*account -= commision;
			private_balance += commision;
			output_log << "Bank: commissions of " << percent << "% were charged, the bank gained " << commision << " $ from account " << account->get_ID() << endl;
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

//**************************************************************************************
// function name: insert_account
// Description: Inserts an account into the bank
// Parameters: Account *account - A pointer to the account to add
// Returns: bool
//**************************************************************************************
bool Bank::insert_account(Account *account){
	int id = account->get_ID();
	if(account_map.find(id) == account_map.end()) {
		account_map[id] = account;
		return true;
	}
	return false;
}
