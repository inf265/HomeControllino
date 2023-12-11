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




<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta charset="utf-8">

  <title>JSONEditor | Basic usage</title>

  <link href="https://cdn.jsdelivr.net/npm/jsoneditor@8.6.3/dist/jsoneditor.css" rel="stylesheet" type="text/css">
  <script src="https://cdn.jsdelivr.net/npm/jsoneditor@8.6.3/dist/jsoneditor.js"></script>

  <style type="text/css">
    #jsoneditor {
      width: 500px;
      height: 500px;
    }
  </style>
</head>
<body>
<p>
  <button id="setJSON">Set JSON</button>
  <button id="getJSON">Get JSON</button>
</p>
<div id="jsoneditor"></div>

<script>
  // create the editor
  const container = document.getElementById('jsoneditor')
  const options = {}
  const editor = new JSONEditor(container, options)

  // set json
  document.getElementById('setJSON').onclick = function () {
    const json = {
      'array': [1, 2, 3],
      'boolean': true,
      'color': '#82b92c',
      'null': null,
      'number': 123,
      'object': {'a': 'b', 'c': 'd'},
      'time': 1575599819000,
      'string': 'Hello World',
      'onlineDemo': 'https://jsoneditoronline.org/'
    }
    editor.set(json)
  }

  // get json
  document.getElementById('getJSON').onclick = function () {
    const json = editor.get()
    alert(JSON.stringify(json, null, 2))
  }
</script>
</body>
</html>