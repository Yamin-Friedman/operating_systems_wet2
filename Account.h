
#ifndef WET2_ACCOUNT_H
#define WET2_ACCOUNT_H

#include <cstring>
#include <pthread.h>
#include <string>


class Account {
private:
	int id_;
	std::string password_;
	int balance_;
	bool VIP_;
public:
	Account(int id, std::string password, int balance, bool VIP): id_(id),balance_(balance),VIP_(VIP){
		password_ = password;
		pthread_mutex_init(&wrl, NULL);
		pthread_cond_init(&c, NULL);
		read_count = 0;
		write_flag = false;
	}
	~Account(){
		pthread_mutex_destroy(&wrl);
		pthread_cond_destroy(&c);
	};
	int get_ID() const {
		return id_;
	}
	int get_balance() const {
		return balance_;
	}

	std::string get_password() const {
		return password_;
	}

	bool check_password(std::string password) const {
		if(password == password_)
			return true;
		return false;
	}
	bool is_VIP() const {
		return VIP_;
	}
	void change_VIP_status(bool is_VIP){
		VIP_ = is_VIP;
	}
	Account &operator+=(int &rhs){
		balance_ += rhs;
		return *this;
	}

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
