main: main.c
	gcc mpc.c main.c -o main -std=c11 -ledit -Wall -lm

his: his.c
	gcc mpc.c his.c -o his -std=c99 -ledit -Werror -Wall -lm

aorb: aorb.c
	gcc mpc.c aorb.c -o aorb -std=c99 -ledit -Werror -Wall -lm

consec: consec.c
	gcc mpc.c consec.c -o consec -std=c99 -ledit -Werror -Wall -lm

words: words.c
	gcc mpc.c words.c -o words -std=c99 -ledit -Werror -Wall -lm
