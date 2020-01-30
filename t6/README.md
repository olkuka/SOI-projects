**Polecenie:**

System plików.


**Testowanie:**

Napisałam 4 skrypty testowe, które sprawdzają poprawność działania wirtualnego dysku. Testy należy uruchamiać w następujący sposób: w pliku main.c przejść do funkcji main() i odkomentować odpowiednią linijkę testującą, odpowiadającą skryptowi, który chce się uruchomić (przy każdej linijce jest napisana nazwa skryptu, który trzeba uruchomić). Pozostałe linijki trzeba zakomentować, a następnie uruchomić odpowiedni skrypt. Pliki, które będę próbowała dodać na dysk to: test1.txt, test2.txt, test3.txt, test4.txt (oraz w pierwszym teście main.c, ponieważ ma bardzo duży rozmiar). Dysk, który testuję nazywa się dyskwirtest i ma rozmiar 12000 bajtów.

TEST DODAWANIE PLIKÓW (sprawdzone dodanie pliku, który jest za duży, żeby się
zmieścić na dysku)

W celu obejrzenia tego testu należy uruchomić skrypt1.sh. W rezultacie otrzymamy informacje co po kolei robił wirtualny dysk i stosowny komunikat, że za duży plik (czyli main.c) nie mieści się na dysku. Zostaje wyświetlony katalog i mapa zajętości. Dodaję 4 pliki po kolei, następnie próbuję dodać 5, który okazuje się być za duży.

TEST USUWANIE PLIKÓW (sprawdzone usunięcie pliku, który nie istnieje)

Dodaję 4 pliki po kolei. Wyświetlam katalog. Później usuwam plik o nazwie test2.txt. Następnie próbuję usunąć plik o nazwie testx.txt, ale okazuję się, że on nie istnieje, więc wyświetla się stosowny komunikat. Na końcu znów wyświetlam komunikat i mapę zajętości.

TEST KOPIOWANIA PLIKU Z WIRTUALNEGO DYSKU NA MINIX 

W tym celu dodaję jeden plik test4.txt na vd, a później kopiuję ten sam plik z powrotem na Minix pod nazwą test4kopia.txt. Udało mi się naprawić swój wcześniejszy błąd i funkcja działa prawidłowo, co widać po wywołaniu cmp.

TEST USUWANIA WIRTUALNEGO DYSKU

W tym teście dodaję po kolei 4 pliki, wyświetlam katalog, a następnie usuwam cały dysk.
