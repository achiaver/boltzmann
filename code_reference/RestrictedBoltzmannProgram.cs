using System;
namespace RestrictedBoltzmann
{
  class RestrictedBoltzmannProgram
  {
    static void Main(string[] args)
    {
      Console.WriteLine("\nBegin Restricted Boltzmann Machine demo \n");

      Console.WriteLine("Films: Alien, Inception, Spy, EuroTrip,");
      Console.WriteLine(" Gladiator, Spartacus \n");

      // type A person = likes only Science Fiction films
      // type B person = likes only Comedy
      // type C person = likes only History

      int[][] trainData = new int[12][];
      trainData[0] = new int[] { 1, 1, 0, 0, 0, 0 };  // A
      trainData[1] = new int[] { 0, 0, 1, 1, 0, 0 };  // B
      trainData[2] = new int[] { 0, 0, 0, 0, 1, 1 };  // C

      trainData[3] = new int[] { 1, 1, 0, 0, 0, 1 };  // noisy A
      trainData[4] = new int[] { 0, 0, 1, 1, 0, 0 };  // B
      trainData[5] = new int[] { 0, 0, 0, 0, 1, 1 };  // C

      trainData[6] = new int[] { 1, 0, 0, 0, 0, 0 };  // weak A
      trainData[7] = new int[] { 0, 0, 1, 0, 0, 0 };  // weak B
      trainData[8] = new int[] { 0, 0, 0, 0, 1, 0 };  // weak C

      trainData[9] = new int[] { 1, 1, 0, 1, 0, 0 };  // noisy A
      trainData[10] = new int[] { 1, 0, 1, 1, 0, 0 };  // noisy B
      trainData[11] = new int[] { 0, 0, 1, 0, 1, 1 };  // noisy C

      Console.WriteLine("Film like-dislike data is: \n");
      ShowData(trainData, 5, true);

      int numVisible = 6;
      int numHidden = 3;
      Console.WriteLine("Creating a RBM ");
      Console.WriteLine("Setting numVisible = " + numVisible);
      Console.WriteLine("Setting numHidden  = " + numHidden + "\n");

      Machine rbm = new Machine(numVisible, numHidden);

      double learnRate = 0.01;
      int maxEpochs = 1000;
      Console.WriteLine("Training RBM using CD1 algorithm ");
      Console.WriteLine("Setting learnRate = " + learnRate.ToString("F3"));
      Console.WriteLine("Setting maxEpochs = " + maxEpochs);
      rbm.Train(trainData, learnRate, maxEpochs);
      Console.WriteLine("Training complete \n");

      Console.WriteLine("Trained machine's weights and biases are: \n");
      rbm.Dump(false, true, true);

      Console.WriteLine("Using trained machine . . . \n");

      //for (int j = 0; j < trainData.Length; ++j)
      //{
      //  int[] computedHidden = rbm.HiddenFromVis(trainData[j]);
      //  ShowVector(computedHidden, true);
      //}

      int[] visibles = new int[] { 1, 1, 0, 0, 0, 0 };
      int[] computedHidden = rbm.HiddenFromVis(visibles);
      Console.Write("visible = ");
      ShowVector(visibles, false);
      Console.Write("  ->  ");
      ShowVector(computedHidden, true);

      int[] hiddens = new int[] { 1, 1, 0 };
      int[] computedVisibles = rbm.VisibleFromHid(hiddens);
      Console.Write("hidden = ");
      ShowVector(hiddens, false);
      Console.Write("  ->  ");
      ShowVector(computedVisibles, true);

      Console.WriteLine("\nEnd RBM demo \n");
      Console.ReadLine();
    } // Main

    static void ShowData(int[][] data, int numRows, bool showLast)
    {
      for (int i = 0; i < numRows; ++i)
      {
        Console.Write("[" + i.ToString().PadLeft(2) + "]   ");
        for (int j = 0; j < data[i].Length; ++j)
        {
          Console.Write(data[i][j] + "  ");
        }
        Console.WriteLine("");
      }
      if (showLast == true)
      {
        Console.WriteLine(" . . . ");
        Console.Write("[" + (data.Length - 1).ToString().PadLeft(2) + "]   ");
        for (int j = 0; j < data[data.Length - 1].Length; ++j)
        {
          Console.Write(data[data.Length-1][j] + "  ");
        }
        Console.WriteLine("");
      }
      Console.WriteLine("");
    }

    static void ShowVector(int[] vector, bool newline)
    {
      for (int i = 0; i < vector.Length; ++i)
        Console.Write(vector[i] + " ");
      if (newline == true)
        Console.WriteLine("");
    }

  }  // Program

  public class Machine
  {
    public Random rnd;

    public int numVisible;
    public int numHidden;

    public int[] visValues; // visible node values (0, 1)
    public double[] visProbs;
    public double[] visBiases;

    public int[] hidValues;
    public double[] hidProbs;
    public double[] hidBiases;

    public double[][] vhWeights;

    public Machine(int numVisible, int numHidden)
    {
      this.rnd = new Random(0);

      this.numVisible = numVisible;
      this.numHidden = numHidden;

      // allocate arrays & the weights matrix
      visValues = new int[numVisible];
      visProbs = new double[numVisible];
      visBiases = new double[numVisible];

      hidValues = new int[numHidden];
      hidProbs = new double[numHidden];
      hidBiases = new double[numHidden];

      vhWeights = new double[numVisible][];  // visible-to-hidden
      for (int i = 0; i < numVisible; ++i)
        vhWeights[i] = new double[numHidden];

      // small random values for initial weights & biases
      double lo = -0.40;
      double hi = +0.40;
      for (int i = 0; i < numVisible; ++i)
        for (int j = 0; j < numHidden; ++j)
          vhWeights[i][j] = (hi - lo) * rnd.NextDouble() + lo;

      for (int i = 0; i < numVisible; ++i)
        visBiases[i] = (hi - lo) * rnd.NextDouble() + lo;

      for (int i = 0; i < numHidden; ++i)
        hidBiases[i] = (hi - lo) * rnd.NextDouble() + lo;
    } // ctor

    //public void SetWeights(double[] wts)  // for debugging
    //{
    //  // order: weights, vBiases, hBiases
    //  int idx = 0;
    //  for (int i = 0; i < numVisible; ++i)
    //    for (int j = 0; j < numHidden; ++j)
    //      vhWeights[i][j] = wts[idx++];
    //  for (int i = 0; i < numVisible; ++i)
    //    visBiases[i] = wts[idx++];
    //  for (int j = 0; j < numHidden; ++j)
    //    hidBiases[j] = wts[idx++];
    //}

    //public double[] GetWeights()  // for debugging
    //{
    //  int numWts = numVisible * numHidden + numVisible + numHidden;
    //  double[] result = new double[numWts];
    //  int idx = 0;
    //  for (int i = 0; i < numVisible; ++i)
    //    for (int j = 0; j < numHidden; ++j)
    //      result[idx++] = vhWeights[i][j];
    //  for (int i = 0; i < numVisible; ++i)
    //    result[idx++] = visBiases[i];
    //  for (int j = 0; j < numHidden; ++j)
    //    result[idx++] = hidBiases[j];
    //  return result;
    //}

    public void Train(int[][] trainData, double learnRate, int maxEpochs)
    {
      int[] indices = new int[trainData.Length];
      for (int i = 0; i < indices.Length; ++i)
        indices[i] = i;

      int epoch = 0;
      while (epoch < maxEpochs)
      {
        Shuffle(indices);

        for (int idx = 0; idx < indices.Length; ++idx) // each data item
        {
          int i = indices[idx];  // i points to curr train data

          // copy visible values from train data into Machine
          for (int j = 0; j < numVisible; ++j)
            visValues[j] = trainData[i][j];

          // compute hidden node values ('h' in Wikipedia)
          for (int h = 0; h < numHidden; ++h)
          {
            double sum = 0.0;
            for (int v = 0; v < numVisible; ++v)
              sum += visValues[v] * vhWeights[v][h];
 
            sum += hidBiases[h]; // add the hidden bias
            hidProbs[h] = LogSig(sum); // compute prob of h activation
            double pr = rnd.NextDouble();  // determine 0/1 h node value
            if (hidProbs[h] > pr)
              hidValues[h] = 1;
            else
              hidValues[h] = 0;
          }

          // compute positive gradient =  outer product of v & h
          int[][] posGrad = OuterProd(visValues, hidValues);

          // reconstruct visual Nodes as v'
          int[] vPrime = new int[numVisible];  // v' in Wikipedia
          for (int v = 0; v < numVisible; ++v)
          {
            double sum = 0.0;
            for (int h = 0; h < numHidden; ++h)
              sum += hidValues[h] * vhWeights[v][h];
            sum += visBiases[v]; // add visible bias
            double probActiv = LogSig(sum);
            double pr = rnd.NextDouble();
            if (probActiv > pr)
              vPrime[v] = 1;
            else
              vPrime[v] = 0;
          }

          // compute new hidden Nodes as h', using v'
          int[] hPrime = new int[numHidden];
          for (int h = 0; h < numHidden; ++h)
          {
            double sum = 0.0;
            for (int v = 0; v < numVisible; ++v)
              sum += vPrime[v] * vhWeights[v][h];
            sum += hidBiases[h]; // add the hidden bias
            double probActiv = LogSig(sum); // apply activation
            double pr = rnd.NextDouble();  // determine 0/1 node value
            if (probActiv > pr)
              hPrime[h] = 1;
            else
              hPrime[h] = 0;
          }

          // compute negative grad using v' and h'
          int[][] negGrad = OuterProd(vPrime, hPrime);

          // update weights
          for (int row = 0; row < numVisible; ++row)
            for (int col = 0; col < numHidden; ++col)
              vhWeights[row][col] += learnRate * (posGrad[row][col] - negGrad[row][col]);
 
          // update visBiases
          for (int v = 0; v < numVisible; ++v)
            visBiases[v] += learnRate * (visValues[v] - vPrime[v]);
          // update hidBiases
          for (int h = 0; h < numHidden; ++h)
            hidBiases[h] += learnRate * (hidValues[h] - hPrime[h]);

        } // for-each train data

        ++epoch;
      } // while
    } // Train

    public static int[][] OuterProd(int[] visValues, int[] hidValues)
    {
      int rows = visValues.Length;
      int cols = hidValues.Length;
      int[][] result = new int[rows][];
      for (int i = 0; i < rows; ++i)
        result[i] = new int[cols];

      for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
          result[i][j] = visValues[i] * hidValues[j];

      return result;
    }

    public double LogSig(double x)
    {
      if (x < -20.0) return 0.0000000001;
      else if (x > 20.0) return 0.9999999999;
      else return 1.0 / (1.0 + Math.Exp(-x));
    }

    public void Shuffle(int[] indices)
    {
      for (int i = 0; i < indices.Length; ++i)
      {
        int ri = rnd.Next(i, indices.Length);
        int tmp = indices[i];
        indices[i] = indices[ri];
        indices[ri] = tmp;
      }
    }

    public int[] HiddenFromVis(int[] visibles)
    {
      int[] result = new int[numHidden];
      
      for (int h = 0; h < numHidden; ++h)
      {
        double sum = 0.0;
        for (int v = 0; v < numVisible; ++v)
          sum += visibles[v] * vhWeights[v][h];
 
        sum += hidBiases[h]; // add the hidden bias
        double probActiv = LogSig(sum); // compute prob of h activation
        // Console.WriteLine("Hidden [" + h + "] activation probability = " + probActiv.ToString("F4"));
        double pr = rnd.NextDouble();  // determine 0/1 h node value
        if (probActiv > pr)
          result[h] = 1;
        else
          result[h] = 0;
      }
      return result;
    }

    public int[] VisibleFromHid(int[] hiddens)
    {
      int[] result = new int[numVisible];

      for (int v = 0; v < numVisible; ++v)
      {
        double sum = 0.0;
        for (int h = 0; h < numHidden; ++h)
          sum += hiddens[h] * vhWeights[v][h];
        sum += visBiases[v]; // add visible bias
        double probActiv = LogSig(sum);
        // Console.WriteLine("Visible [" + v + "] activation probability = " + probActiv.ToString("F4"));
        double pr = rnd.NextDouble();
        if (probActiv > pr)
          result[v] = 1;
        else
          result[v] = 0;
      }
      return result;
    }

    public void Dump(bool showValues, bool showWeights, bool showBiases)
    {
      if (showValues == true)
      {
        for (int i = 0; i < numVisible; ++i)
        {
          Console.Write("visible node [" + i + "] value = " + visValues[i]);
          Console.WriteLine("  prob = " + visProbs[i].ToString("F4"));
        }
        Console.WriteLine("");

        for (int j = 0; j < numHidden; ++j)
        {
          Console.Write("hidden node [" + j + "] value = " + hidValues[j]);
          Console.WriteLine("  prob = " + hidProbs[j].ToString("F4"));
        }
        Console.WriteLine("");
      }

      if (showWeights == true)
      {
        for (int i = 0; i < numVisible; ++i)
        {
          for (int j = 0; j < numHidden; ++j)
          {
            double x = vhWeights[i][j];
            if (x >= 0.0)
              Console.Write(" ");
            Console.Write(vhWeights[i][j].ToString("F4") + "  ");
          }
          Console.WriteLine("");
        }
        Console.WriteLine("");
      }

      if (showBiases == true)
      {
        for (int i = 0; i < numVisible; ++i)
          Console.WriteLine("visible bias [" + i + "] value = " +
            visBiases[i].ToString("F4"));
        Console.WriteLine("");

        for (int j = 0; j < numHidden; ++j)
          Console.WriteLine("hidden bias [" + j + "] value = " +
            hidBiases[j].ToString("F4"));
        Console.WriteLine("");
      }
    } // Dump

  } // Boltzmann Machine

} // ns
