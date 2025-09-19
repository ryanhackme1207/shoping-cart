// main.cpp
#include "menu.h"
#include "auth.h"
#include "product.h"
#include "cartitem.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
    while (true) {
        if (!Auth::isUserLoggedIn()) {
            Auth::showAuthMenu();
        }

        if (Auth::isUserLoggedIn()) {
            // Initialize products (120 items, multiple categories)
            vector<Product> products = {
                // Electronics
                Product(1, "Laptop", 2500.0, 5, "Electronics"),
                Product(2, "Mouse", 50.0, 20, "Electronics"),
                Product(3, "Keyboard", 150.0, 15, "Electronics"),
                Product(4, "Headphones", 200.0, 12, "Electronics"),
                Product(5, "Smartphone", 3000.0, 10, "Electronics"),
                Product(6, "Tablet", 1800.0, 8, "Electronics"),
                Product(7, "Smartwatch", 900.0, 10, "Electronics"),
                Product(8, "Monitor 24\"", 700.0, 6, "Electronics"),
                Product(9, "Monitor 32\"", 1200.0, 5, "Electronics"),
                Product(10, "External Hard Drive", 400.0, 20, "Electronics"),
                Product(11, "USB Flash Drive 64GB", 60.0, 30, "Electronics"),
                Product(12, "Gaming Console", 2200.0, 4, "Electronics"),
                Product(13, "Bluetooth Speaker", 300.0, 15, "Electronics"),
                Product(14, "Wireless Charger", 120.0, 20, "Electronics"),
                Product(15, "Power Bank 20000mAh", 180.0, 25, "Electronics"),

                // Kitchen
                Product(16, "Microwave", 400.0, 8, "Kitchen"),
                Product(17, "Frying Pan", 80.0, 20, "Kitchen"),
                Product(18, "Rice Cooker", 250.0, 10, "Kitchen"),
                Product(19, "Blender", 300.0, 12, "Kitchen"),
                Product(20, "Toaster", 180.0, 15, "Kitchen"),
                Product(21, "Electric Kettle", 100.0, 25, "Kitchen"),
                Product(22, "Knife Set", 200.0, 10, "Kitchen"),
                Product(23, "Cutting Board", 50.0, 30, "Kitchen"),
                Product(24, "Dish Rack", 120.0, 15, "Kitchen"),
                Product(25, "Pressure Cooker", 350.0, 8, "Kitchen"),

                // Clothing
                Product(26, "Men T-Shirt", 50.0, 40, "Clothing"),
                Product(27, "Women Dress", 120.0, 25, "Clothing"),
                Product(28, "Jeans", 100.0, 30, "Clothing"),
                Product(29, "Jacket", 200.0, 15, "Clothing"),
                Product(30, "Sneakers", 250.0, 20, "Clothing"),
                Product(31, "Formal Shoes", 300.0, 12, "Clothing"),
                Product(32, "Cap", 40.0, 50, "Clothing"),
                Product(33, "Socks (5 pairs)", 30.0, 60, "Clothing"),
                Product(34, "Belt", 70.0, 25, "Clothing"),
                Product(35, "Scarf", 60.0, 30, "Clothing"),

                // Books
                Product(36, "Novel: The Great Gatsby", 45.0, 20, "Books"),
                Product(37, "Novel: 1984", 50.0, 15, "Books"),
                Product(38, "Programming in C++", 120.0, 12, "Books"),
                Product(39, "Python for Beginners", 130.0, 10, "Books"),
                Product(40, "Data Structures & Algorithms", 150.0, 8, "Books"),
                Product(41, "Cooking Recipes", 70.0, 20, "Books"),
                Product(42, "Self-help: Atomic Habits", 90.0, 25, "Books"),
                Product(43, "Business: Rich Dad Poor Dad", 85.0, 20, "Books"),
                Product(44, "Kids: Harry Potter 1", 60.0, 15, "Books"),
                Product(45, "Comics: Marvel Avengers", 55.0, 18, "Books"),

                // Furniture
                Product(46, "Office Chair", 500.0, 10, "Furniture"),
                Product(47, "Dining Table", 1500.0, 5, "Furniture"),
                Product(48, "Sofa Set", 3000.0, 3, "Furniture"),
                Product(49, "Bookshelf", 700.0, 7, "Furniture"),
                Product(50, "Wardrobe", 2000.0, 4, "Furniture"),
                Product(51, "Bed Frame", 2200.0, 5, "Furniture"),
                Product(52, "Mattress", 1800.0, 6, "Furniture"),
                Product(53, "Coffee Table", 400.0, 12, "Furniture"),
                Product(54, "Study Desk", 600.0, 8, "Furniture"),
                Product(55, "TV Cabinet", 750.0, 7, "Furniture"),

                // Sports
                Product(56, "Football", 80.0, 30, "Sports"),
                Product(57, "Basketball", 90.0, 25, "Sports"),
                Product(58, "Tennis Racket", 200.0, 15, "Sports"),
                Product(59, "Badminton Set", 150.0, 20, "Sports"),
                Product(60, "Yoga Mat", 100.0, 25, "Sports"),
                Product(61, "Dumbbell Set", 300.0, 12, "Sports"),
                Product(62, "Treadmill", 3500.0, 2, "Sports"),
                Product(63, "Skipping Rope", 30.0, 40, "Sports"),
                Product(64, "Golf Clubs", 2500.0, 3, "Sports"),
                Product(65, "Hiking Backpack", 250.0, 10, "Sports"),

                // Beauty
                Product(66, "Lipstick", 70.0, 50, "Beauty"),
                Product(67, "Foundation", 120.0, 25, "Beauty"),
                Product(68, "Perfume", 300.0, 20, "Beauty"),
                Product(69, "Shampoo", 40.0, 40, "Beauty"),
                Product(70, "Conditioner", 45.0, 35, "Beauty"),
                Product(71, "Moisturizer", 80.0, 30, "Beauty"),
                Product(72, "Face Wash", 50.0, 45, "Beauty"),
                Product(73, "Nail Polish Set", 90.0, 20, "Beauty"),
                Product(74, "Sunscreen", 70.0, 25, "Beauty"),
                Product(75, "Hair Dryer", 200.0, 15, "Beauty"),

                // Toys
                Product(76, "Toy Car", 50.0, 40, "Toys"),
                Product(77, "Lego Set", 300.0, 12, "Toys"),
                Product(78, "Teddy Bear", 80.0, 25, "Toys"),
                Product(79, "Puzzle 1000 pcs", 100.0, 15, "Toys"),
                Product(80, "Remote Control Helicopter", 250.0, 8, "Toys"),
                Product(81, "Doll", 60.0, 30, "Toys"),
                Product(82, "Board Game: Monopoly", 150.0, 20, "Toys"),
                Product(83, "Rubik’s Cube", 25.0, 50, "Toys"),
                Product(84, "Water Gun", 40.0, 30, "Toys"),
                Product(85, "Drone", 1200.0, 5, "Toys"),

                // Automotive
                Product(86, "Car Wax", 80.0, 20, "Automotive"),
                Product(87, "Car Vacuum Cleaner", 200.0, 10, "Automotive"),
                Product(88, "Motor Oil 4L", 150.0, 25, "Automotive"),
                Product(89, "Car Battery", 400.0, 8, "Automotive"),
                Product(90, "Car Cover", 180.0, 12, "Automotive"),
                Product(91, "Dash Cam", 350.0, 6, "Automotive"),
                Product(92, "Air Freshener", 20.0, 40, "Automotive"),
                Product(93, "Car Jack", 220.0, 10, "Automotive"),
                Product(94, "Wiper Blades", 50.0, 30, "Automotive"),
                Product(95, "Tool Kit", 300.0, 15, "Automotive"),

                // Groceries
                Product(96, "Milk 1L", 6.0, 50, "Groceries"),
                Product(97, "Bread", 4.0, 40, "Groceries"),
                Product(98, "Eggs (12 pcs)", 12.0, 60, "Groceries"),
                Product(99, "Rice 10kg", 50.0, 20, "Groceries"),
                Product(100, "Cooking Oil 5L", 40.0, 25, "Groceries"),
                Product(101, "Sugar 1kg", 5.0, 30, "Groceries"),
                Product(102, "Salt 1kg", 2.0, 40, "Groceries"),
                Product(103, "Coffee Powder 500g", 25.0, 20, "Groceries"),
                Product(104, "Tea Bags (100)", 18.0, 25, "Groceries"),
                Product(105, "Instant Noodles (5 pack)", 12.0, 30, "Groceries"),
                Product(106, "Butter 250g", 15.0, 20, "Groceries"),
                Product(107, "Cheese 200g", 20.0, 15, "Groceries"),
                Product(108, "Chicken 1kg", 18.0, 30, "Groceries"),
                Product(109, "Beef 1kg", 35.0, 15, "Groceries"),
                Product(110, "Apples (1kg)", 10.0, 40, "Groceries"),
                Product(111, "Bananas (1kg)", 8.0, 35, "Groceries"),
                Product(112, "Oranges (1kg)", 12.0, 30, "Groceries"),
                Product(113, "Potatoes (1kg)", 6.0, 40, "Groceries"),
                Product(114, "Onions (1kg)", 5.0, 35, "Groceries"),
                Product(115, "Tomatoes (1kg)", 7.0, 30, "Groceries"),
                Product(116, "Carrots (1kg)", 8.0, 25, "Groceries"),
                Product(117, "Broccoli (500g)", 9.0, 20, "Groceries"),
                Product(118, "Fish (1kg)", 25.0, 18, "Groceries"),
                Product(119, "Shrimp (1kg)", 40.0, 12, "Groceries"),
                Product(120, "Ice Cream 1L", 18.0, 20, "Groceries")
            };

            vector<CartItem> cart;

            Menu::showMenu(products, cart);
        }
    }

    return 0;
}
