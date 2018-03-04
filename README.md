
This is a playground project, exploring the integration of [mruby](https://github.com/mruby/mruby) with [FreeRTOS](https://github.com/aws/amazon-freertos) and [libopencm3](https://github.com/libopencm3/libopencm3/) for the STM32F412ZG (available as a low-cost Nucleo developer board).

## Dependencies

- [GCC ARM compiler](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads) (preferably version 5 or higher)
- Gradle
- Ruby and standard tools for building mruby (see [mruby documentation](https://github.com/mruby/mruby/blob/master/doc/guides/compile.md))
- (optional) [stlink](https://github.com/texane/stlink) for flashing and debugging the Nucleo board

## Building and flashing

Run the following command to build the firmware (omit the `--info` for less verbose output).
```cmdline
gradle build --info
```
The gradle script also checks for the presence of the necessary library and if needed checks out their source code and builds them. These steps can also be performed manually beforehand:
```cmdline
git submodule init
git submodule update
cd libs/libopencm3 && make
```
For building mruby, a configuration file (`build_config.rb`) needs to be copied to the mruby root directory before calling `./minirake` inside the ruby directory. This repository supplies a `build_config.rb` example in the `libs/config` folder. The provided config file specifies the same compiler flags as used for building the rest of the firmware. It also configures the standard gems for math, printing and compiling mruby code. 

Finally, the firmware can be flashed to the Nucleo board (using st-flash from the [stlink project](https://github.com/texane/stlink)):
```cmdline
gradle flash
```

## Porting to other microcontrollers

A number of changes need to be performed when porting this example project to other STM32 microcontrollers. 

Note that the current configuration requires approximately 350kB of flash when building with debug symbols.

1. RAM and ROM sizes need to be adapted in the linker script (`generated.stm32f412zg.ld`)
2. The `main.cpp` file specifies the pins as used on the Nucleo-F412ZG board
3. Inside the `build.gradle` file, the C definition for the correct chip needs to be changed (in the `firmware(NativeExecutableSpec)` block)

In case the target processor is part of a different processor family, e.g., the STM32F3 family:

4. The definition of the family needs to be also changed inside the `build.gradle` file (similar to point 3 above).
5. A different version of `libopencm3` needs to be linked. This requires the definition of a new `libs(PrebuiltLibraries)` block with the new library file name (see below). Finally, both the C and C++ source sets need to be aware of the different library: `c.lib library: "opencm3_f3", linkage: "static"` and `cpp.lib library: "opencm3_f3", linkage: "static"`.

```groovy
opencm3_f3 {
    headers.srcDir "libs/libopencm3/include"
    binaries.withType(StaticLibraryBinary) {
        staticLibraryFile = file("libs/libopencm3/lib/libopencm3_stm32f3.a")
    }
}
```