from tunnel import Tunnel
import sys

tunnel = Tunnel(sys.argv[1])
tunnel.command("FORMAT")
