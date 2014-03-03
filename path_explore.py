#! /usr/bin/python
# Path exploration for small code snippets.

# Read in file
import sys
fh=sys.stdin

# Compile RE for objdump listing
import re
dump_matcher = re.compile('\s*(?P<pc>[0-9a-f]+):\s*(?P<instruction>([0-9a-f]{2}\s)+)\s*(?P<opcode>[a-z]+)\s*(?P<arguments>.+)');

# Read every line
for line in fh.readlines():
  line.strip()
  match_op = dump_matcher.match(line);
  if (match_op != None):
    print match_op.group('pc')
    print match_op.group('instruction')
    print match_op.group('opcode')
    print match_op.group('arguments')
