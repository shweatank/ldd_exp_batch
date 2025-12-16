savedcmd_/home/pi/Documents/pavan/i2c/i2c_driver.mod := printf '%s\n'   i2c_driver.o | awk '!x[$$0]++ { print("/home/pi/Documents/pavan/i2c/"$$0) }' > /home/pi/Documents/pavan/i2c/i2c_driver.mod
