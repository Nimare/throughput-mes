#!/usr/bin/env python3
import tcp_client
import measure
import sys
import warnings

CLIENT_SEND = 0
CLIENT_RECV = 1
STREAM_ENDMARK = bytes.fromhex('ff')

def main():
    client = tcp_client.TcpClient()
    try:
        client.connect('localhost', 55555)
    except OSError as e:
        print(e.strerror, "Errno:", e.errno)
        client.close()
        sys.exit(1)
    length = 10000000
    while True:
        mode = input("Measure (D)ownlink, (U)plink or (E)xit? Please press one alternative:")
        if mode == 'D' or mode == 'd':
            measure_downlink(client, length)
        elif mode == 'U' or mode == 'u':
            measure_uplink(client, length)
        elif mode == 'E' or mode == 'e':
            break
        else:
            continue
    client.close()
    

def measure_downlink(client, num):
    mes = measure.MeasureTime()
    client.send_control_word(CLIENT_RECV,
    num)
    mes.start()
    client.recv(num)
    end_mark = client .recv(1)
    mes.stop()
    if (end_mark != STREAM_ENDMARK):
        raise RuntimeError("Stream endmark not matching")
    print("Time of upload %f", mes.get())
    print("Approximate uplink speed is ", num/mes.get(), " bytes/sec")

def measure_uplink(client, num):
    mes = measure.MeasureTime()
    client.send_control_word(CLIENT_SEND,
    num)
    mes.start()
    client.send(bytes(num))
    end_mark = client .recv(1)
    mes.stop()
    if (end_mark != STREAM_ENDMARK):
        raise RuntimeError("Stream endmark not matching %d", end_mark)
    print("Time of upload", mes.get())
    print("Approximate uplink speed is ", num/mes.get(), " bytes/sec")

if __name__ == "__main__":
    main()
