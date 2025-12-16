savedcmd_/home/pi/Project/irsensor/ir_sensor_char.mod := printf '%s\n'   ir_sensor_char.o | awk '!x[$$0]++ { print("/home/pi/Project/irsensor/"$$0) }' > /home/pi/Project/irsensor/ir_sensor_char.mod
