from udp_utils import send_to_visualization_engine, generate_bytes_packet
from time import sleep
from random import random


while(True):
    data = generate_bytes_packet({
                                'type': 1,
                                'dist': 500.0,
                                'angle': 45
                            })
    send_to_visualization_engine(data)
    sleep(1)