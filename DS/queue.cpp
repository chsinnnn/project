#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <cstdlib> // atoi, system
#include <cmath>
using namespace std;
class Type{
	public:
	string OID;  
	string Arrival;  
	string Duration;  
	string TimeOut;
	int cid;
	int Delay;
	int Abort;
	int Departure;
};
class Category {
	public:
	Type typee;
};

class Type2{
	public:
		int oid;
		int arrival;
		int duration;
		int timeout;
};
class Type3{
	public:
		int oid;
		int cid;
		int delay;
		int abort;
};
class Category1{
	public:
		Type2 typee1;
};

class Queuelist{
	public:
	vector<Type2>main;
	
	int length(){
		int size = main.size();
		return size;
	}  //  length()
	
	bool isEmpty(){
		if(main.size()==0)return true;
		
		return false;
	}  // isEmpty()
	
	bool isFull(){
		if(main.size()==3) return true;
		
		return false;
	}  // isFull()
	
	void enQueue(Type2 data){
		main.push_back(data);	
	}
	
	void getFront(Type2 &front){
		front=main[0];
	}  // getFront()

	void deQueue(){
		main.erase(main.begin());
	}  // deQueue()
};

class Queue{
	
	public:
	vector<Category> main;
	int cid;
	int free = 0;
	int length() {
		int size = main.size();
		return size;
	}  // length()
	
	bool isEmpty() {
		if ( main.size() == 0 )  return true;
		
		return false; 
	}  // isEmpty()
	
	bool isFull() {  // 佇列裡面一次最多只能有三個東西 
		if ( main.size() >= 3 )  return true;
		
		return false; 
	}  // isFull()
	
	void enQueue( Category m ) {
		main.push_back(m);
	}  // enQueue()
	
	Category getFront(  ) {
		
		return main[0] ;
	}  // getFront()
	
	void deQueue() {
		main.erase( main.begin() );
	}  // deQueue()
};

int small ( vector <Queue> chef ) {  // 找最短的佇列 
	int small = chef[0].length();
	int c = 0;
	for ( int i = 1 ; i < chef.size() ; i++ ) {
		if( chef[i].length() < small ) {
			small = chef[i].length();
			c = i;
		}  // if
	}  // for
	return c;
}  // small()

void Sort() {
	string filename;
    int run1 = 0;
    fstream file;
    string input = "\0" ;
    vector<Category> main;
	Category data;
	double rstart,rend; 
	double sstart,send; 
	double wstart,wend; 
   	while ( run1 == 0 ) {
		cout << "\n" << "Input a file number (e.g., 401, 402, 403, ...): ";
    	cin >> filename;
    	input = "\0" ;
		input = input + "input";
		input = input + filename;
		input = input + ".txt";
		
		file.open( input );
		
		if( file.is_open() == true ) {  // 確認在同個目錄下是否存在這個檔案
			string line;
    		string sort = "\0" ;
			sort = sort + "sort";
			sort = sort + filename;
			sort = sort + ".txt";
			ofstream newFile;
			newFile.open( sort.c_str() );
			bool first = false;
			int num = 0;
			rstart = clock();
			while ( getline( file, line ) ) {  // 先存下所有切割完的東西 
				int tabb = 0;
				if ( first == false ) {  // 第一行標題直接寫入
					cout << "     " << line << "\n"; 
					newFile << line << "\n";
					first = true;
				}  // if
				else {
					for( int i = 0 ; i < line.size() ; i++ ) {
						if( line[i] == '\t' )  tabb++;
						if( tabb == 0 && line[i] != '\t' )  data.typee.OID = data.typee.OID + line[i];
						if( tabb == 1 && line[i] != '\t' )  data.typee.Arrival = data.typee.Arrival + line[i];
						if( tabb == 2 && line[i] != '\t' )  data.typee.Duration = data.typee.Duration + line[i];
						if( tabb == 3 && line[i] != '\t' )  data.typee.TimeOut = data.typee.TimeOut + line[i];
					}  // for
					main.push_back( data );
					data = {"\0"};  // 清空 
				}  // else
			}  // while
			rend = clock();
			sstart = clock();
			int h = main.size()/2;
			for( int h = (main.size()/2) ; h > 0 ; h = h/2 ) {
				for ( int i = h ; i < main.size() ; i++ ) {
					
					string oo = main[i].typee.OID;
					string aa = main[i].typee.Arrival;
					string dd = main[i].typee.Duration;
					string tt = main[i].typee.TimeOut;
					int a2 = atoi(aa.c_str());
					int o2 = atoi(oo.c_str());
					int j = i; 
					// 左邊arrival比右邊大或是左右兩個arrival一樣大左邊oid比較大就交換 
					while ( j >= h && (((atoi((main[j-h].typee.Arrival).c_str())) > a2) || 
							(((atoi((main[j-h].typee.Arrival).c_str())) == a2) && atoi((main[j-h].typee.OID).c_str()) > o2)) ) {
						main[j] = main[j-h];
						j = j - h;
					}  // while
					main[j].typee.OID = oo;
					main[j].typee.Arrival = aa;
					main[j].typee.Duration = dd;
					main[j].typee.TimeOut = tt;
				}  // for
				
			}  // for
			send = clock();
			
			for ( int i = 0 ; i < main.size() ; i++ ) {
				newFile << main[i].typee.OID << "\t";
				newFile << main[i].typee.Arrival << "\t";
				newFile << main[i].typee.Duration << "\t";
				newFile << main[i].typee.TimeOut << "\n";
			}  // for
			wstart = clock();
			for ( int i = 0 ; i < main.size() ; i++ ) {
				cout << "("<< i+1 << ")  " << main[i].typee.OID << "\t" << main[i].typee.Arrival 
				     << "\t"<< main[i].typee.Duration << "\t" << main[i].typee.TimeOut << "\n";
			}  // for
			wend = clock();
			cout << "reading data: " << rend-rstart << "ms\n";
			cout << "sorting data: " << send-sstart << "ms\n";
			cout << "writing data: " << wend-wstart << "ms\n";
			main.clear();
			newFile.close();
			file.close();
			run1++;
		} // if
		else {
			run1 = 1;
			cout << "\n" << "###" << input << " does not exist! ###" << "\n";	
		}  // else
	} // while
}  // Sort()

void Two(){
	int time = 0;
	string filename;
    int run1 = 0;
    fstream file;
    string sort = "\0" ;
    vector<Category> main;
    vector<Category> abort;
    vector<Category> timeout;
	Category t;
	Category tempp;
	Category data;
	vector <Queue> chef ;
	
	for(int i = 0 ; i < 1; i++){  // 產生多(2)個佇列 
		Queue temp;
		//temp.cid = i+1;
		temp.free = 0;
		chef.push_back(temp);
	}  // for
    

	cout << "\n" << "Input a file number (e.g., 401, 402, 403, ...): ";
   	cin >> filename;
   	sort = "\0" ;
	sort = sort + "sort";
	sort = sort + filename;
	sort = sort + ".txt";
	file.open( sort );
	if( file.is_open() == true ) {  // 確認在同個目錄下是否存在這個檔案
		string line;

		bool first = false;
			
			
		while ( getline( file, line ) ) {  // 先存下所有切割完的東西 
			int tabb = 0;
			if ( first == false ) {
				first = true;
			}  // if
			else {
				for( int i = 0 ; i < line.size() ; i++ ) {
					if( line[i] == '\t' )  tabb++;
					if( tabb == 0 && line[i] != '\t' )  data.typee.OID = data.typee.OID + line[i];
					if( tabb == 1 && line[i] != '\t' )  data.typee.Arrival = data.typee.Arrival + line[i];
					if( tabb == 2 && line[i] != '\t' )  data.typee.Duration = data.typee.Duration + line[i];
					if( tabb == 3 && line[i] != '\t' )  data.typee.TimeOut = data.typee.TimeOut + line[i];
				}  // for
				main.push_back( data );
				data = {"\0"};  // 清空 	
			}  // else

		}  // while
			
		for ( int i = 0 ; i < main.size() ; i++ ) {  // 把所有佇列中所有訂單處理完
			
			for ( int n = 0 ; n < chef.size() ; n++ ) {	
				// 確認目前手上訂單可不可以完成 (可完成)
				while ( chef[n].free <= atoi(main[i].typee.Arrival.c_str()) && !chef[n].isEmpty() ) {   
					tempp = chef[n].getFront();							
					// 處理佇列中第一筆訂單 
					int lastnow = chef[n].free;
					if( chef[n].free < atoi(tempp.typee.TimeOut.c_str()) ) {  // 訂單會繼續做
						
						//  佇列第一筆訂單完成時間
						chef[n].free =  chef[n].free + atoi(tempp.typee.Duration.c_str()); 
								
						if ( chef[n].free > atoi(tempp.typee.TimeOut.c_str()) ) {  // 但訂單逾時 
							t.typee.OID = tempp.typee.OID;
							t.typee.cid = chef[n].cid;
								
							// 如果上一筆訂單完成時間比佇列第一筆訂單抵達時間小代表沒有delay
							if ( lastnow <= atoi(tempp.typee.Arrival.c_str()) )   
								t.typee.Delay = 0;
							else  
								t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str());
											
							t.typee.Departure = chef[n].free;
							timeout.push_back(t);
							t = {"\0"};  // 清空	
						}  // if
							
		  			}  // if
					else {  // 取消訂單
						t.typee.OID = tempp.typee.OID;
						//t.typee.cid = chef[n].cid;
						t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str()) ;
						t.typee.Abort = lastnow;
						abort.push_back(t);
						t = {"\0"};  // 清空
					}  // else
					
					chef[n].deQueue();  // 處理好了移除佇列中第一個訂單
				}  // while


			}  // for
			
			
			bool deal = false; 
			if ( chef[small(chef)].length() == 0  ) {
								
				for( int j = 0 ; j < chef.size() ; j++ ) {  // 看看有沒有廚師手上沒是並且是空柱列 
					if( chef[j].length() == 0 && chef[j].free <= atoi(main[i].typee.Arrival.c_str()) ) {
						chef[j].free = atoi(main[i].typee.Arrival.c_str())+atoi(main[i].typee.Duration.c_str());
						deal = true;
						break;		
					}  // if
				}  // for
			}  // if

			
			// 佇列不是滿的而且現在的訂單還沒被處理過就可以放入最短的佇列
			if ( chef[small(chef)].length() < 3 ) { 

				if ( !deal )
					chef[small(chef)].enQueue(main[i]);
					
			}  // else if
			else if ( chef[small(chef)].length() == 3 ) { // 佇列全滿 
			
				t.typee.OID = main[i].typee.OID;
				//t.typee.cid = 0;
				t.typee.Delay = 0 ;
				t.typee.Abort = atoi(main[i].typee.Arrival.c_str());
				abort.push_back(t);
				t = {"\0"};  // 清空
				
			}  // else if
				
		}  // for
			
			
		for ( int i = 0 ; i < chef.size() ; i++ ) {  // 把所有佇列中所有訂單處理完
			while ( chef[i].isEmpty() != true ) {   
				tempp = chef[i].getFront();
				int lastnow = chef[i].free;
					
				if( lastnow < atoi(tempp.typee.TimeOut.c_str()) ) {  // 訂單會繼續做
					chef[i].free = chef[i].free + atoi(tempp.typee.Duration.c_str());
					if ( chef[i].free > atoi(tempp.typee.TimeOut.c_str()) ) {  // 但訂單逾時 
						t.typee.OID = tempp.typee.OID;
						//t.typee.cid = chef[i].cid;
						// 如果上一筆訂單完成時間比佇列第一筆訂單抵達時間小代表沒有delay
						if ( lastnow <= atoi(tempp.typee.Arrival.c_str()) )   
							t.typee.Delay = 0;
						else  
							t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str());
										
						t.typee.Departure = chef[i].free;
						timeout.push_back(t);
						t = {"\0"};  // 清空	
					}  // if
	 
				}  // if 
				else {  // 取消訂單
					t.typee.OID = tempp.typee.OID;
					//t.typee.cid = chef[i].cid;
					t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str()) ;
					t.typee.Abort = lastnow;
					abort.push_back(t);
					t = {"\0"};  // 清空
				}  // else
				chef[i].deQueue();  // 完成後移除
					
			}  // while	
		}  // for

    	string one = "\0" ;
		one = one + "one";
		one = one + filename;
		one = one + ".txt";
		ofstream newFile;
		newFile.open( one.c_str() );
		int late = 0;

		for ( int i = abort.size()-1 ; i > 0 ; i-- ) {
			for ( int j = 0 ; j <= i-1 ; j++ ) {
				if ( abort[j].typee.Abort > abort[j+1].typee.Abort ) {
					t = abort[j];
					abort[j] = abort[j+1];
					abort[j+1] = t;
					t = {"\0"};
				}  // if
			}  // for
		}  // for

		if ( abort.size() != 0 ) {
			cout <<  "	[Abort List]\n	OID	CID	Delay	Abort\n";
			newFile << "	[Abort List]\n" << "	OID	CID	Delay	Abort\n";
			for ( int i = 0 ; i < abort.size() ; i++ ) {
				late = late + abort[i].typee.Delay;
				cout << "[" << i+1 << "]\t" <<abort[i].typee.OID  << "\t" << abort[i].typee.Delay << "\t" << abort[i].typee.Abort << "\n";
				newFile << "[" << i+1 << "]\t" <<abort[i].typee.OID << "\t" << abort[i].typee.Delay << "\t" << abort[i].typee.Abort << "\n";
			}  // for	
		}  // if
			
		if ( timeout.size() != 0 ) {
			cout << "	[Timeout List]\n	OID	CID	Delay	Departure\n";
			newFile << "	[Timeout List]\n" << "	OID	CID	Delay	Departure\n";
			for ( int i = 0 ; i < timeout.size() ; i++ ) {
				late = late + timeout[i].typee.Delay;
				cout << "[" << i+1 << "]\t" << timeout[i].typee.OID <<  "\t" << timeout[i].typee.Delay << "\t" << timeout[i].typee.Departure << "\n";
				newFile << "[" << i+1 << "]\t" << timeout[i].typee.OID << "\t" << timeout[i].typee.Delay << "\t" << timeout[i].typee.Departure << "\n";
			}  // for	
		}  // if
		
		cout << "[Total Delay]\n" << late << " min.\n";
		newFile << "[Total Delay]\n" << late << " min.\n";
		float Failure = round(float((abort.size()+timeout.size()))/main.size()*100.0*100)/100;
		cout << "[Failure Percentage]\n";
		newFile << "[Failure Percentage]\n" << Failure << "%\n";
		cout<< Failure << "%\n" ;
		main.clear();
		newFile.close();
		file.close();
	} // if
	else  cout << "\n" << "###" << sort << " does not exist! ###" << "\n";
}  // Two()

void Three() {
	int time = 0;
	string filename;
    int run1 = 0;
    fstream file;
    string sort = "\0" ;
    vector<Category> main;
    vector<Category> abort;
    vector<Category> timeout;
	Category t;
	Category tempp;
	Category data;
	vector <Queue> chef ;
	
	for(int i = 0 ; i < 2; i++){  // 產生多(2)個佇列 
		Queue temp;
		temp.cid = i+1;
		temp.free = 0;
		chef.push_back(temp);
	}  // for
    

	cout << "\n" << "Input a file number (e.g., 401, 402, 403, ...): ";
   	cin >> filename;
   	sort = "\0" ;
	sort = sort + "sort";
	sort = sort + filename;
	sort = sort + ".txt";
	file.open( sort );
	if( file.is_open() == true ) {  // 確認在同個目錄下是否存在這個檔案
		string line;

		bool first = false;
			
			
		while ( getline( file, line ) ) {  // 先存下所有切割完的東西 
			int tabb = 0;
			if ( first == false ) {
				first = true;
			}  // if
			else {
				for( int i = 0 ; i < line.size() ; i++ ) {
					if( line[i] == '\t' )  tabb++;
					if( tabb == 0 && line[i] != '\t' )  data.typee.OID = data.typee.OID + line[i];
					if( tabb == 1 && line[i] != '\t' )  data.typee.Arrival = data.typee.Arrival + line[i];
					if( tabb == 2 && line[i] != '\t' )  data.typee.Duration = data.typee.Duration + line[i];
					if( tabb == 3 && line[i] != '\t' )  data.typee.TimeOut = data.typee.TimeOut + line[i];
				}  // for
				main.push_back( data );
				data = {"\0"};  // 清空 	
			}  // else

		}  // while
			
		for ( int i = 0 ; i < main.size() ; i++ ) {  // 把所有佇列中所有訂單處理完
			
			for ( int n = 0 ; n < chef.size() ; n++ ) {	
				// 確認目前手上訂單可不可以完成 (可完成)
				while ( chef[n].free <= atoi(main[i].typee.Arrival.c_str()) && !chef[n].isEmpty() ) {   
					tempp = chef[n].getFront();							
					// 處理佇列中第一筆訂單 
					int lastnow = chef[n].free;
					if( chef[n].free < atoi(tempp.typee.TimeOut.c_str()) ) {  // 訂單會繼續做
						
						//  佇列第一筆訂單完成時間
						chef[n].free =  chef[n].free + atoi(tempp.typee.Duration.c_str()); 
								
						if ( chef[n].free > atoi(tempp.typee.TimeOut.c_str()) ) {  // 但訂單逾時 
							t.typee.OID = tempp.typee.OID;
							t.typee.cid = chef[n].cid;
								
							// 如果上一筆訂單完成時間比佇列第一筆訂單抵達時間小代表沒有delay
							if ( lastnow <= atoi(tempp.typee.Arrival.c_str()) )   
								t.typee.Delay = 0;
							else  
								t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str());
											
							t.typee.Departure = chef[n].free;
							timeout.push_back(t);
							t = {"\0"};  // 清空	
						}  // if
							
		  			}  // if
					else {  // 取消訂單
						t.typee.OID = tempp.typee.OID;
						t.typee.cid = chef[n].cid;
						t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str()) ;
						t.typee.Abort = lastnow;
						abort.push_back(t);
						t = {"\0"};  // 清空
					}  // else
					
					chef[n].deQueue();  // 處理好了移除佇列中第一個訂單
				}  // while


			}  // for
			
			
			bool deal = false; 
			if ( chef[small(chef)].length() == 0  ) {
								
				for( int j = 0 ; j < chef.size() ; j++ ) {  // 看看有沒有廚師手上沒是並且是空柱列 
					if( chef[j].length() == 0 && chef[j].free <= atoi(main[i].typee.Arrival.c_str()) ) {
						chef[j].free = atoi(main[i].typee.Arrival.c_str())+atoi(main[i].typee.Duration.c_str());
						deal = true;
						break;		
					}  // if
				}  // for
			}  // if

			
			// 佇列不是滿的而且現在的訂單還沒被處理過就可以放入最短的佇列
			if ( chef[small(chef)].length() < 3 ) { 

				if ( !deal )
					chef[small(chef)].enQueue(main[i]);
					
			}  // else if
			else if ( chef[small(chef)].length() == 3 ) { // 佇列全滿 
			
				t.typee.OID = main[i].typee.OID;
				t.typee.cid = 0;
				t.typee.Delay = 0 ;
				t.typee.Abort = atoi(main[i].typee.Arrival.c_str());
				abort.push_back(t);
				t = {"\0"};  // 清空
				
			}  // else if
				
		}  // for
			
			
		for ( int i = 0 ; i < chef.size() ; i++ ) {  // 把所有佇列中所有訂單處理完
			while ( chef[i].isEmpty() != true ) {   
				tempp = chef[i].getFront();
				int lastnow = chef[i].free;
					
				if( lastnow < atoi(tempp.typee.TimeOut.c_str()) ) {  // 訂單會繼續做
					chef[i].free = chef[i].free + atoi(tempp.typee.Duration.c_str());
					if ( chef[i].free > atoi(tempp.typee.TimeOut.c_str()) ) {  // 但訂單逾時 
						t.typee.OID = tempp.typee.OID;
						t.typee.cid = chef[i].cid;
						// 如果上一筆訂單完成時間比佇列第一筆訂單抵達時間小代表沒有delay
						if ( lastnow <= atoi(tempp.typee.Arrival.c_str()) )   
							t.typee.Delay = 0;
						else  
							t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str());
										
						t.typee.Departure = chef[i].free;
						timeout.push_back(t);
						t = {"\0"};  // 清空	
					}  // if
	 
				}  // if 
				else {  // 取消訂單
					t.typee.OID = tempp.typee.OID;
					t.typee.cid = chef[i].cid;
					t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str()) ;
					t.typee.Abort = lastnow;
					abort.push_back(t);
					t = {"\0"};  // 清空
				}  // else
				chef[i].deQueue();  // 完成後移除
					
			}  // while	
		}  // for

    	string two = "\0" ;
		two = two + "two";
		two = two + filename;
		two = two + ".txt";
		ofstream newFile;
		newFile.open( two.c_str() );
		int late = 0;

		for ( int i = abort.size()-1 ; i > 0 ; i-- ) {
			for ( int j = 0 ; j <= i-1 ; j++ ) {
				if ( abort[j].typee.Abort > abort[j+1].typee.Abort ) {
					t = abort[j];
					abort[j] = abort[j+1];
					abort[j+1] = t;
					t = {"\0"};
				}  // if
			}  // for
		}  // for

		if ( abort.size() != 0 ) {
			cout <<  "	[Abort List]\n	OID	CID	Delay	Abort\n";
			newFile << "	[Abort List]\n" << "	OID	CID	Delay	Abort\n";
			for ( int i = 0 ; i < abort.size() ; i++ ) {
				late = late + abort[i].typee.Delay;
				cout << "[" << i+1 << "]\t" <<abort[i].typee.OID << "\t" << abort[i].typee.cid << "\t" << abort[i].typee.Delay << "\t" << abort[i].typee.Abort << "\n";
				newFile << "[" << i+1 << "]\t" <<abort[i].typee.OID << "\t" << abort[i].typee.cid << "\t" << abort[i].typee.Delay << "\t" << abort[i].typee.Abort << "\n";
			}  // for	
		}  // if
			
		if ( timeout.size() != 0 ) {
			cout << "	[Timeout List]\n	OID	CID	Delay	Departure\n";
			newFile << "	[Timeout List]\n" << "	OID	CID	Delay	Departure\n";
			for ( int i = 0 ; i < timeout.size() ; i++ ) {
				late = late + timeout[i].typee.Delay;
				cout << "[" << i+1 << "]\t" << timeout[i].typee.OID << "\t" << timeout[i].typee.cid << "\t" << timeout[i].typee.Delay << "\t" << timeout[i].typee.Departure << "\n";
				newFile << "[" << i+1 << "]\t" << timeout[i].typee.OID << "\t" << timeout[i].typee.cid << "\t" << timeout[i].typee.Delay << "\t" << timeout[i].typee.Departure << "\n";
			}  // for	
		}  // if
		
		cout << "[Total Delay]\n" << late << " min.\n";
		newFile << "[Total Delay]\n" << late << " min.\n";
		float Failure = round(float((abort.size()+timeout.size()))/main.size()*100.0*100)/100;
		cout << "[Failure Percentage]\n";
		newFile << "[Failure Percentage]\n" << Failure << "%\n";
		cout<< Failure << "%\n" ;
		main.clear();
		newFile.close();
		file.close();
	} // if
	else  cout << "\n" << "###" << sort << " does not exist! ###" << "\n";
 
}  // Three()

void Four() {
	int time = 0;
	string filename;
    int run1 = 0;
    fstream file;
    string sort = "\0" ;
    vector<Category> main;
    vector<Category> abort;
    vector<Category> timeout;
	Category t;
	Category tempp;
	Category data;
	vector <Queue> chef ;

	cout << "\n" << "Input a file number (e.g., 401, 402, 403, ...): ";
   	cin >> filename;
   	sort = "\0" ;
	sort = sort + "sort";
	sort = sort + filename;
	sort = sort + ".txt";
	file.open( sort );
	if( file.is_open() == true ) {  // 確認在同個目錄下是否存在這個檔案
		string num ;
		int m = -1;
		
		bool isnum = true;
		bool notend = true;
		while ( notend ) {
			isnum = true;
			cout << "\nInput the number of queues:" ;
			cin >> num;
			for (int f = 0; f < num.size(); f++) {  // 判斷讀入的字串是否為數字 
			    int temp = (int)num[f];
			    if (temp >= 48 && temp <= 57) {
			       	continue;
			   	}  // if
			    else {
					isnum = false;
				}  // else 
			}  // for
			if ( !isnum )  notend = true;
			else  notend = false;
			
		} // while
	    
	    m = atoi(num.c_str());  // 字串轉int
	    
		for(int i = 0 ; i < m ; i++){  // 產生多個佇列 
			Queue temp;
			temp.cid = i+1;
			temp.free = 0;
			chef.push_back(temp);
		}  // for
		
		string line;

		bool first = false;
			
			
		while ( getline( file, line ) ) {  // 先存下所有切割完的東西 
			int tabb = 0;
			if ( first == false ) {
				first = true;
			}  // if
			else {
				for( int i = 0 ; i < line.size() ; i++ ) {
					if( line[i] == '\t' )  tabb++;
					if( tabb == 0 && line[i] != '\t' )  data.typee.OID = data.typee.OID + line[i];
					if( tabb == 1 && line[i] != '\t' )  data.typee.Arrival = data.typee.Arrival + line[i];
					if( tabb == 2 && line[i] != '\t' )  data.typee.Duration = data.typee.Duration + line[i];
					if( tabb == 3 && line[i] != '\t' )  data.typee.TimeOut = data.typee.TimeOut + line[i];
				}  // for
				main.push_back( data );
				data = {"\0"};  // 清空 	
			}  // else

		}  // while
			
		for ( int i = 0 ; i < main.size() ; i++ ) {  // 把所有佇列中所有訂單處理完
			
			for ( int n = 0 ; n < chef.size() ; n++ ) {	
				// 確認目前手上訂單可不可以完成 (可完成)
				while ( chef[n].free <= atoi(main[i].typee.Arrival.c_str()) && !chef[n].isEmpty() ) {   
					tempp = chef[n].getFront();							
					// 處理佇列中第一筆訂單 
					int lastnow = chef[n].free;
					if( chef[n].free < atoi(tempp.typee.TimeOut.c_str()) ) {  // 訂單會繼續做
						
						//  佇列第一筆訂單完成時間
						chef[n].free =  chef[n].free + atoi(tempp.typee.Duration.c_str()); 
								
						if ( chef[n].free > atoi(tempp.typee.TimeOut.c_str()) ) {  // 但訂單逾時 
							t.typee.OID = tempp.typee.OID;
							t.typee.cid = chef[n].cid;
								
							// 如果上一筆訂單完成時間比佇列第一筆訂單抵達時間小代表沒有delay
							if ( lastnow <= atoi(tempp.typee.Arrival.c_str()) )   
								t.typee.Delay = 0;
							else  
								t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str());
											
							t.typee.Departure = chef[n].free;
							timeout.push_back(t);
							t = {"\0"};  // 清空	
						}  // if
							
		  			}  // if
					else {  // 取消訂單
						t.typee.OID = tempp.typee.OID;
						t.typee.cid = chef[n].cid;
						t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str()) ;
						t.typee.Abort = lastnow;
						abort.push_back(t);
						t = {"\0"};  // 清空
					}  // else
					
					chef[n].deQueue();  // 處理好了移除佇列中第一個訂單
				}  // while


			}  // for
			
			
			bool deal = false; 
			if ( chef[small(chef)].length() == 0  ) {
								
				for( int j = 0 ; j < chef.size() ; j++ ) {  // 看看有沒有廚師手上沒是並且是空柱列 
					if( chef[j].length() == 0 && chef[j].free <= atoi(main[i].typee.Arrival.c_str()) ) {
						chef[j].free = atoi(main[i].typee.Arrival.c_str())+atoi(main[i].typee.Duration.c_str());
						deal = true;
						break;		
					}  // if
				}  // for
			}  // if

			
			// 佇列不是滿的而且現在的訂單還沒被處理過就可以放入最短的佇列
			if ( chef[small(chef)].length() < 3 ) { 

				if ( !deal )
					chef[small(chef)].enQueue(main[i]);
					
			}  // else if
			else if ( chef[small(chef)].length() == 3 ) { // 佇列全滿 
			
				t.typee.OID = main[i].typee.OID;
				t.typee.cid = 0;
				t.typee.Delay = 0 ;
				t.typee.Abort = atoi(main[i].typee.Arrival.c_str());
				abort.push_back(t);
				t = {"\0"};  // 清空
				
			}  // else if
				
		}  // for
			
			
		for ( int i = 0 ; i < chef.size() ; i++ ) {  // 把所有佇列中所有訂單處理完
			while ( chef[i].isEmpty() != true ) {   
				tempp = chef[i].getFront();
				int lastnow = chef[i].free;
					
				if( lastnow < atoi(tempp.typee.TimeOut.c_str()) ) {  // 訂單會繼續做
					chef[i].free = chef[i].free + atoi(tempp.typee.Duration.c_str());
					if ( chef[i].free > atoi(tempp.typee.TimeOut.c_str()) ) {  // 但訂單逾時 
						t.typee.OID = tempp.typee.OID;
						t.typee.cid = chef[i].cid;
						// 如果上一筆訂單完成時間比佇列第一筆訂單抵達時間小代表沒有delay
						if ( lastnow <= atoi(tempp.typee.Arrival.c_str()) )   
							t.typee.Delay = 0;
						else  
							t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str());
										
						t.typee.Departure = chef[i].free;
						timeout.push_back(t);
						t = {"\0"};  // 清空	
					}  // if
	 
				}  // if 
				else {  // 取消訂單
					t.typee.OID = tempp.typee.OID;
					t.typee.cid = chef[i].cid;
					t.typee.Delay = lastnow - atoi(tempp.typee.Arrival.c_str()) ;
					t.typee.Abort = lastnow;
					abort.push_back(t);
					t = {"\0"};  // 清空
				}  // else
				chef[i].deQueue();  // 完成後移除
					
			}  // while	
		}  // for

    	string more = "\0" ;
		more = more + "more";
		more = more + filename;
		more = more + ".txt";
		ofstream newFile;
		newFile.open( more.c_str() );
		int late = 0;

		for ( int i = abort.size()-1 ; i > 0 ; i-- ) {
			for ( int j = 0 ; j <= i-1 ; j++ ) {
				if ( abort[j].typee.Abort > abort[j+1].typee.Abort ) {
					t = abort[j];
					abort[j] = abort[j+1];
					abort[j+1] = t;
					t = {"\0"};
				}  // if
			}  // for
		}  // for

		if ( abort.size() != 0 ) {
			cout <<  "	[Abort List]\n	OID	CID	Delay	Abort\n";
			newFile << "	[Abort List]\n" << "	OID	CID	Delay	Abort\n";
			for ( int i = 0 ; i < abort.size() ; i++ ) {
				late = late + abort[i].typee.Delay;
				cout << "[" << i+1 << "]\t" <<abort[i].typee.OID << "\t" << abort[i].typee.cid << "\t" << abort[i].typee.Delay << "\t" << abort[i].typee.Abort << "\n";
				newFile << "[" << i+1 << "]\t" <<abort[i].typee.OID << "\t" << abort[i].typee.cid << "\t" << abort[i].typee.Delay << "\t" << abort[i].typee.Abort << "\n";
			}  // for	
		}  // if
		else {
			cout <<  "	[Abort List]\n	OID	CID	Delay	Abort\n";
			newFile << "	[Abort List]\n" << "	OID	CID	Delay	Abort\n";
		}  // else
		if ( timeout.size() != 0 ) {
			cout << "	[Timeout List]\n	OID	CID	Delay	Departure\n";
			newFile << "	[Timeout List]\n" << "	OID	CID	Delay	Departure\n";
			for ( int i = 0 ; i < timeout.size() ; i++ ) {
				late = late + timeout[i].typee.Delay;
				cout << "[" << i+1 << "]\t" << timeout[i].typee.OID << "\t" << timeout[i].typee.cid << "\t" << timeout[i].typee.Delay << "\t" << timeout[i].typee.Departure << "\n";
				newFile << "[" << i+1 << "]\t" << timeout[i].typee.OID << "\t" << timeout[i].typee.cid << "\t" << timeout[i].typee.Delay << "\t" << timeout[i].typee.Departure << "\n";
			}  // for	
		}  // if
		else {
			cout << "	[Timeout List]\n	OID	CID	Delay	Departure\n";
			newFile << "	[Timeout List]\n" << "	OID	CID	Delay	Departure\n";	
		}  // else
		
		cout << "[Total Delay]\n" << late << " min.\n";
		newFile << "[Total Delay]\n" << late << " min.\n";
		float Failure = round(float((abort.size()+timeout.size()))/main.size()*100.0*100)/100;
		cout << "[Failure Percentage]\n";
		newFile << "[Failure Percentage]\n" << Failure << "%\n";
		cout<< Failure << "%\n" ;
		main.clear();
		newFile.close();
		file.close();
	} // if
	else  cout << "\n" << "###" << sort << " does not exist! ###" << "\n";
 
}  // Four()

int main() {
	int run = 0;
      
    while ( run == 0 ) {
    	bool functionisnum = true;
		string function;
		int command = 0;
		
    	cout << "\n" << "**** Simulate FIFO Queues by SQF *****" << "\n" << "* 0. Quit                            *" << "\n";
    	cout << "* 1. Sort a file                     *" << "\n" << "* 2. Simulate one FIFO queue         *" << "\n";
		cout << "* 3. Simulate two queues by SQF      *" << "\n* 4. Simulate multiple queues by SQF *\n" ;
		cout << "**************************************" << "\n" << "Input a choice(0, 1, 2, 3, 4):" ;
		cin >> function;	  
		for (int f = 0; f < function.size(); f++) {  // 判斷讀入的字串是否為數字 
        	int temp = (int)function[f];
        	if (temp >= 48 && temp <= 57) {
        		continue;
        	}  // if
        	else  functionisnum = false;
    	}  // for
    
    	if ( functionisnum )  command = atoi(function.c_str());  // 字串轉int
    	if ( functionisnum && command == 0 )  run++;
    	
    	else if ( functionisnum && command == 1 ) {
    		Sort();
	    }  // else if
	    
		else if ( functionisnum && command == 2 ) {
			Two();
		}  // else if
		
		else if ( functionisnum && command == 3 ) {
			Three();
		}  // else if
		
		else if ( functionisnum && command == 4 ) {
			Four();
		}  // else if
		else {
			cout << "\nthis command does not exist";
		}
	}  // while
}  // main
