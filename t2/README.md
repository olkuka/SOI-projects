**Polecenie:**

**1. Podzielić procesy użytkownika na dwie grupy: A – gdy identyfikator procesu jest podzielny bez reszty przez 2, B – gdy identyfikator procesu nie jest
podzielny bez reszty przez 2.
2. Wykonać niezbędne modyfikacje funkcji systemowych umożliwiające przenoszenie procesów między tymi grupami.
3. Wykonać usługę systemową o prototypie int set_scheduler(int x), która ustali proporcje czasowe, w jakich scheduler ma wybierać zadania A i B do wykonania. X należy do przedziału [0; 100]. A – x% czasu, B – (100-x)% czasu. Usługa zwraca 0, gdy udało się wykonać zmianę proporcji czasowych, a -1 – gdy proces został przydzielony do grupy B, a procesom tej grupy nie można zmieniać proporcji czasowych.
4. Opracować metodę weryfikacji poprawności rozwiązania.**

Utworzyłam dwa programy testujące i skrypt.
• Program time_test.c:
Użytkownik uruchamia program z argumentem – kwantem czasu, który chce przydzielić procesowi z grupy A (B dostaje odpowiednio 100 minus ten kwant). Program informuje czy proces jest z grupy A. Jeśli tak – zmienia kwant czasu. Jeśli nie (czyli jeśli jest z grupy B) – nie zmienia.
• Program test.c:
Główny program testujący. Zawiera obciążającą pętle (z pierwiastkowaniem) i mierzy czas wykonywania się procesu. Wyświetla pid procesu oraz czas w jakim on się wykonał.
• Skrypt runtests:
Wywołuje dwa programy test.c współbieżnie w tle.

