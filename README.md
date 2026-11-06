# ColorDial

A hands-on RGB and HSI color mixer with live LCD feedback and a 25-pixel WS2812B matrix.

## Build and upload

This project uses PlatformIO. Required libraries are listed in `platformio.ini`.

```sh
pio run
pio run -t upload
pio device monitor -b 115200
```

The Makefile also provides `make build`, `make upload`, and `make monitor`.
