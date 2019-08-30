# 앞선 echo server와 통신하기 위한 클라이언트
# host와 port의 값은 server의 설정값에 따라 가변적이다.
# server에 문자열을 보내고 수신하는 방식
# 리눅스 환경에서의 기본 client 보다 느림

import socket
client = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
host = '52.38.184.177'
port = 9000
client.connect((host,port))
while True:
    str = input()
    client.send(str.encode())
    data = client.recv(1024)
    print('data is : '+data.decode("utf-8"))
    if(str == '-1'):
        exit(1)
