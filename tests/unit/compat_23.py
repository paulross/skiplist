import math
import sys

if sys.version_info[0] == 3:
    int_type = int
    math_nan = math.nan
elif sys.version_info[0] == 2:
    int_type = long    
    math_nan = float('NaN')

example_int_value = int_type(8000)
example_int_seq = tuple([int_type(v) for v in (1000, 2000, 4000, 8000,)])
