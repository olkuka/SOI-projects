**Polecenie:**

**Utworzenie usługi systemowej, która przyjmie jeden argument (np.: liczba 32-bitowa) i ten argument w swojej implementacji dodaje do numeru procesu ją wywołującego. Szczególną uwagę zwrócić na sposób sprawdzenia czy rozwiązanie faktycznie działa.**

   Rekompilacja jądra:
   
      1. cd /usr/src/tools
      2. make hdboot
      3. cd
      4. shutdown
      5. boot
  
   Uruchomienie programu testującego:
   
      1. cd /SOI_lab/t1
      2. cc test.c -o ./test
      3. ./test
