## Running ##

```
cd MAC5742-0219-EP1/src
sudo ./run-measurements.sh <iterations> <k>
```

Where `iterations` will set the upper bound for the image size (with the same semantics as the `ITERATIONS` variable at `run_measurements.sh`), while `k` will set the upper bound on the number of threads (measurements will be done between 2 and 2 ** k threads)

## Modifications from the original repository ##

The pthreads version accepts the number of threads as an extra argument. With OpenMP, we set the number of threads via the `OMP_NUM_THREADS` environment variable, which the `run_measurements.sh` exports.