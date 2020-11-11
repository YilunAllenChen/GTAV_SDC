import socket

VISUALIZATION_ENGINE = 62000
KEYBOARD_EMULATOR = 62001
_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


def send_to_visualization_engine(data, target='127.0.0.1'):
    _socket.sendto(bytes(data, 'utf-8'), (target, VISUALIZATION_ENGINE))

def send_to_keyboard_emulator(data, target='127.0.0.1'):
    _socket.sendto(bytes(data, 'utf-8'), (target, VISUALIZATION_ENGINE))


if __name__ == "__main__":
    while(True):
        msg = input("Please input command:\n")
        send_to_visualization_engine(msg)