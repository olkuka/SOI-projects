**Polecenie:**

**Utworzenie usługi systemowej, która przyjmie jeden argument (np.: liczba 32-bitowa) i ten argument w swojej implementacji dodaje do numeru procesu ją wywołującego. Szczególną uwagę zwrócić na sposób sprawdzenia czy rozwiązanie faktycznie działa.**

   Rekompilacja jądra:
   
      cd /usr/src/tools
      make hdboot
      cd
      shutdown
      boot
  
   Uruchomienie programu testującego:
   
      cd /SOI_lab/t1
      cc test.c -o ./test
      ./test

**Uwaga** W folderze znajdują się wszystkie pliki, które zostały zmienione na potrzeby polecenia w systemie Minix 2.0.
