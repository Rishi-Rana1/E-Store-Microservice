#include "RequestHandlers.h"
#include <cstdio>
/*
* dissasembles object
 * ------------------------------------------------------------------
 * add_item_handler --
 *
 *      Handle an AddItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
add_item_handler(void *args)
{
    // TODO: Your code here.
    AddItemReq* AddReq = static_cast<AddItemReq*>(args);
    EStore* EstoreHand = AddReq->store;
    EstoreHand->addItem(AddReq->item_id, AddReq->quantity, AddReq->price, AddReq->discount);
    printf("Handling AddItemReq: item_id - %d, quantity - %d, price - $%.2f, discount - %.2f\n", AddReq->item_id, AddReq->quantity, AddReq->price, AddReq->discount);
    //std::cout << "Handling AddItemReq: item_id - " << AddReq->item_id << ", quantity - " << AddReq->quantity << ", price - $" << AddReq->price << ", discount - " << AddReq->discount << std::endl;
    delete AddReq;
}

/*
 * ------------------------------------------------------------------
 * remove_item_handler --
 *
 *      Handle a RemoveItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
remove_item_handler(void *args)
{
    // TODO: Your code here.
    RemoveItemReq* RemReq = static_cast<RemoveItemReq*>(args);
    EStore* EstoreHand = RemReq->store;
    EstoreHand->removeItem(RemReq->item_id);
    printf("Handling RemoveItemReq: item_id - %d\n", RemReq->item_id);
    //std::cout << "Handling RemoveItemReq: item_id - " << RemReq->item_id << std::endl;
    delete RemReq;
}

/*
 * ------------------------------------------------------------------
 * add_stock_handler --
 *
 *      Handle an AddStockReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
add_stock_handler(void *args)
{
    // TODO: Your code here.
    AddStockReq* AddReq = static_cast<AddStockReq*>(args);
    EStore* EstoreHand = AddReq->store;
    EstoreHand->addStock(AddReq->item_id, AddReq->additional_stock);
    printf("Handling AddStockReq: item_id - %d, added quantity - %d\n", AddReq->item_id, AddReq->additional_stock);
    //std::cout << "Handling AddStockReq: item_id - "  << AddReq->item_id << ", added quantity - " << AddReq->additional_stock << std::endl;
    delete AddReq;
}

/*
 * ------------------------------------------------------------------
 * change_item_price_handler --
 *
 *      Handle a ChangeItemPriceReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
change_item_price_handler(void *args)
{
    // TODO: Your code here.
    ChangeItemPriceReq* ChangePReq = static_cast<ChangeItemPriceReq*>(args);
    EStore* EstoreHand = ChangePReq->store;
    EstoreHand->priceItem(ChangePReq->item_id, ChangePReq->new_price);
    printf("Handling ChangeItemPriceReq: item_id - %d, price - %.2f\n", ChangePReq->item_id, ChangePReq->new_price);
    //std::cout << "Handling ChangeItemPriceReq: item_id - "  << ChangePReq->item_id << ", price - " << ChangePReq->new_price << std::endl;
    delete ChangePReq;
}

/*
 * ------------------------------------------------------------------
 * change_item_discount_handler --
 *
 *      Handle a ChangeItemDiscountReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
change_item_discount_handler(void *args)
{
    // TODO: Your code here.
    ChangeItemDiscountReq* ChangeDReq = static_cast<ChangeItemDiscountReq*>(args);
    EStore* EstoreHand = ChangeDReq->store;
    EstoreHand->discountItem(ChangeDReq->item_id, ChangeDReq->new_discount);
    printf("Handling ChangeItemDiscountReq: item_id - %d, discount - %.2f\n", ChangeDReq->item_id, ChangeDReq->new_discount);
    //std::cout << "Handling ChangeItemDiscountReq: item_id - "  << ChangeDReq->item_id << ", discount - " << ChangeDReq->new_discount << std::endl;
    delete ChangeDReq;
}

/*
 * ------------------------------------------------------------------
 * set_shipping_cost_handler --
 *
 *      Handle a SetShippingCostReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
set_shipping_cost_handler(void *args)
{
    // TODO: Your code here.
    SetShippingCostReq* ChangeScReq = static_cast<SetShippingCostReq*>(args);
    EStore* EstoreHand = ChangeScReq->store;
    EstoreHand->setShippingCost(ChangeScReq->new_cost);
    printf("Handling SetShippingCostReq: new_cost - %.2f\n", ChangeScReq->new_cost);
    //std::cout << "Handling SetShippingCostReq: new_cost - "  << ChangeScReq->new_cost << std::endl;
    delete ChangeScReq;
}

/*
 * ------------------------------------------------------------------
 * set_store_discount_handler --
 *
 *      Handle a SetStoreDiscountReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
set_store_discount_handler(void *args)
{
    // TODO: Your code here.
    SetStoreDiscountReq* ChangeSDReq = static_cast<SetStoreDiscountReq*>(args);
    EStore* EstoreHand = ChangeSDReq->store;
    EstoreHand->setStoreDiscount(ChangeSDReq->new_discount);
    printf("Handling SetStoreDiscountReq: new_discount - %.2f\n", ChangeSDReq->new_discount);
    //std::cout << "Handling SetStoreDiscountReq: new_discount - "  << ChangeSDReq->new_discount << std::endl;
    delete ChangeSDReq;
}

/*
 * ------------------------------------------------------------------
 * buy_item_handler --
 *
 *      Handle a BuyItemReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
buy_item_handler(void *args)
{
    // TODO: Your code here.
    BuyItemReq* BuyReq = static_cast<BuyItemReq*>(args);
    EStore* EstoreHand = BuyReq->store;
    EstoreHand->buyItem(BuyReq->item_id, BuyReq->budget);
    printf("Handling BuyItemReq: item_id - %d, budget - %.2f\n", BuyReq->item_id, BuyReq->budget);
    //std::cout << "Handling BuyItemReq: item_id - " << BuyReq->item_id << ", budget - " << BuyReq->budget << std::endl;
    delete BuyReq;
}

/*
 * ------------------------------------------------------------------
 * buy_many_items_handler --
 *
 *      Handle a BuyManyItemsReq.
 *
 *      Delete the request object when done.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void
buy_many_items_handler(void *args)
{
    // TODO: Your code here.
    BuyManyItemsReq* BuyManReq = static_cast<BuyManyItemsReq*>(args);
    EStore* EstoreHand = BuyManReq->store;
    EstoreHand->buyManyItems(&BuyManReq->item_ids, BuyManReq->budget);
    printf("Handling buy_many_items_handler: item_ids - ");
    for (unsigned long int i = 0; i < (BuyManReq->item_ids).size(); i++){
        printf(" %d", ((BuyManReq->item_ids)[i]));
    }
    printf("\n");
    delete BuyManReq;
}

/*
 * ------------------------------------------------------------------
 * stop_handler --
 *
 *      The thread should exit.
 *
 * Results:
 *      None.
 *
 * ------------------------------------------------------------------
 */
void 
stop_handler(void* args)
{
    sthread_exit();
}

