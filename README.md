# Resize pictures with some help frome Pthread

__compile:__

```
make clean
make
```

__execute:__

```sh
# resize all pictures in pics folder into 512x512 size
./app pics 512 512
# resize all pictures in pics folder into 512x512 size, using 12 threads
./app pics 512 512 12
```

__help:__

```
$ ./app
format:  ./app path cols rows [n_thread]
example: ./app pics 1920 1080 8  
example: ./app pics 1920 1080
```
