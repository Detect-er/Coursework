from socket import *

HOST = '192.168.137.1'
PORT = 4567
BUFSIZ =1024
ADDR = (HOST,PORT)

tcpCliSock = socket(AF_INET,SOCK_STREAM)
tcpCliSock.connect(ADDR)
while True:
     data1 = "client2 need info"
     if not data1:
         break
     tcpCliSock.send(data1.encode())
     data1 = tcpCliSock.recv(BUFSIZ)
     if data1:
         print(data1.decode('utf-8'))
         break
tcpCliSock.close()
