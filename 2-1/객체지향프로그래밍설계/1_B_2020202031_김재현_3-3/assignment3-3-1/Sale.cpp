#include "Sale.h"

/* class Sale*/
Sale::Sale() {
	Price = 0;
}
Sale::Sale(double ThePrice) {
	Price = ThePrice;
}
Sale::~Sale(){}

double Sale::GetPrice() const{ // Sale을 상속받은 클래스들이 Price에 접근할 수 있는 장치
	return Price;
}
double Sale::Bill() {
	return Price;
}
double Sale::Savings(Sale& Other) { // 할인을 받지 않았으므로 절약한 비용은 0이다.
	return 0;
}
bool Sale::operator<(Sale& Other) { // 다른 가격과 비교
	return (Price < Other.Price);
}



/* class DiscountSale*/
DiscountSale::DiscountSale() {
	Discount = 0;
}
DiscountSale::DiscountSale(double ThePrice, double theDiscount)
	:Sale(ThePrice)
{
	Discount = theDiscount;
}
DiscountSale::~DiscountSale() {}

double DiscountSale::GetDiscount() { // 할인율 반환
	return Discount;
}
void DiscountSale::SetDiscount() { // 할인율 입력받기
	cout << "Insert discount percent: ";
	cin >> Discount;
}
double DiscountSale::Bill() const {
	return GetPrice() * (100 - Discount) / 100; // 할인율을 적용한 가격
}
double DiscountSale::Savings(DiscountSale& Other) const{
	return GetPrice() * Discount / 100;
}
bool DiscountSale::operator<(const DiscountSale& second) { // 할인율을 적용한 가격끼리 비교
	return (Bill()<second.Bill());
}