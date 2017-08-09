Prerequisites:
sudo apt install libyaml-cpp-dev libsfml-dev

To build:
mkdir build
cd build

And then there are two options.

To build in debug mode for testing:
cmake ../ -DCMAKE_BUILD_TYPE=DEBUG
make

To build in release mode for better performance:
cmake ../ -DCMAKE_BUILD_TYPE=RELEASE
make

After which you can run the program with
./Swoop

You can remove the build folder at any time to clean up and start over.
