#!/usr/bin/python

# Generate code for pink noise by voss-mccartney method
import sys

def generate_sequence(n):
  if max(n) < 1:
    return None
  if n == [1]:
    return n 
  else:
    subsequence = generate_sequence([max(n)-1])
    return subsequence + n + subsequence


if __name__ == '__main__':
  output = None
  arg = sys.argv[1]
  if arg.isdigit():
    input = int(arg)
    output = generate_sequence([input])
  if output:
    output.append(0)
    print output
  else:
    print "Input must be a number"
