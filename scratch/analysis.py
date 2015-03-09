'''
 *
 * Copyright (c) 2015 Tingzhi Li, and Marco Falke
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Tingzhi Li <vincentltz at gmail dot com>
 *	   Marco Falke
 *
'''

import sys
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt 
import matplotlib.patches as patches
import statsUtility

class result:
	def __init__ (self):
		self.expectation = 0
		self.median = 0
		self.stdev = 0
		self.var = 0
		self.mini = 0
		self.maxi = 0
		self.data = []

	def printResult(self, units):
		print "Average = {:.3f}{:s}".format(self.expectation,units) 
		print "Median = {:.3f}{:s}".format(self.median,units) 
		print "Standard Dev = {:.3f}{:s}".format(self.stdev,units)
		print "Variance = {:.3f}".format(self.var)
		print "Minimum = {:.3f}{:s}".format(self.mini,units) 
		print "Maximum = {:.3f}{:s}".format(self.maxi,units) 

	def printResultNoText(self):
		print "{:.3f},{:.3f}".format(self.expectation, self.stdev)

	def plotHistogram(self, numBin, xlab):
		fig, ax = plt.subplots()
		n, bins, patches = plt.hist(self.data, numBin, normed=0, facecolor='green', alpha=0.5)
		
		# get the corners of the histogram
		left = np.array(bins[:-1])
		right = np.array(bins[1:])
		bottom = np.zeros(len(left))
		top = bottom + n

		ax.set_xlim(left[0], right[-1])
		ax.set_ylim(bottom.min(), top.max()*1.2)

		plt.plot(bins)
		plt.xlabel(xlab)
		plt.ylabel('Frequency')
		plt.title('Distribution of ' + xlab)

		# Tweak spacing to prevent clipping of ylabel
		plt.subplots_adjust(left=0.15)
		plt.show()

def printCombinedResults(timeResults, hopResults):
	print "{:.3f}".format(timeResults.expectation),
	print ",",
        print "{:.3f}".format(timeResults.stdev),
	print ",",
	print "{:.3f}".format(hopResults.expectation),
	print ",",
	print "{:.3f}".format(hopResults.stdev)

#	print timeResults.printResultNoText()

def runStats(ls):
	ret = result()
	ret.mini = min(ls)
	ret.maxi = max(ls)
	ret.expectation = np.mean(ls)
	ret.var = np.var(ls)
	ret.median = np.median(ls)
	ret.stdev = np.std(ls)
	ret.data = ls
	return ret

def main():
	statsUtility.CheckArgs(2,"<time in file> <hops in file>")
	templs = statsUtility.ReadFileLines(sys.argv[1])
	spreadtime = []
	for el in templs:
		spreadtime.append(float(el))
	
	templs = statsUtility.ReadFileLines(sys.argv[2])
	spreadhops = []
	for el in templs:
		stripped = el.strip("\n")
		spreadhops.append(int(stripped))

	timeResult = runStats(spreadtime)
	hopResult = runStats(spreadhops)

#	print "Max Time Analysis Result"
#	timeResult.printResult("s")
#	print ""
	
#	print "Max Hops Analysis Result"
#	hopResult.printResult("")
	printCombinedResults(timeResult, hopResult)
#	timeResult.plotHistogram(int(timeResult.maxi/2)+1, "Max Time")
#	hopResult.plotHistogram(hopResult.maxi+1, "Max Hops")

if __name__ == '__main__':
	main()
