An implementation of EFANNA k-nearest-neighbours search in C++. This should be significantly more efficient than using standard approaches like a KDTree or NN-expansion with graphs.
No public interface exists yet, this is just a prototype for testing at the moment.

You can compile by running
```
./configure
./compile
```
And then ./efanna to try the program.

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
* Implement graph-clustering
* Combine them all to perform faster knn search
* Improve testing strategy - accuracy of results is pseudorandom, but given a set seed the output will be deterministic.
* Add cmake build options to support disabling sanitize for cachegrind, or disabling specific flags for mac.
* Add benchmarks, compare to the original program and other implementations of KNN.
* Make KDTree builds and graph builds parallel for better performance.

Currently only linux is supported for testing, to take full advantage of the
testing options with clang for this platform. Release builds for
other platforms should work without issues.
