from socket import *

HOST = '192.168.137.1'
PORT = 4567
BUFSIZ =1024
ADDR = (HOST,PORT)

tcpCliSock = socket(AF_INET,SOCK_STREAM)
tcpCliSock.connect(ADDR)
while True:
     data2 = "client1 need info"
     if not data2:
         break
     tcpCliSock.send(data2.encode())
     data2 = tcpCliSock.recv(BUFSIZ)
     if data2:
         print(data2.decode('utf-8'))
         break
tcpCliSock.close()

