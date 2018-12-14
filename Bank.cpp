//
// Created by Yamin on 12/12/2018.
//

#include "Bank.h"

Bank::Bank(){
	if (pthread_mutex_init(&account_map_write_lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return;
    }
}

bool Bank::insert_account(Account *account){
	int id = account->get_ID();
	if(account_map.find(id) == account_map.end()) {
		account_map[id] = account;
		return true;
	}
	return false;
}
