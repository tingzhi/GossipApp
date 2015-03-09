#example for 11 file output
python scratch/analysis.py scratch/maxtimecmd.txt scratch/maxhopscmd.txt > output_100.txt
#python scratch/analysis.py scratch/maxtimecmd_250.txt scratch/maxhopscmd_250.txt > output_250.txt

#example for 1 file output
# rm output.txt #remove the old output or it will grow out of control
# "For 100 nodes" >> output.txt
#python scratch/analysis.py scratch/maxtimecmd.txt scratch/maxhopscmd.txt >> output.txt
# "For 250 nodes" >> output.txt
#python scratch/analysis.py scratch/maxtimecmd_250.txt scratch/maxhopscmd_250.txt >> output.txt
