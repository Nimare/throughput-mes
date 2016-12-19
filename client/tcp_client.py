#!/usr/bin/env python
import socket

class TcpClient:
    def __init__(self, socket=None):
        if socket is None:
            self.sock = socket.socket(socket.AF_INET,
                                      socket.SOCK_STREAM)
        else:
            self.sock = socket

    def connect(self, host, port):
        self.sock.connect(host, port)

    def send(message):
        self.sock.send(message)

    def recv():
        
