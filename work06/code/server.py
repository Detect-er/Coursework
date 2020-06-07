from socket import *
from time import ctime

HOST = '127.0.0.1'
PORT = 4567
BUFSIZ = 1024
ADDR = (HOST,PORT)
filename = "/mnt/ext4scull"

#1、创建服务端的socket对象
tcpSerSock = socket(AF_INET,SOCK_STREAM)
#2、绑定一个ip和端口
tcpSerSock.bind(ADDR)
#3、服务器端一直监听是否有客户端进行连接
tcpSerSock.listen(5)
while True:
    print('waiting for connection...')
    # 4、如果有客户端进行连接、则接受客户端的连接
    tcpCliSock, addr = tcpSerSock.accept() #返回客户端socket通信对象和客户端的ip
    print('...connnecting from:', addr)
    while True:
        # 5、客户端与服务端进行通信
        data = tcpCliSock.recv(BUFSIZ).decode()
        if not data:
            break
        print("From client: %s"%data)
        # 6、从filename文件中读取scull设备的信息
        with open(filename) as f:
            content = f.read()
            f.close()
        # 7、服务端给客户端回消息
        tcpCliSock.send(('the time is: [%s]\ntemperature is: %s\nhumidity is: %s' % (
        ctime(), content.split()[0], content.split()[1])).encode())
    # 8、关闭socket对象
    tcpCliSock.close()
tcpSerSock.close()
