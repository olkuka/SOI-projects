**Polecenie:**

Napisz usluge "chat" dajaca uzytkownikom mozliwosc komunikacji miedzy soba.
Zaloz, ze istnieja trzy grupy uzytkownikow tej uslugi:
- administratorzy,
- uzytkownicy VIP,
- zwykli uzytkownicy.
Uzytkownicy VIP maja podwyzszone prawa i ich wiadomosci "wskakuja" na poczatek kolejki rozsylanych wiadomosci do pozostalych uzytkownikow. Administratorzy w celach prewencyjnych (np.: po przeczytaniu wulgarnegokomunikatu) moga w dowolnie wybranym przez siebie monecie wyczyscic cala kolejke wiadomosci.
Zaloz, ze wszyscy uzytkownicy komunikuja sie w wylacznie jednym temacie.
Przemysl metode testowania powstalego systemu, w szczegolnosci zwroc uwage na pokazanie w dzialaniu rownoczesnego dzialania wszystkich grup uzytkownikow.
Zwroc uwage, ze test powinien wykonywac sie automatycznie - bez udzialu czlowieka podczas samego testu.
Generowanie fikcyjnych tekstowych wiadomosci jest tutaj jak najbardziej wskazane.


**Testowanie:**

Testy uruchamiać poprzez odkomentowanie jednego z nich w funkcji main().

    • test_zwyklychuzytkownikow() - uruchamiam wątki 4 użytkowników (2 razy user1 i 2 razy user5), czekam na ich zakońćzenie oraz uruchamiam wątek admina, aby sprawdzić, czy nie ma przekleństwa i wyświetlić wiadomości na wyjście standardowe

    • test_1VIP() - uruchamiam wątki 4 użytkowników, w tym 1 VIPa, czekam na ich zakońćzenie oraz uruchamiam wątek admina, aby wyświetlić wiadomości na wyjście standardowe

    • test_2VIP() - uruchamiam wątki 5 użytkowników, w tym 2 VIPów, czekam na ich zakońćzenie oraz uruchamiam wątek admina, aby wyświetlić wiadomości na wyjście standardowe. VIP, który uruchomi się jako drugi, wskakuje przed VIPa, który uruchomił się jako pierwszy.

    • test_brzydkieslowo() - uruchamiam wątki 5 użytkowników, w tym jednego, który wysyła wiadomość z brzydkim słowem, czekam na ich zakońćzenie oraz uruchamiam wątek admina, aby wyświetlić wiadomości na wyjście standardowe. Admin odrzuca wszystkie wiadomości, ponieważ w jednej z nich było brzydkie słowo. W wyniku tego na wyjście standardowe nie zostaje wyświetlona żadna wiadomość.
    
    • test_przepelnieniebufora() - uruchamiam wątki 10 użytkowników. 5 z nich wchodzi do kolejki, a kolejne 5 nie może. Po 5 sekundach (zgodnie z implementacją sem_timedwait(…, …) program wypisuje stosowny komunikat, że nie da się dodać kolejnych procesów do kolejki, gdyż jest ona przepełniona, a nie został uruchomiony admin, który czyści kolejkę. 

    • test_czytaniepustegobufora() - uruchamiam wątek admina. Po 5 sekundach (zgodnie z implementacją funkcji sem_timedwait(…,…) program wypisuje stosowny komunikat, że nie da się czytać wiadomości z kolejki, bo jest ona pusta.
