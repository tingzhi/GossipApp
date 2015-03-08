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

import statsUtility
import sys
import numpy
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt 

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

	def plotHistogram(self, numBin, xlab):
		n, bins, patches = plt.hist(self.data, numBin, normed=0, facecolor='green', alpha=0.5)

		#histfit(self.data)
		#y = mlab.normpdf(bins)
		plt.plot(bins)
		plt.xlabel(xlab)
		plt.ylabel('Frequency')
		plt.title(r'Distribution of ' + xlab)

		# Tweak spacing to prevent clipping of ylabel
		plt.subplots_adjust(left=0.15)
		plt.show()

def runStats(ls):
	ret = result()
	ret.mini = min(ls)
	ret.maxi = max(ls)
	ret.expectation = numpy.mean(ls)
	ret.var = numpy.var(ls)
	ret.median = numpy.median(ls)
	ret.stdev = numpy.std(ls)
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
		spreadhops.append(int(el))

	timeResult = runStats(spreadtime)
	hopResult = runStats(spreadhops)

	print "Max Time Analysis Result"
	timeResult.printResult("s")
	print ""
	
	print "Max Hops Analysis Result"
	hopResult.printResult("")

	timeResult.plotHistogram(int(timeResult.maxi/5)+1, "Max Time")
	hopResult.plotHistogram(hopResult.maxi, "Max Hops")

if __name__ == '__main__':
	main()
