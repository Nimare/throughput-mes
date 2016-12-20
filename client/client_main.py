#!/usr/bin/env python3
import tcp_client
import socket

CLIENT_SEND = 0
CLIENT_RECV = 1

def main():
    client = tcp_client.TcpClient()
    try:
        client.connect('localhost', 55555)
    except OSError as e:
        print(e.strerror, "Errno:", e.errno)
        self.sock.close()
        sys.exit(-1)
    length = 29999
    client.send_control_word(CLIENT_SEND,
                                    length)
    sent = client.send(bytes(length))
    print("Bytes sent:", sent)
    
if __name__ == "__main__":
    main()
