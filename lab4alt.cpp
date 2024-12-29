// lab4alt.cpp: определяет точку входа для приложения.
//

#include "lab4alt.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <memory>
#include <sstream>

using namespace std;

class Product{
protected: string name; double price; int quantity;
public:
    Product(const string name, double price, int quantity): name(name), price(price), quantity(quantity){}
    virtual void displayInfo() const{cout<<"Product Name: "<<name<<", Price: "<<price<<", Quantity: "<<quantity<<endl;}
    double getPrice() const{return price;}; const string getName() const{return name;}; int getQuantity() const{return quantity;}
    void setPrice(double newPrice){price=newPrice;}; void setQuantity(int newQuantity){quantity=newQuantity;}
    virtual ~Product()=default;
};

class Clothing: public Product{
public:
    Clothing(const string name, double price, int quantity): Product(name, price, quantity){}
    void displayInfo() const override{cout<<"Clothing - "; Product::displayInfo();}
};

class Electronics: public Product{
public:
    Electronics(const string name, double price, int quantity): Product(name, price, quantity){}
    void displayInfo() const override{cout<<"Electronics - "; Product::displayInfo();}
};

class Store{
private: vector<unique_ptr<Product>> products;
public:
    void addProduct(const string filename){
        string name; double price=0.0; int quantity=0; int category=0;
        cout<<"Enter product name: "; cin>>name;
        cout<<"Enter product price: "; cin>>price;
        cout<<"Enter product quantity: "; cin>>quantity;
        cout<<"Select category (1 - Clothing, 2 - Electronics): "; cin>>category;
        if(category==1){
            products.push_back(make_unique<Clothing>(name, price, quantity)); ofstream outFile(filename, ios::app);
            outFile<<name<<","<<price<<","<<quantity<<endl; outFile.close();}
        else if(category==2){
            products.push_back(make_unique<Electronics>(name, price, quantity)); ofstream outFile(filename, ios::app);
            outFile<<name<<","<<price<<","<<quantity<<endl; outFile.close();}
        else{cout<<"Invalid category selected."<<endl;}}
    void searchProduct(){
        string name; cout<<"Enter product name to search: "; cin>>name;
        for(const auto& product: products){if(product->getName()==name){product->displayInfo(); return;}}
        cout<<"Product not found."<<endl;}
    void sortProductsByPrice(){
        sort(products.begin(), products.end(), 
            [](const unique_ptr<Product>& a, const unique_ptr<Product>& b){return a->getPrice()<b->getPrice();});
        cout<<"Products sorted by price."<<endl;}
    void loadFromFile(const string filename){
        ifstream inFile(filename); string line;
        while(getline(inFile, line)){
            string name; double price=0.0; int quantity=0; char delimiter; istringstream ss(line);
            getline(ss, name, ','); ss>>price>>delimiter>>quantity;
            products.push_back(make_unique<Product>(name, price, quantity));}
        inFile.close();}//      Пришлось обойтись без saveToFile, зато теперь ее функции включены в addProduct
    void modifyProduct(const string& productName){
        for(auto& product: products){
            if(product->getName()==productName){
                double newPrice=0.0; int newQuantity=0;
                cout<<"Enter new price: "; cin>>newPrice;
                cout<<"Enter new quantity: "; cin>>newQuantity;
                product->setPrice(newPrice); product->setQuantity(newQuantity); return;}}
        cout<<"Product not found."<<endl;}
    void displayAllProducts() const{
        int f=1; for(const auto& product: products){f=0; product->displayInfo();}; if(f){cout<<"Products not found."<<endl;}}
};

int main(){
    Store store; int choice=-1;
    while(true){
        cout<<"Menu:\n1. Add Product\n2. Search Product\n3. Sort Products by Price\n4. Display All Products\n5. Load Products from File\n"
            <<"6. Modify Product\n0. Exit\n\nEnter your choice: ";
        cin>>choice; cout<<"\n\n";
        switch(choice){
            case 1: store.addProduct("savedproducts.txt"); cout<<"\n\n\n\n\n"; break;
            case 2: store.searchProduct(); cout<<"\n\n\n\n\n"; break;
            case 3: store.sortProductsByPrice(); cout<<"\n\n\n\n\n"; break;
            case 4: store.displayAllProducts(); cout<<"\n\n\n\n\n"; break;
            case 5: store.loadFromFile("savedproducts.txt"); cout<<endl; break;
            case 6: {string name; cout<<"Enter product name to modify: "; cin>>name; store.modifyProduct(name); cout<<"\n\n\n\n\n"; break;}
            case 0: return 0;
            default: cout<<"Invalid choice. Try again.\n\n\n\n\n\n";}}
    return 0;
}
