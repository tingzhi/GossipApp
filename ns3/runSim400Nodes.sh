
FILES=./scratch/Net_400_*

for f in $FILES
do

	echo $f
	#./waf
	./waf --run "topoTest --infile="$f" --outHopsFile=scratch/maxhopscmd_400.txt --outTimeFile=scratch/maxtimecmd_400.txt"
done
