**Polecenie:**

Zarządzanie pamięcią


**Testowanie:**

Programy i skrypt testujący, których używam, pochodzą ze strony: http://www.ia.pw.edu.pl/~tkruk/edu/soib/lab/t5.txt.
Na ekranie wypisywane są tylko rozmiary wolnych bloków pamięci. Do gotowego skryptu ze strony internetowej dodałam wyświetlanie wszystkich bloków pamięci przed jakąkolwiek alokacją zarówno dla algorytmu first fit, jak i worst fit.

F I R S T F I T

Widać, że proces potrzebował pamięci równej 9 kB, co jest spowodowane tym, że wartością chmem ustawiliśmy na 8000 bajtów, ale proces zużywa jeszcze trochę pamięci dla jakichś swoich celów, więc wychodzi trochę więcej. Proces szuka pierwszego wolnego bloku, który ma wystarczający rozmiar. Gdy go znajdzie – pobiera z niego kawałek pamięci, który jest mu potrzebny. I tak dzieje się 10 razy, zgodnie z pętlą w skrypcie testującym. Kiedy dochodzi do dealokacji dziury zaczynają się powiększać. Dlatego na końcu bloki pamięci są identyczne jak na początku.

W O R S T F I T

Dla tego algorytmu widać, że potrzebna pamięć jest brana z największego kawałka, czyli z tego na końcu. Dzieje się tak za każdym razem. Tutaj między kolejnymi alokacjami pamięci pojawiają się dodatkowe wolne bloki o rozmiarze 62, chociaż w first fit tak nie było. Dzieje się tak, ponieważ istnieje jakaś kernelowa struktura wielkości 62, która uruchamia się przy alokacji pamięci dla kolejnych procesów. Kiedy dochodzi do dealokacji dziury są konsolidowane, w rezultacie czego po całkowitej dealokacji dziury mają identyczny rozmiar, jak na początku (wystarczy porównać pierwszą i ostatnią linijkę).

**Uwaga** W folderze znajdują się wszystkie pliki, które zostały zmienione na potrzeby polecenia w systemie Minix 2.0.
