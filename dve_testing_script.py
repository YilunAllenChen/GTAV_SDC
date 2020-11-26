from udp_utils import send_to_visualization_engine, generate_bytes_packet
from time import sleep
from random import random


while(True):
    data = generate_bytes_packet({
                                'type': 1,
                                'dist': 10.0,
                                'angle': 3.1415926
                            })
    send_to_visualization_engine(data)
    sleep(1)