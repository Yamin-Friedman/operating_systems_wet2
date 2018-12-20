
#include "Bank.h"
#include "ATM.h"
#include "Account.h"
#include <vector>
#include <unistd.h>

using namespace std;


//Function for ATM threads
void *ATM_run_commands(void *ATM_pointer) {
    ATM *ATM = (*ATM)ATM_pointer;
    string line;
    string delim = " ";
    string command;
    int id;
    string password;

    while ( getline (ATM->myfile,line) ){
        vector<string> args;
        istringstream stream(line);
        string arg;

        while (getline(stream, arg, delim)) {
            args.push_back(arg);
        }

        command = args[0];

        switch(command) {
            case "O" :
                id = stoi(args[1]);
                password = args[2];
                int init_balance = stoi(args[3]);
                ATM->open_account(id,password,init_balance);
                break;
            case "L" :
                id = stoi(args[1]);
                password = args[2];
                ATM->make_VIP(id,password);
                break;
            case "D" :
                id = stoi(args[1]);
                password = args[2];
                int amount = stoi(args[3]);
                ATM->deposit(id,password,amount);
                break;
            case "W" :
                id = stoi(args[1]);
                password = args[2];
                int amount = stoi(args[3]);
                ATM->withdrawl(id,password,amount);
                break;
            case "B" :
                id = stoi(args[1]);
                password = args[2];
                ATM->check_balance(id,password);
                break;
            case "T" :
                id = stoi(args[1]);
                password = args[2];
                int target_id = stoi(args[3]);
                int amount = stoi(args[4]);
                ATM->transfer_money(id,password,target_id,amount);
                break;
        }
        usleep(100);
    }
    ATM->input_file.close();
    delete ATM;
    pthread_exit(NULL);
}

// This is the function that the thread that charges commisions runs
void *bank_charge_commisions(void *bank_ptr){
    Bank *bank = (*Bank)bank_ptr;
    while (bank->bank_open){
        bank->charge_commision();
        sleep(3);
    }
    pthread_exit(NULL);
}

// This is the function that the thread that prints the status runs
void *bank_print_status(void *bank_ptr){
    Bank *bank = (*Bank)bank_ptr;
    while (bank->bank_open){
        bank->print_status();
        usleep(500);
    }
    pthread_exit(NULL);
}

main(int argc,char **argv) {
    Bank bank;
    int i, res;


    // Check input

    int num_ATM = stoi(argv[0]);
    bank.output_log.open("log.txt");

    // Init ATMs and start threads
    pthread_t threads[num_ATM + 2];
    for (i = 0; i < num_ATM; i++) {
        new_ATM = new ATM(ATM_id,&bank);
        string input = argv[i + 1];
        new_ATM->input_file.open(input);
        res = pthread_create(&threads[i],NULL, ATM_run_commands, (void*)new_ATM);

        if (res) {
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }

    // Start bank threads
    res = pthread_create(&threads[i],NULL, bank_charge_commisions, (void*)&bank);
    if (res) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }
    res = pthread_create(&threads[i + 1],NULL, bank_print_status, (void*)&bank);
    if (res) {
        cout << "Error:unable to create thread," << rc << endl;
        exit(-1);
    }

    // wait until the ATMs are done
    for (i = 0; i < num_ATM; i++){
        pthread_join(threads[i],NULL);
    }
    bank.bank_open = false;
    pthread_join(threads[i],NULL);
    pthread_join(threads[i + 1],NULL);

    bank.output_log.close();

}
