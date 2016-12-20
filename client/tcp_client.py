#!/usr/bin/env python3
import socket
import os
import sys
import struct

class TcpClient:
    def __init__(self, sock=None):
        if sock is None:
            self.sock = socket.socket(socket.AF_INET,
                                      socket.SOCK_STREAM)
        else:            self.sock = sock

    def connect(self, host, port):
        self.sock.connect((host, port))

    def send(self, message):
        totalsent = 0
        while totalsent < len(message):
            sent = self.sock.send(message[totalsent:])
            if sent == 0:
                raise RuntimeError("socket connection broken")
            totalsent = totalsent + sent
        return totalsent

    def send_control_word(self,server_mode, length):
        control_word = bytes([server_mode]) + struct.pack('!I', length)
        sent = self.sock.send(control_word)
        if sent != 5:
            raise RuntimeError("socket connection broken")

        
    def recv(self, msg_len):
        chunks = []
        bytes_recd = 0
        while bytes_recd < msg_len:
            chunk = self.sock.recv(min(msg_len - bytes_recd, 4096))
            if chunk == '':
                raise RuntimeError("socket connection broken")
            chunks.append(chunk)
            bytes_recd = bytes_recd + len(chunk)
        return b''.join(chunks)
        
