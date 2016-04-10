/* Telephone Billing and Payment System	 */
#include<iostream>
#include<fstream.h>
#include<math.h>
#include<conio.h>
#include<stdio.h>
#include<dos.h>
#include<graphics.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
fstream f1,f2;
class bill
{
	char username[20];
	unsigned long int phone_no;
	long cust_id;
	char emailID[40];
	char address[200];
	char status;
	float localprice,stdprice,discount,taxes;
	float localtime,stdtime,amount,netamount,outstanding;
	void calcnet()
	{
		amount=(localtime*localprice)+(stdtime*stdprice);
		netamount=(((amount*(1+taxes))*(1-discount))) +outstanding;
		outstanding=netamount;
	}
	public:
	bill()
	{
		strcpy(username,"\0");
		phone_no=0;
		strcpy(emailID,"\0");
		strcpy(address,"\0");
		status='A';
		cust_id=0;
		localtime=stdtime=amount=netamount=outstanding=0.0;
		localprice=0.50;
		 stdprice=1.00;
		 discount=0.1;
		taxes=0.15;

	}
	void input();
	void output();
	void pay();
	void changestatus()
	{
	  if(outstanding>500)
	  {
	   status='U';
	  }
	  if(outstanding<500)
	  {
	  status='A';	  }
	}
	int comparenumber(unsigned long int ph)
	{
		if(ph==phone_no)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	char returncustid()
	{
		return cust_id;
	}
	char getstatus()
	{
	return status;
	}
};
void bill::input()
	{
		cout<<"Enter the name of user"<<endl;
		gets(username);
		cout<<"Enter the phone no of user(7 digits)"<<endl;
		cin>>phone_no;
		cout<<"Enter Customer ID"<<endl;
		cin>>cust_id;
		cout<<"Enter the email ID"<<endl;
		gets(emailID);
		cout<<"Enter the address of the user"<<endl;
		gets(address);
		cout<<"Enter Local Call Duration in minutes"<<endl;
		cin>>localtime;
		cout<<"Enter Std Call Duration in minutes"<<endl;
		cin>>stdtime;
		calcnet();
	}
void bill :: output()
	{
		cout<<"Name : "<<username<<endl;
		cout<<"Phone No: "<<phone_no<<endl;
		cout<<"Customer ID: "<<cust_id<<endl;
		cout<<"Email ID: "<<emailID<<endl;
		cout<<"Address: "<<address<<endl;
		cout<<"Status : "<<status<<endl;
		cout<<"Amount (Exlusive of Taxes & Discount): "<<amount<<endl;
		cout<<"Outstanding Amount for next month "<<outstanding<<endl;
		cout<<"Amount (Inclusive of Taxes & Discount): "<<netamount<<endl;

	}
void bill::pay()
{
	float paid;
	if(status=='U')
	{
		cout<<"Your Account is deactivated. \n You have been fined Rs 100 for reactivation of your account"<<endl;
		netamount+=100;
	}
	cout<<"Your Net amount due is "<<netamount<<endl;
	a:
	cout<<"Enter the amount You wish to pay (0 to cancel)"<<endl;
	cin>>paid;
	if (paid==0)
	{
		cout<<"Transaction Cancelled"<<endl;
	}
	else if (paid>=(0.8*netamount))
	{
		outstanding=netamount-paid;
		localtime=stdtime=amount=netamount=0.0;
		cout<<paid <<" Amount Paid "<<endl;
		cout<<"Your Outsatnding amount for next Month is "<<outstanding<<endl;
	}
	else
	{
		cout<<"The Minimum amount to be paid is "<<0.8*netamount<<endl;
		cout<<"Please pay this much amount for your connection to be active"<<endl;
		goto a;
	}
	changestatus();

}

bill b1,b2;

void updatestatus()
{
	if(!f1)
	{
		cout<<"No records Present"<<endl;
	}
	else
	{
		f1.open("bill.dat",ios::in);
		f2.open("te.dat",ios::out|ios::app);
		f1.seekg(0,ios::beg);
		f1.read((char*)&b1, sizeof(b1));
		while(!f1.eof())
		{
			b1.changestatus();
			f2.write((char*)&b1, sizeof(b1));
			f1.read((char*)&b1, sizeof(b1));
			getch();
		}
		f1.close();
		f2.close();
		remove("bill.dat");
		rename("te.dat","bill.dat");
		cout<<"Status Updated"<<endl;
		getch();
	}
}
void paybill(unsigned long int ph)
{
	f1.open("bill.dat",ios::in|ios::out);
	f1.seekg(0,ios::beg);
	f1.read((char*)&b1, sizeof(b1));
	while(!f1.eof())
	{
		if(b1.comparenumber(ph)==1)
		{
			b1.pay();
			f1.seekp(f1.tellg()-sizeof(b1),ios::beg);
			f1.write((char*)&b1,sizeof(b1));
		}
		f1.read((char*)&b1, sizeof(b1));
	}
	f1.close();
	}
void addrecord()
{
	clrscr();
	cout<<"Enter information of new user"<<endl;
	b1.input();
	f1.open("bill.dat",ios::out|ios::app);
	f1.write((char*)&b1,sizeof(b1));
	f1.close();
	cout<<"***NEW RECORD ADDED***"<<endl;
	getch();
}

void deleterecord()
{

	int ch,m,flag=0;
	unsigned long int ph;
	cout<<"*****SUB MENU - DELETION*****"<<endl;
	cout<<"1.Delete Using Customer ID"<<endl;
	cout<<"2.Delete Using Phone number"<<endl;
	a1:
	cin>>ch;
	switch(ch)
	{
		case 1:
		cout<<" \n Enter customer ID you want to delete"<<endl;
		cin>>m;
		f1.open("bill.dat",ios::in)  ;
		f2.open("temp.dat",ios::out|ios::app);
		f1.seekg(0,ios::beg);
		f1.read((char*)&b1,sizeof(b1));
		if(!f1)
		{
		cout<<"No records present"<<endl;
		}
		else
		{
		while(!f1.eof())
		{
			if(b1.returncustid()!=m)
			{
				f2.write((char*)&b1,sizeof(b1));
			}
			if(b1.returncustid()==m)
			{
			   flag=1;
			}
			f1.read((char*)&b1,sizeof(b1));
		}
		}
		f1.close();
		f2.close();
		remove("bill.dat");
		rename("temp.dat","bill.dat");
		if(flag==1)
		{
			cout<<"Record Deleted"<<endl;
		}
		else
		{
			cout<<"Record not present"<<endl;
		}
		break;
		case 2:
		cout<<" \n Enter phone number you want to delete(7 digits)"<<endl;
		cin>>ph;
		f1.open("bill.dat",ios::in)  ;
		f2.open("temp.dat",ios::out|ios::app);
		f1.seekg(0,ios::beg);
		f1.read((char*)&b1,sizeof(b1));
		if(!f1)
		{
		cout<<"No records present"<<endl;
		}
		else
		{
		while(!f1.eof())
		{
			if(b1.comparenumber(ph)!=0)
			{
				f2.write((char*)&b1,sizeof(b1));
			}
			if(b1.comparenumber(ph)==0)
			{
				flag=1;
			}
			f1.read((char*)&b1,sizeof(b1));
		}
		}
		f1.close();
		f2.close();
		if(flag==1)
		{
			cout<<"Record Deleted"<<endl;
		}
		else
		{
			cout<<"Record not present"<<endl;
		}
		remove("bill.dat");
		rename("temp.dat","bill.dat");
		break;
		default:
		cout<<"Invalid Choice Entered.....Please Retry"<<endl;
		goto a1;
	}
	getch();
}

void display(unsigned long int ph=0)
	{
		if(ph!=0)
		{
			goto dis;
		}
		else
		{
			int ch,m;
			cout<<"*****SUB MENU - DISPLAY*****"<<endl;
			cout<<"1.Display All records"<<endl;
			cout<<"2.Display unactivated connections"<<endl;
			cout<<"3.Search and display using Customer ID"<<endl;
			cout<<"4.Search and display using Phone No"<<endl;
			cout<<"Enter choice"<<endl;
			cin>>ch;
			switch(ch)
			{
				case 1:
				f1.open("bill.dat",ios::in);
				f1.seekg(0,ios::beg);
				f1.read((char*)&b1, sizeof(b1));
				if(!f1)
				{
					cout<<"No records present"<<endl;
				}
				else
				{
				while(!f1.eof())
				{
					b1.output();
					cout<<endl;
					getch();
					f1.read((char*)&b1, sizeof(b1));
				}
				}
				f1.close();
				break;
				case 2:
				f1.open("bill.dat",ios::in);
				f1.seekg(0,ios::beg);
				f1.read((char*)&b1, sizeof(b1));
				if(!f1)
				{
					cout<<"No records present"<<endl;
				}
				else
				{
				while(!f1.eof())
				{
					if(toupper(b1.getstatus())=='U')
					{
						b1.output();
					}
					getch();
					f1.read((char*)&b1, sizeof(b1));
				}
				}
				f1.close();
				break;
				case 3:
				cout<<"Enter the Customer ID"<<endl;
				cin>>m;
				f1.open("bill.dat",ios::in);
				f1.seekg(0,ios::beg);
				f1.read((char*)&b1, sizeof(b1));
				if(!f1)
				{
					cout<<"No records present"<<endl;
				}
				else
				{
				while(!f1.eof())
				{
					if(b1.returncustid()==m)
					{
						b1.output();
					}
					getch();
					f1.read((char*)&b1, sizeof(b1));
				}
				}
				f1.close();
				break;
				case 4:
				cout<<"Enter the Phone No(7 digits)"<<endl;
				cin>>ph;
				dis:
				f1.open("bill.dat",ios::in);
				f1.seekg(0,ios::beg);
				f1.read((char*)&b1, sizeof(b1));
				if(!f1)
				{
					cout<<"No records present"<<endl;
				}
				else
				{
				while(!f1.eof())
				{
					if(b1.comparenumber(ph))
					{
						b1.output();
					}
					getch();
					f1.read((char*)&b1, sizeof(b1));
				}
				}
				f1.close();
				break;
				default:
				cout<<"Wrong Choice"<<endl;
			}
		}
	getch();
}
void updaterecord()
{
		unsigned long int ph;
		int flag=0;
		cout<<"The record can be changed exclusively on the basis of Phone no only"<<endl;
		cout<<"Enter the Phone No whose record has to be modified(7 digits)"<<endl;
		cin>>ph;
		f1.open("bill.dat",ios::in|ios::out);
		f1.seekg(0,ios::beg);
		f1.read((char*)&b1, sizeof(b1));
		if(!f1)
		{
			cout<<"No records present"<<endl;
		}
		else
		{
		while(!f1.eof())
		{
			if(b1.comparenumber(ph))
			{
				b1.input();
				f1.seekp(f1.tellg()-sizeof(b1),ios::beg);
				f1.write((char*)&b1,sizeof(b1));
				cout<<"Record Updated"<<endl;
				flag=1;
			}
			f1.read((char*)&b1, sizeof(b1));
		}
		if(flag==0)
		{
		 cout<<"Record Not present Please add the record first"<<endl;
		}
		}
		f1.close();
		getch();

}
void user()
{
	int flag=0;
	unsigned long int number;
	char chbill;
	cout<<"Enter your phone number(7 digits)"<<endl;
	cin>>number;
	f1.open("bill.dat",ios::in);
	f1.seekg(0,ios::beg);
	f1.read((char*)&b1,sizeof(b1));
	while(!f1.eof())
	{
		if(b1.comparenumber(number)==1)
		{
			flag=1;
			break;
		}
		f1.read((char*)&b1,sizeof(b1));
	}
	f1.close();
	if (flag==1)
	{
		cout<<" \n  \t \t Login Successful"<<endl;
		getch();
		clrscr();
		cout<<"Your Bill For this Month is as Follows"<<endl;
		display(number);
		cout<<"\n \n Do you Wish to pay the bill now ? (y/n) "<<endl;
		cin>>chbill;
		if(tolower(chbill)=='y')
		{
		  paybill(number);
		}
		else
		{
			cout<<"Thank you for using the Billing Services"<<endl;
			getch();
			exit(0);
		}
	}
	else
	{
		cout<<"Invalid phone number entered"<<endl;
	}
}
void admin(char choice='n')
{
	if (tolower(choice) =='y')
	{
	goto adminlo;
	}
	else
	{
		char pass[20];
		cout<<"Enter Password"<<endl;
		gets(pass);
		if(strcmp(pass,"admin123")==0)
		{
			adminlo:
			clrscr();
			cout<<"Choose The option"<<endl;
			cout<<"\n 1. Add Record \n 2. Delete Record \n 3. Update Record \n 4. Dispay Record \n 5. Auto Change Status of the connections"<<endl;
			int adminch;
			adminre:
			cin>>adminch;
			switch(adminch)
			{
				case 1 : addrecord();
				break;
				case 2:
				deleterecord();
				break;
				case 3:
				updaterecord();
				break;
				case 4:
				display();
				break;
				case 5:
				updatestatus();
				break;
				default :
				cout<<"Invalid Choice Entered \n Please Reneter "<<endl;
				goto adminre;
			}
		}
		else
		{
			cout<<"Invalid Password Entered  ABORTING...."<<endl;
			getch();
		exit(0);
		}
	}
}
void main()
{
	clrscr();
	int login,ans;
	char ch='n',chapp;
	do
	{
		//clrscr();
		cout<<" \t \t \t Welcome To BSNL Telephone Billing System "<<endl;
		cout<<"\n Choose the Option "<<endl;
		cout<<"\n 1. Login as Administrator "<<endl;
		cout<<" 2. Login as User "<<endl;
		cin>>login;
		switch(login)
		{
			case 1:
			do{
			admin(ch);
			cout<<" \n \n Do you wish to continue as Administrator (y/n)"<<endl;
			cin>>ch;
			}while(tolower(ch)=='y');
			break;
			case 2:
			user();
			break;
			default:
			cout<<"Invalid Option entered"<<endl;
		}
		ch='n';
		cout<<"Do you wish to continue with the application (y/n)"<<endl;
		cin>>chapp;
		}while(tolower(chapp)=='y');
}
