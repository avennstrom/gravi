# Gravi

Small puzzle game written in C targeting Linux and the web (HTML5/WASM/JavaScript).

![Screenshot](docs/screenshot.png)

# Building

## Linux

### Prerequisites
* GCC
* SDL2

Build
```
make linux
```
Enjoy
```
./gravi
```

## Web

### Prerequisites
* Emscripten

Build
```
$ make web
```
Run the test server
```
$ ./serve.py
```
Navigate to http://localhost:8000/gravi.html
