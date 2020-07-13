# Restricted Boltzmann Machines Using C#

By [James McCaffrey](https://docs.microsoft.com/en-us\archive\msdn-magazine\authors\James_McCaffrey)

![James McCaffrey](https://docs.microsoft.com/en-us/archive/msdn-magazine/2017/june/images/mt809120.mccaffrey_headshot(en-us%2cmsdn.10).jpg)

A restricted Boltzmann machine (RBM) is a fascinating software component that has some similarities to a basic neural network. An RBM has two sets of nodes—visible and hidden. Each set of nodes can act as either inputs or outputs relative to the other set. Each node has a value of zero or one and these values are calculated using probability rather than deterministically.

Each visible-layer node is conceptually connected to each hidden-­layer node with a numeric constant called a weight, typically a value between -10.0 and +10.0. Each node has an associated numeric constant called a bias. The best way to start to understand what an RBM is is by way of a diagram. Take a look at **Figure 1**.

![An Example of a Restricted Boltzmann Machine](https://docs.microsoft.com/en-us/archive/msdn-magazine/2017/june/images/mt809120.mccaffrey_figure1_hires(en-us%2cmsdn.10).png)
**Figure 1 An Example of a Restricted Boltzmann Machine**

In **Figure 1**, the visible nodes are acting as the inputs. There are six visible (input) nodes and three hidden (output) nodes. The values of the visible nodes are (1, 1, 0, 0, 0, 0) and the computed values of the hidden nodes are (1, 1, 0). There are 6 * 3 = 18 weights connecting the nodes. Notice that there are no visible-to-visible or hidden-to-hidden weights. This restriction is why the word “restricted” is part of the RBM name.

Each of the red weight arrows in **Figure 1** points in both directions, indicating that information can flow in either direction. If nodes are zero-base indexed, then the weight from visible[0] to hidden[0] is 2.78, the weight from visible[5] to hidden[2] is 0.10 and so on. The bias values are the small green arrows pointing into each node so the bias for visible[0] is -0.62 and the bias for hidden[0] is +1.25 and so on. The p value inside each node is the probability that the node takes a value of one. So, hidden[0] has p = 0.995, which means that its calculated value will almost certainly be one, and in fact it is, but because RBMs are probabilistic, the value of hidden[0] could have been zero.

You probably have many questions right about now, such as where the weights and bias values come from, but bear with me—you’ll see how all the parts of the puzzle fit together shortly. In the sections that follow, I’ll describe the RBM input-output mechanism, explain where the weights and bias values come from, present a demo program that corresponds to **Figure 1**, and give an example of how RBMs can be used.

This article assumes you have intermediate or better programming skills, but doesn’t assume you know anything about RBMs. The demo program is coded using C#, but you should have no trouble refactoring the code to another language such as Python or JavaScript if you wish. The demo program is too long to present in its entirety, but the complete source code is available in the file download that accompanies this article. All error checking was removed to keep the main ideas as clear as possible

## The RBM Input-Output Mechanism

The RBM input-output mechanism is (somewhat surprisingly) relatively simple and is best explained by an example. Suppose, as in **Figure 1**, the visible nodes act as inputs and have values (1, 1, 0, 0, 0, 0). The value of hidden node[0] is computed as follows: The six weights from the visible nodes into hidden[0] are (2.78, 1.32, 0.80, 2.23, -4.27, -2.22) and the bias value for hidden[0] is 1.25.

The p value for hidden[0] is the logistic sigmoid value of the sum of the products of input values multiplied by their associated weights, plus the target node bias. Put another way, multiply each input node value by the weight pointing from the node into the target node, add those products up, add in the target node bias value and then take the logistic sigmoid of that sum:

```xml
p[0] = logsig( (1 * 2.78) + (1 * 1.32) + (0 * 0.80) +
               (0 * 2.23) + (0 * -4.27) + (0 * -2.22) + 1.25 )
     = logsig( 2.78 + 1.32 + 1.25 )
     = logsig( 5.36 )
     = 0.995
```

The logistic sigmoid function, which appears in many machine learning algorithms, is defined as:

```xml
logsig(x) = 1.0 / (1.0 + exp(-x))
```

where the exp function is defined as:

```xml
exp(x) = e^x
```

where e (Euler’s number) is approximately 2.71828.

So, at this point, the p value for hidden[0] has been calculated as 0.995. To calculate the final zero or one value for the hidden[0] node you’d use a random number generator to produce a pseudo-random value between 0.0 and 1.0. If the random number is less than 0.995 (which it will be 99.5 percent of the time), the node value is set to one; otherwise (0.05 percent of the time), it’s set to zero.

The other hidden nodes would be computed in the same way. And if the hidden nodes were acting as inputs, the values of the visible nodes would be calculated as output values in the same way.

## Determining the Weights and Bias Values

Determining a set of RBM output values for a given set of input values is easy, but from where do the weights and bias values come? Unlike neural networks, which require a set of training data with known input values and known, correct, output values, RBMs can essentially train themselves, so to speak, using only a set of values for the visible nodes. Interesting! Suppose you have a set of 12 data items, like so:

```xml
(1, 1, 0, 0, 0, 0) // A
(0, 0, 1, 1, 0, 0) // B
(0, 0, 0, 0, 1, 1) // C
(1, 1, 0, 0, 0, 1) // noisy A
(0, 0, 1, 1, 0, 0) // B
(0, 0, 0, 0, 1, 1) // C
(1, 0, 0, 0, 0, 0) // weak A
(0, 0, 1, 0, 0, 0) // weak B
(0, 0, 0, 0, 1, 0) // weak C
(1, 1, 0, 1, 0, 0) // noisy A
(1, 0, 1, 1, 0, 0) // noisy B
(0, 0, 1, 0, 1, 1) // noisy C
```

Because RBM visible node values are zero and one, you can think of them as individual binary features (such as “like” and “don’t like”) or as binary-encoded integers. Suppose each of the 12 data items represents a person’s like or don’t like opinion for six films: “Alien,” “Inception,” “Spy,” “EuroTrip,” “Gladiator,” “Spartacus.” The first two films are science fiction. The next two films are comedy (well, depending on your sense of humor) and the last two films are history (sort of).

The first person likes “Alien” and “Inception,” but doesn’t like the other four films. If you look at the data, you can imagine that there are three types of people. Type “A” people like only science fiction films. Type “B” like only comedy films and type “C” like only history films. Notice that there’s some noise in the data, and there are weak and noisy versions of each person type.

The number of visible nodes in an RBM is determined by the number of dimensions of the input data—six in this example. The number of hidden nodes is a free parameter that you must choose. Suppose you set the number of hidden nodes to three. Because each RBM node value can be zero or one, with three hidden nodes there are a total of eight people types that can be detected: (0, 0, 0), (0, 0, 1), . . . (1, 1, 1).

There are several ways to train an RBM. The most common algorithm is called CD-1, which stands for contrastive divergence, single-step. The algorithm is very clever and isn’t at all obvious. The CD-1 training algorithm is presented in high-level pseudo-code in **Figure 2**.

Figure 2 The CD-1 Training Algorithm:

```xml
(v represents the visible nodes)
(h represents the hidden nodes)
(lr is a small learning rate value)
loop n times
  for each data item
    compute h from v
    let posGrad = outer product(v, h)
    compute v' from h
    compute h' from v'
    let negGrad = outer product(v', h')
    let delta W = lr * (posGrad - negGrad)
    let delta vb = lr * (v - v')
    let delta hb = lr * (h - h')
  end for
end loop
```

The goal of training is to find a set of weights and bias values so that when the RBM is fed a set of input values in the visible nodes and generates a set of output nodes in the hidden nodes, then when the hidden nodes are used as inputs, the original visible nodes values will (usually) be regenerated. The only way I was able to understand CD-1 was by walking through a few concrete examples.

Suppose the learning rate is set to 0.01 and that at some point the current training input item is (0, 0, 1, 1, 0, 0) and the 18 weights are:

```xml
0.01  0.02  0.03
0.04  0.05  0.06
0.07  0.08  0.09
0.10  0.11  0.12
0.13  0.14  0.15
0.16  0.17  0.18
```

The row index, 0 to 5, represents the index of a visible node and the column index, 0 to 2, represents the index of a hidden node. So the weight from visible[0] to hidden[2] is 0.03.

The first step in CD-1 is to compute the h values from the v values using the probabilistic mechanism described in the previous section. Suppose h turns out to be (0, 1, 0). Next, the positive gradient is computed as the outer product of v and h:

```xml
0  0  0
0  0  0
0  1  0
0  1  0
0  0  0
0  0  0
```

The outer product isn’t very common in machine learning algorithms (or any other algorithms for that matter), so it’s quite possible you haven’t seen it before. The Wikipedia article on the topic gives a pretty good explanation. Notice that the shape of the positive gradient matrix will be the same as the shape of the weight matrix.

Next, the h values are used as inputs, along with the current weights, to produce new output values v'. Suppose v' turns out to be (0, 1, 1, 1, 0, 0). Next, v' is used as the input to compute a new h'. Suppose h' is (0, 0, 1).

The negative gradient is the outer product of v' and h' and so is:

```xml
0  0  0
0  0  1
0  0  1
0  0  1
0  0  0
0  0  0
```

The result of posGrad - negGrad is:

```xml
0   0   0
0   0  -1
0  +1  -1
0  +1  -1
0   0   0
0   0   0
```

If you review the algorithm carefully, you’ll see that cell values in the delta gradient matrix can only be one of three values: 0, +1 or -1. Delta gradient values of +1 correspond to weights that should be increased slightly. Values of -1 correspond to weights that should be decreased slightly. Clever! The amount of increase or decrease is set by a learning rate value. So the weight from visible[1] to hidden[2] would be decreased by 0.01 and the weight from visible[2] to hidden[1] would be increased by 0.01. A small learning rate value makes training take longer, but a large learning rate can skip over good weight values.

So, how many iterations of training should be performed? In general, setting the number of training iterations and choosing a value of the learning rate are matters of trial and error. In the demo program that accompanies this article, I used a learning rate of 0.01 and a maximum number of iterations set to 1,000. After training, I got the weights and bias values shown in **Figure 1**.

## Interpreting a Restricted Boltzmann Machine

OK, so it’s possible to take a set of data where each value is zero or one, then set a number of hidden nodes, and get some weights and bias values. What’s the point?

One way to think of an RBM is as a kind of compression machine. For the example film preference data, if you feed a type A person as input (1, 1, 0, 0, 0, 0), you’ll usually get (1, 1, 0) as output. If you feed (1, 1, 0) as input to the hidden nodes, you almost always get (1, 1, 0, 0, 0, 0) as output in the visible nodes. In other words, (1, 1, 0, 0, 0, 0) and slight variations are mapped to (1, 1, 0). This behavior is closely related to, but not quite the same as, factor analysis in classical statistics.

Take a look at the demo program in **Figure 3**. The demo corresponds to the film like-dislike example. The demo creates a 6-3 RBM and trains it using the 12 data items presented in the previous section. The hardcoded data is set up like so:

```csharp
int[][] trainData = new int[12][];
trainData[0] = new int[] { 1, 1, 0, 0, 0, 0 };
trainData[1] = new int[] { 0, 0, 1, 1, 0, 0 };
...
trainData[11] = new int[] { 0, 0, 1, 0, 1, 1 };
```

![Demo of a Restricted Boltzmann Machine](https://msdn.microsoft.com/en-us/mt809120.McCaffreyTRun_Figure%203_hires(en-us,MSDN.10).png)
**Figure 3 Demo of a Restricted Boltzmann Machine**

In most situations you’d read data from a text file using a helper method. The demo RBM is created and trained like this:

```csharp
int numVisible = 6;
int numHidden = 3;
Machine rbm = new Machine(numVisible, numHidden);
double learnRate = 0.01;
int maxEpochs = 1000;
rbm.Train(trainData, learnRate, maxEpochs);
```

The choice of setting the number of hidden nodes to three was arbitrary and the values for learnRate and maxEpochs were determined by trial and error. After training, the RBM is exercised like this:

```csharp
int[] visibles = new int[] { 1, 1, 0, 0, 0, 0 };
int[] computedHidden = rbm.HiddenFromVis(visibles);
Console.Write("visible = ");
ShowVector(visibles, false);
Console.Write("  ->  ");
ShowVector(computedHidden, true);
```

If you experiment with the code, you’ll notice that the computed hidden values are almost always one of three patterns. Person type A (or weak or noisy version) almost always generates (1, 1, 0). Type B generates (1, 0, 1). And type C generates (0, 1, 1). And if you feed the three patterns as inputs, you’ll almost always get the three primary input patterns back. The point is the RBM has deduced that the data can be placed into one of three buckets. The specific bit patterns aren’t important.

Yet another interpretation of this behavior is that an RBM acts as an auto-encoder. And it’s also possible to chain several RBMs together to create a prediction system called a deep belief network (DBN). In fact, this is arguably the most common use of RBMs.

## Implementing a Restricted Boltzmann Machine

Once you understand how RBMs work, they’re actually quite simple. But coding a demo program is a bit more complex than you might expect. There are many design possibilities for an RBM. Take a look at the demo run in **Figure 3**.

The demo illustrates the film preference example from the previous sections of this article so there are six visible nodes. The demo program defines a Machine object with 10 member fields. The first six fields in the class definition are:

```csharp
public class Machine
{
  public Random rnd;
  public int numVisible;
  public int numHidden;
  public int[] visValues;
  public double[] visProbs;
  public double[] visBiases;
...
```

All fields are declared with public scope for simplicity. The Random object is used when converting a node probability to a concrete zero or one value. Variables numVisible and numHidden (OK, OK, I know they’re objects) hold the number of hidden and visible nodes. Integer array visValues holds the zero or one values of the visible nodes. Note that you can use a Boolean type if you wish. Double array visBiases holds the bias values associated with each visible node. Double array visProbs holds visible node probabilities. Note that the visProbs array isn’t necessary because node values can be computed on the fly; however, storing the probability values is useful if you want to examine the behavior of the RBM during runtime.

The other four Machine class fields are:

```csharp
public int[] hidValues;
public double[] hidProbs;
public double[] hidBiases;
public double[][] vhWeights;
```

Arrays hidValues, hidBiases, and hidProbs are the node values, associated bias values, and node probabilities, respectively. The vhWeights object is an array-of-arrays style matrix where the row index corresponds to a visible node and the column index corresponds to a hidden node.

The key class method computes the values of the hidden nodes using values in a parameter that corresponds to the visible nodes. That method’s definition begins with:

```csharp
public int[] HiddenFromVis(int[] visibles)
{
  int[] result = new int[numHidden];
  ...
```

Next, the calculations of the hidden-layer nodes are done node by node:

```csharp
for (int h = 0; h < numHidden; ++h) {
  double sum = 0.0;
  for (int v = 0; v < numVisible; ++v)
    sum += visibles[v] * vhWeights[v][h];
  sum += hidBiases[h]; // Add the hidden bias
  double probActiv = LogSig(sum); // Compute prob
  double pr = rnd.NextDouble();  // Determine 0/1
  if (probActiv > pr) result[h] = 1;
  else result[h] = 0;
}
```

The code mirrors the explanation of the input-output mechanism explained earlier. Function LogSig is a private helper function, because the Microsoft .NET Framework doesn’t have a built-in logistic sigmoid function (at least that I’m aware of).

The key method concludes by returning the computed hidden node values to the caller:

```csharp
...
  return result;
}
```

The rest of the demo code implements the CD-1 training algorithm as described earlier. The code isn’t trivial, but if you examine it carefully, you should be able to make the connections between RBM concepts and implementation.

## Wrapping Up

Restricted Boltzmann machines are simple and complicated at the same time. The RBM input-output mechanism is both deterministic and probabilistic (sometimes called stochastic), but is relatively easy to understand and implement. The more difficult aspect of RBMs, in my opinion, is understanding how they can be useful.

As a standalone software component, an RBM can act as a lossy compression machine to reduce the number of bits needed to represent some data, or can act as a probabilistic factor analysis component that identifies core concepts in a data set. When concatenated, RBMs can create a deep neural network structure called a “deep belief network” that can make predictions.

RBMs were invented in 1986 by my Microsoft colleague Paul Smolensky, but gained increased attention relatively recently when the CD-1 training algorithm was devised by researcher and Microsoft collaborator Geoffrey Hinton. Much of the information presented in this article is based on personal conversations with Smolensky, and the 2010 research paper, “A Practical Guide to Training Restricted Boltzmann Machines,” by Hinton.

------

**Dr. James McCaffrey** *works for Microsoft Research in Redmond, Wash. He has worked on several Microsoft products including Internet Explorer and Bing. Dr. McCaffrey can be reached at [jammc@microsoft.com](mailto:jammc@microsoft.com).*

[REFERENCE](https://docs.microsoft.com/en-us/archive/msdn-magazine/2017/june/test-run-restricted-boltzmann-machines-using-csharp#test-run)