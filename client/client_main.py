#!/usr/bin/env python3
import tcp_client
import socket
import sys

CLIENT_SEND = 0
CLIENT_RECV = 1
STREAM_ENDMARK = bytes.fromhex('ff')

def main():
    client = tcp_client.TcpClient()
    try:
        client.connect('localhost', 55555)
    except OSError as e:
        print(e.strerror, "Errno:", e.errno)
        self.sock.close()
        sys.exit(1)
    length = 10000
    while True:
        mode = input("Measure (D)ownlink, (U)plink or (E)xit? Please press one alternative:")
        if (mode == 'D' or mode == 'd'):
            measure_downlink(client, length)
        elif (mode == 'U' or mode == 'u'):
            measure_uplink(client, length)
        elif (mode == 'E' or mode == 'e'):
            break
        else:
            continue
    sent = client.send(bytes(length))
    print("Bytes sent:", sent)
    

def measure_downlink(client, num):
    client.send_control_word(CLIENT_RECV,
    num)
    client.recv(num)
    end_mark = client .recv(1)
    if (end_mark != STREAM_ENDMARK):
        raise RuntimeWarning("Stream endmark not matching")


def measure_uplink(client, num):
    client.send_control_word(CLIENT_SEND,
    num)
    client.send(bytes(num))
    end_mark = client .recv(1)
    if (end_mark != STREAM_ENDMARK):
        raise RuntimeWarning("Stream endmark not matching %d", end_mark)

if __name__ == "__main__":
    main()
