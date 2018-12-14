//
// Created by Yamin on 12/12/2018.
//

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
}

Bank::~Bank(){
	pthread_mutex_destroy(&wrl);
	pthread_cond_destroy(&c);
}

bool Bank::insert_account(Account *account){
	int id = account->get_ID();
	if(account_map.find(id) == account_map.end()) {
		account_map[id] = account;
		return true;
	}
	return false;
}
