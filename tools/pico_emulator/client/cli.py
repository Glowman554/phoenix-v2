from tunnel import Tunnel
import sys

tunnel = Tunnel(sys.argv[1])
while True:
	print(tunnel.command(input("> ")))
