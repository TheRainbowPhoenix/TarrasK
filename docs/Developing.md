# Hacking into TarrasK
## Prerequisites

You'll need a Cross Compiling env for `sh4eb-nofpu-elf`.
An up-to-date tutorial is available [here](https://github.com/SnailMath/hollyhock-2/blob/master/doc/user/developing.md)

> Note: The tutorial builds "sh4-elf" but it's exactly the same than "sh4eb-nofpu-elf". You can alias in in the "[Config.mk](Config.mk)"
 
## Setting up environment variables
Most of the setup part is done in the "[Config.mk](Config.mk)" file. 

First, you'd need to change the "SDK_DIR" to the actual directory of your Classpad II SDK:
```
SDK_DIR:=~/hollyhock-2/sdk/
```

Then you'll need to update the PATH variable in order to get the GCC cross compiler working :
```
PATH:=~/opt/cross/bin:$(PATH)
```

You may then need to change the cross building toolset prefix:
```
PREFIX=sh4-elf
```

Finally ensure everything's compiling with `make all`

## Project Structure

- **Sourcecode** is always put into the [src](src) directory.
- Every **documentation** shall be in the [docs](docs) directory, in a Markdown (md) or Text (txt) format.
    - The picture and any other files related to the documentation have to be put into the "[docs/res](docs/res)" directory
- The **building directory** "target" should not be versioned. 
- Binaries files like the "TarrasK.hhk" should be placed on the "Github release" and not versioned.
- Some "[tests](tests)" directory is available for you to test and play with snippets of code. It was at first an attempt to add unit testing but then I realised simple program could be really useful to understand more about c and c++ so I let then that way. Feel free to ignore this director for now.

## IDE configuration

You should be able to get this project into [CLion](https://www.jetbrains.com/clion/) if you setup the "WSL" toolchain with the proper paths.
Open the project as a makefile one and host it locally.

More work has to be done for CLion to detect and add libraries and do some proper completion.
If you have some solid IDE configuration, feel free to PR this project with the config...

