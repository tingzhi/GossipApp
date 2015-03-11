
FILES=./scratch/Net_1450_*

for f in $FILES
do

	echo $f
	#./waf
	./waf --run "topoTest --infile="$f" --outHopsFile=scratch/maxhopscmd_1450.txt --outTimeFile=scratch/maxtimecmd_1450.txt"
done
