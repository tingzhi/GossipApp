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
	statsUtility.CheckArgs(3,"<time in file> <hops in file> <outfile>")

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

	hopResult.plotHistogram(hopResult.maxi, "Max Hops")
	timeResult.plotHistogram(int(timeResult.maxi/5)+1, "Max Time")

if __name__ == '__main__':
	main()