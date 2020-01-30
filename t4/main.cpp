/* © 2019 Aleksandra Kukawka */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <pthread.h>
#include "queue.h"
#include "monitor.h"

#define MAX_USR 10

Queue messages;     // monitor wiadomosci         
std::vector<std::string> examplesMsg;       // losowe wiadomosci rozsylane przez userow (A-Z)
std::vector<std::string> examplesUsr;       // losowe numery dla userow (1-10)
int i_m, i_u;       // liczba aktualnych wiadomosci i userow

// wczytanie z plikow i losowanie wiadomosci oraz userow, przez ktorych zostaly one wyslane 
void generate() {
    srand(time(NULL));
    std::string txt;
    std::ifstream in1, in2;
    in1.open("examples_of_messages.txt");
    in2.open("examples_of_users.txt");
    for (size_t i = 0; i < 26; i++) {
        std::getline(in1, txt);
        examplesMsg.push_back(txt);
    }
    std::random_shuffle(examplesMsg.begin(), examplesMsg.end());
    for (size_t i = 0; i < 10; i++) {
        std::getline(in2, txt);
        examplesUsr.push_back(txt);
    }
    std::random_shuffle(examplesUsr.begin(), examplesUsr.end());
    in1.close();
    in2.close();
}

// rozeslanie wiadomosci przez wszystkich userow
void* userx(void *param) {
	while(i_u < 10) {
    	Message* msg = new Message(examplesMsg[i_m], examplesUsr[i_u], std::rand() % 2);    
    	i_m++; i_u++;
        messages.send_message(msg);
	}
	return nullptr;
}

// rozeslanie wiadomosci TYLKO zwyklych przez wszystkich userow
void* userxz(void *param) {
	while(i_u < 10) {
    	Message* msg = new Message(examplesMsg[i_m], examplesUsr[i_u], 0);    
    	messages.send_message(msg);
    	i_m++; i_u++;
	}
	return nullptr;
}

// rozeslanie wiadomosci tylko przez 5 userow (5 to maksymalna ilosc wiadomosci w kolejce)
void* userMAX(void *param) {
	while(i_u < messages.getMAX_MSG()) {
    	Message* msg = new Message(examplesMsg[i_m], examplesUsr[i_u], std::rand() % 2);    
    	messages.send_message(msg);
    	i_m++; i_u++;
	}
	return nullptr;
}

// przeczytanie wiadomosci z bufora przez readera
void* readerx(void* param) {
    Message* read;
	//int size = messages.getSize();
    //for (int i = 0; i < size; i++) {
	while(1) {
    	read = messages.read_message();
    	std::cout << "\n" << read->user << ", priorytet " << read->priority << ": " << read->content << "\n\n";
    	delete read;
    }

    return nullptr;
}

// dodawanie wiadomosci wszystkich uzytkownikow (wlacznie z priorytetem 1) az do przepelnienia bufora
void test_VIP() {
    i_m = 0;
    i_u = 0;
    pthread_t users;
    pthread_create(&users, NULL, &userx, NULL);
    sleep(4);
}

// dodawanie wiadomosci tylko zwyklych uzytkownikow az do przepelnienia bufora
void test_uzytkownikow() {
    i_m = 0;
    i_u = 0;
    pthread_t users;
    pthread_create(&users, NULL, &userxz, NULL);
    sleep(4);
}

// przeczytanie zawartosci calego bufora przez readera 
void test_readera() {
    i_m = 0;
    i_u = 0;
    pthread_t users;
    pthread_create(&users, NULL, &userMAX, NULL);
    sleep(4);
    pthread_t reader;
	int r = 1000;
    pthread_create(&reader, NULL, &readerx, &r);
    sleep(5);
    pthread_join(users, NULL);
    pthread_cancel(reader);
}

// test na przepelnienie bufora, ktory jest po prostu testem uzytkownikow 
void test_przepelniony() {
    test_VIP();
}

// ogolny test pracy calego programu
void test() {
    i_m = 0;
    i_u = 0;
    pthread_t users;
    pthread_create(&users, NULL, &userx, NULL);
    sleep(4);
    pthread_t reader;
	int r = 1000;
    pthread_create(&reader, NULL, &readerx, &r);
    sleep(4);
    pthread_join(users, NULL);
}

// test, ktory jest proba czytania przez readera z pustego bufora
void test_pusty() {
    pthread_t reader;
	int r = 1000;
    pthread_create(&reader, NULL, &readerx, &r);
    sleep(4);
}

int main() {

    generate();

    /* testowanie odbywa sie poprzez
        odkomentowanie JEDNEJ z funkcji testujacych */
    
    //test_uzytkownikow();
    //test_VIP();
    test_readera();
    //test_pusty();
    //test_przepelniony();
    //test();
    return 0;
}