project2 : main.o route-records.o
	gcc main.o route-records.o -o project2

main.o : main.c route-records.h
	gcc -c main.c

route-records.o : route-records.c route-records.h
	gcc -c route-records.c

clean:
    rm -rf *.o
    rm project2