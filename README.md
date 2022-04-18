# Spectrum

Arduino code to control the Fotobiolab's bioreactors.

# Compiling

## Over a network

The script `build.py` allows this code to be compiled and uploaded to Arduino boards connected to a HTTP server over the network, provided that they have all necessary Arduino libraries and `arduino-cli`.

To compile the code on a network `192.168.1.1/24`, do:

    python3 build.py --network 192.168.1.1/24

Instead, if you wish to upload the code only for a single host, say, 192.168.1.103:

    python3 build.py --network 192.168.1.103

By default, the script will prompt Arduino's CLI to save a log file for both the compile and upload processes in the home directory. In order to change the log level, just pass it with the flag `--log_level`. For example:

    python3 build.py --network 192.168.1.1/24 --log_level info

The allowed log levels are the same as in the Arduino CLI: trace, debug, info, warn, error, fatal, panic

## Over SSH

Since each reactors has an open SSH port, the code can be compiled in the local machine and then be uploaded via SFTP:

    arduino-cli --build-path <COMPILE DIRECTORY> --fqbn arduino:avr:mega Spectrum
    cd <COMPILE DIRECTORY>
    sftp pi@<IP>
    cd Spectrum
    put *
    exit

    ssh pi@<IP>
    cd Spectrum
    arduino-cli upload --verbose --fqbn arduino:avr:mega --input-dir . -t -p /dev/ttyUSB0 Spectrum