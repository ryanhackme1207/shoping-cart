// cartitem.h
#ifndef CARTITEM_H
#define CARTITEM_H

#include "product.h"

struct CartItem {
    Product product;
    int quantity;
    
    CartItem(Product p, int qty) : product(p), quantity(qty) {}
};

#endif