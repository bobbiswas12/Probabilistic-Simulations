build:
	gcc pi/pi.c -o Pi -lraylib -lm -lpthread -ldl -lrt
	gcc random_walks/main.c -o random_walk -lraylib -lm -lpthread -ldl -lrt
