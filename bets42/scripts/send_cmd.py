#!/usr/bin/env python

"""Send a single command to a 42bets server

Connects to the command port of a server and sends a command, outputting 
whatever message is received in response"""

import optparse
import os
import socket
import sys

class TCPCommandSender:
    """Wrapper around TCP client for sending commands"""
    def __init__(self, host, port):
        """Setup connection to host:port"""
        self.__host = host
        self.__port = port

        self.__socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__socket.connect((host, port))

    def __enter__(self):
        """Return self"""
        return self

    def __exit__(self, type, value, traceback):
        """Tear down connection if connected"""
        if self.__socket:
           self.__socket.close()

    def send(self, cmd):
        """Send admin command to server and return received message"""
        try:
            self.__socket.sendall(cmd)
            data = self.__socket.recv(4096)
            return data
        except EnvironmentError, msg:
            return "ERROR"

def main():
    #define command line arguments
    parser = optparse.OptionParser("Usage: %prog [options]")
    parser.add_option('--host', dest='host', help='Server to send commands to', type='string')
    parser.add_option('--port', dest='port', help='Server admin port to send commands to', type='int')
    parser.add_option('--command', dest='command', help='Command to send to server', type='string')
    (options, args) = parser.parse_args()

    #check we have all required command line arguments
    if options.host and options.port and options.command:
        try:
            with TCPCommandSender(options.host, options.port) as cmd_sender:
                sys.stdout.write(cmd_sender.send(options.command + '\n'))
        except EnvironmentError, msg:
            sys.stderr.write("Failed to connect to %s:%s; reason=%s" % (options.host, options.port, msg))
            sys.exit(1)
    else:
        sys.stderr.write(str(parser.print_help()))

if __name__ == "__main__":
    main()

