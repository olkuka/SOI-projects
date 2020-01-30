#!/bin/sh

cc main.c 
echo "USUNIECIE WIRTUALNEGO DYSKU:"
echo -e "test1.txt\ntest2.txt\ntest3.txt\ntest4.txt" | ./a.out dyskwirtest 12000 

