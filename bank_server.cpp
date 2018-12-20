
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

    while ( getline (myfile,line) ){
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
    pthread_exit(NULL);
}

main(int argc,char **argv) {
    Bank bank;

    // Init ATMs and start threads

    // Start bank threads

    // Poll to see if the ATMs are done



}
