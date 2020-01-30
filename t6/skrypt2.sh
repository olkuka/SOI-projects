#!/bin/sh

cc main.c 
echo "TEST USUNIECIE PLIKU:"
echo -e "test1.txt\ntest2.txt\ntest3.txt\ntest4.txt\ntest2.txt\ntestx.txt\n" | ./a.out dyskwirtest 12000 

