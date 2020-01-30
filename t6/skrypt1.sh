#!/bin/sh

cc main.c 
echo "TEST DODAWANIE PLIKOW:"
echo -e "test1.txt\ntest2.txt\ntest3.txt\ntest4.txt\nmain.c\n" | ./a.out dyskwirtest 12000 

