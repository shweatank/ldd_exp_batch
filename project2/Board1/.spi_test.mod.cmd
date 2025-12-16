savedcmd_/home/pi/spi_displaytest/spi_test.mod := printf '%s\n'   spi_test.o | awk '!x[$$0]++ { print("/home/pi/spi_displaytest/"$$0) }' > /home/pi/spi_displaytest/spi_test.mod
