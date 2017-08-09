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
* Implement neighbour heap (In Progress)
* Implement kdtrees for filtering
* Implement graph-clustering
* Combine them all to perform an EFANNA KNN search

Currently only linux is supported for testing, to take full advantage of the
testing options with clang for this platform. Release builds for
other platforms should work without issues.
