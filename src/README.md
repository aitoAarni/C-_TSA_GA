# Instructions for Running the Code

These instructions are for Linux environments. The commands work if you are executing them from the src directory.

### Generetaing input data

To generate input data, run the following command in the terminal:

```bash
python3 generate_input.py
```

### Running the code

First you have to build the code using the following command:

```bash
make build
```

Then you can run the code using the following command:

```bash
./tsp_solver
```

You have additional command line arguments:

```bash
-t <number of threads> : specify the number of threads to use (default is 1)
-i <input file> : specify the input file (default is ../run/input.dat)
-g <number of generations> : specify the number of generations to run (default is 100)
-p <population size> : specify the population size (default is 100)
-m <migration interval> : specify the migration interval (default is 50)

```
