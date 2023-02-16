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
ucontext_t* joiner;
struct listentry* current_ctx;
// ucontext_t *mtc;

void mythread_init()      // Initialize threads list
{
    printf("mythread_init done\n");
    thread_list = list_new();
    // current_ctx = thread_list->head;
    joiner = (ucontext_t*)malloc(sizeof (ucontext_t));
}


static void f_tester(){
    printf("started f_tester\n");
}

ucontext_t* mythread_create(void func(void*), void* arg)  // Create a new thread
{
    printf("Creating thread\n");
    ucontext_t *mtc = (ucontext_t *)malloc(sizeof (ucontext_t));
    getcontext(mtc);
    // char st =;
    mtc->uc_stack.ss_sp = malloc(64000);
    mtc->uc_stack.ss_size = 64000;
    mtc->uc_link = joiner;
    if(mtc->uc_link == joiner){
        printf("working1 \n");
    }
    // makecontext(mtc, f_tester, 0);
    // swapcontext(&joiner, mtc);
    makecontext(mtc, func, 1, arg);
    printf("%s\n",arg);
    list_add(thread_list, mtc);
    printf("Creating thread 2\n");
    return mtc;
}


void mythread_join()  // Waits for other thread to complete. It is used in case of dependent threads.
{
    printf("Joining thread \n");
//    ucontext_t self;
    
    // ucontext_t * tester_ctx = t->data;
    // if(tester_ctx->uc_link == joiner){
    //     printf("working \n");
    // }
    // printf(tester_ctx->uc_link);
    while (thread_list->head != NULL){
        current_ctx = thread_list->head;
        printf("hi loop\n");
//        ucontext_t &p = *t->data;
        swapcontext(joiner, (ucontext_t*)(current_ctx->data)); // to improve
        list_rm(thread_list, current_ctx);
        
    }
    // swapcontext(&joiner, mtc);
    // swapcontext(joiner, (ucontext_t*)(t->data)); // to improve
    
    printf("1 \n");
}

void mythread_yield()  // Perform context switching here
{
    struct listentry *sel;
    sel = current_ctx;
    if(current_ctx == thread_list->tail){
        current_ctx = thread_list->head;
    } else{
        current_ctx = current_ctx->next;
    }
    swapcontext(sel->data, (ucontext_t*)(current_ctx->data)); // to improve
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
