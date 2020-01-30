/* © 2019 Aleksandra Kukawka */

#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
#include "monitor.h"

struct Message {
    std::string content;
    std::string user;
    int priority;
    Message* next;
    Message* prev;

    Message(std::string msg, std::string user, int p) : content(msg), user(user), priority(p), next(nullptr), prev(nullptr) {}

    // dodawanie wiadomosci PO podanej 
    static void insert_after(Message* first, Message* msg) {
        if (first == nullptr) {
            msg->next = nullptr;
            msg->prev = nullptr;
            first = msg;
        }
        else {
            msg->next = first->next;
            first->next = msg;
            msg->prev = first;
        }
    }

    // dodawanie wiadomosci PRZED podana
    static void insert_before(Message* first, Message *msg) {
        msg->next = first;
        first->prev = msg;
        msg->prev = nullptr;
    }

    // zwraca ilosc znakow w zawartosci wiadomosci 
    static int size(Message* msg) {
        return msg->content.size();
    }
};

class Queue: private Monitor {
private:
    int MAX_MSG;
    int size;
    Message* first;
    Message* last;
    Message *lastPriorityMsg;       // wskaznik na ostatnia dodana wiadomosc z priorytetem 1
    Condition empty;
    Condition full;
public:
    Queue(): Monitor(), MAX_MSG(5), size(0), first(nullptr), last(nullptr), lastPriorityMsg(nullptr), empty{},full{} {}

    // destruktor, ktory usuwa cala kolejke
    ~Queue() {
        if(size == 0) return;

        auto it = first;
        while (it->prev != nullptr) {
            delete it->prev;
            it = it->next;
        }
        delete last;
    }

    // zwraca aktualna ilosc wiadomosci w kolejce 
    int getSize() {
        return this->size;
    }

    int getMAX_MSG() {
        return this->MAX_MSG;
    }

    void send_message(Message* msg) {
        if (Message::size(msg) < 8 || Message::size(msg) > 64) {
            std::cout << "\nWiadomosc " << msg->user << " powinna zawierac od 8 do 64 znakow! Nie moge wlozyc jej do kolejki.\n\n";
            std::cout << " *** Aktualna liczba wiadomosci w buforze: " << size << std::endl;
            return;
        }

        enter(); 
        if(size == MAX_MSG) {
            std::cout << " *** " << msg->user << " zostal wstrzymany na zmiennej full\n";
            wait(full);
            std::cout << " *** " << msg->user << " zostal zostal wznowiony\n";
        }
        
        if(msg->priority == 1) {
            if (lastPriorityMsg != nullptr)
                Message::insert_after(lastPriorityMsg, msg);
            else {
                Message::insert_before(first, msg);
            }
            
            lastPriorityMsg = msg;

            if(lastPriorityMsg->prev == nullptr)
                first = lastPriorityMsg;

            if(lastPriorityMsg->next == nullptr)
                last = lastPriorityMsg;

            std::cout << " *** " << msg->user << " dodal nowa wiadomosc priorytetowa do bufora\n";
        }
        else {
            Message::insert_after(last, msg);

            last = msg;
            if(last->prev == nullptr)
                first = last;

            std::cout << " *** " << msg->user << " dodal nowa wiadomosc zwykla do bufora\n";
        }

        ++size;

        std::cout << " *** Aktualna liczba wiadomosci w buforze: " << size << std::endl;

        if(size == 1) signal(empty);

        leave();
    }

    Message* read_message() {
        enter();
        if(size == 0) {
            std::cout << " *** Reader zostal wstrzymany na zmiennej empty\n";
            wait(empty);
            std::cout << " *** Reader zostal wznowiony\n";
        }

        if(first == lastPriorityMsg)
            lastPriorityMsg = nullptr;
        
        Message* ptr = first;
        first = first->next;
        std::cout << " *** Reader pobral wiadomosc z bufora\n";

        if(first != nullptr)
            first->prev = nullptr;

       --size;

        std::cout << " *** Aktualna liczba wiadomosci w buforze: " << size << "\n";
        if(size == 0)
            first = lastPriorityMsg = last = nullptr;
        
        if(size == MAX_MSG - 1) signal(full);

        leave();
        return ptr;
    }

    // wypisuje cala kolejke - funkcja testowa 
    /*void print_queue() {
        auto it = first;
        while (it->next != nullptr) {
            std::cout <<(*it).user << (*it).content << "\n";
            it = it->next;
        }
    }*/
};

#endif // QUEUE_H
