#!/usr/bin/env python # 
"""
Test WebServices HTTP Server
Usage: first-server
"""


from flask import Flask
from datetime import datetime
import json
import socket
import random


app = Flask(__name__, static_url_path='/static')

@app.route('/', methods=['GET', 'POST'])
def root():
    return "<HTML><BODY><H1>Flask Test Page</H1></BODY?</HTML>"


@app.route('/time', methods=['GET', 'POST'])
def time():
    a = datetime.now()
    t = {
        'year': a.year,
        'month': a.month,
        'day': a.day,
        'hour': a.hour,
        'minute': a.minute,
        'second': a.second
        }

    return json.dumps(t)


# Tasks
# 0 'Name'
# 1 'Id'
# 2 'Current Priorty'
# 3'Base Priorty'
# 4 'High Water Mark'
# 5 'Runtime Counter'

tasks = [
        ["Task A", 1, 10, 10, 256,  0],
        ["Task B", 2, 10, 10, 300,  0],
        ["Task C", 3, 10, 10, 500,  0],
        ["Task D", 4, 10, 10, 800,  0],
        ["Task E", 5, 10, 10, 100,  0]
        ]

@app.route('/metrics/taskStats', methods=['GET', 'POST'])
def taskStats():
    for l in tasks:
        l[5] = l[5] + random.randint(0, 100)

        l[4] = l[4] - random.randint(0, 10)
        if (l[4] < 0):
            l[4] = 0
                

    return json.dumps(tasks)

# Heap
# 0 Avl in Bytes
# 1 Free Blocks
# 2 Malloc
# 3 Free

heap = [2048, 1, 0, 0]
@app.route('/metrics/heapStats', methods=['GET', 'POST'])
def heapStats():
    heap[0] = heap[0]- random.randint(0, 100)
    if (heap[0] < 0):
        heap[0] = 0
    
    m = random.randint(0, 5)
    f = random.randint(0, 5)
    if (f > m):
        f = m
    heap[2] = heap[2] + m
    heap[3] = heap[3] + f
                
    return json.dumps(heap)

@app.route('/metrics/msgs', methods=['GET', 'POST'])
def msgs():
    m = []
    
    count = random.randint(0, 2)
    
    if count > 0:
        for i in range(count):
            m.append("Test Message %d"%(i))
                
    return json.dumps(m)




if __name__ == "__main__":
    app.secret_key = 'DRJONEA'
    app.config['SESSION_TYPE'] = 'filesystem'
    

    hostname = socket.gethostname()
    ip_address = socket.gethostbyname(hostname)
    print(f"Hostname: {hostname}")
    print(f"IP Address: {ip_address}")
    
    app.run(host="0.0.0.0", port=8080)