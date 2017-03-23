all:
	gcc serialtester.c Lab4_IO.c -o serialtester -lm
	mpicc -g -Wall -o main main.c Lab4_IO.c -lm
	mpicc -g -Wall -o main1 main2.c Lab4_IO.c -lm
	mpicc -g -Wall -o main3 main3.c Lab4_IO.c -lm
	mpicc -g -Wall -o main4 main4.c Lab4_IO.c -lm
	gcc datatrim.c -o datatrim
