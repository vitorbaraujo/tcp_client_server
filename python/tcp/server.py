import socket

def calculate(operation):
    op, first_member, second_member = operation.split()

    try:
        first_member = int(first_member)
    except:
        return 'first member is invalid'

    try:
        second_member = int(second_member)
    except:
        return 'second member is invalid'

    if op == 'add':
        return first_member + second_member
    elif op == 'sub':
        return first_member - second_member
    elif op == 'mul':
        return first_member * second_member
    elif op == 'div':
        try:
            return first_member / second_member
        except:
            return 'invalid division'
    else:
        return 'invalid operation'

# socket initialization
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 10002
server_address = ('localhost', port)
print('starting up on %s port %s' % server_address)

sock.bind(server_address)
sock.listen(1)

while True:
    # wait for a connection
    print('waiting for a connection')
    connection, client_address = sock.accept()

    try:
        print('connection from', client_address)

        while True:
            data = connection.recv(1024)

            print('received [%s]' % data)

            if data == 'close':
                break
            else:
                connection.sendall(str(calculate(data)))
    finally:
        # Clean up the connection
        connection.close()
