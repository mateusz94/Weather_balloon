# List of all the board related files.
SENSORSSRC = $(CHIBIOS)/sensors/sensor_hub.c \
			$(CHIBIOS)/sensors/bmp085/bmp085.c \
			$(CHIBIOS)/sensors/bmp085/bmp085_lld.c \
			$(CHIBIOS)/sensors/lsm330/lsm330.c

# Required include directories
SENSORSINC = $(CHIBIOS)/sensors \
			$(CHIBIOS)/sensors/bmp085 \
			$(CHIBIOS)/sensors/lsm330
