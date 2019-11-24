#!/usr/bin/env python3
import sys
import re
for line in sys.stdin:
    line = re.sub(r"[^A-Za-z]"," ", line.strip())
    words = line.split()
    for word in words:
        print("%s\t %s"%(word, 1))