import socket
import struct
from pprint import pprint as pp

VISUALIZATION_ENGINE = 62000
KEYBOARD_EMULATOR = 62001
_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


def send_to_visualization_engine(data, target='127.0.0.1'):
    # print('sending', data)
    if type(data) != bytes:
        data = bytes(data, 'utf-8')
    _socket.sendto(data, (target, VISUALIZATION_ENGINE))

def send_to_keyboard_emulator(data, target='127.0.0.1'):
    if type(data) != bytes:
        data = bytes(data, 'utf-8')  
    _socket.sendto(data, (target, KEYBOARD_EMULATOR))

def generate_bytes_command(command_list: str) -> bytes:
    # 8 bits: w | a | s | d | e | _ | _ | _
    byte_command = 0b00000000
    if 'w' in command_list:
        byte_command = byte_command | 0b10000000
    if 'a' in command_list:
        byte_command = byte_command | 0b01000000
    if 's' in command_list:
        byte_command = byte_command | 0b00100000
    if 'd' in command_list:
        byte_command = byte_command | 0b00010000
    if 'e' in command_list:
        byte_command = byte_command | 0b00001000
    if ' ' in command_list:
        byte_command = byte_command | 0b00000100
    return struct.pack('B', byte_command)




def generate_bytes_packet(data: dict) -> bytes:
    ''' 
    Generates customized UDP packet in bytes using data of the following format:

    {
        'type': 1 (int),
        'dist': 1.0234 (float),
        'angle': 34.21 (float)
    }
    '''
    type_bytes = bytearray(struct.pack('I', data['type']))
    dist_bytes = bytearray(struct.pack('f', data['dist']))
    angle_bytes = bytearray(struct.pack('f', data['angle']))
    packet = b''.join([type_bytes, dist_bytes, angle_bytes])
    return packet


if __name__ == "__main__":
    sample_data = {
        'type': 0,
        'dist': 0,
        'angle': 0
    }

    print(generate_bytes_command('we'))
    # packet = bytes(12)

    # while(True):
    #     msg = input("Please input command: ")
    #     if msg == 'send':
    #         packet = generate_bytes_packet(sample_data)
    #         send_to_visualization_engine(packet)
    #     elif 'type' in msg:
    #         sample_data['type'] = int(msg.split()[1])
    #         pp(sample_data)
    #     elif 'dist' in msg:
    #         sample_data['dist'] = float(msg.split()[1])
    #         pp(sample_data)
    #     elif 'angle' in msg:
    #         sample_data['angle'] = float(msg.split()[1])
    #         pp(sample_data)
