//*****************************************************************************************************************************************************************************************************
//										NAME : AMNA SHAFIQ
//										ROLL NO. : 19i-1978
//										SECTION :  ' A '
//										SEMESTER PROJECT
//*****************************************************************************************************************************************************************************************************
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<sys/wait.h>
#include<sys/stat.h>
#include<cstring>
#include<fcntl.h>
#include<fstream>
#include"Order_Class.cpp"

using namespace std;

class Customer     //ADT FOR STORING CUSTOMER'S CREDENTIALS AND ORDER
{
	private:
		string name;
		int customer_ID;
		Order customer_order;
	
	public:
		Customer()     // DEFAULT CONSTRUCTOR
		{
			name = " ";
			customer_ID = 0;
		}
		
		Customer(string n, int ID)     // PARAMETERIZED CONSTRUCTOR
		{
			name = n;
			customer_ID = ID;
		}
		
		// SETTER FUNCTIONS
		
		void setName(string n)
		{
			name = n;
		}
		
		void setCustomerID(int ID)
		{
			customer_ID = ID;
		}
		
		// GETTER FUNCTIONS
		
		string getName()
		{
			return name;
		}
		
		int getCustomerID()
		{
			return customer_ID;
		}
		
		Order* getCustomerOrder()
		{
			Order * ptr = &customer_order;
			return ptr;
		}
};

