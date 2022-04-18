from nmap import PortScanner
import requests
import argparse
import pandas as pd
import os
from urllib.parse import urljoin
from pssh.clients import ParallelSSHClient
from pssh.clients import SSHClient
from gevent import joinall
from glob import glob

path = f"{os.getcwd()}/*.ino"
print(path)

def get_servers(net,port="5000"):
    port_scanner = PortScanner()
    results = port_scanner.scan(net,port,"--open",timeout=10)
    hosts = list(results["scan"].keys())
    servers = []
    for host in hosts:
        try:
            uri = f"http://{host}:{str(port)}"
            v = requests.get(urljoin(uri,"ping"),timeout=2)
            if v.ok:
                metadata = v.json()
                metadata["addr"] = host
                servers.append(metadata)
        except Exception as e:
            pass
    return servers

if __name__=="__main__":
    parser = argparse.ArgumentParser(description="Reactor scan")
    parser.add_argument("--network", help="Network to be scanned (ex: 192.168.0.1/24)")
    parser.add_argument("--log_level", default="info")

    args = parser.parse_args()

    servers = pd.DataFrame(get_servers(args.network))
    servers = servers.sort_values(by="id")

    densidades = pd.read_csv("densidades.txt",sep="\t",index_col=False,header=None)

    clients = {row.id:SSHClient(row.addr,user="pi") for i,row in servers.iterrows()}

    servers.drop_duplicates(subset=['id'],inplace=True)
    servers.set_index('id',inplace=True)

    print("Copying files")

    for i,client in clients.items():
        print(i)
        os.system(f"sed -i \"s/ID = .*;/ID = {i};/\" Spectrum.ino")
        densidade = str(densidades.iloc[int(i-1),0])
        os.system(f"sed -i \"s@/\*#\*/DensidadeAtual = .*;@/\*#\*/DensidadeAtual = {densidade}@\" h_Densidade.ino")
        #out = client.run_command("sudo rm -r /home/pi/Spectrum")
        #out = client.run_command("mkdir /home/pi/Spectrum; sudo chmod 0777 /home/pi/Spectrum")
        #for file in glob(path):
        #    client.scp_send(local_file=file,remote_file=os.path.join("/home/pi/Spectrum",os.path.basename(file)))
        client.copy_file(local_file=".",remote_file="/home/pi/Spectrum",recurse=True)
    
    print("Compiling")
    client = ParallelSSHClient(servers.addr.to_list(),user="pi")
    client.run_command("rm /home/pi/arduino-*.log")
    output = client.run_command(f"cd /home/pi/Spectrum; /home/pi/bin/arduino-cli compile --log-file /home/pi/arduino-cli-compile.log --log-level {args.log_level} --fqbn arduino:avr:mega Spectrum")
    print("Uploading to board")
    for i,client in clients.items():
        print("Board", servers.loc[i])
        out = client.run_command(f"cd /home/pi/Spectrum; /home/pi/bin/arduino-cli upload --log-file /home/pi/arduino-cli-upload.log --log-level {args.log_level} --fqbn arduino:avr:mega -t -p {servers.loc[i].serial_port} Spectrum")
        print(out)
        for row in out.stdout:
            print(row)
    print("Done")