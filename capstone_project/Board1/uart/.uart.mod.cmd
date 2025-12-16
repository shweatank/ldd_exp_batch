savedcmd_/home/pi/spi_displaytest/uart/uart.mod := printf '%s\n'   uart.o | awk '!x[$$0]++ { print("/home/pi/spi_displaytest/uart/"$$0) }' > /home/pi/spi_displaytest/uart/uart.mod
