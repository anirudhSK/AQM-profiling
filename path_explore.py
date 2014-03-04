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

    # If it's a retq point it to a negative pc.
    if (opcode == "retq"):
      program[pc] = (opcode, args, -2 * pc)

# Utility functions
# Pretty-print path
def pretty_print_path(path):
  for instruction_node in path:
    print instruction_node[0], instruction_node[1]
  #print "\nLENGTH:", len(path), "\n";

# Prepend an instruction_node to a path
def prepend_instruction_node_to_path(instruction_node, path):
  return [instruction_node] + path

# Prepend an instruction_node to a list of paths
def prepend_instruction_node_to_paths(instruction_node, paths):
  ret = []
  for path in paths:
    ret.append(prepend_instruction_node_to_path(instruction_node, path))
  return ret

# Recursively explore paths starting from current_pc
def explore_paths(current_pc):

  # Get current instruction_node
  instruction_node = program[current_pc]

  # Get opcode
  opcode = program[current_pc][0]

  # Get default_target
  default_target = current_pc + program[current_pc][2]

  # Regex to determine conditional branches
  conditional_jumps = re.compile('j[a-z]+');

  # If it's an unconditional branch
  if (opcode == "jmp"):
    branch_target = int(program[current_pc][1].split()[0], 16)
    return prepend_instruction_node_to_paths(instruction_node, explore_paths(branch_target));

  # If it's a conditional branch
  elif (conditional_jumps.match(opcode)):
    branch_target = int(program[current_pc][1].split()[0], 16)
    return prepend_instruction_node_to_paths(instruction_node, explore_paths(default_target)) + prepend_instruction_node_to_paths(instruction_node, explore_paths(branch_target));

  # If it's neither, it's sequential code
  else :
    if (default_target >= 0):
      return prepend_instruction_node_to_paths(instruction_node, explore_paths(default_target))
    else: # Reached end-of-function
      return [[instruction_node]]

# Start executing the program and building up paths
start_pc = int(sys.argv[1], 16)
assert(start_pc in program);
paths = explore_paths(start_pc)

#for path in paths:
#  pretty_print_path(path)

max_length = 0
argmax = []
for path in paths:
   if (len(path) > max_length):
     max_length = len(path)
     argmax = path

pretty_print_path(argmax)
