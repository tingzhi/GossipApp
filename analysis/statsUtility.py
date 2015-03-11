'''
   Copyright 2015 Kendall Bailey

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

'''

'''
	Modified by Tingzhi Li
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

def ReadFile(filename):
	try:
	        f = open(filename)
	except IOError as e:
        	print "Problem opening file {2}. Error ({0}) : {1}".format(e.errno, e.strerror, filename)
        	sys.exit(2)

	text = f.read()
	f.close
	return text

def CheckArgs(numArgs, pattern):
	if len(sys.argv) != (numArgs + 1):
        	print "Error: " + pattern
        	print "length = " + str(len(sys.argv))
        	sys.exit(2)

def OverwriteFile(filename, data):
	try:
		with open(filename, 'w') as outfile:
			outfile.write(data)
			outfile.close()	
	except UnicodeEncodeError as e:
		print "Unicode Encode Error: Couldn't write --" +  data + "-- to file"
		sys.exit(2)

def AppendToFile(filename, data):
	try:
		with open(filename, 'a') as outfile:
			outfile.write(data)
			outfile.close()
	except UnicodeEncodeError as e:
		print "Unicode Encode Error: Couldn't write --" +  data + "-- to file"
		sys.exit(2)

