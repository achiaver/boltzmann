#!/usr/bin/env python3

import numpy as np

num_examples = 2
num_times = 30

num_layers = 2
num_hidden = 4

maxepochs = 1000
epsilonw = 0.01
epsilonvb = 0.01
epsilonhb = 0.01

def add_examples (data, num_examples, num_visible):
    return np.concatenate((data, np.random.randint(2, size = (num_examples, num_visible))))

pattern = np.array([[1, 0, 0, 1, 0, 1, 0, 1, 0]])
num_visible = pattern.shape[1]

dataset = add_examples(pattern, num_examples, num_visible)

for i in range(num_times):
    dataset = np.concatenate((dataset, pattern))
    dataset = add_examples(dataset, num_examples, num_visible)

dataset = dataset.astype(np.uint8)

print(dataset)

datapath = '../c_lang/dataset/'
filename = 'make_smolensky_training.bin'

file = open(datapath + filename, 'wb')
file.write(dataset)
file.close()
