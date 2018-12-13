# Forbidden Desert

## Installing

### Dependencies
```
sudo apt-get install liballegro5-dev
sudo apt-get install freeglut3-dev libftgl-dev
```

### Debugging allegro 5
Inside `testingAllegro/` folder you can run:
```
gcc hello.c -o hello $(pkg-config allegro-5 allegro_font-5 --libs --cflags)
```
