import socket
import struct

VISUALIZATION_ENGINE = 62000
KEYBOARD_EMULATOR = 62001
_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


def send_to_visualization_engine(data, target='127.0.0.1'):
    print('sending',data)
    if type(data) != bytes:
        data = bytes(data, 'utf-8')
    _socket.sendto(data, (target, VISUALIZATION_ENGINE))



p_type = 1
p_dist = 1.03402
p_angle = 6.969696

packet = bytes(9)


def bytes_gen():
    type_bytes = bytearray(struct.pack('I',p_type))
    dist_bytes = bytearray(struct.pack('f',p_dist))
    angle_bytes = bytearray(struct.pack('f', p_angle))
    packet = b''.join([type_bytes, dist_bytes, angle_bytes])
    print("packet generated: {}, length is {} bytes".format(packet, len(packet)))
    return packet


if __name__ == "__main__":
    while(True):
        msg = input("Please input command:\n")
        if msg == 'send':
            send_to_visualization_engine(packet)
        else:
            packet = bytes_gen()
