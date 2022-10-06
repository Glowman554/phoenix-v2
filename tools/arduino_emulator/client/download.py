from tunnel import Tunnel

import sys
import tqdm

tunnel = Tunnel(sys.argv[1])

with open(sys.argv[2], 'wb') as f:
    for i in tqdm.tqdm(range(0, 1024)):
        f.write(int(tunnel.command(f"READ {i}")).to_bytes(1, "little"))
