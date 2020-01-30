#!/bin/sh

cc main.c 
echo "TEST KOPIOWANIE PLIKU Z DYSKU WIRTUALNEGO NA MINIXA:"
echo -e "test4.txt\ntest4.txt\ntest4kopia.txt" | ./a.out dyskwirtest 12000 

