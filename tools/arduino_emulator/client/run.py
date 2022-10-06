from tunnel import Tunnel
import sys
import tqdm

tunnel = Tunnel(sys.argv[1])
tunnel.command("EXIT")
