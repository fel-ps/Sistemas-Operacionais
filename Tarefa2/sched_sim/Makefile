
OBJS = main.o \
	   proc.o \
	   queue.o \
	   sched.o \
	   utils.o \
	   time.o \
	   thread.o \
	   stats.o 

default: fifo

all: fifo sjf ljf prio_static prio_dynamic prio_dynamic_quantum

fifo: $(OBJS) scheduler_fifo.o proc_init.o proc_interrupt.o
	gcc -pthread $(OBJS) scheduler_fifo.o proc_init.o proc_interrupt.o -o main_fifo

sjf: $(OBJS) scheduler_sjf.o proc_init.o proc_interrupt.o
	gcc -pthread $(OBJS) scheduler_sjf.o proc_init.o proc_interrupt.o -o main_sjf

ljf: $(OBJS) scheduler_ljf.o proc_init.o proc_interrupt.o
	gcc -pthread $(OBJS) scheduler_ljf.o proc_init.o proc_interrupt.o -o main_ljf

prio_static: $(OBJS) scheduler_prio_static.o proc_init.o proc_interrupt.o
	gcc -pthread $(OBJS) scheduler_prio_static.o proc_init.o proc_interrupt.o -o main_prio_static

prio_dynamic: $(OBJS) scheduler_prio_dynamic.o proc_init.o proc_interrupt.o
	gcc -pthread $(OBJS) scheduler_prio_dynamic.o proc_init.o proc_interrupt.o -o main_prio_dynamic

prio_dynamic_quantum: $(OBJS) scheduler_prio_dynamic_quantum.o proc_init.o proc_interrupt.o
	gcc -pthread $(OBJS) scheduler_prio_dynamic_quantum.o proc_init.o proc_interrupt.o -o main_prio_dynamic_quantum

main.o: main.c
	gcc -c main.c

proc.o: proc.c proc.h
	gcc -c proc.c

proc_init.o: proc_init.c proc_init.h
	gcc -c proc_init.c

proc_interrupt.o: proc_interrupt.c proc_interrupt.h
	gcc -c proc_interrupt.c

queue.o: queue.c queue.h
	gcc -c queue.c

sched.o: sched.c sched.h
	gcc -c sched.c

utils.o: utils.c utils.h
	gcc -c utils.c

time.o: time.c time.h
	gcc -c time.c

thread.o: thread.c thread.h
	gcc -c thread.c

stats.o: stats.c stats.h
	gcc -c stats.c

scheduler_fifo.o: scheduler_fifo.c scheduler.h
	gcc -c scheduler_fifo.c

scheduler_sjf.o: scheduler_sjf.c scheduler.h
	gcc -c scheduler_sjf.c

scheduler_ljf.o: scheduler_ljf.c scheduler.h
	gcc -c scheduler_ljf.c

scheduler_prio_static.o: scheduler_prio_static.c scheduler.h
	gcc -c scheduler_prio_static.c

scheduler_prio_dynamic.o: scheduler_prio_dynamic.c scheduler.h
	gcc -c scheduler_prio_dynamic.c

scheduler_prio_dynamic_quantum.o: scheduler_prio_dynamic_quantum.c scheduler.h
	gcc -c scheduler_prio_dynamic_quantum.c

clean:
	rm -f *.o
	rm -f main_fifo
	rm -f main_sjf
	rm -f main_ljf
	rm -f main_prio_static
	rm -f main_prio_dynamic
	rm -f main_prio_dynamic_quantum

