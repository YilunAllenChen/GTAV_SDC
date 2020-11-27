from udp_utils import send_to_visualization_engine, generate_bytes_packet
from time import sleep
from random import random


while(True):
    for i in range(4):
        data = generate_bytes_packet({
                                    'type': 1 if i%2 == 0 else 3,
                                    'dist': 100*i,
                                    'angle': 20*i
                                })
        send_to_visualization_engine(data)
    sleep(1)