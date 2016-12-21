#!/usr/bin/env python3
import tcp_client
import measure
import sys
import warnings
import argparse

CLIENT_SEND = 0
CLIENT_RECV = 1
STREAM_ENDMARK = bytes.fromhex('ff')
MAX_BYTES = pow(2,32)

def main():
    #parse_arguments()
    parser = argparse.ArgumentParser()
    parser.add_argument("-p",
                        "--port",
                        help="port number of the remote server",
                        type=int,
                        default=55555)
    parser.add_argument("server",
                        help="host or ip of the remote server")

    args = parser.parse_args()
    client = tcp_client.TcpClient()
    try:
        client.connect(args.server, args.port)
    except OSError as e:
        print(e.strerror, "Errno:", e.errno)
        client.close()
        sys.exit(1)
    length = 10000000
    while True:
        mode = input("Measure (D)ownlink, (U)plink or (E)xit? Please press one alternative:")
        if mode == 'D' or mode == 'd':
            length = prompt_number_of_bytes()
            measure_downlink(client, length)
        elif mode == 'U' or mode == 'u':
            length = prompt_number_of_bytes()
            measure_uplink(client, length)
        elif mode == 'E' or mode == 'e':
            break
        else:
            continue
    client.close()

def prompt_number_of_bytes():
    while True:
        ret = input("Please add the length of the measurement stream between 0 and 2^32:")
        if (ret.isdigit()):
            length = int(ret)
            if (length >= 0 and length <= MAX_BYTES):
                return length


def measure_downlink(client, num):
    mes = measure.MeasureTime()
    mes.start()
    client.send_control_word(CLIENT_RECV,
    num)
    client.recv(num)
    end_mark = client .recv(1)
    mes.stop()
    if (end_mark != STREAM_ENDMARK):
        raise RuntimeError("Stream endmark not matching")
    print("Time of download", mes.get())
    display_speed(num, mes.get(), "downlink")


def measure_uplink(client, num):
    mes = measure.MeasureTime()
    mes.start()
    client.send_control_word(CLIENT_SEND,
    num)
    client.send(bytes(num))
    end_mark = client .recv(1)
    mes.stop()
    if (end_mark != STREAM_ENDMARK):
        raise RuntimeError("Stream endmark not matching %d", end_mark)
    print("Time of upload", mes.get())
    display_speed(num, mes.get(), "uplink")


def display_speed(num_of_bytes, time, direction):
    speed = num_of_bytes / time
    magnitude = ['bytes',
                 'kilobytes',
                 'megabytes',
                 'gigabytes']
    for elem in list(enumerate(magnitude)):
        unit = elem[1]
        if (speed <1024):
            break;
        speed /= 1024

    print("Approximate", direction, "speed is", speed, unit,"/sec")



if __name__ == "__main__":
    main()
