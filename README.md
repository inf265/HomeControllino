## Template for cmake with arduino, made for Controllino

In order to compile the Arduino stuff, configure the project

    ```sh
    cmake -S. -Bbuild -D ARDUINO_PORT=/dev/ttyACM0 \
    -D CMAKE_TOOLCHAIN_FILE=cmake/toolchain/controllino.mega.toolchain.cmake -D CMAKE_BUILD_TYPE=MinSizeRel
    ```

end then build the artifacts with

    ```sh
    cmake --build build -j -t all
    ```

Finally, build and upload the example “blink” program:
```sh
    cmake --build build -j -t upload-blink
```

To compile the program without uploading, you can use 

```sh
    cmake --build build -j -t blink
```

If you're using an Arduino with a native USB interface (e.g. Leonardo),
you'll have to press the reset button before uploading. You could 
automate this by opening its serial port at 1200 baud as part of the 
upload process.
