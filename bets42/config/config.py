#!/usr/bin/env python

"""
Run script for trading engines running built upon the 42bets trading framework. 

The script connects to a trading engine via it's admin port (TCP) and then 
executes a serious of admin commands described in the provided config file; 
the config file is a Python module which describes how a particular algo 
should be configured.

By using a Python module to configure the engine far more control can be had 
over say a static XML file. The resultant configuration is also in general 
shorter and easier understood.
"""

import importlib
import logging
import optparse
import os
import socket
import sys

SCRIPT_NAME = os.path.basename(__file__)
VERSION = SCRIPT_NAME + '0.1'

class TCPCommandSender:
    """Wrapper around TCP client for sending commands"""
    def __init__(self, host, port):
        """Setup connection to host:port"""
        logging.info("Connecting to server %s:%s", host, port)

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
           logging.info("Disconnecting from server %s:%s", self.__host, self.__port)
           self.__socket.close()

    def send(self, cmd):
        """Send admin command to server and return received message"""
        try:
            logging.info("[SENDING] %s", cmd)
            self.__socket.sendall(cmd)
            data = self.__socket.recv(4096)
            logging.info("[RECEIVED] %s", data)
            return data
        except EnvironmentError, msg:
            logging.critical("Failed to send admin cmd; reason=%s", msg)
            return "ERROR"

def main():
    #customise logging
    logging.basicConfig(filename='/tmp/' + SCRIPT_NAME + '.log', 
                        filemode='a',
                        format='%(asctime)s %(levelname)s %(message)s',
                        datefmt='%Y-%m-%d %H:%M:%S',
                        level=logging.INFO) 

    logging.info("Starting %s", SCRIPT_NAME)

    #define command line arguments
    parser = optparse.OptionParser("Usage: %prog [options]", version=VERSION)
    parser.add_option('--host', dest='host', help='Server to send commands to', type='string')
    parser.add_option('--port', dest='port', help='Server admin port to send commands to', type='int')
    parser.add_option('--config', dest='config', help='Python configuration module to configure algo', metavar='FILE')
    (options, args) = parser.parse_args()

    #check we have all required command line arguments
    if options.host and options.port and options.config:
        try:
            #create TCP connection
            with TCPCommandSender(options.host, options.port) as cmd_sender:

                logging.info("Running configuration module %s", options.config)

                #load a Python module to perform algo specific configuration
                try:
                    importlib.import_module(options.config).configure(logging.getLogger(), cmd_sender)
                except ImportError, msg:
                    logging.critical("Failed to import configuration module %s; reason=%s", options.config, msg)
                    sys.exit(1)
                except EnvironmentError, msg:
                    logging.critical("Network error while running configuration module %s; reason=%s", options.config, msg)
                    sys.exit(1)

                logging.info("Finished configuration module %s", options.config)

        except EnvironmentError, msg:
            logging.critical("Failed to connect to server %s:%s; reason=%s", options.host, options.port, msg)
            sys.exit(1)
    else:
        parser.error("Incorrect number/set of arguments provided, see --help")  

    logging.info("Ending %s", SCRIPT_NAME)

if __name__ == "__main__":
    main()

