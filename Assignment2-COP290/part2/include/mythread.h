#ifndef THREAD_H
#define THREAD_H
#define _XOPEN_SOURCE 600
#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include<signal.h>
#include<sys/time.h>
#include<unistd.h>
#include"rajat_list.h"

struct list *thread_list;
ucontext_t joiner;
struct listentry* current_ctx;

void mythread_init()      // Initialize threads list
{
    thread_list = list_new();
    current_ctx = thread_list->head;
}



ucontext_t* mythread_create(void func(void*), void* arg)  // Create a new thread
{
    ucontext_t *mtc = (ucontext_t *)malloc(sizeof (ucontext_t *));
    getcontext(mtc);
    char st[8192];
    mtc->uc_stack.ss_sp = st;
    mtc->uc_stack.ss_size = sizeof st;
    mtc->uc_link = &joiner;
    makecontext(mtc, (void(*)(void))func, 1, arg);
    list_add(thread_list, &mtc);
    return mtc;
}


void mythread_join()  // Waits for other thread to complete. It is used in case of dependent threads.
{
//    ucontext_t self;
    struct listentry *t = thread_list->head;
    while (t != thread_list->tail){
        current_ctx = t;
//        ucontext_t &p = *t->data;
        swapcontext(&joiner, (ucontext_t*)(t->data)); // to improve
        t = t->next;
    }
    swapcontext(&joiner, (ucontext_t*)(t->data)); // to improve
    //printf("1%d\n");
}

void mythread_yield()  // Perform context switching here
{
    if(current_ctx == thread_list->tail){
        current_ctx = thread_list->head;
    } else{
        current_ctx = current_ctx->next;
    }
    ucontext_t sel;
    swapcontext(&sel, (ucontext_t*)(current_ctx->data)); // to improve
}

struct lock {
	ucontext_t* ctx;
};

struct lock* lock_new()   // return an initialized lock object
{
    struct lock* lk = (struct lock*) malloc(sizeof (struct lock*));
    lk->ctx = NULL;
    return lk;
}

void lock_acquire(struct lock* lk)   // Set lock. Yield if lock is acquired by some other thread.
{
    while (lk->ctx != NULL){
        mythread_yield();
    }
    lk->ctx = (ucontext_t*)current_ctx->data;
}

int lock_release(struct lock* lk)   // Release lock
{
    lk->ctx = NULL;
    return 0;}
#endif
