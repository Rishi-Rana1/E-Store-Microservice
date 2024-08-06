#include <cstring>
#include <cstdlib>
#include <iostream>
#include <cstdio>


#include "EStore.h"
#include "TaskQueue.h"
#include "RequestGenerator.h"

class Simulation {
    public:
    TaskQueue supplierTasks;
    TaskQueue customerTasks;
    EStore store;

    int maxTasks;
    int numSuppliers;
    int numCustomers;

    explicit Simulation(bool useFineMode) : store(useFineMode) { }
};

/*
 * ------------------------------------------------------------------
 * supplierGenerator --
 *
 *      The supplier generator thread. The argument is a pointer to
 *      the shared Simulation object.
 *
 *      Enqueue arg->maxTasks requests to the supplier queue, then
 *      stop all supplier threads by enqueuing arg->numSuppliers
 *      stop requests.
 *
 *      Use a SupplierRequestGenerator to generate and enqueue
 *      requests.
 *
 *      This thread should exit when done.
 *
 * Results:
 *      Does not return. Exit instead.
 *
 * ------------------------------------------------------------------
 */
static void*
supplierGenerator(void* arg)
{
    // TODO: Your code here.
    //use enqueueTasks to load all tasks and enqueueStops for stops
    Simulation* argSim = static_cast<Simulation*>(arg);
    SupplierRequestGenerator gen(&(argSim->supplierTasks));
    gen.enqueueTasks(argSim->maxTasks, &(argSim->store));
    gen.enqueueStops(argSim->numSuppliers);
    sthread_exit();
    return NULL; // Keep compiler happy.
}

/*
 * ------------------------------------------------------------------
 * customerGenerator --
 *
 *      The customer generator thread. The argument is a pointer to
 *      the shared Simulation object.
 *
 *      Enqueue arg->maxTasks requests to the customer queue, then
 *      stop all customer threads by enqueuing arg->numCustomers
 *      stop requests.
 *
 *      Use a CustomerRequestGenerator to generate and enqueue
 *      requests.  For the fineMode argument to the constructor
 *      of CustomerRequestGenerator, use the output of
 *      store.fineModeEnabled() method, where store is a field
 *      in the Simulation class.
 *
 *      This thread should exit when done.
 *
 * Results:
 *      Does not return. Exit instead.
 *
 * ------------------------------------------------------------------
 */
static void*
customerGenerator(void* arg)
{
    // TODO: Your code here.
    Simulation* argSim = static_cast<Simulation*>(arg);
    CustomerRequestGenerator gen(&(argSim->customerTasks), argSim->store.fineModeEnabled());
    gen.enqueueTasks(argSim->maxTasks, &(argSim->store));
    gen.enqueueStops(argSim->numCustomers);
    sthread_exit();
    return NULL; // Keep compiler happy.
}

/*
 * ------------------------------------------------------------------
 * supplier --
 *
 *      The main supplier thread. The argument is a pointer to the
 *      shared Simulation object.
 *
 *      Dequeue Tasks from the supplier queue and execute them.
 *
 * Results:
 *      Does not return.
 *
 * ------------------------------------------------------------------
 */
static void*
supplier(void* arg)
{
    // TODO: Your code here.
    //dequeue all tasks and initiate them, helped by creating a local task variable.
    Simulation* argSim = static_cast<Simulation*>(arg);
    while (true) {
        task res = (argSim->supplierTasks).dequeue();
        res.handler(res.arg);
    }
    return NULL; // Keep compiler happy.
}

/*
 * ------------------------------------------------------------------
 * customer --
 *
 *      The main customer thread. The argument is a pointer to the
 *      shared Simulation object.
 *
 *      Dequeue Tasks from the customer queue and execute them.
 *
 * Results:
 *      Does not return.
 *
 * ------------------------------------------------------------------
 */
static void*
customer(void* arg)
{
    // TODO: Your code here.
    Simulation* argSim = static_cast<Simulation*>(arg);
    while (true) {
        task res = (argSim->customerTasks).dequeue();
        res.handler(res.arg);
    }
    return NULL; // Keep compiler happy.
}

/*
 * ------------------------------------------------------------------
 * startSimulation --
 *      Create a new Simulation object. This object will serve as
 *      the shared state for the simulation. 
 *
 *      Create the following threads:
 *          - 1 supplier generator thread.
 *          - 1 customer generator thread.
 *          - numSuppliers supplier threads.
 *          - numCustomers customer threads.
 *
 *      After creating the worker threads, the main thread
 *      should wait until all of them exit, at which point it
 *      should return.
 *
 *      Hint: Use sthread_join.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
static void
startSimulation(int numSuppliers, int numCustomers, int maxTasks, bool useFineMode)
{
    // TODO: Your code here.
    //generator threads are created first, the supplier and generator threads, the generator threads are joined, and finally the supplier and customer threads are all joined
    Simulation sim(useFineMode);
    sim.numSuppliers = numSuppliers;
    sim.numCustomers = numCustomers;
    sim.maxTasks = maxTasks;
    sthread_t SupGen;
    sthread_create(&SupGen, supplierGenerator, &sim);
    sthread_t ConGen;
    sthread_create(&ConGen, customerGenerator, &sim);
    sthread_t threadArr[numCustomers + numSuppliers];
    int i;
    for (i = 0; i < numSuppliers; i++)
    {
        sthread_t Sup;
        sthread_create(&Sup, supplier, &sim);
        threadArr[i] = Sup;
    }

    for (int k = i; k < (i + numCustomers); k++)
    {
        sthread_t Con;
        sthread_create(&Con, customer, &sim);
        threadArr[k] = Con;
    }
    sthread_join(ConGen);
    sthread_join(SupGen);
    for (int l = 0; l < (numCustomers + numSuppliers); l++)
    {
        sthread_join(threadArr[l]);
    }
}

int main(int argc, char **argv)
{
    bool useFineMode = false;

    // Seed the random number generator.
    // You can remove this line or set it to some constant to get deterministic
    // results, but make sure you put it back before turning in.
    srand(time(NULL));

    if (argc > 1)
        useFineMode = strcmp(argv[1], "--fine") == 0;
    startSimulation(10, 10, 100, useFineMode);
    return 0;
}

