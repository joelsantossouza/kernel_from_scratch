#!/bin/python3

# Author: Joel Souza
# Description: Python program to check if Assembly files
#              contains its BITS directive

from typing import Optional
import re
import os
import sys


REGEX_BITS: str = r"\[BITS\s+(16|32|64)\]"


def check_asm_bits(filename: str, root: Optional[str] = "") -> None:
    """Check if a .asm file contain proper BITS directive"""
    extension: str = os.path.splitext(filename)[1]
    if extension != ".asm":
        return
    filepath: str = os.path.join(root, filename)
    try:
        with open(filepath, "r") as file:
            for line in file:
                if re.fullmatch(REGEX_BITS, line.rstrip()):
                    return
            raise ValueError(
                "Missing [BITS <16||32||64>] directive"
            )
    except Exception as e:
        print(f"{filepath} => {e}")


if __name__ == "__main__":
    base_path: str = "./"
    if len(sys.argv) > 1:
        base_path = sys.argv[1]
    if os.path.isfile(base_path):
        check_asm_bits(base_path)
    else:
        for root, dirs, files in os.walk(base_path):
            for file in files:
                check_asm_bits(file, root)
