#pragma once

typedef struct sx__thread_s sx_thread;
// typedef struct sx_queue_spsc sx_queue_spsc;
typedef struct sx_mutex_s sx_mutex;
typedef struct sx_signal_s sx_signal;

struct node;

typedef void(func_for_node)(struct node *node);

struct thread_note
{
    sx_thread *thread;
    // sx_queue_spsc *to_queue;
    sx_mutex *mutex_in;
    sx_mutex *mutex_out;
    sx_signal *signal;
    sx_signal *signal_done;
    struct node *node_to_run;
    func_for_node *func_to_run;
};

struct thread_note **thread_notes;

struct thread_note default_thread_note;


void init_threads_subsystem();

struct thread_note *make_thread_note();

void free_thread_note(struct thread_note *thread_note);

void send_func_to_thread(func_for_node *func_to_run, struct node *node);