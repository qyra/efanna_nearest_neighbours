Work in progress implementation of EFANNA in C++ because the existing
implementation was a bit messy.

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

Currently only linux is supported for testing, to take full advantage of the
testing options with clang for this platform. Release builds for
other platforms should work without issues.
