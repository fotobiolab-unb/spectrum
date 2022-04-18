#while read p; do
#	echo $p
#	for file in "$1/$p/*.cpp"; do
#
#		echo $file
#		cat $file | grep "Serial.print"
#	done	
#done < libraries.txt

for i in $1/*/*.cpp; do
	echo "[FILE] $i"
	cat $i | grep "Serial.print"
done
