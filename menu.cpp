#include "menu.h"
#include <iostream>
using namespace std;

void Menu::showMenu(vector<Product>& products, vector<CartItem>& cart) {
    int choice;
    do {
        cout << R"(          _____                    _____                   _______                   _____                    _____                    _____
         /\    \                  /\    \                 /::\    \                 /\    \                  /\    \                  /\    \
        /::\    \                /::\____\               /::::\    \               /::\    \                /::\    \                /::\    \
       /::::\    \              /:::/    /              /::::::\    \             /::::\    \              /::::\    \              /::::\    \
      /::::::\    \            /:::/    /              /::::::::\    \           /::::::\    \            /::::::\    \            /::::::\    \
     /:::/\:::\    \          /:::/    /              /:::/~~\:::\    \         /:::/\:::\    \          /:::/\:::\    \          /:::/\:::\    \
    /:::/__\:::\    \        /:::/____/              /:::/    \:::\    \       /:::/__\:::\    \        /:::/__\:::\    \        /:::/__\:::\    \
    \:::\   \:::\    \      /::::\    \             /:::/    / \:::\    \     /::::\   \:::\    \      /::::\   \:::\    \      /::::\   \:::\    \
  ___\:::\   \:::\    \    /::::::\    \   _____   /:::/____/   \:::\____\   /::::::\   \:::\    \    /::::::\   \:::\    \    /::::::\   \:::\    \
 /\   \:::\   \:::\    \  /:::/\:::\    \ /\    \ |:::|    |     |:::|    | /:::/\:::\   \:::\____\  /:::/\:::\   \:::\    \  /:::/\:::\   \:::\    \
/::\   \:::\   \:::\____\/:::/  \:::\    /::\____\|:::|____|     |:::|    |/:::/  \:::\   \:::|    |/:::/__\:::\   \:::\____\/:::/__\:::\   \:::\____\
\:::\   \:::\   \::/    /\::/    \:::\  /:::/    / \:::\    \   /:::/    / \::/    \:::\  /:::|____|\:::\   \:::\   \::/    /\:::\   \:::\   \::/    /
 \:::\   \:::\   \/____/  \/____/ \:::\/:::/    /   \:::\    \ /:::/    /   \/_____/\:::\/:::/    /  \:::\   \:::\   \/____/  \:::\   \:::\   \/____/
  \:::\   \:::\    \               \::::::/    /     \:::\    /:::/    /             \::::::/    /    \:::\   \:::\    \       \:::\   \:::\    \
   \:::\   \:::\____\               \::::/    /       \:::\__/:::/    /               \::::/    /      \:::\   \:::\____\       \:::\   \:::\____\
    \:::\  /:::/    /               /:::/    /         \::::::::/    /                 \::/____/        \:::\   \::/    /        \:::\   \::/    /
     \:::\/:::/    /               /:::/    /           \::::::/    /                   ~~               \:::\   \/____/          \:::\   \/____/
      \::::::/    /               /:::/    /             \::::/    /                                      \:::\    \               \:::\    \
       \::::/    /               /:::/    /               \::/____/                                        \:::\____\               \:::\____\
        \::/    /                \::/    /                 ~~                                               \::/    /                \::/    /
         \/____/                  \/____/                                                                    \/____/                  \/____/
    )" << "\n\n";


        cout << "\n--- Shopping Cart Menu ---\n";
        cout << "1. Show Products\n";
        cout << "2. Add to Cart\n";
        cout << "3. View Cart\n";
        cout << "4. Update Cart Item\n";      // new
        cout << "5. Remove Item from Cart\n"; // new
        cout << "6. Checkout\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "\n--- Product List ---\n";
            for (auto &p : products) {
                p.display();
            }
        }
        else if (choice == 2)
        {
            int id, qty;
            cout << "Enter Product ID: ";
            cin >> id;
            cout << "Enter Quantity: ";
            cin >> qty;

            bool found = false;
            for (auto &p : products)
            {
                if (p.getId() == id)
                {
                    if (qty <= p.getStock())
                    {
                        bool inCart = false;
                        for (auto &item : cart)
                        {
                            if (item.product.getId() == id)
                            {
                                item.quantity += qty; // update existing
                                inCart = true;
                                break;
                            }
                        }
                        if (!inCart)
                        {
                            cart.push_back(CartItem(p, qty)); // add new
                        }
                        p.reduceStock(qty);
                        cout << "Added to cart!\n";
                    }
                    else
                    {
                        cout << "Not enough stock!\n";
                    }
                    found = true;
                    break;
                }
            }
            if (!found)
                cout << "Invalid Product ID!\n";
        }

        else if (choice == 3) {
            cout << "\n--- Your Cart ---\n";
            if (cart.empty()) cout << "Cart is empty.\n";
            else {
                for (auto &item : cart) {
                    cout << item.product.getName() << " x " << item.quantity 
                         << " (RM " << item.product.getPrice() * item.quantity << ")\n";
                }
            }
        }

        else if (choice == 4)
        {
            int id, newQty;
            cout << "Enter Product ID to update: ";
            cin >> id;
            cout << "Enter new quantity: ";
            cin >> newQty;

            bool updated = false;
            for (auto &item : cart)
            {
                if (item.product.getId() == id)
                {
                    int diff = newQty - item.quantity; // difference in quantity
                    if (diff > 0)
                    {
                        if (diff <= item.product.getStock())
                        {
                            item.product.reduceStock(diff);
                            item.quantity = newQty;
                            updated = true;
                        }
                        else
                        {
                            cout << "Not enough stock!\n";
                        }
                    }
                    else
                    {
                        item.quantity = newQty;
                        updated = true;
                    }
                    break;
                }
            }
            if (updated)
                cout << "Cart updated!\n";
            else
                cout << "Item not found in cart!\n";
        }

        else if (choice == 5)
        {
            int id;
            cout << "Enter Product ID to remove: ";
            cin >> id;

            bool removed = false;
            for (auto it = cart.begin(); it != cart.end(); ++it)
            {
                if (it->product.getId() == id)
                {
                    // restore stock
                    for (auto &p : products)
                    {
                        if (p.getId() == id)
                        {
                            p.reduceStock(-it->quantity); // negative restores stock
                            break;
                        }
                    }
                    cart.erase(it);
                    removed = true;
                    break;
                }
            }
            if (removed)
                cout << "Item removed from cart!\n";
            else
                cout << "Item not found in cart!\n";
        }

        else if (choice == 6) {
            Checkout::processOrder(cart);
        }

    } while (choice != 7);
}
