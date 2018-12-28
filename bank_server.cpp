
#include "Bank.h"
#include "ATM.h"
#include "Account.h"
#include <vector>
#include <unistd.h>
#include <string>
#include <iostream>
#include <sstream>
#include <pthread.h>

#define SEC 1000000

using namespace std;


//Function for ATM threads
void *ATM_run_commands(void *ATM_pointer) {
    ATM *atm = (ATM*)ATM_pointer;
    string line;
    string delim = " ";
    char command;
    int id;
	int amount;
	int target_id;
	int init_balance;
    string password;
    while ( getline(atm->input_file,line) ){
        vector<string> args;
	    string token;
	    istringstream ss(line);
	    if (line == "")
		    break;
	    while(getline(ss,token,' '))
	    {
		    args.push_back(token);
	    }

        command = args[0].front();

        switch(command) {
            case 'O' :
                id = stoi(args[1]);
                password = args[2];
                init_balance = stoi(args[3]);
		        atm->open_account(id,password,init_balance);
                break;
            case 'L' :
                id = stoi(args[1]);
                password = args[2];
		        atm->make_VIP(id,password);
                break;
            case 'D' :
                id = stoi(args[1]);
                password = args[2];
                amount = stoi(args[3]);
		        atm->deposit(id,password,amount);
                break;
            case 'W' :
                id = stoi(args[1]);
                password = args[2];
                amount = stoi(args[3]);
		        atm->withdrawl(id,password,amount);
                break;
            case 'B' :
                id = stoi(args[1]);
                password = args[2];
		        atm->check_balance(id,password);
                break;
            case 'T' :
                id = stoi(args[1]);
                password = args[2];
                target_id = stoi(args[3]);
                amount = stoi(args[4]);
		        atm->transfer_money(id,password,target_id,amount);
                break;
	        default:
		        break;
        }
        usleep(SEC/10);
    }
	atm->input_file.close();
    delete atm;
    pthread_exit(NULL);
}

// This is the function that the thread that charges commisions runs
void *bank_charge_commisions(void *bank_ptr){
    Bank *bank = (Bank*)bank_ptr;
    while (bank->bank_open){
        bank->charge_commision();
        usleep(SEC*3);
    }
    pthread_exit(NULL);
}

// This is the function that the thread that prints the status runs
void *bank_print_status(void *bank_ptr){
    Bank *bank = (Bank*)bank_ptr;
    while (bank->bank_open){
        bank->print_status();
        usleep(SEC/2);
    }
    pthread_exit(NULL);
}

int main(int argc,char **argv) {
    Bank bank;
    int i, res;


    // Check input
	if (argc < 2) {
		cerr << "illegal arguments" << endl;
		exit(-1);
	}
    int num_ATM = stoi(argv[1]);
	if (argc != num_ATM + 2) {
		cerr << "illegal arguments" << endl;
		exit(-1);
	}
    bank.output_log.open("log.txt",fstream::out);

    // Init ATMs and start threads
	pthread_t *threads;
    threads = new pthread_t[num_ATM + 2];
    for (i = 0; i < num_ATM; i++) {
	    string ATM_filename = argv[i + 2];
	    int ATM_id = i + 1;
        ATM *new_ATM = new ATM(ATM_id,&bank);
        new_ATM->input_file.open(ATM_filename);
        res = pthread_create(&threads[i],NULL, ATM_run_commands, (void*)new_ATM);

        if (res) {
	        cerr << "Error:unable to create thread," << endl;
            exit(-1);
        }
    }

    // Start bank threads
    res = pthread_create(&threads[i],NULL, bank_charge_commisions, (void*)&bank);
    if (res) {
        cerr << "Error:unable to create thread," << endl;
        exit(-1);
    }
    res = pthread_create(&threads[i + 1],NULL, bank_print_status, (void*)&bank);
    if (res) {
        cerr << "Error:unable to create thread," << endl;
        exit(-1);
    }

    // wait until the ATMs are done
    for (i = 0; i < num_ATM; i++){
        pthread_join(threads[i],NULL);
    }
    bank.bank_open = false;

	// wait for the bank threads to exit
    pthread_join(threads[i],NULL);
    pthread_join(threads[i + 1],NULL);


	delete[] threads;
    bank.output_log.close();

	return 0;

}
