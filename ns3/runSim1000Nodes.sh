
FILES=./scratch/Net_1000_*

for f in $FILES
do

	echo $f
	#./waf
	./waf --run "topoTest --infile="$f" --outHopsFile=scratch/maxhopscmd_1000.txt --outTimeFile=scratch/maxtimecmd_1000.txt"
done
