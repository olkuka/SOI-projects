**Polecenie:**

**Utworzenie usługi systemowej, która przyjmie jeden argument (np.: liczba 32-bitowa) i ten argument w swojej implementacji dodaje do numeru procesu ją wywołującego. Szczególną uwagę zwrócić na sposób sprawdzenia czy rozwiązanie faktycznie działa.**

    1. Dodanie 
       #define ADDNRTOPROC 78 
       i zmiana NCALLS na o jeden większy w pliku usr/include/minix/callnr.h.

    2. Dodanie procedury obługi 
       do_addnrtoproc do pliku usr/src/mm/main.c (plik w załączniku). 
       System call zwraca numer procesu, który go wywołuje powiększonego o pid, czyli liczbę, która została podana, jako
       argument (pid to mm_in.m1_i1).
       
    3. Umieszczenie prototypu funkcji w usr/src/mm/proto.h pod main.c, który wygląda tak:

	_PROTOTYPE( int do_addnrtoproc, (void)   );


   4. Dodanie w pliku usr/src/mm/table.c adresu funkcji:

	do_addnrtoproc, /*78*/

   5. Umieszczenie w pliku usr/src/fs/table.c pustego adresu funkcji: 
 
	no_sys, /*79*/

   6. Napisanie programu testującego (test.c – komentarze znajdują się w pliku).
   
   6. Rekompilacja jądra:
      1. cd /usr/src/tools
      2. make hdboot
      3. cd
      4. shutdown
      5. boot
  
   7. Uruchomienie programu testującego:
      1. cd /SOI_lab/t1
      2. cc test.c -o ./test
      3. ./test
