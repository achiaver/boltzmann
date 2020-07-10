#!/usr/bin/env python3

import os.path
import numpy as np

num_examples = 2
num_times = 40

num_layers = 2
num_hidden = 4

maxepochs = 1000
epsilonw = 0.01
epsilonvb = 0.01
epsilonhb = 0.01

def add_examples (data, num_examples, num_visible):
    return np.concatenate((data, np.random.randint(2, size=(num_examples, num_visible))))

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

print("Writing binary file...")
if not os.path.exists(datapath + filename):
    print("Binary {} does not exist...".format(datapath+filename))
    file = open(datapath + filename, 'wb')
    file.write(dataset)
    file.close()
    print("Writing done.")
else:
    print("Binary already exists with name {}".format(datapath + filename))

print("\n\n")
print("Computing Weight Matrix for make example of Smolensky (Fig. 6)")
# Knowledge Matrix
K = np.array([[ 1, 0, 0, 0],
              [-1, 0, 0, 0],
              [-1, 0, 0, 0],
              [ 1, 1, 0, 0],
              [-1, -1, 0, 0],
              [ 0, 1, 1, -1],
              [ 0, -1, -1, 1],
              [ 0, 0, 1, -1],
              [ 0, 0, -1, -1]])

print("Matrix K => each column is a knowledge vector")
print(K)

# Strength Matrix
Sigma = np.ones(K.shape)
print("Sigma Matrix")
print(Sigma)

# Weight Matrix
L1 = np.sum(np.abs(K), axis=0)
W = (K * Sigma/L1).astype(np.float64)
print("Weight Matrix")
print(W)

datapath = '../c_lang/dataset/'
filename_weight = 'make_smolensky_weights.bin'

print("Writing binary file...")
if not os.path.exists(datapath + filename_weight):
    print("Binary {} does not exist...".format(datapath+filename_weight))
    file = open(datapath + filename_weight, 'wb')
    file.write(W)
    file.close()
    print("Writing done.")
else:
    print("Binary already exists with name {}".format(datapath + filename_weight))


