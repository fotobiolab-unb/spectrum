from nmap import PortScanner
import requests
import argparse
import pandas as pd
import os
import re
from urllib.parse import urljoin
from pssh.clients import ParallelSSHClient
from pssh.clients import SSHClient

path = f"{os.getcwd()}/*.ino"
print(path)

match_level = re.compile(r"level=(.*?) ")
match_time  = re.compile(r"time=\"(.*?)\"")
match_msg   = re.compile(r"msg=\"(.*?)\"")

def get_servers(net,port="5000"):
    port_scanner = PortScanner()
    results = port_scanner.scan(net,port,"--open",timeout=30)
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

def print_log(rows):
    for row in rows:
        level = match_level.search(row)
        level = "NONE" if level is None else level.group(0)
        msg   = match_msg.search(row)
        msg   = "NONE" if msg is None else msg.group(0)
        print(f"[{level}]",msg)

def run(input,clients,servers):
    for i,client in clients.items():
        print("Board",servers.loc[i])
        out = client.run_command(input)
    for row in out.stdout:
        yield(row)

if __name__=="__main__":
    #Arguments
    parser = argparse.ArgumentParser(description="Reactor scan")
    parser.add_argument("--network", help="Network to be scanned (ex: 192.168.0.1/24)")
    parser.add_argument("--log_level", default="info")
    args = parser.parse_args()

    #Server search via nmap
    servers = pd.DataFrame(get_servers(args.network))
    servers = servers.sort_values(by="id")
    clients = {row.id:SSHClient(row.addr,user="pi") for i,row in servers.iterrows()}
    servers.drop_duplicates(subset=['id'],inplace=True)
    servers.set_index('id',inplace=True)

    #Auto edit densidades file
    densidades = pd.read_csv("densidades.txt",sep="\t",index_col=False,header=None)

    print("Copying files")
    for i,client in clients.items():
        print("Copying to",servers.loc[i])
        os.system(f"sed -i \"s/ID = .*;/ID = {i};/\" Spectrum2.ino")
        #densidade = str(densidades.iloc[int(i-1),0])
        #os.system(f"sed -i \"s@/\*#\*/DensidadeAtual = .*;@/\*#\*/DensidadeAtual = {densidade}@\" h_Densidade.ino")
        client.copy_file(local_file=".",remote_file="/home/pi/Spectrum2",recurse=True)
    
    print("Compiling")
    client = ParallelSSHClient(servers.addr.to_list(),user="pi")
    client.run_command("rm /home/pi/arduino-*.log")

    compile_input = f"""\
        cd /home/pi/Spectrum2;\
        /home/pi/bin/arduino-cli compile --log-file /home/pi/arduino-cli-compile.log --log-level {args.log_level} --fqbn arduino:avr:mega Spectrum2;\
        tail /home/pi/arduino-cli-compile.log"""
    compile_cmd = run(compile_input,clients,servers)
    print_log(compile_cmd)

    print("Uploading")
    upload_input = f"""\
        cd /home/pi/Spectrum2;\
        /home/pi/bin/arduino-cli upload --log-file /home/pi/arduino-cli-upload.log --log-level {args.log_level} --fqbn arduino:avr:mega -t -p {servers.loc[i].serial_port} Spectrum2;\
        tail /home/pi/arduino-cli-upload.log"""
    upload_cmd = run(upload_input,clients,servers)
    print_log(upload_cmd)
