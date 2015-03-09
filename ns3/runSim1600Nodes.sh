
FILES=./scratch/Net_1600_*

for f in $FILES
do

	echo $f
	#./waf
	./waf --run "topoTest --infile="$f" --outHopsFile=scratch/maxhopscmd_1600.txt --outTimeFile=scratch/maxtimecmd_1600.txt"
done
