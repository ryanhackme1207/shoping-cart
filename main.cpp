#include "menu.h"
#include <vector>
using namespace std;

int main() {
    vector<Product> products = {
        Product(1, "Laptop", 2500.00, 5),
        Product(2, "Mouse", 50.00, 10),
        Product(3, "Keyboard", 150.00, 7)
    };

    vector<CartItem> cart;

    Menu::showMenu(products, cart);

    return 0;
}
