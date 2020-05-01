all: thread1 thread2 thread3 thread4 thread5 thread6
.PHONY:all
obj1 = thread1.c
obj2 = thread2.c
obj3 = thread3.c
obj4 = thread4.c
obj5 = thread5.c
obj6 = thread6.c
thread1:$(obj)
	gcc -o thread1 $(obj1) -g -lpthread
thread2:$(obj)
	gcc -o thread2 $(obj2) -g -lpthread	
thread3:$(obj)
	gcc -o thread3 $(obj3) -g -lpthread -D_REENTRANT	
thread4:$(obj)
	gcc -o thread4 $(obj4) -g -lpthread -D_REENTRANT	
thread5:$(obj)
	gcc -o thread5 $(obj5) -g -lpthread -D_REENTRANT	
thread6:$(obj)
	gcc -o thread6 $(obj6) -g -lpthread -D_REENTRANT			
.PHONY:clean
clean:
	rm -rf *.o thread1 thread2 thread3 thread4 thread5 thread6
