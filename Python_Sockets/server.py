import socket

PORT = 1247
BUFFER = 1024

sock = socket.socket()
host = socket.gethostname()
sock.bind((host,PORT))
sock.listen(5)

while True:
    returnSock, addr = sock.accept()
    print("Connection accepted from " + repr(addr[1]))

    returnSock.send("Server approved connection\n".encode())
    print(f"{repr(addr[1])} : {returnSock.recv(BUFFER).decode()}")
    returnSock.close()