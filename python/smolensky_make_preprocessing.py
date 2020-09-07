#!/usr/bin/env python3

import os.path
import numpy as np


weightpath = "../weights/smolensky_make/"
biasespath = "../biases/smolensky_make/"

if not os.path.exists(weightpath):
    os.makedirs(weightpath)

if not os.path.exists(biasespath):
    os.makedirs(biasespath)


