# Ronin
Ronin is an application, aiming to be a hooking framework for Android. It is based on an technique called PLT overwriting, used mainly for offensive purposes. Ronin utilizes this for defensive purposes and in combination with some otherproperties of the Android system aims to be a modular hooking framework for non-rooted devices.

## Getting Started
In order to be able to compile the library one must download NDK files from https://developer.android.com/ndk/downloads/index.html.
After validating the checksums and extracting the files, cd into Ronin folder and run /path to android ndk folder/ndk-build X, where X can be:

- clean for cleaning object files and other local ones participating in the build process,
- NDK_DEBUG=0 or 1, depending on the level of the optimizations you want to omit,

Currently, we use the ndk-build instead of the cmake one. It will be replaced shortly.
Also, mips ABI is not supported yet.

## Debugging
If you want to debug the library without using Android Studio, it is recommended to install gef (https://github.com/hugsy/gef ) and the gdb-multiarch package from yours distro repository.
After successfully installing them, compile Ronin with NDK_DEBUG=1. This, will result in producing for every ABI a gdb.setup file and a gdbserver executable.
First, execute the command ```adb forward tcp:5039 tcp:5039``` in order to forward the traffic originating from the device.
Then copy gdbserver and gdb.setup into /data/local/tmp and attach or start the process you want to debug.
Finally, start gdb-multiarch and attach to port with number 5039.

## Roadmap
The roadmap of Ronin is:
- Fix bugs with the dt_* hashes in order to perform faster symbol searches ( right now a linear search is performed )
- Change the code to hook also code in the .dyn section ( That would be fairly easy. )
- Finish the implementation of an interface so as to make easier calls of Java code.
- Include the creation of a new thread, responsible for performing the hooks and pausing other threads (if they exist)
- Evaluate the option of performing the whole hooking procedure during the constructor of the library or during the JNI_onLoad
- Implement the parser for the config file in which information about the hooking procedure will be stored to, resulting in the automatization of the task.


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* gef (https://github.com/hugsy/gef) because it is awesome
* evilsocket for his arminject project as a partial inspiration
* frameworks such as Cydia Substrate and Frida
* a lot of people who have written articles about locating in memory ELF parts and PLT/GOT overwritting 
* gdb for being reliable and easy to install even in old systems.
