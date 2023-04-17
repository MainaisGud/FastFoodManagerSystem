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
//*****************************************************************************************************************************************************************************************************
void DisplayMenu(string str)                             // FUNCTION TO DISPLAY MENU FROM FILE THROUGH FILE READING
{
	string input_str;                      // STRING TO TAKE LINE WISE INPUT
	ifstream read;                         // FILE READING OBJECT
	read.open(str);                 // OPENING FILE
	while(!read.eof())                     // LOOP TO READ FILE TILL ITS END
	{	
		getline(read, input_str);      // STORING EACH LINE IN STRING
		cout << input_str << endl;     // DISPLAYING EACH LINE OF MENU CARD
	}
	read.close();                          // CLOSING FILE
}
//*****************************************************************************************************************************************************************************************************
int main()
{
	int customerNo = 0;       // VARIABLE TO STORE DEFINITE CUSTOMER NUMBER
	
	int fd = open("manager_customer", O_RDONLY);    // SYSTEM CALL TO OPEN PIPE FOR READ ONLY
	if(fd==-1)
	{
		cout << "ERROR IN PIPE!" << endl;       // ERROR PROMPT
	}
	else
	{
		read(fd,&customerNo,sizeof(int));       // READ NO OF CUSTOMERS FROM THE MANAGER TO MANAGE ITERATION
	}
	close (fd);              // CLOSE PIPE FOR READING
	
	for(int k=0; k<customerNo; k++)                // LOOP TO ITERATE ACCORDING TO THE TOTAL NO OF CUSTOMERS TO TAKE ORDER
	{
		int fd = open("manager_customer",O_WRONLY);     // SYSTEM CALL TO OPEN PIPE FOR WRITE ONLY
		if(fd==-1)
		{
			cout << "ERROR IN PIPE!" << endl;   // ERROR PROMPT
		}
		else
		{
			char name[30];      // BUFFER CHAR ARRAY TO STORE CUSTOMER'S NAME
			int ID, num, choice;    
			
			cout << "*****************************************************************************" << endl;
			cout << "*                            CUSTOMER  NO. "<<k+1<<"                                *" << endl;
			cout << "*****************************************************************************" << endl;
			
			cout << "NAME : ";
			cin.getline(name, 30);     // TAKING INPUT FOR CUSTOMER'S NAME
			write(fd,&name,strlen(name)+1);   // WRITING THE NAME TO THE MANAGER
			
			do
			{
				cout << "CUSTOMER-ID (4-DIGIT) : ";
				cin  >> ID;        // TAKING 4-DIGIT NAMEAS INPUT
				if(ID<1000 || ID>9999)     // INPUT VALIDATION
				{
					cout << "ENTERED ID IS OUT OF BOUND !" << endl;    // PROMPT FOR USER
				}
			}
			while(ID<1000 || ID>9999);
			write(fd,&ID,sizeof(ID));    // WRITE THE CUSTOMER ID TO MANAGER
			
			cout << endl;
			int fd1 = mkfifo("customer_manager",0666);     // named pipe for reading menu from manager
			fd1 = open("customer_manager",O_RDONLY);
			char file[10];                                 // buffer array to read from the manager
			read(fd1,&file,sizeof(file));                  // read from manger
			string fileName = file;                        // set string with char array stored string 
			DisplayMenu(fileName);                         // call to display menu by passing read meanu file name from manager
			close(fd1);                                    // close pipe for reading
			
			cout << "NO. OF ITEMS : ";
			cin  >> num;
			write(fd,&num,sizeof(num));   // WRITE THE NUMBER OF ITEMS IN ORDER TO THE MANAGER
			
			for(int i=0; i<num; i++)      // LOOP TO NOTE CUSTOMER ORDER CHOICE ACCORDING TO THE NUMBER OF ITEMS IN THE ORDER
			{
				do
				{
					cout << "CHOICE : ";
					cin  >> choice;      
					if (choice<1 || choice>15)    // INPUT VALIDATION
					{
						cout << "CHOICE OUT OF BOUND !" << endl;
					}
				}
				while(choice<1 || choice>15);
				write(fd,&choice,sizeof(choice));    // WRITING EACH CHOICE ONE BY ONE TO THE MANAGER
			}
		}
		close(fd);     // CLOSE PIPE FOR WRITING
		
		cin.clear();   // CLEARING INPUT BUFFER FOR THE NEXT CUSTOMER
		cin.ignore();
	}
	
	return 0;
}
//*****************************************************************************************************************************************************************************************************
