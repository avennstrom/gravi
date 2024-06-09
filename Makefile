
all: web linux

web: gravi.tar gravi.js
linux: gravi

gravi.js: game.c
	emcc -o $@ -s EXPORTED_FUNCTIONS="['_malloc','_game_create','_game_update']" -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap']" $^
	wc -l gravi.js
	wc -l gravi.wasm

gravi.tar: gravi.js gravi.html gravi.wasm
	tar -cf $@ $^

gravi: main.c game.c
	gcc -std=c11 -g -o $@ $^ -lSDL2 -lm

clean:
	rm -f gravi
	rm -f gravi.js
	rm -f gravi.wasm
	rm -f gravi.wat
	rm -f gravi.tar