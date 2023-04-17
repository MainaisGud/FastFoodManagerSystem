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
#include"Customer_Class.cpp"

using namespace std;
//*****************************************************************************************************************************************************************************************************
int customerNo = 0, cookNo = 0, waiterNo = 0, daily_sales = 0;  // GLOBAL VARIABLES TO STORE DEFINITE NUMBER OF CUSTOMERS, COOKS & WAITERS
int pricelist[15] = {550, 150, 900, 750, 1150, 650, 500, 800, 600, 400, 350, 300, 110, 60, 50};
string menulist[15] = {"BURGER", "FRIES", "REGULAR PIZZA", "PASTA", "STEAK", "SOUP", "FISH N CHIPS", "CHINESE BOWL", "CHIKEN WINGS", "TIRAMISU", "PUDDING", "CAKE SLICE", 			
                       "FRESH LIME", "SOFT DRINK", "MINERAL WATER"};
int preptime[15] = {10, 8, 15, 10, 15, 10, 10, 10, 10, 5, 5, 5, 5, 2, 2};
Customer ** CustomerList;                       // GLOBAL POINTER FOR AN ARRAY TO STORE CUSTOMERS ORDERWISE 
int currentOrder = 0, customerCount = 0;
pthread_t *cook_tid;                           // Array to store thread ids' of the threads
pthread_mutex_t *cookLock;
//*****************************************************************************************************************************************************************************************************
void *Cook(void *argv)    // Starting routine for cook thread
{
	uint64_t a = (uint64_t)argv;
    	
	while(1)
	{
		if(customerCount-1!=currentOrder)   // INCASE NO ORDER IS YET PLACED
			continue;
			
		int item = -1, timer = 0, ind;
		int * ptr = CustomerList[currentOrder]->getCustomerOrder()->getOrderDescription();   // TEMPORARY POINTER TO POINT THE CUSTOMER ORDER DESCRIPTION ARRAY TO ACCESS IT GIVEN ITS PRIVATE
		pthread_mutex_lock(&cookLock[a]);     // MUTEX LOCK FOR CRITICAL SECTION
		
		if(customerCount-1!=currentOrder)     // INCASE NO ORDER IS YET PLACED THEN UNLOCK 
		{
			pthread_mutex_unlock(&cookLock[a]);
			continue;
		}
		
		for(int i=0; i<CustomerList[currentOrder]->getCustomerOrder()->getItemNo(); i++)    
		{
			if(ptr[i]!=-1)            // condition check to see if the item in order is not already assigned to any cook
			{
				item = ptr[i];    // update the item for cook to start preparing
				ind = i;
				timer = preptime[ptr[i]-1];     // set preparation time for selected item
				ptr[i] = -1;      // reset the order description 
				break;                     
				
			}	
		}
		
		if(item == -1)   // IF ALL ITEMS OF ONE ORDER ARE ALREADY ASSIGNED TO A COOK THEN MOVE TO NEXT CUSTOMER AND UNLOCK
		{
			currentOrder++;
			pthread_mutex_unlock(&cookLock[a]);
			continue;
		}
		
		ind = item;
		cout << "[COOK HAS STARTED PREPARING " << menulist[item - 1] <<"]"<< endl;
		
		sleep(timer);      // delay for item's prep time
		
		cout << "{COOK HAS FINISHED PREPARING " << menulist[item-1] <<"}"<< endl;
		
		pthread_mutex_unlock(&cookLock[a]);
	}
	pthread_exit(NULL);
}
//*****************************************************************************************************************************************************************************************************
int main()
{
	cout << "ENTER THE NUMBER OF CUSTOMERS : ";    // PROMPTS FOR USER TO INSERT INPUT
	cin  >> customerNo;
	cout << "ENTER THE NUMBER OF COOKS : ";
	cin  >> cookNo;
	cook_tid = new pthread_t [cookNo];
	cookLock =  new pthread_mutex_t [cookNo];
	for(int i =0; i < cookNo; i++)
	{
		cookLock[i] = PTHREAD_MUTEX_INITIALIZER;
	}
	cout << "ENTER THE NUMBER OF WAITERS : ";
	cin  >> waiterNo;
	
	CustomerList = new Customer* [customerNo];      // DECLARING CUSTOMER ARRAY WITH SIZE EQUAL TO NO OF CUSTOMERS
	int currentCustomer = 1;                       // VARIABLE TO KEEP TRACK OF THE CURRENT CUSTOMER NUMBER IN CURRENT ITERATION
	
	int fd = mkfifo("manager_customer",0666);      // SYSTEM CALL FOR MAKING PIPE
	
	fd = open("manager_customer", O_WRONLY);       // OPENING PIPE FOR WRITE ONLY
	if(fd==-1)
	{
		cout << "ERROR IN PIPE!" << endl;      // ERROR PROMPT
	}
	else
	{
		write(fd,&customerNo,sizeof(int));     // WRITE THE NUMBER OF CUSTOMERS THROUGH PIPE
	}
	close (fd);                                   // CLOSE PIPE
	
	for(int i=0; i<cookNo; i++)
	{
		uint64_t arg = i;
		pthread_create(&cook_tid[i],NULL,Cook,(void *)arg);
	}
	
	for(int k=0; k<customerNo; k++)                // LOOP THAT ITERATES FOR THE TOTAL NUMBER OF CUSTOMERS TO TAKE ORDERS ONE BY ONE
	{
		fd = open("manager_customer",O_RDONLY);   //OPEN PIPE FOR READ ONLY
		if(fd==-1)
		{
			cout << "ERROR IN PIPE!" << endl;   // ERROR PROMPT
		}
		else
		{
			char name[30];          // CHAR ARRAY TO STORE CUSTOMERS NAME
			string n;               // STRING TO STORE NAME IN CUSTOMER ADT 
			int ID, num, bill = 0;
			
			cout << "*****************************************************************************" << endl;
			cout << "*                         MANAGER TAKING ORDER NO. "<<k+1<<"                        *" << endl;
			cout << "*****************************************************************************" << endl;	
			cout << "HEY THERE! I'M THE MANAGER HERE AT \'FINE DINERS\' AND I'LL BE TAKING YOUR ORDER." << endl;
			cout << "PLEASE ENTER YOUR NAME AND 4-DIGIT CUSTOMER-ID TO PROCEED." << endl;
			
			read(fd,&name,sizeof(name));   // READ CUSTOMER NAME FROM CUSTOMER
			n = name;	
			cout << "NAME RECEIVED!" << endl;    // PROMPT FOR USER
			read(fd,&ID,sizeof(ID));             // READ CUSTOMER ID FROM CUSTOMER
			cout << "CUSTOMER-ID RECEIVED!" << endl;    // PROMP FOR USER
			
			CustomerList[customerCount] = new Customer(n, ID);     // DECLARE CUSTOMER AND INITIALIZE THROUGH PARAMETERIZED CONSTRUCTOR
			
			int fd1 = mkfifo("customer_manager",0666);     // named pipe creation for sending the menu.txt to the customer
			fd1 = open("customer_manager",O_WRONLY);         
			write(fd1,"menu.txt",9);                       // write the menu file name
			close(fd1);                                    // system call to close the pipe
			
			cout << "MENU SENT TO THE CUSTOMER" <<endl;        // PROMPT FOR USER
			
			cout << "ENTER THE NUMBER OF ITEMS YOU WANT TO ORDER." << endl;
		 	read(fd,&num,sizeof(num));           // READ THE NUMBER OF ITEMS IN ORDER FROM CUSTOMER
		 	cout << "NO. OF ITEMS RECEIVED." << endl;
		 	CustomerList[customerCount]->getCustomerOrder()->setItemNo(num);       // UPDATE THE READ VALUE INTO CUSTOMER OBJECT FEILD
		 	
		 	cout << "ENTER THE NUMBER CORRESPONDING TO YOUR DESIRED ITEM TO PLACE THE ORDER." << endl;
		 	
		 	int *temp = new int [num];   // TEMPORARY ARRAY TO STORE ORDER CHOICES OF CUSTOMER
				
			for(int i=0; i<num; i++)
			{
				read(fd,&temp[i],sizeof(int));
			}
			
			CustomerList[customerCount]->getCustomerOrder()->setOrderDesc(temp);     // DEEP COPY THE INPUT CHOICES THROUGH TEMP ARRAY TO THE CUSTOMER OBJECT'S FEILDS
			delete temp;     // DELETE TEMPORARY ARRAY AND UNDANGLE THE POINTER
			temp = NULL;     // UNDANGLE THE POINTER
			
			cout << endl << "YOUR ORDER HAS BEEN PLACED!" << endl; 
			
			int * temp1 = CustomerList[customerCount]->getCustomerOrder()->getOrderDescription();// TEMPORARY POINTER TO POINT THE CUSTOMER ORDER DESCRIPTION ARRAYTOACCESSITGIVENITSPRIVATE
			
			cout << "======================================="<<endl;
			cout << "CUSTOMER NAME : "<<n<< endl;
			cout << "CUSTOMER ID : "<<ID<< endl;
			cout << "======================================="<<endl;
			cout << "CUSTOMER'S ORDER :"<< endl;
			
			for(int i=0; i < CustomerList[customerCount]->getCustomerOrder()->getItemNo(); i++)       // LOOP TO DISPLAY THE THE CONFIRMED ORDER OF THE CUSTOMER
			{
				for(int j=0; j<15; j++)            
				{
					if(temp1[i]-1 == j)
						cout << "=> " << menulist[j] << endl;
				}
			}
			
			cout << "======================================="<<endl;
			
			for(int i=0; i < CustomerList[customerCount]->getCustomerOrder()->getItemNo(); i++)      // LOOP TO GENERATE THE CUSTOMER'S BILL
			{
				for(int j=0; j<15; j++)
				{
					if(temp1[i]-1 == j)
						bill += pricelist[j];
				}
			}
			
			daily_sales += bill;                       // UPDATE DAILY SALES 
			
			cout << "YOUR BILL : Rs. " << bill << endl;
			cout << "======================================="<<endl<<endl;
			
			currentCustomer++;
			customerCount++;
			
		}
		close (fd);                                                         // CLOSE PIPE
	}
	
	sleep(25);    // to ensure that all orders are prepared before termination
	cout << endl<< "======================================="<<endl;
	cout << "DAILY SALES : Rs. "<< daily_sales << endl;
	cout << "======================================="<<endl;
	return 0;
}
//*****************************************************************************************************************************************************************************************************
