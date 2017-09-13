import socket
import sys

operator_dict = { '+': 'add', '-': 'sub', '*' : 'mul', '/' : 'div' }

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = int(sys.argv[1])
server_address = ('localhost', port)
print('connecting to %s port %s' % server_address)
sock.connect(server_address)

try:
    while True:
        input = raw_input()

        if input == 'close':
            sock.sendall(input)
            break

        first_member, operator, second_member = input.split()

        data = operator_dict[operator] + ' ' + first_member + ' ' + second_member

        sock.sendall(data)

        received_result = sock.recv(1024)

        print('response from server: %s' % received_result)

finally:
    print('closing socket')
    sock.close()
