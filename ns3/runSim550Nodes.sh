
FILES=./scratch/Net_550_*

for f in $FILES
do

	echo $f
	#./waf
	./waf --run "topoTest --infile="$f" --outHopsFile=scratch/maxhopscmd_550.txt --outTimeFile=scratch/maxtimecmd_550.txt"
done
