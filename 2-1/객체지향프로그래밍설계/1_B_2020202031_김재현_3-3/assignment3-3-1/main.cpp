#include "Sale.h"

int main()
{
	double price = 0;
	double discount = 0;
	cout << "===================================\n";
	cout << "Price Compare Program\n";
	cout << "===================================\n";
	
	cout << "Insert item1 price: "; cin >> price; Sale sale1(price);
	cout << "Insert item2 price: "; cin >> price; Sale sale2(price);

	cout << endl;
	cout << "-----------------------------------\n";
	cout << "Insert discount percent: "; cin >> discount;
	DiscountSale discountSale1(sale1.GetPrice(), discount);
	DiscountSale discountSale2(sale2.GetPrice(), discount);
	cout << "-----------------------------------\n";
	cout << "Result:" << endl;
	if (discountSale1 < discountSale2) {
		cout << "Discount price of item1 is cheaper.\n";
		cout << "Saving discount price is $"; cout << discountSale1.Savings(discountSale2);
	}
	else {
		cout << "Discount price of item2 is cheaper.\n";
		cout << "Saving discount price is $"; cout << discountSale2.Savings(discountSale1);
	}
}