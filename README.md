# Forbidden Desert

## Installing

### Dependencies
```
sudo apt-get install liballegro5-dev
sudo apt-get install freeglut3-dev libftgl-dev
sudo apt-get install liballegro-image5-dev liballegro-ttf5-dev liballegro-audio5-dev liballegro-video5-dev liballegro-physfs5-dev liballegro-acodec5-dev liballegro-acodec5-dev liballegro-dialog5-dev
```

### Debugging allegro 5
Inside `testingAllegro/` folder you can run:
```
gcc hello.c -o hello $(pkg-config allegro-5 allegro_font-5 --libs --cflags)
```
