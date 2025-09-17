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
        cout << "4. Checkout\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "\n--- Product List ---\n";
            for (auto &p : products) {
                p.display();
            }
        }
        else if (choice == 2) {
            int id, qty;
            cout << "Enter Product ID: ";
            cin >> id;
            cout << "Enter Quantity: ";
            cin >> qty;

            bool found = false;
            for (auto &p : products) {
                if (p.getId() == id) {
                    if (qty <= p.getStock()) {
                        cart.push_back(CartItem(p, qty));
                        p.reduceStock(qty);
                        cout << "Added to cart!\n";
                    } else {
                        cout << "Not enough stock!\n";
                    }
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Invalid Product ID!\n";
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
        else if (choice == 4) {
            Checkout::processOrder(cart);
        }

    } while (choice != 5);
}
