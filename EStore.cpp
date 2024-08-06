#include <cassert>

#include "EStore.h"
#include <cstdio>

using namespace std;


Item::
Item() : valid(false)
{ }

Item::
~Item()
{ }


EStore::
EStore(bool enableFineMode)
    : fineMode(enableFineMode)
{
    // TODO: Your code here.
    smutex_init(&LockS);
    scond_init(&CondS);
    ShippingCost = 3.0;
    StoreDiscount = 0.0;
    if (fineModeEnabled()){
        for (int k = 0; k < INVENTORY_SIZE; k++) {
        smutex_t mutex;
        smutex_init(&mutex);
        LockArr[k] = mutex;
        }
    }
}

EStore::
~EStore()
{
    // TODO: Your code here.
    smutex_destroy(&LockS);
    scond_destroy(&CondS);
    if (fineModeEnabled())
    {for (int k = 0; k < INVENTORY_SIZE; k++) {
        smutex_t mutex = LockArr[k];
        smutex_destroy(&mutex);
    }}
}

/*
 * ------------------------------------------------------------------
 * buyItem --
 *
 *      Attempt to buy the item from the store.
 *
 *      An item can be bought if:
 *          - The store carries it.
 *          - The item is in stock.
 *          - The cost of the item plus the cost of shipping is no
 *            more than the budget.
 *
 *      If the store *does not* carry this item, simply return and
 *      do nothing. Do not attempt to buy the item.
 *
 *      If the store *does* carry the item, but it is not in stock
 *      or its cost is over budget, block until both conditions are
 *      met (at which point the item should be bought) or the store
 *      removes the item from sale (at which point this method
 *      returns).
 *
 *      The overall cost of a purchase for a single item is defined
 *      as the current cost of the item times 1 - the store
 *      discount, plus the flat overall store shipping fee.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */

void EStore::
setLock(int item_id)
{
    //custom method
    if (fineModeEnabled()) {
        smutex_lock(&LockArr[item_id]);
    }
    else {
        smutex_lock(&LockS);
    }
}

void EStore::
unsetLock(int item_id)
{
    //custom method
    if (fineModeEnabled()) {
        smutex_unlock(&LockArr[item_id]);
    }
    else {
        smutex_unlock(&LockS);
    }
}

void EStore::
wakeCondBroad(int item_id)
{
    //custom method
    if (fineModeEnabled()) {
        scond_broadcast(&CondS, &LockArr[item_id]);
    }
    else {
        scond_broadcast(&CondS, &LockS);
    }
}

void EStore::
buyItem(int item_id, double budget)
{
    assert(!fineModeEnabled());

    // TODO: Your code here.
    //Locks store and puts thread into waiting if item is not affordable or it is sold out, if it becomes invalid we give up on the purchase.
    smutex_lock(&LockS);
    Item product = inventory[item_id];
    while ((budget < ((inventory[item_id]).price * (1 - (inventory[item_id]).discount) * (1 - StoreDiscount) + ShippingCost) || (inventory[item_id]).quantity == 0) && (inventory[item_id]).valid) {
        scond_wait(&CondS, &LockS);
    }
    if (!inventory[item_id].valid){
        smutex_unlock(&LockS);
        return;
    }
    (inventory[item_id]).quantity -= 1;
    smutex_unlock(&LockS);
    return;
}

/*
 * ------------------------------------------------------------------
 * buyManyItem --
 *
 *      Attempt to buy all of the specified items at once. If the
 *      order cannot be bought, give up and return without buying
 *      anything. Otherwise buy the entire order at once.
 *
 *      The entire order can be bought if:
 *          - The store carries all items.
 *          - All items are in stock.
 *          - The cost of the the entire order (cost of items plus
 *            shipping for each item) is no more than the budget.
 *
 *      If multiple customers are attempting to buy at the same
 *      time and their orders are mutually exclusive (i.e., the
 *      two customers are not trying to buy any of the same items),
 *      then their orders must be processed at the same time.
 *
 *      For the purposes of this lab, it is OK for the store
 *      discount and shipping cost to change while an order is being
 *      processed.
 *
 *      The cost of a purchase of many items is the sum of the
 *      costs of purchasing each item individually. The purchase
 *      cost of an individual item is covered above in the
 *      description of buyItem.
 *
 *      Challenge: For bonus points, implement a version of this
 *      method that will wait until the order can be fulfilled
 *      instead of giving up. The implementation should be efficient
 *      in that it should not wake up threads unecessarily. For
 *      instance, if an item decreases in price, only threads that
 *      are waiting to buy an order that includes that item should be
 *      signaled (though all such threads should be signaled).
 *
 *      Challenge: For bonus points, ensure that the shipping cost
 *      and store discount does not change while processing an
 *      order.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
buyManyItems(vector<int>* item_ids, double budget)
{
    assert(fineModeEnabled());
    bool NoProblems = true;
    //bool value is used depending on if entire order is able to be purchased
    int total = 0;
    //keeps track of an orders total
    for (unsigned long int i = 0; i < item_ids->size(); i++) {
        setLock(item_ids->at(i));
        total += (inventory[item_ids->at(i)]).price * (1 - (inventory[item_ids->at(i)]).discount) * (1 - StoreDiscount) + ShippingCost;
        //while ((budget < ((inventory[item_ids->at(i)]).price * (1 - (inventory[item_ids->at(i)]).discount) * (1 - StoreDiscount) + ShippingCost) || (inventory[item_ids->at(i)]).quantity == 0) && ((inventory[item_ids->at(i)]).valid) && (total <= budget)) {
        //    scond_wait(&CondS, &LockArr[i]);
        //only for challange exercise.
        if (!(inventory[item_ids->at(i)]).valid || (total > budget)){
            NoProblems = false;
            unsetLock(item_ids->at(i));
            break;
        }
        unsetLock(item_ids->at(i));
        }
    //if order is purchaseable, then we remove quantity
    if (NoProblems) {
        for (unsigned long int j = 0; j < item_ids->size(); j++) {
            setLock(item_ids->at(j));
            (inventory[item_ids->at(j)]).quantity -= 1;
            unsetLock(item_ids->at(j));
        }
    }
    return;
    // TODO: Your code here.

}

/*
 * ------------------------------------------------------------------
 * addItem --
 *
 *      Add the item to the store with the specified quantity,
 *      price, and discount. If the store already carries an item
 *      with the specified id, do nothing.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
addItem(int item_id, int quantity, double price, double discount)
{
    // TODO: Your code here.
    //if item is already valid, we return, otherwise we set the items data types to the correct values
    setLock(item_id);
    if ((inventory[item_id]).valid){
        unsetLock(item_id);
        return;
    }
    (inventory[item_id]).quantity = quantity;
    (inventory[item_id]).price = price;
    (inventory[item_id]).discount = discount;
    (inventory[item_id]).valid = true;
    unsetLock(item_id);
    return;
}

/*
 * ------------------------------------------------------------------
 * removeItem --
 *
 *      Remove the item from the store. The store no longer carries
 *      this item. If the store is not carrying this item, do
 *      nothing.
 *
 *      Wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
removeItem(int item_id)
{
    // TODO: Your code here.
    //if item is not avalible, then we cant remove it, otherwise we set data members to indicate it is no longer avalible
    setLock(item_id);
    if (!(inventory[item_id]).valid){
        unsetLock(item_id);
        return;
    }
    (inventory[item_id]).quantity = 0;
    (inventory[item_id]).price = 0;
    (inventory[item_id]).discount = 0;
    (inventory[item_id]).valid = false;
    if (fineModeEnabled()) {
        scond_broadcast(&CondS, &LockArr[item_id]);
    }
    else {
        scond_broadcast(&CondS, &LockS);
    }
    unsetLock(item_id);
    return;
}

/*
 * ------------------------------------------------------------------
 * addStock --
 *
 *      Increase the stock of the specified item by count. If the
 *      store does not carry the item, do nothing. Wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
addStock(int item_id, int count)
{
    // TODO: Your code here.
    //if item is avalible we add to its quantity and broadcast to waiting threads to wake them in order to purchase
    setLock(item_id);
    if (!(inventory[item_id]).valid){
        unsetLock(item_id);
        return;
    }
    (inventory[item_id]).quantity += count;
    if (fineModeEnabled()) {
        scond_broadcast(&CondS, &LockArr[item_id]);
    }
    else {
        scond_broadcast(&CondS, &LockS);
    }
    unsetLock(item_id);
    return;
}

/*
 * ------------------------------------------------------------------
 * priceItem --
 *
 *      Change the price on the item. If the store does not carry
 *      the item, do nothing.
 *
 *      If the item price decreased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
priceItem(int item_id, double price)
{
    // TODO: Your code here.
    //if item is avalible, we change the item price and alert waiting threads to recheck if they can purchase
    setLock(item_id);
    if (!(inventory[item_id]).valid){
        unsetLock(item_id);
        return;
    }
    double temp = (inventory[item_id]).price;
    (inventory[item_id]).price = price;
    if (temp > (inventory[item_id]).price) {
        if (fineModeEnabled()) {
        scond_broadcast(&CondS, &LockArr[item_id]);
    }
    else {
        scond_broadcast(&CondS, &LockS);
    }
    }
    unsetLock(item_id);
    return;
}

/*
 * ------------------------------------------------------------------
 * discountItem --
 *
 *      Change the discount on the item. If the store does not carry
 *      the item, do nothing.
 *
 *      If the item discount increased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
discountItem(int item_id, double discount)
{
    // TODO: Your code here.
    //if item is avalible, we change the discount, and if its higher than the old discount, we wake any waiting threads.
    //only threads waiting with the given lock (assosiated with a particular item) should be taken out of waiting.
    setLock(item_id);
    if (!(inventory[item_id]).valid){
        unsetLock(item_id);
        return;
    }
    double temp = (inventory[item_id]).discount;
    (inventory[item_id]).discount = discount;
    if (temp < (inventory[item_id]).discount ) {
        if (fineModeEnabled()) {
        scond_broadcast(&CondS, &LockArr[item_id]);
    }
    else {
        scond_broadcast(&CondS, &LockS);
    }
    }
    unsetLock(item_id);
    return;
}

/*
 * ------------------------------------------------------------------
 * setShippingCost --
 *
 *      Set the per-item shipping cost. If the shipping cost
 *      decreased, wake any waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
setShippingCost(double cost)
{
    // TODO: Your code here.
    //We change the store shipping cost and if the new shipping cost is lower than the old one, we broadcast
    //we restrict the waking to only non fine-mode because in fine-mode buyall requests dont wait on changes in the shipping cost
    //also because it seems to be impossible to wake all threads despite what lock they have in fine-mode since scond_broadcast requires a lock
    //p.s.s I also dont know if these locks are required in fine-grained mode for both this method and the one below it, but it should not have any effect in fine-grained mode besides stopping setShippingCost requests from occuring at the same time.
    smutex_lock(&LockS);
    double temp = ShippingCost;
    ShippingCost = cost;
    if (temp > ShippingCost && (!fineModeEnabled())) {
        scond_broadcast(&CondS, &LockS);
    }
    smutex_unlock(&LockS);
    return;
}

/*
 * ------------------------------------------------------------------
 * setStoreDiscount --
 *
 *      Set the store discount. If the discount increased, wake any
 *      waiters.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void EStore::
setStoreDiscount(double discount)
{
    // TODO: Your code here.
    //same thing as above method pretty much but with the discount. if the discount is higher then we broadcast.
    smutex_lock(&LockS);
    double temp = StoreDiscount;
    StoreDiscount = discount;
    if ((temp < StoreDiscount) && (!fineModeEnabled())) {
        scond_broadcast(&CondS, &LockS);
    }
    smutex_unlock(&LockS);
    return;
}


