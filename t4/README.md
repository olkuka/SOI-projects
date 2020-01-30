**Polecenie:**

Napisz w C++ dla środowiska Linux, system kolekcjonowania krótkich wiadomości tekstowych (maks. 64 znaków, ale nie mniej niż 8 znaków). System ma bazować na synchronizacji dostępu do zasobów wykorzystujący mechanizm monitorów.
Zadaniem budowanego systemu ma być niezawodne zbieranie od klientow wiadomosci, liczba klientów może być duża, ale system musi być gotowy do obsłużenia minimum 5 klientów.
Klienci - pojedynczy pod-proces lub wątek - 'wrzucają' wiadomości do systemu, oprócz samej treści wiadomości wybierają priorytet wrzucanej wiadomość (np.: 0 to zwykły, 1 to priorytetowy).
System może zbierać wiadomości tylko w jednym pojemnym buforze. Mechanizm wkładania nowych wiadomości do tego bufora musi uwzględniać priorytety.
Wszelkie operacje na buforze powinny być optymalizowane w taki sposob by nie kopiować niepotrzebnie wiadomości, oraz zapewnić by czas wkładania wiadomości oraz czas wyjmowania były możliwie jak najkrótsze.
Dodatkowo dla systemu utworzony ma być pod-proces lub wątek 'czytelnik' zebranych wiadomości. Jego zadaniem jest pobieranie z bufora i przedstawianie wiadomości tekstowych na konsoli tekstowej. Zakłada się, że 'czytelnik' będzie pobierał wiadomości z bufora, a w buforze wiadomości będą już poukładane zarówno względem priorytetów jak i czasu ich włożenia.
Przemyśl bardzo dokładnie metodę automatycznego testowania powstałego systemu. W szczególności zwróć uwagę na pokazanie równoczesnego działania wielu procesów (czy wątków) umieszczających wiadomości, w tym także z różnymi priorytetami oraz współdziałnie w tym czasie 'czytelnika'.
Załóż, że program testowy będzie działał automatycznie generując przez
klientów fikcyjne wiadomości wyłącznie tekstowe, a 'czytelnik' pokazywał je na konsoli.


**Testowanie:**

Testy należy uruchamiać poprzed odkomentowanie jednego z nich w funkcji main().

Cały program należy uruchamiać wpisując w linii komend:

g++ -pthread main.cpp

./a.out


Odczytywanie wyjścia standardowego:

• linie poprzedzone „***” - są to linie wypisujące podstawowe komentarze o operacjach, które w danym momencie dzieją się w programie.

• zwykłe linie, niepoprzedzone niczym – są to WIADOMOŚCI, które wyświetla reader.


• test_uzytkownikow() - dodaje wiadomości tylko zwykłych użytkowników, aż do przepełnienia bufora. Uruchamiam jeden wątek odpowiedzialny za rozsyłanie wiadomości przez użytkowników.

• test_VIP() - test taki sam jak powyżej, ale dodaje również wiadomości z priorytetem 1.
• test_readera() - uruchamiam wątek, który dodaje do bufora dokładnie tyle wiadomości, ile wynosi maksymalna jego pojemność. Następnie uruchamiam wątek readera. Trzeba zauważyć, że wyświetlanie wiadomości nie odbywa się od razu w funkcji read_message(), tylko poza nią. Dlatego patrząc na operacje i wyświetlane wiadomości można pomyśleć, że coś jest nie tak. Działa to w następujący sposób:

- reader pobiera wiadomość z bufora (komentarz)

- reader zmniejsza size o 1 i wyświetla (komentarz), ile jest aktualnych wiadomości w buforze.

- dopiero teraz ta wiadomość jest wyświetlana na wyjście standardowe.

• test_pusty() - uruchamiam tylko wątek readera, więc próbuje on przeczytać wiadomość z pustego bufora.

• test_przepelniony() - jest to dokładnie taki sam test, jak test_uzytkownikow() lub test_VIP().

• Test() - test ogólny sprawdzający poprawność działania całego programu dla następujących warunków:

◦ MAX_MSG = 5

◦ liczba wszystkich użytkowników – 10

◦ 1 reader
