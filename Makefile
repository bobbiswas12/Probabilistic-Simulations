build:
	gcc pi/pi.c -o Pi -lraylib -lm -lpthread -ldl -lrt
	gcc random_walks/main.c -o random_walk -lraylib -lm -lpthread -ldl -lrt
	gcc linear_regression/main.c -o linear_regression_visual -lraylib -lm -lpthread
