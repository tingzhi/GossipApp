'''
   Copyright (c) 2015 Tingzhi Li, and Marco Falke, Kendall Bailey

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   Author: Tingzhi Li <vincentltz at gmail dot com>
  	   Marco Falke
  	   Kendall Bailey

'''

import sys

def ReadFileLines(filename):
	try:
	        f = open(filename)
	except IOError as e:
        	print "Problem opening file {2}. Error ({0}) : {1}".format(e.errno, e.strerror, filename)
        	sys.exit(2)

	lines = f.readlines()
	f.close
	return lines

def main():
	'''
	 Usage: python createCSV.py {prefix} {../path/to/simulationResults/*} > {prefix}RAW.csv"
	   Parse all given raw data files that start with 'prefix' (e.g. 'maxhops') and convert them to CSV usable by R"
	   Use at own risk: Input is not verified!
	'''

	prefix = sys.argv[1] + "_"
	suffix = ".txt"
	# print header
	print "#" + "nodes mean-of-" + prefix[:len(prefix)-4] + ",stdev"
	for arg in sys.argv:
		if prefix in arg:
			numOfNodes = arg[arg.index(prefix) + len(prefix): arg.index(suffix)]
			mean = 0
			counter = 0
			var = 0

			lines = ReadFileLines(arg)
			for line in lines:
				# calc mean
				counter = counter + 1
				mean = mean + float(line)
			mean = mean / counter
			for line in lines:
				var = var + pow(mean-float(line),2) #TODO numerically instable!
			var = var / counter
			stdev = pow(var,.5)
			# print data
			print numOfNodes + " " + str(mean) + " " + str(stdev)

if __name__ == '__main__':
	main()
