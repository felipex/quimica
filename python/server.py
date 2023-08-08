import serial
from datetime import datetime

def write(f, line):
	with open(f, "a") as out:
		out.write(line)
		
def read_conf():
    config = {}
    with open("conf.ini") as f:
        line = f.readline().strip()
        while line:
            struct = line.split("=")
            if len(struct) == 2:
                config[struct[0].strip()]=struct[1].strip()

            line = f.readline().strip()

    return config


def read_serial(port):
	with serial.Serial(port) as conn:
		line = conn.readline();
		print(line)
		
		sdate = datetime.now().strftime("%Y%m%d_%H%M%S")
		leituras = "leituras"+sdate+".txt" 
		while line:
			line = conn.readline();
			print(line)
			write(leituras, line.decode())
		
####################

conf = read_conf()
print(conf)
read_serial(conf["porta"])
