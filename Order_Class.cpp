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

using namespace std;

class Order       // ADT FOR STORING ORDER DETAILS
{
	private:
		int item_no;
		int *order_description;
	
	public:
		Order()     // DEFAULT CONSTRUCTOR
		{
			item_no = 0;
			order_description = NULL;
		}
		
		Order(int no, int item)      // PARAMETERIZED CONSTRUCTOR
		{
			item_no = item;
			order_description = new int [item];
			for(int i=0; i<item; i++)
			{
				order_description [i] = 0;
			}
		}
		
		~Order()        // DESTRUCTOR
		{
			delete [] order_description;
			order_description = NULL;
		}
		
		// SETTER FUNCTIONS
		
		void setItemNo(int item)  
		{
			item_no = item;
			order_description = new int [item];
			for(int i=0; i<item; i++)
			{
				order_description [i] = 0;
			}
		}
		
		void setOrderDesc(int *temp)
		{
			for(int i=0; i<this->item_no; i++)
			{
				order_description[i] = temp[i];
			}
		}
		
		// GETTER FUNCTIONS
		
		int getItemNo()
		{
			return item_no;
		}

		int * getOrderDescription()
		{
			return order_description;
		}
};
