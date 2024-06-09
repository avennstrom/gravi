# Gravi

Small puzzle game written in C targeting native Linux and the web (HTML5/WASM/JavaScript).

# Building

## Linux

### Prerequisites
* GCC
* SDL2

```
make linux
./gravi
```

## Web

### Prerequisites
* Emscripten

```
make web
```

Run the test server
```
./serve.py
```
Navigate to http://localhost:8000/gravi.html