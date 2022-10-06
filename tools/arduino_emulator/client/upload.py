from tunnel import Tunnel
import sys
import tqdm

tunnel = Tunnel(sys.argv[1])

with open(sys.argv[2], 'rb') as f:
    data = f.read()
    for i in tqdm.tqdm(range(0, len(data))):
        tunnel.command(f"WRITE {i} {data[i]}")

    for i in tqdm.tqdm(range(0, len(data))):
        if int(tunnel.command(f"READ {i}")) != data[i]:
            print(f"--- ERROR --- Verification at {hex(i)} failed!")
            exit(0)

tunnel.command("EXIT")
tunnel.port.close()
exit(0)
