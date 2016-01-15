# UDPMiddleMan.py
import socket
import time
import sys
import commands
import fcntl
import struct
sys.path.insert(0, '/usr/lib/python2.7/bridge/')
from bridgeclient import BridgeClient


def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15])
    )[20:24])

bridge = BridgeClient()

sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

sock.bind(('',3479))

while True :
        try:
                msg = sock.recvfrom(1024)
                print msg[1] #Get IP, Port
                sock2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
                sock2.connect((msg[1][0],3479))
                
                queryIp = str(msg[1][0])
                setIp = queryIp.split('.')
                del setIp[3]
                setwlan = []
                seteth = []
                a,b = commands.getstatusoutput('cat /sys/class/net/wlan0/operstate')
                if b=='up':
                    ipwlan = get_ip_address('wlan0')
                    setwlan = ipwlan.split('.')
                    del setwlan[3]
                a,b = commands.getstatusoutput('cat /sys/class/net/eth1/operstate')
                if b=='up':
                    iplan = get_ip_address('eth1')
                    seteth = iplan.split('.')
                    del seteth[3]
                
                print setIp, setwlan, seteth
                
                if setIp==setwlan:
                    sock2.sendto( bytes("{\"uid\":\"arduinozero\",\"DisplayName\":\"SamiDevice\",\"ip\":\""+get_ip_address('wlan0')+"\",\"type\":\"Transformer\"}"), (msg[1][0], 3479) )
                elif setIp==seteth:
                    sock2.sendto( bytes("{\"uid\":\"arduinozero\",\"DisplayName\":\"SamiDevice\",\"ip\":\""+get_ip_address('eth1')+"\",\"type\":\"Transformer\"}"), (msg[1][0], 3479) )
                sock2.close()
                bridge.put("to_arduino",msg[1][0]) #server ip address

        except Exception as err:
                print err
                pass
