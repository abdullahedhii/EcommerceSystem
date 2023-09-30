#include<iostream>
#include<string>
#include<fstream>
#include<conio.h>
#include<stdlib.h>
#include<iomanip>
#include<windows.h>
#include<vector>
using namespace std;

//forward declarations
class File;
class Person;
class Customer;
class Seller;
class Product;
class Clothing;
class Shoes;
class Accessories;
class Order;
class Cart;

//templates for products(derived classes have similar functionalities)
template<class T> class WriteProducts;
template<class T> class ReadProducts;
template<class T> class UpdateRecords;
template<class T> void addProductForSale(string);

//templates for Seller and Customer
template<class T> bool Login(T& obj);
template<class T> void SignUp(T& obj);
template<class T> bool readAccount(T& obj, string ID, string Password);
template<class T> void writeAccount(T obj);

//to convert category to upper cases before writing to file
string toUP(string str) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] != '-' && (str[i] >= 'a' && str[i] <= 'z'))
			str[i] -= 32;
	}
	return str;
}

class File {
	string fileName;
public:
	File(string fileName) :fileName(fileName) {}
	void writeOrder(Order&);
	Order readOrder(string, string);
	string updateOrderStatus(string, string);
	vector<Order> readSellerOrders(string);
};


class Person {
protected:
	string firstName, lastName;
	int age;
	string gender, country, city, emailAddress, phoneNumber;
public:
	Person() {}
	string getName() { return firstName + " " + lastName; }
	string virtual getFileName() = 0;
};


class Customer :public Person {
	string accountID, accountPassword;
	template<class T> friend bool Login(T& obj);
	template<class T> friend void SignUp(T& obj);
	template<class T>friend bool readAccount(T& obj, string, string);
	template<class T> friend void writeAccount(T obj);
	friend class Order;
public:
	Customer() {
		srand(time(0));
		accountID = "Customer#" + to_string(rand() % 292929 + 'A') + char(rand() % 25 + 'A');
		phoneNumber = accountPassword = " ";
		firstName = lastName = country = emailAddress = phoneNumber = " ";
		age = 0;
	}
	string getID() { return accountID; }
	void placeOrder();
	void trackOrder();

	friend ostream& operator<<(ostream& os, const Customer& customer) {
		cout << endl << endl << "Customer ID: " << customer.accountID << endl;
		cout << "Name: " << customer.firstName + " " + customer.lastName << endl;
		cout << "Age: " << customer.age << endl;
		cout << "Gender: " << customer.gender << endl;
		cout << "Country: " << customer.country << endl;
		cout << "City: " << customer.city << endl;
		cout << "Email address: " << customer.emailAddress << endl;
		cout << "Phone number: " << customer.phoneNumber << endl;
		return os;
	}
	string getFileName() { return "Customers.txt"; }
};


class Seller :public Person {
	string accountID;
	string accountPassword;
	template<class T> friend bool Login(T& obj);
	template<class T> friend void SignUp(T& obj);
	template<class T>friend bool readAccount(T& obj, string, string);
	template<class T> friend void writeAccount(T obj);
public:
	Seller() {
		accountID = "Seller#" + to_string(rand() % 399393 + 'A') + char(rand() % 25 + 'A');
		phoneNumber = accountPassword = " ";
		firstName = lastName = country = emailAddress = phoneNumber = " ";
		age = 0;
	}
	void addItemForSale();
	void updateOrderStatus();
	void viewMyOrders();
	string getFileName() { return "Sellers.txt"; }
};


template<class T>
void writeAccount(T obj) {
	ofstream file(obj.getFileName(), ios::app);
	file << obj.firstName << endl << obj.lastName << endl << obj.accountID << endl << obj.accountPassword << endl << obj.age << endl << obj.gender << endl << obj.country << endl <<
		obj.city << endl << obj.emailAddress << endl << obj.phoneNumber << endl;
	file.close();
}

template<class T>
bool readAccount(T& obj, string ID, string password) {
	ifstream file(obj.getFileName(), ios::in);
	string line;
	bool found = false;
	while (getline(file >> ws, line) && !found) {
		obj.firstName = line; getline(file >> ws, line);
		obj.lastName = line; getline(file >> ws, line);
		obj.accountID = line; getline(file >> ws, line);
		obj.accountPassword = line; getline(file >> ws, line);
		obj.age = stoi(line); getline(file >> ws, line);
		obj.gender = line; getline(file >> ws, line);
		obj.country = line; getline(file >> ws, line);
		obj.city = line; getline(file >> ws, line);
		obj.emailAddress = line; getline(file >> ws, line);
		obj.phoneNumber = line;
		if (obj.accountPassword == password && obj.accountID == ID) {
			found = true;
		}
	}
	file.close();
	return found;
}


template<class T>
bool Login(T& obj) {
	system("cls");
	bool identity = false;
	string ID; ;
login:
	if (obj.getFileName() == "Customers.txt") {
		identity = true;
		cout << "\n--------------------------------------CUSTOMER LOGIN PORTAL---------------------------------------------" << endl;
	}
	else 	cout << "\n--------------------------------------SELLER LOGIN PORTAL---------------------------------------------" << endl;
	vector<char>temp;
	if (identity)
		cout << endl << "\t\t\tEnter your customer ID: ";
	else cout << endl << "\t\t\tEnter your seller ID: ";
	cin >> ID;
	cout << "\t\t\tEnter your account password: ";
	char ch = _getch();
	while (ch != 13) {
		if (ch != 8) {
			cout << "*";
			temp.push_back(ch);
		}
		else {
			try {
				if (temp.empty()) {
					throw runtime_error("Cannot delete from an empty password!");
				}
				temp.pop_back();
				system("cls");
				if (obj.getFileName() == "Customers.txt") {
					identity = true;
					cout << endl << "\t\t\t\t\tWelcome to customer login portal" << endl;
				}
				else cout << endl << "\t\t\t\t\tWelcome to Seller login portal" << endl;
				if (identity)
					cout << endl << "\t\t\tEnter your customer ID: ";
				else cout << endl << "\t\t\tEnter your seller ID: ";
				cout << ID << endl;
				cout << "\t\t\tEnter your account password: ";
				for (int i = 0; i < temp.size(); i++) cout << "*";
			}
			catch (const runtime_error& e) {
				cerr << "Error: " << e.what() << std::endl;
				cin.clear();
				system("cls");
				goto login;
			}
		}
		ch = _getch();
	}
	string password(temp.begin(), temp.end());
	bool found = readAccount<T>(obj, ID, password);
	if (!found) {
		T d;
		obj = d;
		cout << "\nYour account was not found.Please try again or try creating a new account!";
	}
	else {
		if (obj.gender == "Male")
			cout << endl << "\t\t\tWelcome back!Mr. " << obj.getName();
		else if (obj.gender == "Female") cout << endl << "\tWelcome back!Mrs. " << obj.getName();
	}
	return found;
}


template<class T>
void SignUp(T& obj) {
	int t;
	system("cls");
	if (obj.getFileName() == "Customers.txt") {
		cout << "\n--------------------------------------CUSTOMER SIGNUP PORTAL---------------------------------------------" << endl;
		cout << endl << "Your customer ID is: " << obj.accountID << endl;
	}
	else {
		cout << "\n--------------------------------------SELLER SIGNUP PORTAL---------------------------------------------" << endl;
		cout << endl << "Your seller ID is: " << obj.accountID << endl;
	}
	cout << "Please enter the following details to proceed further:" << endl;
	cout << "First name: "; cin.ignore(); getline(cin, obj.firstName);
	cout << "Last name: "; getline(cin, obj.lastName);
	cout << "Age: "; cin >> obj.age;
	while (true) {
		cout << "Gender(press 1 for male,press 2 for female): "; cin >> t;
		try {
			if (cin.fail()) {
				cin.clear(); cin.ignore(1000, '\n');
				throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
			}
			break;
		}
		catch (const exception& e) {
			cerr << e.what() << std::endl;
		}
	}
	if (t == 1) obj.gender = "Male"; else if (t == 2) obj.gender = "Female";
	cout << "Country: "; cin.ignore(); getline(cin, obj.country);
	cout << "City: "; getline(cin, obj.city);
	cout << "Email Address: "; cin >> obj.emailAddress;
	cout << "Phone number: "; cin.ignore(); getline(cin, obj.phoneNumber);
	vector<char>Password;
setPassword:
	system("cls");
	cout << endl << "\n\n\t\tNow please create a Password for your account." << endl << "The password must contain atleast 1 upper,1 lower , 1 special letter,and no spaces! ";
	cout << "\nEnter here: ";
	char ch = _getch();
	while (ch != 13) {
		if (ch != 8) {
			cout << "*";
			Password.push_back(ch);
		}
		else {
			Password.pop_back();
			system("cls");
			cout << endl << "\n\n\t\tNow please create a Password for your account." << endl << "The password must contain atleast 1 upper,1 lower , 1 special letter,and no spaces! ";
			cout << "\nEnter here: ";
			for (int i = 0; i < Password.size(); i++) cout << "*";
		}
		ch = _getch();
	}
	bool upper = false, lower = false, space = false, special = false;
	for (int i = 0; i < Password.size(); i++) {
		if (Password[i] == 32) {
			space = true;
		}
		else if (Password[i] >= 'a' && Password[i] <= 'z') {
			lower = true;
		}
		else if (Password[i] >= 'A' && Password[i] <= 'Z') {
			upper = true;
		}
		else if (!(Password[i] >= 48 && Password[i] <= 57)) {
			special = true;
		}
	}

	if (special && upper && lower && (!space)) {
		string t(Password.begin(), Password.end());
		obj.accountPassword = t;
		cout << "\nWelcome,Mr/Mrs. " << obj.firstName << " " << obj.lastName << " Account created successfully" << endl << "Hope you have an amazing experience" << endl;
		writeAccount<T>(obj);
	}
	else {
		cout << "\nPlease note that the password must follow the given constraints(1 upper,1 lower , 1 special letter,and no spaces)" << endl;
		for (int i = Password.size(); i > 0; i--) {
			Password.pop_back();
		}
		goto setPassword;
	}
}



class Product {
protected:
	string productID, sellerID;
	string productType, productBrand;
	string demographic, description;
	string category, material;
	string* sizesAvailable, * colorsAvailable;
	int* stockOfeachsize, sizes, colors;
	float price;
public:
	Product() {}
	virtual void displayProductDetails()const {
		cout << endl << endl << setw(30) << "Product ID: " << productID << endl;
		cout << setw(30) << "Product Type: " << productType << endl;
		cout << setw(30) << "Product brand: " << productBrand << endl;
		cout << setw(30) << "Seller ID: " << sellerID << endl;
		cout << setw(30) << "Demographic: " << demographic << endl;
		cout << setw(30) << "Description: " << description << endl;
		cout << setw(30) << "Price: " << price << endl;
	}
	virtual void displayProductDetails(bool ordered)const {
		cout << endl << endl << setw(30) << "Product ID: " << productID << endl;
		cout << setw(30) << "Product Type: " << productType << endl;
		cout << setw(30) << "Product brand: " << productBrand << endl;
		cout << setw(30) << "Demographic: " << demographic << endl;
		cout << setw(30) << "Description: " << description << endl;
		cout << setw(30) << "Price: " << price << endl;
	}
	string getID() { return productID; }
	virtual string getFileName() = 0;
};

class Clothing :public Product {
	template<class T> friend class WriteProducts;
	template<class T> friend class ReadProducts;
	template<class T>friend class UpdateRecords;
	template<class T>  void friend addProductForSale(string);
	friend class Cart;
	friend class File;
	string colorBought, sizeBought;
	int quantityBought;
public:
	Clothing() {}
	void displayProductDetails()const {
		Product::displayProductDetails();
		cout << setw(30) << "Clothing Type: " << category << endl;
		cout << setw(30) << "Material : " << material << endl;
		cout << endl << setw(30) << "Colors avaiable: ";
		for (int i = 0; i < colors; i++) cout << " " << colorsAvailable[i];
		cout << endl << setw(30) << "Sizes avaiable: ";
		for (int i = 0; i < sizes; i++) cout << " " << sizesAvailable[i];
	}
	void displayProductDetails(bool ordered)const {
		Product::displayProductDetails();
		cout << setw(30) << "Color Bought: " << colorBought << endl;
		cout << setw(30) << "Size Bought: " << sizeBought << endl;
		cout << setw(30) << "Quantity Bought: " << quantityBought << endl;
	}
	vector<Clothing> displayAllItems(string demographic, string clothingType);
	string getFileName() { return "Clothing.txt"; }
	void storeRecord(Clothing&);
};

class Shoes :public Product {
	template<class T> friend class WriteProducts;
	template<class T> friend class ReadProducts;
	template<class T>friend class UpdateRecords;
	template<class T>  void friend addProductForSale(string);
	friend class File;
	friend class Cart;
	string colorBought, sizeBought;
	int quantityBought;

public:
	Shoes() {}
	void displayProductDetails()const {
		Product::displayProductDetails();
		cout << setw(30) << "Shoes Type: " << category << endl;
		cout << endl << setw(30) << "Colors avaiable: ";
		for (int i = 0; i < colors; i++) cout << " " << colorsAvailable[i];
		cout << endl << setw(30) << "Sizes avaiable: ";
		for (int i = 0; i < sizes; i++) cout << " " << sizesAvailable[i];
	}
	void displayProductDetails(bool ordered)const {
		Product::displayProductDetails();
		cout << setw(30) << "Color Bought: " << colorBought << endl;
		cout << setw(30) << "Size Bought: " << sizeBought << endl;
		cout << setw(30) << "Quantity Bought: " << quantityBought << endl;
	}
	vector<Shoes> displayAllItems(string demographic, string shoeType);
	string getFileName() { return "Shoes.txt"; }
	void storeRecord(Shoes&);
};


class Accessories :public Product {
	template<class T> friend class WriteProducts;
	template<class T> friend class ReadProducts;
	template<class T>friend class UpdateRecords;
	template<class T>  void friend addProductForSale(string);
	friend class File;
	friend class Cart;
	string colorBought, sizeBought;
	int quantityBought;
public:
	Accessories() {}
	void displayProductDetails() const {
		Product::displayProductDetails();
		cout << setw(30) << "Accessory Type: " << category << endl;
		cout << endl << setw(30) << "Colors avaiable: ";
		for (int i = 0; i < colors; i++) cout << " " << colorsAvailable[i];
		cout << endl << setw(30) << "Sizes avaiable: ";
		for (int i = 0; i < sizes; i++) cout << " " << sizesAvailable[i];
	}
	void displayProductDetails(bool ordered)const {
		Product::displayProductDetails();
		cout << setw(30) << "Color Bought: " << colorBought << endl;
		cout << setw(30) << "Size Bought: " << sizeBought << endl;
		cout << setw(30) << "Quantity Bought: " << quantityBought << endl;
	}
	vector<Accessories> displayAllItems(string demographic, string accessoryType);
	string getFileName() { return "Accessories.txt"; }
	void storeRecord(Accessories&);
};

template<class T>
class WriteProducts {
	T obj;
	string fileName;
public:
	WriteProducts(T objtowrite) {
		obj = objtowrite;
		fileName = obj.getFileName();
		if (fileName == "Clothing.txt")  obj.productType = "Clothing";
		else if (fileName == "Shoes.txt") obj.productType = "Shoes";
		else if (fileName == "Accessories.txt") obj.productType = "Accessories";

		ofstream file(fileName, ios::app);
		file << obj.productID << endl << obj.sellerID << endl << obj.productType << endl << obj.productBrand << endl << obj.demographic << endl << obj.description << endl << obj.price << endl
			<< obj.category << endl << obj.material << endl << obj.colors << endl;
		for (int i = 0; i < obj.colors; i++) file << obj.colorsAvailable[i] << endl;
		file << obj.sizes << endl;
		for (int i = 0; i < obj.sizes; i++) {
			file << obj.sizesAvailable[i] << endl << obj.stockOfeachsize[i] << endl;
		}
		cout << endl << "Product " << obj.productID << " by " << obj.productBrand << " has been successfully added to available items." << endl;
		file.close();
		ifstream file2;
		ofstream file3;
		if (fileName == "Clothing.txt") {
			file2.open("ClothingCategories.txt", ios::in);
			file3.open("ClothingCategories.txt", ios::app);
		}
		else if (fileName == "Shoes.txt") {
			file2.open("ShoeCategories.txt", ios::in);
			file3.open("ShoeCategories.txt", ios::app);
		}
		else if (fileName == "Accessories.txt") {
			file2.open("AccessoryCategories.txt", ios::in);
			file3.open("AccessoryCategories.txt", ios::app);
		}
		string line;
		bool exists = false;
		while (getline(file2, line)) {
			if (line == obj.category) {
				exists = true;
				break;
			}
		}
		if (!exists)file3 << obj.category << endl;
		file2.close();
		file3.close();
	}

};

void Clothing::storeRecord(Clothing& obj) {
	WriteProducts<Clothing>writes(obj);
}

void Shoes::storeRecord(Shoes& obj) {
	WriteProducts<Shoes>writes(obj);
}
void Accessories::storeRecord(Accessories& obj) {
	WriteProducts<Accessories>writes(obj);
}


template<class T>
class ReadProducts {
	vector<T>items;
	T temp;
	string fileName;
	string demographic, category;
public:
	ReadProducts(string demographic, string category) :demographic(demographic), category(category) {
		fileName = temp.getFileName();
	}

	vector<T> readItems() {
		string line;
		ifstream file(fileName, ios::in);
		while (getline(file >> ws, line)) {
			int flag = 0;
			temp.productID = line; getline(file >> ws, line); temp.sellerID = line; getline(file >> ws, line);
			temp.productType = line; getline(file >> ws, line);
			temp.productBrand = line; getline(file >> ws, line); temp.demographic = line; getline(file >> ws, line);
			temp.description = line; getline(file >> ws, line); temp.price = stof(line); getline(file >> ws, line);
			temp.category = line; getline(file >> ws, line); temp.material = line;		getline(file >> ws, line);
			temp.colors = stoi(line); getline(file >> ws, line);
			temp.colorsAvailable = new string[temp.colors];
			for (int i = 0; i < temp.colors; i++) {
				temp.colorsAvailable[i] = line; getline(file >> ws, line);
			}
			temp.sizes = stoi(line); temp.sizesAvailable = new string[temp.sizes];
			temp.stockOfeachsize = new int[temp.sizes];
			for (int i = 0; i < temp.sizes && getline(file >> ws, line); i++) {
				temp.sizesAvailable[i] = line; getline(file >> ws, line);
				temp.stockOfeachsize[i] = stoi(line);
				if (temp.stockOfeachsize[i] > 0)
					flag++;
			}
			if (temp.category == category && temp.demographic == demographic && flag >= 1) {
				items.push_back(temp);
			}
		}
		file.close();
		return items;
	}
};


vector<Clothing>Clothing::displayAllItems(string demographic, string clothingType) {
	ReadProducts<Clothing> obj(demographic, clothingType);
	vector<Clothing>items = obj.readItems();
	for (int i = 0; i < items.size(); i++) {
		cout << endl << "Item #" << i + 1 << endl;
		items[i].displayProductDetails();
	}
	return items;
}


vector<Shoes>Shoes::displayAllItems(string demographic, string shoesType) {
	ReadProducts<Shoes> obj(demographic, shoesType);
	vector<Shoes>items = obj.readItems();
	for (int i = 0; i < items.size(); i++) {
		cout << endl << "Item #" << i + 1 << endl;
		items[i].displayProductDetails();
	}
	return items;
}

vector<Accessories> Accessories::displayAllItems(string demographic, string accessoryType) {
	ReadProducts<Accessories> obj(demographic, accessoryType);
	vector<Accessories>items = obj.readItems();
	for (int i = 0; i < items.size(); i++) {
		cout << endl << "Item #" << i + 1 << endl;
		items[i].displayProductDetails();
	}
	return items;
}


template<class T>
class UpdateRecords {
	string fileName;
	string productID;
	T temp;
public:
	UpdateRecords() {
		fileName = temp.getFileName();
	}
	bool update(string ID, int quantity, string size) {
		string brand;
		string line;
		bool availability = true;
		ifstream file(fileName, ios::in);
		ofstream file2("Newfile.txt", ios::app);
		while (getline(file >> ws, line)) {
			temp.productID = line; getline(file >> ws, line);  temp.sellerID = line; getline(file >> ws, line);
			temp.productType = line; getline(file >> ws, line);
			brand = line; temp.productBrand = line; getline(file >> ws, line); temp.demographic = line; getline(file >> ws, line);
			temp.description = line; getline(file >> ws, line); temp.price = stof(line); getline(file >> ws, line);
			temp.category = line; getline(file >> ws, line); temp.material = line;		getline(file >> ws, line);
			temp.colors = stoi(line); getline(file >> ws, line);
			temp.colorsAvailable = new string[temp.colors];
			for (int i = 0; i < temp.colors; i++) {
				temp.colorsAvailable[i] = line; getline(file >> ws, line);
			}
			temp.sizes = stoi(line); temp.sizesAvailable = new string[temp.sizes];
			temp.stockOfeachsize = new int[temp.sizes];
			for (int i = 0; i < temp.sizes && getline(file >> ws, line); i++) {
				temp.sizesAvailable[i] = line; getline(file >> ws, line);
				temp.stockOfeachsize[i] = stoi(line);
			}
			if (temp.productID == ID) {
				for (int i = 0; i < temp.sizes; i++) {
					if (temp.sizesAvailable[i] == size) {
						if (quantity <= temp.stockOfeachsize[i]) {
							temp.stockOfeachsize[i] -= quantity;
							break;
						}
						else availability = false;
					}
				}
			}
			if (availability) {
				file2 << temp.productID << endl << temp.sellerID << endl << temp.productType << endl << temp.productBrand << endl << temp.demographic << endl << temp.description << endl << temp.price << endl
					<< temp.category << endl << temp.material << endl << temp.colors << endl;
				for (int i = 0; i < temp.colors; i++) file2 << temp.colorsAvailable[i] << endl;
				file2 << temp.sizes << endl;
				for (int i = 0; i < temp.sizes; i++) {
					file2 << temp.sizesAvailable[i] << endl << temp.stockOfeachsize[i] << endl;
			}
			}
		}
		file.close();
		file2.close();
		if (!availability) {
			cout << "Your requested quantity of item" << ID << "by " << brand << " is unavailable!" << endl;
			cout << "___________________________________________________________________________________________________________" << endl;
		}
		else {
			cout << endl << quantity << " items of product ID " << ID << " by " << brand << " have been added to your cart." << endl;
			cout << "____________________________________________________________________________________________________________" << endl;
			remove(fileName.c_str());
			rename("Newfile.txt", fileName.c_str());
		}
		return availability;
	}

};

string getDemo() {
	int t;
	cout << "\n\tPress 1 for men." << endl << "\tPress 2 for women." << endl << "\tPress 3 for kids." << endl << "\tEnter: ";
	cin >> t;
	if (t == 1) return "Men";
	else if (t == 2) return "Women";
	else if (t == 3) return "Kids";
}


template<class T>
void addProductForSale(string sellerID) {
	T obj;
	obj.sellerID = sellerID;
	cout << "______________________________________________________________________________________________________________________" << endl;
	cout << endl << "Enter product ID: "; cin >> obj.productID;
	cout << "Enter product brand: "; cin.ignore(); getline(cin, obj.productBrand);
	cout << "Enter demographic "; obj.demographic = getDemo();
	cout << "Enter a little description of the product: "; cin.ignore(); getline(cin, obj.description);
	cout << "Enter product price: "; cin >> obj.price;
	cout << "Enter category (For e.g for Clothing:T-shirts,shirts,dresses..for Shoes:Sandals,joggers..for accessories:socks,belts..): "; cin.ignore(); getline(cin, obj.category);
	obj.category = toUP(obj.category);
	cout << "Enter material: "; getline(cin, obj.material);
	while (true) {
		cout << "Enter number of colors available "; cin >> obj.colors;
		try {
			if (cin.fail()) {
				cin.clear(); cin.ignore(1000, '\n');
				throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
			}
			break;
		}
		catch (const exception& e) {
			cerr << e.what() << std::endl;
		}
	}
	while (true) {
		cout << "Enter number of sizes available: "; cin >> obj.sizes;
		try {
			if (cin.fail()) {
				cin.clear(); cin.ignore(1000, '\n');
				throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
			}
			break;
		}
		catch (const exception& e) {
			cerr << e.what() << std::endl;
		}
	}
	obj.colorsAvailable = new string[obj.colors];
	for (int i = 0; i < obj.colors; i++) {
		cout << "Enter color#" << i + 1 << ": ";
		cin.ignore();
		getline(cin, obj.colorsAvailable[i]);
	}
	obj.sizesAvailable = new string[obj.sizes];
	obj.stockOfeachsize = new int[obj.sizes];
	for (int i = 0; i < obj.sizes; i++) {
		int t;
	check:
		if (obj.getFileName() == "Clothing.txt") {
			cout << "Enter sizes#" << i + 1 << " ";
			cout << "(Press 1 for small,press 2 for medium,press 3 for large,press 4 for extra large): ";
			cin >> t;
			if (t == 1) obj.sizesAvailable[i] = "Small"; else if (t == 2) obj.sizesAvailable[i] = "Medium";
			else if (t == 3) obj.sizesAvailable[i] = "Large"; else if (t == 4) obj.sizesAvailable[i] = "Extra Large";
			else {
				cout << "Invalid choice!!" << endl;
				goto check;
			}
		}
		else {
			cout << "Enter sizes#" << i + 1 << ": ";
			cin >> obj.sizesAvailable[i];
		}
		while (true) {
			cout << "Enter stock of size  " << obj.sizesAvailable[i] << ": ";
			cin >> obj.stockOfeachsize[i];
			try {
				if (cin.fail()) {
					cin.clear(); cin.ignore(1000, '\n');
					throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
				}
				break;
			}
			catch (const exception& e) {
				cerr << e.what() << std::endl;
			}
		}
	}
	obj.storeRecord(obj);

}


void Seller::addItemForSale() {
	int type;
	cout << endl << "\nWhich type of item would you like to add \nPress 1 for Clothing\nPress 2 for Shoes\nPress 3 for Accessories\nEnter: ";
	cin >> type;
	if (type == 1) {
		addProductForSale<Clothing>(accountID);
	}
	else if (type == 2) {
		addProductForSale<Shoes>(accountID);
	}
	else if (type == 3) {
		addProductForSale<Accessories>(accountID);
	}
	else {
		cout << endl << "Invalid input!" << endl;
		system("cls");
		addItemForSale();
	}

}

template<class T>
string viewCategories() {
	int a;
	T obj;
	ifstream file;
	string line;
	vector<string>categories;
	if (obj.getFileName() == "Clothing.txt")
		file.open("ClothingCategories.txt", ios::in);
	else if (obj.getFileName() == "Shoes.txt")
		file.open("ShoeCategories.txt", ios::in);
	else if (obj.getFileName() == "Accessories.txt")
		file.open("AccessoryCategories.txt", ios::in);
	while (getline(file, line)) {
		categories.push_back(line);
	}
	file.close();
	if (!categories.empty()) {
		for (int i = 0; i < categories.size(); i++) {
			cout << "\t" << i + 1 << ". " << categories[i] << endl;
		}
		cout << endl << "\tEnter your choice: ";
		cin >> a;
		return categories[a - 1];
	}
	else return "Empty";
}

class Cart {
	vector<Clothing>clothing;
	vector<Accessories>accessories;
	vector<Shoes>shoes;
	friend class File;
	friend class Order;
	float totalBill;
	template<class T>
	T addItemToCart(string demo, string category) {
		ReadProducts<T>obj(demo, category);
		vector<T>x = obj.readItems();
	l:
		if (!x.empty()) {
			cout << "\n\nThe following items are available with your specific criteria" << endl << endl;
			for (int i = 0; i < x.size(); i++) {
				cout << endl << "Item#" << i + 1 << endl;
				x[i].displayProductDetails();
			}
			int num, t;
			cout << endl << endl << "\n\tEnter number # of item you would like to add: ";
			cin >> num;
			if (num > x.size()) {
				cout << "Invalid input!" << endl;
				cout << "Press any key to continue.."; _getch();
				goto l;
			}
		m:
			cout << "\t\tEnter the color you would like to add\n";
			for (int i = 0; i < x[num - 1].colors; i++) {
				cout << "\t\tPress " << i + 1 << " for " << x[num - 1].colorsAvailable[i] << endl;
			}
			cout << "\t\tEnter: ";
			cin >> t;
			if (t > x[num - 1].colors) {
				cout << "Invalid choice!!" << endl;
				goto m;
			}
			x[num - 1].colorBought = x[num - 1].colorsAvailable[t - 1];
		e:
			cout << "\n\tEnter the size you would like add to cart\n ";
			for (int i = 0; i < x[num - 1].sizes; i++) {
				cout << "\t\tPress " << i + 1 << " for " << x[num - 1].sizesAvailable[i] << endl;
			}
			cout << "\t\tEnter: ";
			cin >> t;
			if (t > x[num - 1].sizes) {
				cout << "Invalid choice!!" << endl;
				goto e;
			}
			x[num - 1].sizeBought = x[num - 1].sizesAvailable[t - 1];
			cout << "\n\tEnter quantity you would like to add: ";
			cin >> x[num - 1].quantityBought;
			return x[num - 1];
		}
		else {
			cout << "\n\n\t\tSorry nothing meets your desired criteria!" << endl;
			T temp;
			temp.productID = "Empty";
			return temp;

		}
	}
public:

	void addClothing(string demo, string category) {
		Clothing temp = addItemToCart<Clothing>(demo, category);
		if (temp.productID != "Empty")
			clothing.push_back(temp);
	}
	void addShoes(string demo, string category) {
		Shoes temp = addItemToCart<Shoes>(demo, category);
		if (temp.productID != "Empty")
			shoes.push_back(temp);
	}
	void addAccessories(string demo, string category) {
		Accessories temp = addItemToCart<Accessories>(demo, category);
		if (temp.productID != "Empty")
			accessories.push_back(temp);
	}
	void removeItem(string ID) {
		bool found = false;
		if (!clothing.empty()) {
			for (auto it = clothing.begin(); it != clothing.end(); ++it) {
				if (it->productID == ID) {
					found = true;
					clothing.erase(it);
					break;
				}
			}
		}
		if (!shoes.empty()) {
			for (auto it = shoes.begin(); it != shoes.end(); ++it) {
				if (it->productID == ID) {
					found = true;
					shoes.erase(it);
					break;
				}
			}
		}
		if (!accessories.empty()) {
			for (auto it = accessories.begin(); it != accessories.end(); ++it) {
				if (it->productID == ID) {
					found = true;
					accessories.erase(it);
					break;
				}
			}
		}
		if (found)
			cout << "Item " << ID << " has been removed from your cart!" << endl;
		else cout << "No product with such ID was found in your cart!" << endl;
		cout << "________________________________________________________________________________________________" << endl;
	}

	float finaliseBill() {
		if (!clothing.empty()) {
			for (int i = 0; i < clothing.size(); i++)totalBill += clothing[i].price;
		}
		if (!shoes.empty()) {
			for (int i = 0; i < shoes.size(); i++)totalBill += shoes[i].price;
		}
		if (!accessories.empty()) {
			for (int i = 0; i < accessories.size(); i++)totalBill += accessories[i].price;
		}
		return totalBill;
	}

	friend ostream& operator<<(ostream& os, Cart& cart) {
		cout << "\t\tTotal number of items: " << cart.accessories.size() + cart.clothing.size() + cart.shoes.size() << endl;
		if (!cart.clothing.empty()) {
			cout << "----------------------------------Clothing items-----------------------------------------------" << endl;
			for (int i = 0; i < cart.clothing.size(); i++) {
				cart.clothing[i].displayProductDetails(true);
				cout << endl;
			}
		}
		if (!cart.shoes.empty()) {
			cout << "------------------------------------Shoe items------------------------------------------------- " << endl;
			for (int i = 0; i < cart.shoes.size(); i++) {
				cart.shoes[i].displayProductDetails(true);
				cout << endl;
			}
		}
		if (!cart.accessories.empty()) {
			cout << "------------------------------------Accessory items--------------------------------------------- " << endl;
			for (int i = 0; i < cart.accessories.size(); i++) {
				cart.accessories[i].displayProductDetails(true);
				cout << endl;
			}
		}
		cout << "_____________________________________________________________________________________________________" << endl;
		return os;
	}

	string updateInventory() {

		for (int i = 0; i < clothing.size(); i++) {
			UpdateRecords<Clothing>obj;
			bool check= obj.update(clothing[i].productID, clothing[i].quantityBought, clothing[i].sizeBought);
			if (!check) return clothing[i].productID;
		}
		for (int i = 0; i < shoes.size(); i++) {
			UpdateRecords<Shoes>obj;
			bool check = obj.update(shoes[i].productID, shoes[i].quantityBought, shoes[i].sizeBought); 	
			if (!check) return shoes[i].productID;
		}
		for (int i = 0; i < accessories.size(); i++) {
			UpdateRecords<Accessories>obj;
			bool check = obj.update(accessories[i].productID, accessories[i].quantityBought, accessories[i].sizeBought);	
			if (!check) return accessories[i].productID;
		}
		return "All ok";
	}
};


class Order {
	string customerID, customerName, OrderID;
	float totalBill;
	string paymentType;
	string cardNumber, bankdetails;
	string shippingAddress, postalCode;
	Cart cart;
	friend class File;
	friend class Customer;
	string status;
public:
	Order() {}
	Order(string customerID, string customerName) :customerID(customerID), customerName(customerName) {
		OrderID = "Order#" + to_string(rand() % 100000 + 'A');
		status = "Pending";
	}
	void addItem() {
		int a = 0, b = 0;
		cout << "__________________________________________________________________________________________________" << endl;
	l:
		cout << "\nWhich type of item would you like to add?\nPress 1 for clothing\nPress 2 for Shoes\nPress 3 for Accessories\nEnter here: ";
		cin >> a;
		string category, demo;
		int c;
		cout << "\n\tAvailable Categories: " << endl;
		if (a == 1) category = viewCategories<Clothing>();
		else if (a == 2) category = viewCategories<Shoes>();
		else if (a == 3) category = viewCategories<Accessories>();
		else {
			cout << "Invalid choice!!" << endl;
			goto l;
		}
		if (category != "Empty") {
		m:
			cout << "\nEnter preferred demographic(press 1 for men,2 for women,3 for kids): ";
			cin >> c;
			if (c == 1) demo = "Men";
			else if (c == 2) demo = "Women";
			else if (c == 3) demo = "Kids";
			else {
				cout << "Invalid choice!";
				goto m;
			}
			if (a == 1) cart.addClothing(demo, category);
			else if (a == 2) cart.addShoes(demo, category);
			else if (a == 3) cart.addAccessories(demo, category);
			cout << "___________________________________________________________________________________________________" << endl;
		}
		else {
			cout << "\tNo stock found" << endl;
			addItem();
		}
	}

	void removeItem() {
		string ID;
		cout << "Your cart: " << endl;
		cout << cart;
		cout << endl << "Which item would you like to remove(Enter ID of that product) : ";
		cin >> ID;
		cart.removeItem(ID);
	}

	void CODpayment() {
		cout << "\n\t\tEnter your postal code: ";
		cin >> postalCode;
		cout << "\n\t\tEnter your shipping/billing address: ";
		cin.ignore(); getline(cin, shippingAddress);
	}
	void OnlinePayment() {
		cout << "\n\t\tEnter bank name: ";
		cin.ignore(); getline(cin, bankdetails);
		cout << "\n\t\tEnter card Number: ";
		cin >> cardNumber;
		cout << "\n\t\tA payment of " << totalBill << " rupees has been made from card number " << cardNumber << " under name " << customerName << endl;
	}

	void checkOut() {
		int i;
		string check = cart.updateInventory();
		if (check=="All ok") {
			File file("Orders.txt");
			file.writeOrder(*this);
			cout << *this;
			cout << cart;
			cout << endl << "Please come back within 2-3 days to track your order" << endl;
		}
		else {
			int t;
			cart.removeItem(check);
			cout << "Your updated cart is: " << endl;
			cout << cart;
			while (true) {
				cout << "Would you like to add another item instead(press 0 for no ,otherwise press any number): ";
				cin >> t;
				try {
					if (cin.fail()) {
						cin.clear(); cin.ignore(1000, '\n');
						throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
					}
					break;
				}
				catch (const exception& e) {
					cerr << e.what() << std::endl;
				}
			}
			if (t == 0) {
				customerMenu(customerName);
			}
			else addItem();
		}
	l:
		if (check == "All ok"){
			cout << "\n\t\tPlease enter your payment method\n\t\tPress 1 for Cash on Delivery\n\t\tPress 2 for Online payment\n\t\tEnter:";
			cin >> i;
			if (i == 1) paymentType = "Cash On Delivery";
			else if (i == 2)paymentType = "Online Payment";
			else {
				cout << "Invalid choice!";
				goto l;
			}
			totalBill = cart.finaliseBill();
			if (paymentType == "Cash On Delivery")
				CODpayment();
			else OnlinePayment();

		}	
	}
	friend ostream& operator<<(ostream& os, Order& order) {
		cout << "_______________________________________________________________________________________________" << endl;
		cout << "\t\tOrder ID: " << order.OrderID << endl;
		cout << "\t\tCustomer Name: " << order.customerName << endl;
		cout << "\t\tCustomer ID: " << order.customerID << endl;
		cout << "\t\tTotal bill: " << order.totalBill << " rupees." << endl;
		cout << "\t\tPayment method: " << order.paymentType << endl;
		cout << "\t\tStatus: " << order.status << endl;
		if (order.paymentType == "Cash On Delivery") {
			cout << "\t\tShipping address: " << order.shippingAddress << endl;
			cout << "\t\tPostal Code: " << order.postalCode << endl;
		}
		else if (order.paymentType == "Online Payment") {
			cout << "\t\tBanking details: " << order.bankdetails << endl;
		}
		return os;
	}
	void setStatus(string newStatus) {
		status = newStatus;
	}
	void viewOrderStatus() {
		cout << "\n\n\tOrderID: " << OrderID << endl;
		cout << "\tCustomer name: " << customerName << endl;
		cout << "\tTotal bill: " << totalBill << " rupees." << endl;
		cout << "\tPaymnent method: " << paymentType << endl;
		if (paymentType == "Cash On Delivery") {
			cout << "\tShipping address: " << shippingAddress << endl;
			cout << "\tPostal Code: " << postalCode << endl;
		}
		else {
			cout << "\tBanking details: " << bankdetails << endl;
		}
		cout << "\tOrder status: " << status << endl;
	}

};



void File::writeOrder(Order& obj) {
	ofstream file(fileName, ios::app);
	file << obj.OrderID << endl << obj.customerID << endl << obj.customerName << endl << obj.status << endl << obj.totalBill << endl << obj.paymentType << endl;
	if (obj.paymentType == "Cash On Delivery") {
		file << obj.postalCode << endl << obj.shippingAddress << endl;
	}
	else {
		file << obj.bankdetails << endl << obj.cardNumber << endl;
	}
	file << obj.cart.clothing.size() << endl;
	for (int i = 0; i < obj.cart.clothing.size(); i++) {
		file << obj.cart.clothing[i].productID << endl << obj.cart.clothing[i].sellerID << endl << obj.cart.clothing[i].colorBought << endl
			<< obj.cart.clothing[i].sizeBought << endl << obj.cart.clothing[i].quantityBought << endl;
	}
	file << obj.cart.shoes.size() << endl;
	for (int i = 0; i < obj.cart.shoes.size(); i++) {
		file << obj.cart.shoes[i].productID << endl << obj.cart.shoes[i].sellerID << endl << obj.cart.shoes[i].colorBought << endl
			<< obj.cart.shoes[i].sizeBought << endl << obj.cart.shoes[i].quantityBought << endl;
	}
	file << obj.cart.accessories.size() << endl;
	for (int i = 0; i < obj.cart.accessories.size(); i++) {
		file << obj.cart.accessories[i].productID << endl << obj.cart.shoes[i].sellerID << endl << obj.cart.accessories[i].colorBought << endl
			<< obj.cart.accessories[i].sizeBought << endl << obj.cart.accessories[i].quantityBought << endl;
	}
	file.close();
}

Order File::readOrder(string customerID, string orderID) {
	Order order;
	ifstream file(fileName, ios::in);
	string line;
	bool found = false;
	while (getline(file >> ws, line)) {
		order.OrderID = line; getline(file >> ws, line);
		order.customerID = line; getline(file >> ws, line);
		order.customerName = line; getline(file >> ws, line);
		order.status = line; getline(file >> ws, line);
		order.totalBill = stof(line); getline(file >> ws, line);
		order.paymentType = line; getline(file >> ws, line);
		if (order.paymentType == "Cash On Delivery") {
			order.postalCode = line; getline(file >> ws, line);
			order.shippingAddress = line;
		}
		else {
			order.bankdetails = line; getline(file, line);
			order.cardNumber = line;
		}getline(file, line);
		int clothingitems = stoi(line);
		for (int i = 0; i < clothingitems; i++) {
			Clothing temp;
			getline(file >> ws, line); temp.productID = line;
			getline(file >> ws, line); temp.sellerID = line;
			getline(file >> ws, line); temp.colorBought = line;
			getline(file >> ws, line); temp.sizeBought = line;
			getline(file >> ws, line); temp.quantityBought = stoi(line);
			order.cart.clothing.push_back(temp);
		}getline(file >> ws, line);
		int shoeitems = stoi(line);
		for (int i = 0; i < shoeitems; i++) {
			Shoes temp;
			getline(file >> ws, line); temp.productID = line;
			getline(file >> ws, line); temp.sellerID = line;
			getline(file >> ws, line); temp.colorBought = line;
			getline(file >> ws, line); temp.sizeBought = stoi(line);
			getline(file >> ws, line); temp.quantityBought = stoi(line);
			order.cart.shoes.push_back(temp);
		}getline(file >> ws, line);
		int accessoryitems = stoi(line);
		for (int i = 0; i < accessoryitems; i++) {
			Accessories temp;
			getline(file >> ws, line); temp.productID = line;
			getline(file >> ws, line); temp.sellerID = line;
			getline(file >> ws, line); temp.colorBought = line;
			getline(file, line); temp.sizeBought = stoi(line);
			getline(file >> ws >> ws, line); temp.quantityBought = stoi(line);
			order.cart.accessories.push_back(temp);
		}
		if (order.customerID == customerID && order.OrderID == orderID) {
			found = true;
			break;
		}
	}
	file.close();
	if (found) {
		return order;
	}
	else {
		Order d;
		order = d;
		cout << "No such order was found!" << endl;
		return d;
	}
}

string File::updateOrderStatus(string customerID, string orderID) {
	ifstream file(fileName, ios::in);
	File replaceFile("Updated.txt");
	string line;
	bool found = false;
	string s;
	while (getline(file >> ws, line)) {
		Order order;
		order.OrderID = line; getline(file >> ws, line);
		order.customerID = line; getline(file >> ws, line);
		order.customerName = line; getline(file >> ws, line);
		order.status = line; getline(file >> ws, line);
		order.totalBill = stof(line); getline(file >> ws, line);
		order.paymentType = line; getline(file >> ws, line);
		if (order.paymentType == "Cash On Delivery") {
			order.postalCode = line; getline(file >> ws, line);
			order.shippingAddress = line;
		}
		else {
			order.bankdetails = line; getline(file, line);
			order.cardNumber = line;
		}getline(file, line);
		int clothingitems = stoi(line);
		for (int i = 0; i < clothingitems; i++) {
			Clothing temp;
			getline(file >> ws, line); temp.productID = line;
			getline(file >> ws, line); temp.sellerID = line;
			getline(file >> ws, line); temp.colorBought = line;
			getline(file >> ws, line); temp.sizeBought = line;
			getline(file >> ws, line); temp.quantityBought = stoi(line);
			order.cart.clothing.push_back(temp);
		}getline(file >> ws, line);
		int shoeitems = stoi(line);
		for (int i = 0; i < shoeitems; i++) {
			Shoes temp;
			getline(file >> ws, line); temp.productID = line;
			getline(file >> ws, line); temp.sellerID = line;
			getline(file >> ws, line); temp.colorBought = line;
			getline(file >> ws, line); temp.sizeBought = line;
			getline(file >> ws, line); temp.quantityBought = stoi(line);
			order.cart.shoes.push_back(temp);
		}getline(file >> ws, line);
		int accessoryitems = stoi(line);
		for (int i = 0; i < accessoryitems; i++) {
			Accessories temp;
			getline(file >> ws, line); temp.productID = line;
			getline(file >> ws, line); temp.sellerID = line;
			getline(file >> ws, line); temp.colorBought = line;
			getline(file >> ws, line); temp.sizeBought = line;
			getline(file >> ws, line); temp.quantityBought = stoi(line);
			order.cart.accessories.push_back(temp);
		}
		if (order.customerID == customerID && order.OrderID == orderID) {
			found = true;
			int t;
		l:
			while (true) {
				cout << "Enter the new status of this order\nPress 1 for 'Cancelled' status\nPress 2 for 'Delivered' status\nPress 3 for 'Shipped' status\nEnter:  ";
				cin >> t;
				try {
					if (cin.fail()) {
						cin.clear(); cin.ignore(1000, '\n');
						throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
					}
					break;
				}
				catch (const exception& e) {
					cerr << e.what() << std::endl;
				}
			}
			if (t > 3) {
				cout << "Invalid choice!" << endl;
				cout << "Press any key to continue.."; _getch();
				goto l;
			}
			if (t == 1) order.setStatus("Cancelled");
			else if (t == 2)order.setStatus("Delivered");
			else if (t == 3) order.setStatus("Shipped");
			s = order.status;
		}
		replaceFile.writeOrder(order);
	}
	file.close();
	remove(fileName.c_str());
	rename("Updated.txt", fileName.c_str());
	if (found) {
		return "Order with " + orderID + " has been updated  with the new status " + s + "\n";
	}
	else {
		return "No such order was found";
	}
}

void Customer::trackOrder() {
	string orderID;
	cout << "Enter order ID to track: ";
	cin >> orderID;
	File file("Orders.txt");
	Order order = file.readOrder(accountID, orderID);
	order.viewOrderStatus();

}

vector<Order>File::readSellerOrders(string sellerID) {
	vector<Order>x;
	ifstream file(fileName, ios::in);
	string line, IDread;
	while (getline(file >> ws, line)) {
		Order order;
		order.OrderID = line; getline(file >> ws, line);
		order.customerID = line; getline(file >> ws, line);
		order.customerName = line; getline(file >> ws, line);
		order.status = line; getline(file >> ws, line);
		order.totalBill = stof(line); getline(file >> ws, line);
		order.paymentType = line; getline(file >> ws, line);
		if (order.paymentType == "Cash On Delivery") {
			order.postalCode = line; getline(file >> ws, line);
			order.shippingAddress = line;
		}
		else {
			order.bankdetails = line; getline(file, line);
			order.cardNumber = line;
		}getline(file, line);
		int clothingitems = stoi(line);
		for (int i = 0; i < clothingitems; i++) {
			Clothing temp;
			getline(file >> ws, line); temp.productID = line;
			getline(file >> ws, line); temp.sellerID = line; IDread = temp.sellerID;
			getline(file >> ws, line); temp.colorBought = line;
			getline(file >> ws, line); temp.sizeBought = line;
			getline(file >> ws, line); temp.quantityBought = stoi(line);
			order.cart.clothing.push_back(temp);
		}getline(file >> ws, line);
		int shoeitems = stoi(line);
		for (int i = 0; i < shoeitems; i++) {
			Shoes temp;
			getline(file >> ws, line); temp.productID = line;
			getline(file >> ws, line); temp.sellerID = line; IDread = temp.sellerID;
			getline(file >> ws, line); temp.colorBought = line;
			getline(file >> ws, line); temp.sizeBought = line;
			getline(file >> ws, line); temp.quantityBought = stoi(line);
			order.cart.shoes.push_back(temp);
		}getline(file >> ws, line);
		int accessoryitems = stoi(line);
		for (int i = 0; i < accessoryitems; i++) {
			Accessories temp;
			getline(file >> ws, line); temp.productID = line;
			getline(file >> ws, line); temp.sellerID = line; IDread = temp.sellerID;
			getline(file >> ws, line); temp.colorBought = line;
			getline(file, line); temp.sizeBought = line;
			getline(file >> ws >> ws, line); temp.quantityBought = stoi(line);
			order.cart.accessories.push_back(temp);
		}
		if (IDread == sellerID) {
			if (order.status != "Delivered" && order.status != "Cancelled")
				x.push_back(order);
		}
	}
	file.close();
	return x;
}

void Seller::viewMyOrders() {
	File file("Orders.txt");
	vector<Order>orders = file.readSellerOrders(accountID);
	for (int i = 0; i < orders.size(); i++) {
		cout << orders[i];
	}
}

void Customer::placeOrder() {
	Order order(accountID, firstName + " " + lastName);
	int choice;
l:
	while (true) {
		cout << endl << "Press 1 to add an item to cart\nPress 2 to view your cart\nPress 3 to remove an item\nPress 4 to check out\nEnter here: ";
		cin >> choice;
		try {
			if (cin.fail()) {
				cin.clear(); cin.ignore(1000, '\n');
				throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
			}
			break;
		}
		catch (const exception& e) {
			cerr << e.what() << std::endl;
		}
	}
	if (choice > 5) {
		cout << "Invalid choice!";
		cout << "Press any key to continue..."; _getch();
		goto l;
	}
	do {
		if (choice == 1) {
			order.addItem();
		}
		else if (choice == 2) { cout << order.cart; }

		else if (choice == 3) { order.removeItem(); }
		cout << "Press any key to continue..."; _getch();
	in:
		while (true) {
			cout << endl << "Press 1 to add another item\nPress 2 to view your cart\nPress 3 to remove an item\nPress 4 to check out\nEnter here: ";
			cin >> choice;
			try {
				if (cin.fail()) {
					cin.clear(); cin.ignore(1000, '\n');
					throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
				}
				break;
			}
			catch (const exception& e) {
				cerr << e.what() << std::endl;

			}
		}
		if (choice > 5) {
			cout << "Invalid choice!";
			cout << "Press any key to continue..."; _getch();
			goto in;

		}
		system("cls");
	} while (choice != 4);
	system("cls");
	cout << "--------------------------------------CHECK OUT WINDOW---------------------------------------------" << endl;
	order.checkOut();
}

void Seller::updateOrderStatus() {
	string orderID, customerID;
	cout << "Enter orderID: ";
	cin >> orderID;
	cout << "Enter customer ID associated with this order: ";
	cin >> customerID;
	File file("Orders.txt");
	cout << file.updateOrderStatus(customerID, orderID);
}


int mainMenu() {
	cout << "+--------------------------------------------+" << endl;
	cout << "|                                            |" << endl;
	cout << "|             ONLINE SHOPPING                |" << endl;
	cout << "|                PROJECT BY                  |" << endl;
	cout << "|                                            |" << endl;
	cout << "|              Abdullah Edhi                 |" << endl;
	cout << "|                                            |" << endl;
	cout << "|                                            |" << endl;
	cout << "|                                            |" << endl;
	cout << "+--------------------------------------------+" << endl;
	int role;
	cout << endl << "\t\t\tHow should we recognise you?" << endl;
	cout << "\t\t\t|Press 1 for Customer.|" << endl;
	cout << "\t\t\t|Press 2 for Seller   |." << endl;
	while (true) {
		cout << "\t\t\tEnter here: ";
		cin >> role;
		try {
			if (cin.fail()) {
				cin.clear(); cin.ignore(1000, '\n');
				throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
			}
			break;
		}
		catch (const exception& e) {
			cerr << e.what() << std::endl;
		}
	}
	if (role > 2) {
		cout << "Invalid choice!!" << endl;
		cout << "Press any key to continue..."; _getch();
		mainMenu();
	}
	return role;
}

int customerMenu(string name) {
	int task;
	system("cls");
	cout << endl << "\n\t\tDear Customer, " << name << endl;
	cout << "\t\t\t|Press 1 for placing an order.    |" << endl;
	cout << "\t\t\t|Press 2 for viewing your profile.|" << endl;
	cout << "\t\t\t|Press 3 for tracking an order.   |" << endl;
	cout << "\t\t\t|Press 4 to logout.               |" << endl;
	cout << "\t\t\t|Press 5 to exit.                 |" << endl;
	while (true) {
		cout << "\t\t\tEnter here: ";
		cin >> task;
		try {
			if (cin.fail()) {
				cin.clear(); cin.ignore(1000, '\n');
				throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
			}
			break;
		}
		catch (const exception& e) {
			cerr << e.what() << std::endl;
			cin.clear(); cin.ignore(1000, '\n');
		}
	}
	if (task > 5) {
		cout << "Invalid choice!!" << endl;
		cout << "Press any key to continue..."; _getch();
		customerMenu(name);
	}
	return task;
}

int sellerMenu(string name) {
	int task;
	system("cls");
	cout << endl << "\t\tHello seller, " << name << endl;
	cout << "\t\t\t|Press 1 for adding an item to sell.                |" << endl;
	cout << "\t\t\t|Press 2 for updating status of an order.           |" << endl;
	cout << "\t\t\t|Press 3 to view your pending orders.               |" << endl;
	cout << "\t\t\t|Press 4 to logout.                                 |" << endl;
	cout << "\t\t\t|Press 5 to exit.                                   |" << endl;
	while (true) {
		cout << "\t\t\tEnter here: ";
		cin >> task;
		try {
			if (cin.fail()) {
				cin.clear(); cin.ignore(1000, '\n');
				throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
			}
			break;
		}
		catch (const exception& e) {
			cerr << e.what() << std::endl;
		}
	}
	if (task > 5) {
		cout << "Invalid choice!!" << endl;
		cout << "Press any key to continue..."; _getch();
		sellerMenu(name);
	}
	return task;
}

int main() {
	system("Color F4");
again:
	int role = mainMenu();
	Customer customer;
	Seller seller;

	if (role == 1) {
		int flag, count = 0;;
		while (true) {
			cout << setw(75) << "\n\n\tDo you have an existing account(press 0 for no,otherwise press any number): ";
			cin >> flag;
			try {
				if (cin.fail()) {
					cin.clear(); cin.ignore(1000, '\n');
					throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
				}
				break;
			}
			catch (const exception& e) {
				cerr << e.what() << std::endl;
			}
		}
		if (flag != 0) {
			bool check = Login<Customer>(customer);
			while (!check && count <= 3) {
				count++;
				cout << endl;
				check = Login<Customer>(customer);
			}
			if (!check)
				cout << endl << "Too many unsuccessful attempts,please try making a new account" << endl;
		}
		else {
			SignUp<Customer>(customer);
		}
		cout << "\n\n\nPress any key to continue..."; _getch();
		int task = customerMenu(customer.getName());
		while (task != 5) {
			switch (task) {
			case 1:
				system("cls");
				cout << "--------------------------------------PLACE ORDER---------------------------------------------" << endl;
				customer.placeOrder();
				break;
			case 2:
				system("cls");
				cout << "--------------------------------------CUSTOMER PROFILE WINDOW---------------------------------------------" << endl;
				cout << customer;
				break;
			case 3:
				system("cls");
				cout << "--------------------------------------ORDER TRACKING WINDOW---------------------------------------------" << endl;
				customer.trackOrder();
				break;
			case 4:
				system("cls");
				Customer s;
				customer = s;
				goto again;
				break;
			}
			cout << "__________________________________________________________________________________________" << endl;
			cout << "Press any key to continue..."; _getch();
			task = customerMenu(customer.getName());
		}
	}
	else if (role == 2) {
		int flag, count = 0;;
		while (true) {
			cout << setw(75) << "\n\nDo you have an existing account(press 0 for no,otherwise press any number): ";
			cin >> flag;
			try {
				if (cin.fail()) {
					cin.clear(); cin.ignore(1000, '\n');
					throw invalid_argument("\n\t\t\tInvalid input. Please enter an integer.");
				}
				break;
			}
			catch (const exception& e) {
				cerr << e.what() << std::endl;

			}
		}
		if (flag != 0) {
			bool check = Login<Seller>(seller);
			while (!check && count <= 3) {
				count++;
				cout << endl;
				check = Login<Seller>(seller);
			}
			if (!check)
				cout << endl << "Too many unsuccessful attempts,please try making a new account" << endl;
		}
		else {
			SignUp<Seller>(seller);
		}
		cout << "\n\n\nPress any key to continue..."; _getch();
		int task = sellerMenu(seller.getName());
		while (task != 5) {
			switch (task) {
			case 1:
				system("cls");
				cout << "--------------------------------------ADD TO SALE WINDOW---------------------------------------------" << endl;
				seller.addItemForSale();
				break;
			case 2:
				system("cls");
				cout << "--------------------------------------UPDATE STATUS WINDOW---------------------------------------------" << endl;
				seller.updateOrderStatus();
				break;
			case 3:
				system("cls");
				cout << "--------------------------------------PENDING ORDERS WINDOW---------------------------------------------" << endl;
				seller.viewMyOrders();
				break;
			case 4:
				system("cls");
				Seller s;
				seller = s;
				goto again;
				break;
			}
			cout << "_______________________________________________________________________________________________" << endl;
			cout << "Press any key to continue..."; _getch();
			task = sellerMenu(seller.getName());
		}
	}
	else {
		cout << "Invalid choice!";
		exit(0);
	}
}
