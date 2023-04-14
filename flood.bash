#!/bin/bash

password="PASS abc\r\n"
nick='NICK baba\r\n'
user='USER BabaVanga caca caca :Baba Vanga la voyeuse\r\n'

msg=$password$nick$user

echo -ne $msg

for i in {1..20}
do
    nick="NICK baba"$i"D\r\n"
    msg=$password$nick$user
    echo -ne $msg | nc -C localhost 6667 &
done