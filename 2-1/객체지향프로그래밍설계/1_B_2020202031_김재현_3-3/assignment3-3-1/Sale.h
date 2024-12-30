#pragma once
#include<iostream>

using namespace std;

class Sale
{
private:
	double Price;

public: 
	Sale();
	Sale(double ThePrice);
	~Sale();
	
	double GetPrice() const;
	virtual double Bill();
	double Savings(Sale& Other);
	bool operator<(Sale& Other);
};

class DiscountSale : public Sale
{
private:
	double Discount;
public:
	DiscountSale();
	DiscountSale(double ThePrice, double theDiscount);
	~DiscountSale();
	double GetDiscount();
	void SetDiscount();
	double Bill() const;
	double Savings(DiscountSale& Other) const;
	bool operator<(const DiscountSale& second);
};