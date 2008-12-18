CC=gcc
CFLAGS=`sdl-config --cflags` -Wall
LFLAGS=`sdl-config --libs`

# run
runwindowed: executable
	./motrs --windowed --testmap test.map

run: executable
	./motrs

executable: motrs resource/bmp.dat resource/tile.dat resource/map.dat resource/ani.dat resource/char.dat

# link
motrs: src/motrs.o src/gameplay.o src/resource.o src/debug.o
	$(CC) -o motrs $(LFLAGS) src/motrs.o src/gameplay.o src/resource.o src/debug.o

# compile
src/motrs.o: src/motrs.c
	$(CC) $(CFLAGS) -c src/motrs.c -o src/motrs.o

src/gameplay.o: src/gameplay.c src/gameplay.h
	$(CC) $(CFLAGS) -c src/gameplay.c -o src/gameplay.o

src/resource.o: src/resource.c src/resource.h
	$(CC) $(CFLAGS) -c src/resource.c -o src/resource.o

src/debug.o: src/debug.c src/debug.h
	$(CC) $(CFLAGS) -c src/debug.c -o src/debug.o


# resources

resource/bmp.dat: resource/bitmaps/**
	tools/sjsresource resource/bitmaps resource/bmp.dat

resource/tile.dat: resource/tiles/**
	tools/sjsresource resource/tiles resource/tile.dat

resource/map.dat: resource/maps/**
	tools/sjsresource resource/maps resource/map.dat

resource/ani.dat: resource/animations/**
	tools/sjsresource resource/animations resource/ani.dat

resource/char.dat: resource/characters/**
	tools/sjsresource resource/characters resource/char.dat

# compiling text formats to binary formats

resource/animations/**: animations/**
	tools/compile_animations.pl

resource/tiles/**: tiles/**
	tools/compile_props.pl tiles resource/tiles

resource/characters/**: characters/**
	tools/compile_props.pl characters resource/characters

# clean

clean:
		rm -f motrs motrs.exe src/*.o resource/*.dat resource/animations/*.ani resource/tiles/*.tile resource/characters/*.char

