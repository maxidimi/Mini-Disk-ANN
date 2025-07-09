# Mini-Disk-ANN

Project for the course Software Development for Information Systems, DIT UOA, Winter 2024.
Team members:

* Dimitris Dimochronis, [sdi2100034@di.uoa.gr](mailto:sdi2100034@di.uoa.gr),
* Nikos Pentasilis, [sdi2100152@di.uoa.gr](mailto:sdi2100152@di.uoa.gr),
* Giorgos Ragkos, [sdi2100167@di.uoa.gr](mailto:sdi2100167@di.uoa.gr).

This project consists of implementing algorithms described for DiskANN, <em>a fast accurate billion-point nearest neighbor search on a
single node</em>. For our project, we will experiment with datasets up to 1 million points. More about the algorithms implemented can be found in [Resources](#resources)


## Build and Run project

#### To **run** the project:

```
make run
```
The [configuration file](#configuration-file) is by default <em>config.txt</em>.
To run with other file name: ```./bin/vamana <file name>```

#### To run the **tests**:

```
make test
```

#### To **clean** the directory:

```
make clean
```

#### To check for memory leaks in tests:

```
make valgrind_test
```

## Configuration file

Parameters are provided via a configuration file, whose name is given as argument to the program.<br/>
The file must have the following format:

```
dataset=data/SIGMOD/DUMMY/dummy-data.bin
query=data/SIGMOD/DUMMY/dummy-queries.bin
groundtruth=data/SIGMOD/DUMMY/dummy-groundtruth.bin
vamana_function=filtered
graph_name=filtered_tmp
q_idx=
k=100
R=15
L=100
a=1.2
```

* The dataset/query/groundtruth specify the paths of these files.
* The <b>vamana\_function</b> determines the indexing & searching algorithm to run:

  * vamana, for simple Vamana
  * filtered, for Filtered Vamana,
  * stiched, for Stitched Vamana.
* <b>graph\_name</b> is the name of the graph we want to create or use.
* <b>k, R, L, a</b> are the well-known parameters for the functions.
* With <b>q\_idx</b> you can:

  * specify the index of the query in the file you want to test,
  * set -2 to test only one query, randomly chosen by the program,
  * set -3 to only create the index, without running queries,
  * set -4 to create the groundtruth from the dataset and queries, which will be saved with the name given in the corresponding field,
  * leave it empty to run for all queries.

## Performance

- For a dataset of 10,000 points, the algorithms show recall\@100 > 95%. Simple Vamana now requires 16 seconds for indexing, while Filtered and Stitched take 6 seconds.<br/>
- For a dataset of 1M points, the algorithms show recall\@100 > 97%. Simple Vamana never finishes, Filtered needs 12hr for indexing, and Stitched needs 13hr.<br/>
- For unfiltered queries, initial recall\@100 did not exceed 35%. This is due to the lack of edges between nodes with different filters, resulting in no access to the entire graph. To address this issue, the medoid for each filter in the search is taken as the result of Filtered Greedy Search for that filter with k = 1.
Thus, the search is more targeted, starting from the closest point for that filter.

## Greedy Search

#### Simple Greedy Search

* **L\_m\_V** computes the elements in L not in V.
* **greedy\_search** initializes sets L and V. While there are elements in L not in V, it finds the closest point to p using euclidean\_distance, adds the point's neighbors to L, and if L exceeds L\_s in size, it resizes to keep only the L-L\_s closest nodes. Finally, returns a pair of graphs: the first contains the k nearest nodes to q, and the second contains the visited nodes.

#### Filtered Greedy Search

* **filtered\_greedy\_search** initializes sets L and V. L is initialized with nodes s whose filter matches fq. While there are elements in L not in V, it finds the closest point to p using euclidean\_distance, initializes set N, adds neighbors of P with filter Fq not in V to N, adds nodes from N not already in L to L. If L exceeds L\_s, it resizes to keep only the L-L\_s closest nodes. Finally, returns a pair of graphs: the first with the k nearest nodes to q, and the second with the visited nodes.

## Robust Pruning

#### Simple Robust Pruning

* Argument p\_node is the node of point p in the graph.
* Adds all out edges of p to V, checking for duplicates.
* Removes all edges of p from the graph, then while V is not empty and |N\_out(p)| is not equal to R, computes the distance from p to each point in V and adds the closest one to N\_out(p).

#### Filtered Robust Pruning

* Same process as simple robust pruning, with two key differences. First, it has an extra argument, the vector<int> C, containing the filter of each point in the graph. Second, before checking for possible nodes to remove from V (i.e., to delete an edge), it checks if p, p\*, p' have the same filter and if not, it moves to the next point in V (so that it does not delete an edge between points with different filters).

## Vamana Indexing

#### Simple Vamana indexing

Creates a graph from points in dataset P and creates up to R random edges (if enough points exist). Then, finds the graph's medoid and traverses each point in the graph randomly. Runs Greedy Search with q being the current point in that iteration, passing the vector of visited nodes as an argument to Pruning. Then, for each out neighbor, creates reverse edges and prunes out neighbors if they exceed R in number. N\_out\_j\_p is used to convert the set of out-neighbors to a vector to pass to Robust Prune.

#### Filtered Vamana indexing

Creates an empty graph with points from dataset P. Finds the medoid for each filter, which is a random point with that filter, as each point in the dataset has only one filter. Traverses each point in the graph randomly. Runs Filtered Greedy Search with q being the current point in that iteration, passing the vector of visited nodes to Filtered Pruning. Then, for each out neighbor, creates reverse edges and prunes out neighbors if they exceed R in number. N\_out\_j is used to convert the set of out-neighbors to a vector to pass to Robust Prune.

#### Stitched Vamana indexing

For each filter, creates a graph from points with that filter using classic Vamana. Then, joins all graphs as follows:<br/>
   - Stores the mapping of each point in P to its index in the graph of its filter.<br/>
   - Stores the mapping of each point in each filter graph to its index in dataset P.<br/>
   - Adds all points to the final graph S\_stitched.<br/>
   - Using the first two structures, converts the out neighbor indices in each filter graph to match the stitched graph and dataset P.<br/>
Filtered Pruning is skipped, and R\_stitched is applied directly to Vamana since each point has only one filter, so it is not possible to create edges with points from other filters.

## Datasets

* Datasets for simple Vamana Indexing: [Corpus](http://corpus-texmex.irisa.fr/).<br/>
* Datasets for Filtered & Stiched Vamana Indexing: [DB Group](https://dbgroup.cs.tsinghua.edu.cn/sigmod2024/task.shtml?content=datasets).

## Resources

1. **Suhas Jayaram Subramanya, Devvrit, Rohan Kadekodi, Ravishankar Krishaswamy, and Harsha Vardhan Simhadri.** (2019).  
   DiskANN: fast accurate billion-point nearest neighbor search on a single node. Proceedings of the 33rd International Conference on Neural Information Processing Systems. Curran Associates Inc., Red Hook, NY, USA, Article 1233, 13766–13776.  
   [https://dl.acm.org/doi/abs/10.5555/3454287.3455520](https://dl.acm.org/doi/abs/10.5555/3454287.3455520)

2. **Siddharth Gollapudi, Neel Karia, Varun Sivashankar, Ravishankar Krishnaswamy, Nikit Begwani, Swapnil Raz, Yiyong Lin, Yin Zhang, Neelam Mahapatro, Premkumar Srinivasan, Amit Singh, and Harsha Vardhan Simhadri.** (2023).  
   Filtered-DiskANN: Graph Algorithms for Approximate Nearest Neighbor Search with Filters. In Proceedings of the ACM Web Conference 2023 (WWW '23). Association for Computing Machinery, New York, NY, USA, 3406–3416.  
   [https://doi.org/10.1145/3543507.3583552](https://doi.org/10.1145/3543507.3583552)
