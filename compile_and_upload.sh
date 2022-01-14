ARD="/dev/ttyACM0
/dev/ttyUSB0
/dev/ttyUSB1
/dev/ttyUSB2
/dev/ttyUSB3
/dev/ttyUSB4
/dev/ttyUSB5"

i=0

for ard in $ARD
do
    i=$(($i+1))
    echo $i $ard
    sed -i "s/ID = .*;/ID = $i;/" Spectrum.ino
    densidade=$(sed "${i}q;d" densidades.txt)
    echo "${densidade}"
    sed -i "s@DensidadeAtual = .*;@DensidadeAtual = ${densidade}@" h_Densidade.ino
    arduino-cli compile --fqbn arduino:avr:mega Spectrum
    arduino-cli upload --verbose -t -p $ard --fqbn arduino:avr:mega Spectrum
done