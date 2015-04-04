import sys
import re

argvs = sys.argv
argc = len(argvs)

if argc < 2:
	print 'Error: Not enough arguments'
	quit()

f = open(argvs[1], 'r')
if f==None:
	print 'Error: File not found'
	quit()

class Rule:
	"""docstring for Rule"""
	def __init__(self, name, num):
		self.name = name
		self.num = num

rules = []
i = 0
while True:
	line = f.readline()
	line = line.rstrip()
	if line != '':
			cur = Rule(line, i)
			i+=1
			rules.append(cur)
	else:
		break;

print "#define NEZ_RULE_SIZE " + str(i)

for rule in rules:
	print "#define NEZ_" + rule.name +" "+ str(rule.num)

for rule in rules:
	print "rules[NEZ_" + rule.name + "]->rule_name = \"" + rule.name + "\";"
	print "rules[NEZ_" + rule.name + "]->expr = " + rule.name + "();"