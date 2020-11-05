#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# test.py
# Copyright (C) 2020 KunoiSayami
#
# This module is part of openmp-1106-report and is released under
# the AGPL v3 License: https://www.gnu.org/licenses/agpl-3.0.txt
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.
import random
import sys
import subprocess
import logging
import coloredlogs
from typing import Union

v = []


def generate_data(n: int) -> bytes:
    s = b''
    s += str(n).encode()
    s += b'\n'
    for _ in range(n):
        v.append(random.randint(-2147483648, 2147483647))
    s += b' '.join(map(lambda x: str(x).encode(), v))
    s += b'\n'
    return s


def build(release: bool = False) -> subprocess.Popen:
    logging.debug('Building..')
    buildarg = ['g++', '-o', 'out', 'main.cpp', '-O2', '-fopenmp', '-DPYTHONTEST']
    if release:
        buildarg.pop()
    p = subprocess.Popen(buildarg)
    p.wait()
    return p


if __name__ == '__main__':
    n = random.randint(10000, 50000)
    coloredlogs.install(logging.DEBUG)
    if len(sys.argv) > 1:
        if sys.argv[1] == 'build':
            build(True)
            raise SystemExit
        else:
            n = int(sys.argv[-1])
        if sys.argv[1] == 'gen':
            print(generate_data(n).decode())
            raise SystemExit
    if build().returncode != 0:
        raise SystemExit
    logging.debug('Running for %d', n)
    p = subprocess.Popen(['./out'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out = p.communicate(input=generate_data(n))[0].decode().strip()
    except_str = ' '.join(map(str, sorted(v)))
    logging.debug(out == except_str)
    with open('diff1.out', 'w') as fout1, open('diff2.out', 'w') as fout2:
        fout1.write(out)
        fout2.write(except_str)
