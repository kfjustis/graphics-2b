# Build instructions
The easiest way to setup the dev environment is with ```vcpkg```. Install
it from Microsoft's repo here:

    https://github.com/Microsoft/vcpkg

Then, move to the directory containing the ```vcpkg``` executable and run
the following commands...

    vcpkg install glew
    vcpkg install glfw3
    vcpkg install opengl
    vcpkg integrate install

Now, all of the necessary libraries are installed and will be automatically
included by the Visual Studio project. From here, you can open up the
solutions in Visual Studio and run them. Pre-built binaries can also be found
in the ```bin.zip``` file for each part.

### NOTE: Make sure the build target is set to x86!
