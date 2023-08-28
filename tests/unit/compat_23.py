"""Legacy code for Python 2/3 compatibility. Now Python 3 only."""
import math
import sys

assert sys.version_info[0] == 3
int_type = int
math_nan = math.nan

example_int_value = int_type(8000)
example_int_seq = tuple([int_type(v) for v in (1000, 2000, 4000, 8000,)])
