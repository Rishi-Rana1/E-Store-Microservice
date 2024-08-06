
#include "TaskQueue.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

TaskQueue::
TaskQueue()
{
    // TODO: Your code here.
    smutex_init(&lockQ);
    scond_init(&condQ);
}

TaskQueue::
~TaskQueue()
{
    // TODO: Your code here.
    smutex_destroy(&lockQ);
    scond_destroy(&condQ);
}

/*
 * ------------------------------------------------------------------
 * size --
 *
 *      Return the current size of the queue.
 *
 * Results:
 *      The size of the queue.
 *
 * ------------------------------------------------------------------
 */
int TaskQueue::
size()
{
    // TODO: Your code here.
    int size = taskQ.size();
    return size; // Keep compiler happy until routine done.
}

/*
 * ------------------------------------------------------------------
 * empty --
 *
 *      Return whether or not the queue is empty.
 *
 * Results:
 *      The true if the queue is empty and false otherwise.
 *
 * ------------------------------------------------------------------
 */
bool TaskQueue::
empty()
{
    //signals if queue is not empty for dequeues to happen.
    // TODO: Your code here.
    bool status = taskQ.empty();
    //I had this here before thinking that if empty is called, we should wake threads to let them know if the queue is not empty, but this seems unecessary since we already signal in queue
    //and if no threads are waiting is dequeue, then this is useless
    //if (!status){
    //    scond_signal(&condQ, &lockQ);
    //}
    return status; // Keep compiler happy until routine done.
}

/*
 * ------------------------------------------------------------------
 * enqueue --
 *
 *      Insert the task at the back of the queue.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void TaskQueue::
enqueue(Task task)
{
    // TODO: Your code here.
    smutex_lock(&lockQ);
    taskQ.push(task);
    //signal in order to let threads waiting to be dequeued to check empty() again, and then dequeue
    scond_signal(&condQ, &lockQ);
    smutex_unlock(&lockQ);
}

/*
 * ------------------------------------------------------------------
 * dequeue --
 *
 *      Remove the Task at the front of the queue and return it.
 *      If the queue is empty, block until a Task is inserted.
 *
 * Results:
 *      The Task at the front of the queue.
 *
 * ------------------------------------------------------------------
 */
Task TaskQueue::
dequeue()
{
    // TODO: Your code here.
    smutex_lock(&lockQ);
    while (empty()) {
        scond_wait(&condQ, &lockQ);
        //wait if dequeue is called but queue is empty
    }
    task res = taskQ.front();
    taskQ.pop();
    smutex_unlock(&lockQ);
    return res; // Keep compiler happy until routine done.
}

