#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <stack>
#include <time.h>
#include <iomanip>
#include <string>
#include <math.h>
#include <queue>
#include <deque> 
#include <algorithm>
using namespace std ;

struct data{										
	public:
		char ch;
		int count = 0;
};
void read( string &input, int &method, int &frame, string filename ){
	ifstream file;
	string  msg;
	file.open( filename + ".txt" );
	if ( !file.is_open() ) { //開啟失敗 
		cout <<  filename << ".txt does not exist! ###" << endl;
		return;
	}  // if
	else{
		file >> method >> frame;
		file.get(); //'\n'
		getline( file, input );    
	}  // else if
	file.close();
}  // read() 

void FIFO( string input, int framenum, string filename ) {
	ofstream file;
	file.open( "out_" + filename + ".txt" );    
	

	file << "--------------FIFO-----------------------\n"; 
	int pagefault = 0;
	int replace = 0;
	vector<char> page;
	for( int i = 0 ; i < input.length() ; i++ ) {
		bool exist = false;  // 有在裡面了嗎
		bool fault = false;
		if( page.size() < framenum ) {  //還沒滿  直接放進去 
			pagefault++;
			fault = true;
			page.insert( page.begin(), input[i] );
		}  // if
		else if( page.size() == framenum ) {  // 滿了 
			
			for( int j = 0 ; j < page.size() ; j++ ) {
				if( page[j] == input[i] ) {
					exist = true;
					break;
				}  // if
			}  // for
			if( exist == false ) {  // 不在裡面有人要犧牲
				pagefault++;
				fault = true; 
				replace++;
				page.pop_back();
				page.insert( page.begin(), input[i] );
			}  // if
		}  // else if
		file << input[i] << "\t";
		for( int j = 0 ; j < page.size() ; j++ )  file << page[j];
		//file << "";
		if( fault )  file << "\tF\n";
		else  file << "\n";
	}  // for
	
	file << "Page Fault = " << pagefault << "  Page Replaces = " << replace << "  Page Frames = " << framenum << "\n\n";
	
	file.close();
}  // FIFO()

void LRU( string input, int framenum, string filename, bool all ) {  // 已經在裡面 要更新time stamp 
	ofstream file;
	if( all )  file.open( "out_" + filename + ".txt", ios::app );    
	else  file.open( "out_" + filename + ".txt" );
	file << "--------------LRU-----------------------\n"; 
	int pagefault = 0;
	int replace = 0;
	vector<char> page;
	int place = 0;
	for( int i = 0 ; i < input.length() ; i++ ) {
		
		bool exist = false;  // 有在裡面了嗎
		bool fault = false;
		if( page.size() < framenum ) {  //還沒滿  直接放進去 
			pagefault++;
			fault = true;
			page.insert( page.begin(), input[i] );
		}  // if
		else if( page.size() == framenum ) {  // 滿了 
			for( place = 0 ; place < page.size() ; place++ ) {
				if( page[place] == input[i] ) {
					exist = true;
					break;
				}  // if
			}  // for
			if( exist == false ) {  // 不在裡面有人要犧牲
				pagefault++;
				fault = true; 
				replace++;
				page.pop_back();
				page.insert( page.begin(), input[i] );
			}  // if
			else {  // 已經在裡面 要更新time stamp 
				page.erase( page.begin()+place );
				page.insert( page.begin(), input[i] );
			}  // else
		}  // else if
		file << input[i] << "\t";
		for( int j = 0 ; j < page.size() ; j++ )  file << page[j];
		//file << "";
		if( fault )  file << "\tF\n";
		else  file << "\n";
	}  // for
	
	file << "Page Fault = " << pagefault << "  Page Replaces = " << replace << "  Page Frames = " << framenum << "\n\n";
	
	file.close();
}  // LRU()

void LFU_FIFO( string input, int framenum, string filename, bool all ) {
	ofstream file;
	if( all )  file.open( "out_" + filename + ".txt", ios::app );    
	else  file.open( "out_" + filename + ".txt" );
	file << "--------------Least Frequently Used Page Replacement-----------------------\n"; 
	int pagefault = 0;
	int replace = 0;
	vector<data> page;
	int place = 0;
	for( int i = 0 ; i < input.length() ; i++ ) {
		
		bool exist = false;  // 有在裡面了嗎
		bool fault = false;
		if( page.size() < framenum ) {  //還沒滿  直接放進去 
			pagefault++;
			fault = true;
			data temp;
			temp.ch = input[i];
			temp.count = 1;
			page.insert( page.begin(), temp );
		}  // if
		else if( page.size() == framenum ) {  // 滿了 
			for( place = 0 ; place < page.size() ; place++ ) {
				if( page[place].ch == input[i] ) {
					page[place].count++;
					exist = true;
					break;
				}  // if
			}  // for
			if( exist == false ) {  // 不在裡面有人要犧牲 count最小要犧牲 
				pagefault++;
				fault = true; 
				replace++;
				int min = 9999;
				int index = -1;
				for( int j = 0 ; j < page.size() ; j++ ) {
					if( page[j].count <= min ) {
						min = page[j].count;
						index = j;
					}  // if
				}  // for
				// 犧牲者是j 
				page.erase( page.begin()+index );
				data temp;
				temp.ch = input[i];
				temp.count = 1;
				page.insert( page.begin(), temp );
			}  // if
		}  // else if
		file << input[i] << "\t";
		for( int j = 0 ; j < page.size() ; j++ )  file << page[j].ch;
		//file << "";
		if( fault )  file << "\tF\n";
		else  file << "\n";
	}  // for
	
	file << "Page Fault = " << pagefault << "  Page Replaces = " << replace << "  Page Frames = " << framenum << "\n\n";
	
	file.close();
}  // LFU_FIFO()

void MFU_FIFO(  string input, int framenum, string filename, bool all ) {
	ofstream file;
	if( all )  file.open( "out_" + filename + ".txt", ios::app );    
	else  file.open( "out_" + filename + ".txt" );
	file << "--------------Most Frequently Used Page Replacement -----------------------\n"; 
	int pagefault = 0;
	int replace = 0;
	vector<data> page;
	int place = 0;
	for( int i = 0 ; i < input.length() ; i++ ) {
		
		bool exist = false;  // 有在裡面了嗎
		bool fault = false;
		if( page.size() < framenum ) {  //還沒滿  直接放進去 
			pagefault++;
			fault = true;
			data temp;
			temp.ch = input[i];
			temp.count = 1;
			page.insert( page.begin(), temp );
		}  // if
		else if( page.size() == framenum ) {  // 滿了 
			for( place = 0 ; place < page.size() ; place++ ) {
				if( page[place].ch == input[i] ) {
					page[place].count++;
					exist = true;
					break;
				}  // if
			}  // for
			if( exist == false ) {  // 不在裡面有人要犧牲 count最大要犧牲 
				pagefault++;
				fault = true; 
				replace++;
				int max = -1;
				int index = -1;
				for( int j = 0 ; j < page.size() ; j++ ) {
					if( page[j].count >= max ) {
						max = page[j].count;
						index = j;
					}  // if
				}  // for
				// 犧牲者是j 
				page.erase( page.begin()+index );
				data temp;
				temp.ch = input[i];
				temp.count = 1;
				page.insert( page.begin(), temp );
			}  // if
		}  // else if
		file << input[i] << "\t";
		for( int j = 0 ; j < page.size() ; j++ )  file << page[j].ch;
		//file << "";
		if( fault )  file << "\tF\n";
		else  file << "\n";
	}  // for
	
	file << "Page Fault = " << pagefault << "  Page Replaces = " << replace << "  Page Frames = " << framenum << "\n\n";
	
	file.close();
}  // MFU_FIFO()

void LFU_LRU(  string input, int framenum, string filename, bool all ) {
	ofstream file;
	if( all )  file.open( "out_" + filename + ".txt", ios::app );    
	else  file.open( "out_" + filename + ".txt" );
	file << "--------------Least Frequently Used LRU Page Replacement-----------------------\n"; 
	int pagefault = 0;
	int replace = 0;
	vector<data> page;
	int place = 0;
	for( int i = 0 ; i < input.length() ; i++ ) {
		
		bool exist = false;  // 有在裡面了嗎
		bool fault = false;
		if( page.size() < framenum ) {  //還沒滿  直接放進去 
			pagefault++;
			fault = true;
			data temp;
			temp.ch = input[i];
			temp.count = 1;
			page.insert( page.begin(), temp );
		}  // if
		else if( page.size() == framenum ) {  // 滿了 
			for( place = 0 ; place < page.size() ; place++ ) {
				if( page[place].ch == input[i] ) {
					page[place].count++;
					exist = true;
					break;
				}  // if
			}  // for
			if( exist == false ) {  // 不在裡面有人要犧牲 count最小要犧牲 
				pagefault++;
				fault = true; 
				replace++;
				int min = 9999;
				int index = -1;
				for( int j = 0 ; j < page.size() ; j++ ) {
					if( page[j].count <= min ) {
						min = page[j].count;
						index = j;
					}  // if
				}  // for
				// 犧牲者是j 
				page.erase( page.begin()+index );
				data temp;
				temp.ch = input[i];
				temp.count = 1;
				page.insert( page.begin(), temp );
			}  // if
			else {  // 存在要更新time stamp 
				data temp;
				temp.ch = input[i];
				temp.count = page[place].count;
				page.erase( page.begin()+place );
				page.insert( page.begin(), temp );
			}  // else
		}  // else if
		file << input[i] << "\t";
		for( int j = 0 ; j < page.size() ; j++ )  file << page[j].ch;
		//file << "";
		if( fault )  file << "\tF\n";
		else  file << "\n";
	}  // for
	
	file << "Page Fault = " << pagefault << "  Page Replaces = " << replace << "  Page Frames = " << framenum << "\n";
	
	file.close();
}  // LFU_LRU()

void ALL( string input, int framenum, string filename ) { 
	FIFO( input, framenum, filename );
	LRU( input, framenum, filename, true );
	LFU_FIFO( input, framenum, filename, true );
	MFU_FIFO( input, framenum, filename, true );
	LFU_LRU( input, framenum, filename, true );
}  // ALL()


int main(){

	while( true ) {
		string input; 
		string filename;
		cout << "Please enter File Name (eg.  input1 ) :" ;
		cin >> filename;
		int method = 0, framenum = 0;
		read( input, method, framenum, filename );
		if( method == 1 )  FIFO( input, framenum, filename );
	
		else if( method == 2 )  LRU( input, framenum, filename, false );
	
		else if( method == 3 )  LFU_FIFO( input, framenum, filename, false );
	
		else if( method == 4 )  MFU_FIFO( input, framenum, filename, false );
	
		else if( method == 5 )  LFU_LRU( input, framenum, filename, false );
	
		else if( method == 6 )  ALL( input, framenum, filename );		
	}  // while

}  // main()
