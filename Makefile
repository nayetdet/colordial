.PHONY: run build upload monitor reset clean

BAUD_RATE ?= 115200

run: build upload monitor

build:
	pio run

upload:
	pio run -t upload

monitor:
	pio device monitor -b $(BAUD_RATE);

reset:
	pio run -t reset
	$(MAKE) monitor

clean:
	pio run -t clean
