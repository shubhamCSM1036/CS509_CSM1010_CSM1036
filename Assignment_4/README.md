# CS509 – Assignment 4 Buddy Task

## K-Means Clustering and FastMap

This repository contains the implementation of the **buddy component of CS509 Assignment 4**.

The buddy task focuses on the implementation and performance evaluation of:

- K-Means Clustering
- FastMap

Both algorithms were tested using multiple input sizes and their execution times were measured.

---

## Assignment Objective

The objective of this assignment is to implement and evaluate **K-Means Clustering** and **FastMap** algorithms.

The performance of both algorithms is evaluated using different input sizes, with execution time measured for each test case.

### K-Means Clustering

K-Means partitions a set of input points into `K` clusters.

The algorithm repeatedly:

1. Assigns every point to its nearest centroid.
2. Recalculates the centroid of each cluster.
3. Repeats the process until convergence or the maximum number of iterations is reached.

The implementation reports:

- Cluster assignments
- Final centroids
- WCSS
- Number of iterations
- Convergence status
- Execution time

### FastMap

FastMap maps objects into a lower-dimensional coordinate space while attempting to preserve the original distance relationships.

The implementation performs the FastMap projection process and reports:

- Target dimension
- Average distance error
- Execution time
- Test status

---

## Repository

GitHub repository:

https://github.com/dh33rajd3v/CS509_CSM1010_CSM1036

---

## Test Cases

### K-Means

The K-Means benchmark cases used in the reported evaluation are:

```text
km_01.txt
km_02.txt
km_03.txt
km_04.txt
```

The reported datasets contain between 6 and 100,000 points, with dimensions ranging from 2 to 5 and cluster counts ranging from 2 to 10.

### FastMap

The reported FastMap test cases are:

```text
fm_01.txt
fm_02.txt
fm_03.txt
```

The reported tests use:

- N = 10, k = 2
- N = 100, k = 2
- N = 1,000, k = 3

---

## K-Means Results

| Test Case | N | D | K | Max Iter | Actual Iter | Converged | WCSS | Time (ms) | Status |
|-----------|--:|--:|--:|---------:|------------:|:---------:|-----:|----------:|:------:|
| `km_01.txt` | 6 | 2 | 2 | 300 | 3 | Yes | 7.875 | 0.0109 | Pass |
| `km_02.txt` | 1,000 | 2 | 5 | 100 | 2 | Yes | 1988.69622 | 0.0394 | Pass |
| `km_03.txt` | 10,000 | 5 | 8 | 100 | 6 | Yes | 49502.91445 | 2.9680 | Pass |
| `km_04.txt` | 100,000 | 5 | 10 | 100 | 7 | Yes | 496193.2366 | 43.8986 | Pass |

All four reported K-Means test cases passed successfully.

---

## K-Means Observations

The execution time increases as the number of input points increases.

| N | Time (ms) | Iterations |
|---:|----------:|-----------:|
| 6 | 0.0109 | 3 |
| 1,000 | 0.0394 | 2 |
| 10,000 | 2.9680 | 6 |
| 100,000 | 43.8986 | 7 |

The largest dataset containing 100,000 points completed in approximately 43.9 ms and converged after 7 iterations.

The number of iterations does not increase directly with the number of points. The increase in execution time is primarily caused by the larger number of points and dimensions processed during each iteration.

WCSS also increases with dataset size because it represents the accumulated squared distance of points from their respective centroids.

---

## FastMap Results

| Test Case | N | k | Average Distance Error | Time (ms) | Status |
|-----------|--:|--:|-----------------------:|----------:|:------:|
| `fm_01.txt` | 10 | 2 | 0 | 0.009700 | Pass |
| `fm_02.txt` | 100 | 2 | 1.6 | 0.304000 | Pass |
| `fm_03.txt` | 1,000 | 3 | 333.666667 | 15.668200 | Pass |

The reported FastMap test cases completed successfully.

---

## FastMap Observations

FastMap execution time increases significantly as the number of objects increases.

| N | k | Time (ms) |
|---:|---:|----------:|
| 10 | 2 | 0.0097 |
| 100 | 2 | 0.3040 |
| 1,000 | 3 | 15.6682 |

The increase becomes particularly noticeable between 100 and 1,000 objects.

This is expected because FastMap performs distance calculations as part of pivot selection, projection, and distance deflation.

The target dimension also affects computation. The 1,000-object test uses `k = 3`, while the smaller cases use `k = 2`.

The average distance error increases for the larger test cases because FastMap produces an approximation of the original pairwise distance relationships.

---

## Performance Summary

### K-Means

The K-Means implementation successfully handled the reported datasets up to 100,000 points.

The largest test case:

```text
N = 100,000
D = 5
K = 10
Iterations = 7
Time = 43.8986 ms
Status = Pass
```

The measured results show that execution time increases substantially as the dataset size increases.

### FastMap

The FastMap implementation successfully processed the reported test cases up to 1,000 objects.

The largest reported test case:

```text
N = 1,000
k = 3
Average Distance Error = 333.666667
Time = 15.6682 ms
Status = Pass
```

FastMap showed a much more noticeable increase in execution time as the number of objects increased.

---

## Execution Time

The reported execution times demonstrate the computational cost of both algorithms as the input size increases.

For K-Means, the main cost comes from calculating distances between points and centroids during each iteration.

For FastMap, the computation includes pivot selection, projection, and distance deflation, resulting in a significant increase in runtime as the number of objects grows.

The timing results are machine-dependent and should be interpreted relative to the environment in which the programs were executed.

---

## Test Case Summary

| Algorithm | Reported Test Cases | Largest Reported Input | Status |
|-----------|--------------------:|------------------------:|:------:|
| K-Means | 4 | 100,000 points | Pass |
| FastMap | 3 | 1,000 objects | Pass |

All reported test cases completed successfully.

---

## Conclusion

The CS509 Assignment 4 Buddy Task successfully implements and evaluates **K-Means Clustering** and **FastMap**.

K-Means successfully converged for all four reported benchmark cases, including the largest dataset containing 100,000 points. The results demonstrate increasing execution time with increasing dataset size while maintaining successful convergence.

FastMap successfully processed the reported benchmark cases and demonstrated the expected increase in execution time as the number of objects and target dimensions increased. The distance error also increased for the larger datasets, reflecting the approximate nature of the lower-dimensional representation.

Overall, the reported experiments demonstrate successful implementation, execution, and performance evaluation of both K-Means Clustering and FastMap.
