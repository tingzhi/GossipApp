#example for 11 file output

python analysis.py maxtimecmd_100.txt maxhopscmd_100.txt > output_100.txt
python analysis.py maxtimecmd_250.txt maxhopscmd_250.txt > output_250.txt
python analysis.py maxtimecmd_400.txt maxhopscmd_250.txt > output_400.txt
python analysis.py maxtimecmd_550.txt maxhopscmd_250.txt > output_550.txt
python analysis.py maxtimecmd_700.txt maxhopscmd_250.txt > output_700.txt
python analysis.py maxtimecmd_850.txt maxhopscmd_250.txt > output_850.txt
python analysis.py maxtimecmd_1000.txt maxhopscmd_250.txt > output_1000.txt
#python analysis.py maxtimecmd_1150.txt maxhopscmd_250.txt > output_1150.txt
#python analysis.py maxtimecmd_1300.txt maxhopscmd_250.txt > output_1300.txt
#python analysis.py maxtimecmd_1450.txt maxhopscmd_250.txt > output_1450.txt
#python analysis.py maxtimecmd_1600.txt maxhopscmd_250.txt > output_1600.txt

#example for 1 file output
rm outputInOne.txt 	#remove the old output or it will grow out of control
#echo "For 100 nodes" >> outputInOne.txt
python analysis.py maxtimecmd_100.txt maxhopscmd_100.txt >> outputInOne.txt
#echo "For 250 nodes" >> outputInOne.txt
python analysis.py maxtimecmd_250.txt maxhopscmd_250.txt >> outputInOne.txt
#echo "For 400 nodes" >> outputInOne.txt
python analysis.py maxtimecmd_400.txt maxhopscmd_400.txt >> outputInOne.txt
#echo "For 550 nodes" >> outputInOne.txt
python analysis.py maxtimecmd_550.txt maxhopscmd_550.txt >> outputInOne.txt
#echo "For 700 nodes" >> outputInOne.txt
python analysis.py maxtimecmd_700.txt maxhopscmd_700.txt >> outputInOne.txt
#echo "For 850 nodes" >> outputInOne.txt
python analysis.py maxtimecmd_850.txt maxhopscmd_850.txt >> outputInOne.txt

#echo "For 1000 nodes" >> outputInOne.txt
python analysis.py maxtimecmd_1000.txt maxhopscmd_1000.txt >> outputInOne.txt
#echo "For 1150 nodes" >> outputInOne.txt
#python analysis.py maxtimecmd_1150.txt maxhopscmd_1150.txt >> outputInOne.txt
#echo "For 1300 nodes" >> outputInOne.txt
#python analysis.py maxtimecmd_1300.txt maxhopscmd_1300.txt >> outputInOne.txt
#echo "For 1450 nodes" >> outputInOne.txt
#python analysis.py maxtimecmd_1450.txt maxhopscmd_1450.txt >> outputInOne.txt
#echo "For 1600 nodes" >> outputInOne.txt
#python analysis.py maxtimecmd_1600.txt maxhopscmd_1600.txt >> outputInOne.txt
