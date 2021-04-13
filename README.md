# ogdf-demo
A collection of a few simple examples that make use of the [OGDF](https://ogdf.uos.de/).

## Building
Clone the directory with submodules:
```bash
git clone --recurse-submodules https://github.com/leon-richardt/ogdf-demo.git
```

In the newly cloned repository, create a build directory and generate the files your build system uses:
```bash
mkdir build && cd build && cmake ..
```
(Of course, you can pass additional flags to `cmake` if you like.)

Finally, build the project.
With Unix Makefiles, this is accomplished by simply executing
```bash
make
```
in the build directory.
Since the OGDF will be built as well, we suggest using the `-j` argument to `make`.

## Running
After building, the executables will be located in `build/bin`.
The `draw` executable writes two files (`output-naive.svg` and `output-planar.svg`) to the current working directory.
The executables prefixed with `io_` take the path to a GraphML file (of the same schema as `data/example.graphml`).
The `random_dijkstra` executable generates a random graph and expects two arguments: the number of nodes and the number of edges the random graph should have.<sup>[1](#footnote-args)</sup>


<a name="footnote-args">1</a>: Since we generate a random _connected_ graph, the number of nodes may not be greater than the number of edges.
