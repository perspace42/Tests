'''
Author: Scott Field
Date: 3/21/2025
Version: 1.0
Purpose: 
Test TCP/IP Connection with Python
'''


import socket
import sys

'''Constants'''
PORT = 1247
BUFFER = 1024

'''Connect to Host'''
def connect(host: str):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((host, PORT))
    print(sock.recv(BUFFER).decode())
    inpt = input('Enter a message to be sent\n:')
    sock.send(inpt.encode())
    print("Message sent")


if __name__ == "__main__":
    host: str
    if len(sys.argv) < 2:
        host = input("Enter the host IP Address\n:")
    else:
        host = sys.argv[1]

    connect(host)
       