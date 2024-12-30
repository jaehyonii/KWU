#include "Sale.h"

/* class Sale*/
Sale::Sale() {
	Price = 0;
}
Sale::Sale(double ThePrice) {
	Price = ThePrice;
}
Sale::~Sale(){}

double Sale::GetPrice() const{ // Sale�� ��ӹ��� Ŭ�������� Price�� ������ �� �ִ� ��ġ
	return Price;
}
double Sale::Bill() {
	return Price;
}
double Sale::Savings(Sale& Other) { // ������ ���� �ʾ����Ƿ� ������ ����� 0�̴�.
	return 0;
}
bool Sale::operator<(Sale& Other) { // �ٸ� ���ݰ� ��
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

double DiscountSale::GetDiscount() { // ������ ��ȯ
	return Discount;
}
void DiscountSale::SetDiscount() { // ������ �Է¹ޱ�
	cout << "Insert discount percent: ";
	cin >> Discount;
}
double DiscountSale::Bill() const {
	return GetPrice() * (100 - Discount) / 100; // �������� ������ ����
}
double DiscountSale::Savings(DiscountSale& Other) const{
	return GetPrice() * Discount / 100;
}
bool DiscountSale::operator<(const DiscountSale& second) { // �������� ������ ���ݳ��� ��
	return (Bill()<second.Bill());
}