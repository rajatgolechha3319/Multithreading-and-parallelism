#ifndef THREAD_H
#define THREAD_H
#define _XOPEN_SOURCE 600
#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include<signal.h>
#include<sys/time.h>
#include<unistd.h>
#include"list.h"

struct list *threads;  // List of all threads

void mythread_init(){
threads = list_new();  // Initialize threads list
};      
ucontext_t* mythread_self(){
// return the address of the running context
};  
ucontext_t* mythread_create(void func(void*), void* arg){  // Create a new thread
    ucontext_t self;

    makecontext(&self, func, 1, arg);

};
void mythread_join();  // Waits for other thread to complete. It is used in case of dependent threads.
void mythread_yield();  // Perform context switching here

struct lock {
	ucontext_t* ctx;
};
struct lock* lock_new(){   // return an initialized lock object
	struct lock* lk = (struct lock*)malloc(sizeof(struct lock));
	lk->ctx = NULL;
	return lk;
}
void lock_acquire(struct lock* lk){   // Set lock. Yield if lock is acquired by some other thread.
	while(lk->ctx != NULL){
		mythread_yield();
	}
	lk->ctx = mythread_self(); // incomplete
};
int lock_release(struct lock* lk){   // Release lock
	lk->ctx = NULL;
	return 0;
};

#endif