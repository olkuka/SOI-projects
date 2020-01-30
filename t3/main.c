/* © 2019 Aleksandra Kukawka */

#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "queue.c"

// maksymalna liczba wiadomosci w kolejce 
#define MAX_MSG 5

#define DIRTY_WORD "przeklenstwo"

// wskaznik na pierwsza wiadomosc z kolejki
node* messages;

pthread_mutex_t sem_mutex;
sem_t sem_empty;
sem_t sem_full;

// zmienna informujaca czy uzytkownik jest VIPem (1) czy nie (0)
int VIP;

// funkcja uzywana do pobrania obecnego czasu
// po 5 sekundach, jezeli bufor jest przepelniony,
// zwraca wartosc -1 
// dotyczy funkcji userów
int sem_wait10secs() {

    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    tm.tv_sec += 5;
    return (sem_timedwait(&sem_empty, &tm));
}

// dotyczy funkcji admina
int sem_full_wait5secs() {
    struct timespec tm;
    clock_gettime(CLOCK_REALTIME, &tm);
    tm.tv_sec += 5;
    return (sem_timedwait(&sem_full, &tm));
}


void sendMessage(char* msg, int VIP) {
        if (messages == NULL) messages = init(msg);
        else {
            if (VIP == 0) {
                push_end(messages, msg);
            }
            else if (VIP == 1) {
                push_begin(&messages, msg);
            }
        }
}

// funkcja dla uzytkownika1, pozwalajaca na wyslanie wiadomosci
void *user1(void* param) {
    char* msg = "Jestem zwyklym uzytkownikiem user1!";
    sleep(rand() % 2 + 1);                              // uzytkownik dodaje wiadomosc losowo w czasie 1-2 sekundy
    if (sem_wait10secs() == 0) {
        pthread_mutex_lock(&sem_mutex);

        sendMessage(msg, 0);//) 
        printf("\t*** Dodaję wiadomość usera1 do kolejki. ***\n");

        pthread_mutex_unlock(&sem_mutex);
        sem_post(&sem_full);
    }
    else 
        printf("\t*** Nie mozna dodac wiadomosci usera1,\n\t*** poniewaz bufor od 5 sekund jest przepelniony!\n");
}

void *user2(void* param) {
    char* msg = "przeklenstwo";
    sleep(rand() % 2 + 1);
    if (sem_wait10secs() == 0) {
        pthread_mutex_lock(&sem_mutex);

        sendMessage(msg, 0);//) 
        printf("\t*** Dodaję wiadomość usera2 (przeklenstwo) do kolejki. ***\n");

        pthread_mutex_unlock(&sem_mutex);
        sem_post(&sem_full);
    }
    else 
        printf("\t*** Nie mozna dodac wiadomosci usera2,\n\t***  poniewaz bufor od 5 sekund jest przepelniony!\n");
}

void *user3(void* param) {
    char* msg = "Witam, tutaj uzytkownik VIP, czyli userVIP1";
    sleep(rand() % 2 + 1);
    if (sem_wait10secs() == 0) {
        pthread_mutex_lock(&sem_mutex);

        sendMessage(msg, 1);//) 
        printf("\t*** Dodaję wiadomość useraVIP1 do kolejki. ***\n");

        pthread_mutex_unlock(&sem_mutex);
        sem_post(&sem_full);
    }
    else 
        printf("\t*** Nie mozna dodac wiadomosci useraVIP1,\n\t*** poniewaz bufor od 5 sekund jest przepelniony!\n");
}

void *user4(void* param) {
    char* msg = "Czesc jestem VIPem - userVIP2";
    sleep(rand() % 2 + 1);
    if (sem_wait10secs() == 0) {
        pthread_mutex_lock(&sem_mutex);

        sendMessage(msg, 1);
        printf("\t*** Dodaję wiadomość useraVIP2 do kolejki. ***\n");

        pthread_mutex_unlock(&sem_mutex);
        sem_post(&sem_full);
    }
    else 
        printf("\t*** Nie mozna dodac wiadomosci useraVIP2,\n\t*** poniewaz bufor od 5 sekund jest przepelniony!\n");
}

void *user5(void* param) {
    char* msg = "Czesc jestem zwyklym uzytkownikiem - user5";
    sleep(rand() % 2 + 1);
    if (sem_wait10secs() == 0) {
    pthread_mutex_lock(&sem_mutex);

        sendMessage(msg, 0);
        printf("\t*** Dodaję wiadomość usera5 do kolejki. ***\n");

        pthread_mutex_unlock(&sem_mutex);
        sem_post(&sem_full);
    }
    else printf("\t*** Nie mozna dodac wiadomosci usera5,\n\t*** poniewaz bufor od 5 sekund jest przepelniony!\n");
}

void checkMessages(node* msg) {
        while (msg != NULL) {
            if(strcmp(msg->val, DIRTY_WORD) == 0) { 
                printf("ADMIN: Kolejka wiadomosci zostaje usunieta z powodu brzydkiego slowa!\n");
                remove_queue(&messages);
                return;
            }
            msg = msg->next;
        }
        printf("ADMIN: przeczytalem, wyswietlam wiadomosci.\n");
        print_queue(messages);
        remove_queue(&messages);
}

// funkcja dla admina pozwalajaca na usuniecie calej kolejki wiadomosci
void *admin( void* param) {

    if(sem_full_wait5secs() != -1) {
        pthread_mutex_lock(&sem_mutex);

        checkMessages(messages); 

        pthread_mutex_unlock(&sem_mutex);
        sem_post(&sem_empty);
    }
    else 
        printf("ADMIN: Nie udalo sie odczytac z bufora. Przez 5 sekund nie dostalem zadnej wiadomosci.\n");
}

void test_zwyklychuzytkownikow (void) {
    //tworzenie watkow admina i  2 uzytkownikow zwyklych
    pthread_t adminx;
    pthread_t user1x, user5x;

    pthread_create(&user1x, NULL, user1, NULL);
    pthread_create(&user5x, NULL, user5, NULL);
    pthread_create(&user1x, NULL, user1, NULL);
    pthread_create(&user5x, NULL, user5, NULL);

    pthread_join(user1x, NULL);
    pthread_join(user5x, NULL);
    pthread_join(user1x, NULL);
    pthread_join(user5x, NULL);

    pthread_create(&adminx, NULL, admin, NULL);
    pthread_join(adminx, NULL);
}

void test_1VIP(void) {
    pthread_t adminx;
    pthread_t user1x, user3x, user5x;               //uzytkownik 3 to VIP

    pthread_create(&user1x, NULL, user1, NULL);
    pthread_create(&user5x, NULL, user5, NULL);
    pthread_create(&user5x, NULL, user5, NULL);
    pthread_create(&user3x, NULL, user3, NULL);

    pthread_join(user1x, NULL);
    pthread_join(user5x, NULL);
    pthread_join(user5x, NULL);
    pthread_join(user3x, NULL);

    pthread_create(&adminx, NULL, admin, NULL);
    pthread_join(adminx, NULL);
}

void test_2VIP(void) {
    pthread_t adminx;
    pthread_t user1x, user3x, user4x, user5x;               //uzytkownik 3 i 4 to VIP

    pthread_create(&user1x, NULL, user1, NULL);
    pthread_create(&user5x, NULL, user5, NULL);
    pthread_create(&user5x, NULL, user5, NULL);
    pthread_create(&user3x, NULL, user3, NULL);
    pthread_create(&user4x, NULL, user4, NULL);

    pthread_join(user1x, NULL);
    pthread_join(user5x, NULL);
    pthread_join(user5x, NULL);
    pthread_join(user3x, NULL);
    pthread_join(user4x, NULL);

    pthread_create(&adminx, NULL, admin, NULL);
    pthread_join(adminx, NULL);
}

void test_brzydkieslowo(void) {
    pthread_t adminx;
    pthread_t user1x, user2x, user3x, user4x, user5x;               //uzytkownik 3 i 4 to VIP

    pthread_create(&user1x, NULL, user1, NULL);
    pthread_create(&user5x, NULL, user5, NULL);
    pthread_create(&user3x, NULL, user3, NULL);
    pthread_create(&user4x, NULL, user4, NULL);
    pthread_create(&user2x, NULL, user2, NULL);

    pthread_join(user1x, NULL);
    pthread_join(user5x, NULL);
    pthread_join(user3x, NULL);
    pthread_join(user4x, NULL);
    pthread_join(user2x, NULL);

    pthread_create(&adminx, NULL, admin, NULL);
    pthread_join(adminx, NULL);
}

void test_przepelnieniebufora(void) {

    pthread_t adminx;
    pthread_t user1x, user2x, user3x, user4x, user5x;               //uzytkownik 3 i 4 to VIP

    pthread_create(&user1x, NULL, user1, NULL);
    pthread_create(&user1x, NULL, user1, NULL);
    pthread_create(&user1x, NULL, user1, NULL);
    pthread_create(&user1x, NULL, user1, NULL);
    pthread_create(&user1x, NULL, user1, NULL);
    pthread_create(&user1x, NULL, user1, NULL);
    pthread_create(&user5x, NULL, user5, NULL);
    pthread_create(&user3x, NULL, user3, NULL);
    pthread_create(&user4x, NULL, user4, NULL);
    pthread_create(&user2x, NULL, user2, NULL);
    pthread_create(&user1x, NULL, user1, NULL);

    pthread_join(user1x, NULL);
    pthread_join(user1x, NULL);
    pthread_join(user1x, NULL);
    pthread_join(user1x, NULL);
    pthread_join(user1x, NULL);
    pthread_join(user1x, NULL);
    pthread_join(user5x, NULL);
    pthread_join(user3x, NULL);
    pthread_join(user4x, NULL);
    pthread_join(user2x, NULL);
}

void test_czytaniepustegobufora(void) {
    pthread_t adminx;
    pthread_create(&adminx, NULL, admin, NULL);
    pthread_join(adminx, NULL);
}
int main(int argc, char **argv) {

    pthread_mutex_init(&sem_mutex, NULL);
    sem_init(&sem_empty, 0, MAX_MSG);
    sem_init(&sem_full, 0, 0);

    //test_zwyklychuzytkownikow();
    //test_1VIP();
    //test_2VIP();
    //test_brzydkieslowo();
    //test_przepelnieniebufora();
    test_czytaniepustegobufora();

    return 0;

}
