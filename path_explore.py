#! /usr/bin/python
# Path exploration for individual functions

# Read in file
import sys
fh=sys.stdin

# Compile RE for objdump listing
import re
dump_matcher = re.compile('\s*(?P<pc>[0-9a-f]+):\s*(?P<instruction>([0-9a-f]{2}\s)+)\s*(?P<opcode>[a-z]+)\s*(?P<arguments>.+)');

# dictionary to represent program
program = dict()

# Read every line
for line in fh.readlines():
  line.strip()
  match_op = dump_matcher.match(line);
  if (match_op != None):
    # Populate program dictionary
    pc = int(match_op.group('pc'), 16)
    instruction = match_op.group('instruction')
    instruction_length = len(instruction.strip().split())
    opcode = match_op.group('opcode')
    args = match_op.group('arguments')
    assert (pc not in program)
    program[pc] = (opcode, args, instruction_length)

# Make the last instruction point to a negative pc.
program[pc] = (opcode, args, -2 * pc)

# Start executing the program and building up paths
paths = []
print program
assert(0 in program);
pc = 0
while(pc >= 0) :
  print "Current PC: %x"%(pc)
  pc += program[pc][2]
