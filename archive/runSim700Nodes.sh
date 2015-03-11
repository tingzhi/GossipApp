
FILES=./scratch/Net_700_*

for f in $FILES
do

	echo $f
	#./waf
	./waf --run "topoTest --infile="$f" --outHopsFile=scratch/maxhopscmd_700.txt --outTimeFile=scratch/maxtimecmd_700.txt"
done
