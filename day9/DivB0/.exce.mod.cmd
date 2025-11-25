savedcmd_exce.mod := printf '%s\n'   exce.o | awk '!x[$$0]++ { print("./"$$0) }' > exce.mod
