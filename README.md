#EFANNA k-nearest-neighbours search in C++.

###Objective:
This should be significantly more efficient than using standard approaches like a KDTree or NN-expansion with graphs because it uses a hybrid approach to take advantage of the best features of both options. (Fast KDTree builds for small datasets, scalable NN-Expansion performance for arbitrarily large datasets). Vectorized distance calculations with SSE and multithreaded builds also speed up use significantly.

No public interface exists yet, this is just a prototype for testing at the moment.

You can compile by running
```
./configure
./compile
```
And then run `./efanna` to try the program.

If anything goes wrong or you change the CMakeLists.txt file, run `./configure` again to clean the build folder and restart.

Alternatively:

To build in debug mode for testing:

```
mkdir build
cd build
cmake ../ -DCMAKE_BUILD_TYPE=DEBUG
make
```

The output is in the executable file 'efanna'

To build in release mode for better performance, replace the cmake
command with:
```
cmake ../ -DCMAKE_BUILD_TYPE=RELEASE
```

You can remove the build folder at any time to clean up and start over.

Roadmap:
* Implement bounded heap (Done)
* Implement kdtrees for filtering (Done)
* Implement nearest neighbour expansion (In Progress)
* Combine them all to perform faster knn search
* Add cmake build options to support disabling sanitize for cachegrind. Disable unavailable debug flags for mac.
* Add benchmarks, compare to the original program and other implementations of KNN.
* Make KDTree builds and graph builds parallel for better performance.
* Create public API

Currently only linux is supported for testing, to take full advantage of the
testing options with clang for this platform. Release builds for
other platforms should work without issues.
