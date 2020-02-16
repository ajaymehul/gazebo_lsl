# gazebo_lsl

This is a Gazebo plugin that allows models to stream data to Lab Streaming Layer.
Tested on Ubuntu  18.04.3


## Prerequisites

 - Gazebo 9.0.0 [Installation Intructions](http://gazebosim.org/tutorials?tut=install_ubuntu&cat=install)
 - liblsl 13.0.0 [Releases Link](https://github.com/sccn/liblsl/releases) [Tested: Release b-12]
 - Ubuntu 18.04.x LTS [Download](https://ubuntu.com/download/desktop)
 
## Build Instructions

Make sure Gazebo and liblsl are properly installed.  Use CMake to build the plugin. 

To find the appropriate Gazebo libraries and link them, use the following:
   ```cpp
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")
```

To find and link liblsl, use the following:
   ```cpp
find_package(LSL REQUIRED
      HINTS ${LSL_INSTALL_ROOT}
      "${CMAKE_CURRENT_LIST_DIR}/../../LSL/liblsl/build/"
      "${CMAKE_CURRENT_LIST_DIR}/../../LSL/liblsl/build/install"
      PATH_SUFFIXES share/LSL
)
```
Then you can link your target with Gazebo and liblsl with:

   ```cpp
target_link_libraries(${PROJECT_NAME} PUBLIC ${GAZEBO_LIBRARIES} LSL::lsl)
```
    


## Usage

he built plugin to any model in your gazebo world by adding the plugin tag in your world file. The plugin uses the Gazebo model ID as the source_id for LSL, so you can add as many instances of the plugin to different models in the world.

The built plugin from the example can be added using:

    <plugin name="lsl_publish" filename="liblsl_publish.so"/>

The example publishes the XYZ coordinates of the model. You can customize the data published by making changes to the `OnUpdate()` method in the plugin.  

## Running the example

 - Make sure Gazebo and liblsl are installed properly.  
 - Clone this repository and go into the directory.c
 `$ git clone https://github.com/ajaymehul/gazebo_lsl.git ~/gazebo_lsl`
 - Create the `build` directory:
`$ mkdir ~/gazebo_lsl/build`
`$ cd ~/gazebo_lsl/build`

 - Compile the code
`$ cmake ../`
`$ make`
- This will compile the plugin to ``~/gazebo_lsl/build/libhello_world.so``
- Export the plugin path so that Gazebo can use it
`$ export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:~/gazebo_plugin_tutorial/build`
- Launch the example world using
` gzserver ~/gazebo_lsl/example.world --verbose`
- Use [Lab Recorder](https://github.com/labstreaminglayer/App-LabRecorder) or any another app to get the data.


