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
#include <map>
#include <sstream>
using namespace std ;

struct data{										
	public:
		int PID = 0 ;  
		int CPU_Burst = 0 ;
		int Arrival = 0 ;  
		int priority = 0;
		int turnaround = 0;
    	int waiting = 0;
		int remain = 0;  //還剩多久沒做完
		bool finish = false; 
		bool putalready = false;
		float rr;
};

void read( vector<data> &input, int &method, int &time_slice, string filename ){
  ifstream file;
  data temp;
  string text;
  file.open( filename + ".txt" );
  if (!file.is_open() ) { //開啟失敗 
    cout << filename << ".txt does not exist! ###\n";
    return;
  }  // if
  else {
    file >> method >> time_slice;
    file.get(); //'\n'
    getline( file, text );  // 標題 
    while( file >> temp.PID ){
      file >> temp.CPU_Burst >> temp.Arrival >> temp.priority ;
      temp.remain = temp.CPU_Burst;
      temp.finish = false;
      temp.putalready = false;
      file.get(); //'\n'
      input.push_back( temp );
    }  // while
    
  }  // else 
  file.close();
}  // read

// 先按照arrive排序 
void sortarrive( vector<data> &input ) {
	for( int i = input.size()-1 ; i > 0 ; i-- ) {  
		for( int j = 0 ; j < i ; j++ ) {
			if( input[j+1].Arrival <= input[j].Arrival ) {
				if( input[j+1].Arrival == input[j].Arrival ) {
					if( input[j+1].PID < input[j].PID )
						swap( input[j], input[j+1] );
				}  // if
				else  swap( input[j], input[j+1] );
			}  // if
		}  // for
	}  // for
//	for( int i = 0 ; i < input.size() ; i++ ) {
//		cout << input[i].PID << "  " << input[i].Arrival << "   " << input[i].CPU_Burst <<"\n";
//	}  // for
}  // sort

void sortpid( vector<data> &input ) {
	for( int i = input.size()-1 ; i > 0 ; i-- ) {  
		for( int j = 0 ; j < i ; j++ ) {
			if( input[j+1].PID < input[j].PID ) {
				swap( input[j], input[j+1] );	
			}  // if
		}  // for
	}  // for
//	for( int i = 0 ; i < input.size() ; i++ ) {
//		cout << input[i].PID << "  " << input[i].Arrival << "   " << input[i].CPU_Burst <<"\n";
//	}  // for
}  // sort
void sortpriority( vector<data> &input ) {
	for( int i = input.size()-1 ; i > 0 ; i-- ) {  
		for( int j = 0 ; j < i ; j++ ) {
			if( input[j+1].priority < input[j].priority ) {
				swap( input[j], input[j+1] );	
			}  // if
		}  // for
	}  // for
//	for( int i = 0 ; i < input.size() ; i++ ) {
//		cout << input[i].PID << "  " << input[i].Arrival << "   " << input[i].CPU_Burst <<"\n";
//	}  // for
}  // sortpriority
 
void write( vector<data> input , int method, string result, string filename ) {
	sortpid( input );
	string way = "";
	ofstream file;
	file.open( "out_" + filename + ".txt" );
  	if( !file.is_open() ){
    	cout << "fail\n";
    	return;
  	}  // if
	if( method == 1 )  way = "FCFS";
	else if( method == 2 )    way = "RR";
	else if( method == 3 )    way = "SJF";
	else if( method == 4 )    way = "SRTF";
	else if( method == 5 )    way = "HRRN";
	else if( method == 6 )    way = "PPRR";
	if( method < 6 )  file << way << "\n==        " << way << "==\n";
	else  file << "Priority RR\n==        " << way << "==\n";
	file << result << "\n";
	file << "===========================================================\n" << "\nWaiting Time\n";
	
	file << "ID	"<< way << "\n===========================================================\n";
	
	for( int i = 0 ; i < input.size() ; i++ ) {
		file << input[i].PID << "\t" << input[i].waiting << "\n";
	}  // for
	
	file << "===========================================================\n" << "\nTurnaround Time\n";
	file << "ID	"<< way << "\n===========================================================\n";
	
	for( int i = 0 ; i < input.size() ; i++ ) {
		file << input[i].PID << "\t" << input[i].turnaround << "\n";
	}  // for
	file << "===========================================================\n\n";
	
}  // write

void FCFS( vector<data> &input, string filename, int method, string &result ) {
	sortarrive( input ) ;
	for( int i = 0 ; i < input.size() ; i++ ) {    int turnaround = 0;
		if( i == 0 ) {  // 第一個
			input[i].waiting = 0;
			input[i].turnaround = input[i].Arrival + input[i].CPU_Burst - input[i].Arrival;
		}  // if
		else {
			// 前一個做完的時間 - 我抵達時間 
//			cout << input[i-1].PID << " a  " << input[i-1].Arrival << " t  " <<  input[i-1].turnaround << "\n";
			int wait = input[i-1].Arrival + input[i-1].turnaround - input[i].Arrival;
//			cout << input[i].PID << "   " << wait << "\n";
			if( wait < 0 )  input[i].waiting = 0 ;  // 沒有等待
			else   input[i].waiting = wait;
 			input[i].turnaround = input[i].waiting + input[i].CPU_Burst;
		}  // else
//		cout << input[i].PID << "   " << input[i].Arrival << "   " << input[i].CPU_Burst << "   " << input[i].waiting ;
//		cout << "    " << input[i].turnaround << "\n";
	}  // for
	map<int, char> mapping;  // 轉換成字母 
    for (int i = 10; i <= 35; ++i) {
        mapping[i] = 'A' + (i - 10);
    }  // for
	
	stringstream output; 
    int now = 0;
    for (int i = 0; i < input.size(); i++) {
        int finish = input[i].Arrival + input[i].turnaround;
        while (finish > now) {
            if (input[i].Arrival > now) {
                output << "-";
            }  // if
			else {
                if (input[i].PID < 10)  output << input[i].PID;
                else if (input[i].PID >= 10 && input[i].PID <= 35)  output << mapping[input[i].PID];
            }  // else
            now++;
        }  // while
    }  // for
    result = output.str();
//	cout << result << "      ppp\n";
	
	if( method == 1)
		write( input , method, result, filename );
}  // FCFS

void RR( vector<data> &input, string filename, int method , int time_slice, string &result ) {
	queue<data> q;  // 誰正在排隊 
	vector<data> rr;
	sortarrive( input ) ;
	int now = 0;
	int finish = 0 ;
	map<int, char> mapping;  // 轉換成字母 
    for (int i = 10; i <= 35; ++i) {
        mapping[i] = 'A' + (i - 10);
    }  // for
	stringstream output;
	
	while( finish < input.size() ) {
		for( int i = 0 ; i < input.size() ; i++ ) {
			if( q.size() == 0 ) {
				while( now < input[i].Arrival ) {
					output << "-";
					now++;
				}  // while	
			}  // if
			if( now >= input[i].Arrival && input[i].remain > 0 && input[i].putalready != true )  {
				input[i].putalready = true;
				q.push( input[i] );
			}  // if
			
		}  // for
		while( !q.empty() ) {
			data temp = q.front();
			if( temp.remain > time_slice ) {
				now = now + time_slice;
				temp.remain = temp.remain - time_slice;
				int j = 0;
				for( j = 0 ; j < input.size() ; j++ ) {
					if( input[j].PID == temp.PID )  {
						input[j].remain = temp.remain;
						break;
					}  // if
				}  // for
				int i = 0;
				while( i < time_slice )  {
					if (temp.PID < 10)  output << temp.PID;
                	else if (temp.PID >= 10 && temp.PID <= 35)  output << mapping[temp.PID];
                	i++;
				}  // while
				for( int i = 0 ; i < input.size() ; i++ ) {
					if( q.size() == 0 ) {
						while( now < input[i].Arrival ) {
							output << "-";
							now++;
						}  // while	
					}  // if
					if( now >= input[i].Arrival && input[i].remain > 0 && input[i].putalready != true )  {
						input[i].putalready = true;
						q.push( input[i] );
					}  // if
				}  // for
				q.push( input[j] );
			}  // if
			else  {
				now = now + temp.remain;
				int i = 0;
				while( i < temp.remain )  {
					if (temp.PID < 10)  output << temp.PID;
                	else if (temp.PID >= 10 && temp.PID <= 35)  output << mapping[temp.PID];
                	i++;
				}  // while	
				for( int j = 0 ; j < input.size() ; j++ ) {
					if( input[j].PID == temp.PID )  input[j].remain = 0;
				}  // for
				temp.remain = 0;
				finish++;
				temp.turnaround = now - temp.Arrival;
				temp.waiting = temp.turnaround - temp.CPU_Burst;
				rr.push_back( temp );
			}  // else
			q.pop();
		}  // while	
	}  // while
	
	
	
    result = output.str();
    input.clear();
    input = rr;
    if( method == 2 )
		write( rr , method, result, filename );
}  // RR

void SJF( vector<data> &input, string filename, int method, string &result ) {  //  最短優先工作排程法 
	sortarrive( input ) ;
	int now = 0;
	map<int, char> mapping;  // 轉換成字母 
    for (int i = 10; i <= 35; ++i) {
        mapping[i] = 'A' + (i - 10);
    }  // for
    vector<data> wait;
    stringstream output; 
    int finish = 0;  // 計算做完的
	while( finish < input.size() ) {
		vector<data> wait;
		int i = 0;
		
		while( i < input.size() ) {
			if( wait.size() == 0 ) {
				while( input[i].Arrival > now ) {
					output << "-";
					now++;
				}  // while
			}  // if
			if( now >= input[i].Arrival && input[i].finish == false ) {
				wait.push_back( input[i] );
			}  // if
			i++;
		}  // while
//		for( int n = 0 ; n < wait.size() ; n++ ) {
//			cout << wait[n].PID << "\n";
//		}   // for
		if( wait.size() != 0 ) {
			vector<int> cpu;
			for( int k = 0 ; k < wait.size() ; k++ ) {
				cpu.push_back( wait[k].CPU_Burst );
			}  // for
			auto min = min_element(cpu.begin(), cpu.end());  // 找最小 
			int minindex = distance(cpu.begin(), min);

	    	int j = 0;
	    	while( input[j].PID != wait[minindex].PID ) {
	    		j++;
			}  // while
//			cout << input[j].PID << "  322322\n";
			while( input[j].Arrival > now ) {
				output << "-";
				now++;
			}  // while
	    	input[j].waiting = now - input[j].Arrival;
			input[j].finish = true;
			int finishtime = now + input[j].CPU_Burst;
			while( now < finishtime ) {
				if (input[j].PID < 10)  output << input[j].PID;
	            else if (input[j].PID >= 10 && input[j].PID <= 35)  output << mapping[input[j].PID];
	            now++;
			}  // while
			input[j].turnaround = now - input[j].Arrival;			
		}  // if

		finish++;
	}  // while
	result = output.str();
//	sortpid( input );
	if( method == 3 )
		write( input , method, result, filename );
}  // SJF()

void SRTF( vector<data> &input, string filename, int method, string &result ) {  // 可奪取最短剩餘時間 
    sortarrive(input);
    int now = 0;
    map<int, char> mapping;  // 轉換成字母
    for (int i = 10; i <= 35; ++i) {
        mapping[i] = 'A' + (i - 10);
    }  // for
    stringstream output;
    vector<data> srtf;
    int finish = 0;  // 計算做完的
	int time = 0, num = 0,  calcu = 0;
	while( finish < input.size() ){
    	int shortest = -1;
	    for (int i = 0; i < input.size(); i++) {
	        if ( input[i].Arrival <= time && input[i].remain > 0 ) {  // 尋找目前可以執行且剩餘時間最短的
	        	if ( shortest == -1 || input[i].remain < input[shortest].remain ) {
	            	shortest = i;
	          	} // if
	        }  // if
	    }  // for
	    if ( shortest == -1 ) { //沒東西做 
	    	time++;
	      	output << "-";
	    }  // if
	    else {  //有東西
	      	data tmp; 
	      	input[shortest].remain--;
	      	if (input[shortest].PID < 10)  output << input[shortest].PID;
	        else if (input[shortest].PID >= 10 && input[shortest].PID <= 35)  output << mapping[input[shortest].PID];
	
	      	if (input[shortest].remain == 0) {
		        tmp = input[shortest];
		        tmp.turnaround = time+1 - tmp.Arrival ;
		        tmp.waiting = tmp.turnaround - tmp.CPU_Burst;
		        srtf.push_back(tmp);
		        finish++;
		    }  // if
	      time++;
	    }  // else
  	}  // while
  	result = output.str();
//  	cout << result << "\n";
//	sortpid( srtf );
	input.clear();
	input = srtf;
	if( method == 4)
		write( srtf , method, result, filename );
}  // SRTF

void HRRN( vector<data> &input, string filename, int method, string &result ) {
	sortarrive( input ) ;
	int now = 0;
	map<int, char> mapping;  // 轉換成字母 
    for (int i = 10; i <= 35; ++i) {
        mapping[i] = 'A' + (i - 10);
    }  // for
    vector<data> wait;
    stringstream output; 
    int finish = 0;  // 計算做完的
	while( finish < input.size() ) {
		vector<data> wait;
		int i = 0;
		
		while( i < input.size() ) {
			if( wait.size() == 0 ) {
				while( input[i].Arrival > now ) {
					output << "-";
					now++;
				}  // while
			}  // if
			if( now >= input[i].Arrival && input[i].finish == false ) {
				wait.push_back( input[i] );
			}  // if
			i++;
		}  // while
//		for( int n = 0 ; n < wait.size() ; n++ ) {
//			cout << wait[n].PID << "\n";
//		}   // for
		if( wait.size() != 0 ) {
			vector<float> rate;
			for( int k = 0 ; k < wait.size() ; k++ ) {
				float r = static_cast<float>(now - wait[k].Arrival + wait[k].CPU_Burst) / wait[k].CPU_Burst;
//				cout << wait[k].PID << "   " << r << "\n";
				rate.push_back( r );
			}  // for
			auto max = max_element(rate.begin(), rate.end());  // 找最大 
	    	int maxindex = distance(rate.begin(), max);
	    	int j = 0;
	    	while( input[j].PID != wait[maxindex].PID ) {
	    		j++;
			}  // while
//			cout << input[j].PID << "  322322\n";
			while( input[j].Arrival > now ) {
				output << "-";
				now++;
			}  // while
	    	input[j].waiting = now - input[j].Arrival;
			input[j].finish = true;
			int finishtime = now + input[j].CPU_Burst;
			while( now < finishtime ) {
				if (input[j].PID < 10)  output << input[j].PID;
	            else if (input[j].PID >= 10 && input[j].PID <= 35)  output << mapping[input[j].PID];
	            now++;
			}  // while
			input[j].turnaround = now - input[j].Arrival;			
		}  // if

		finish++;
	}  // while
	result = output.str();
//	sortpid( input );
	if( method == 5)
		write( input , method, result, filename );
}  // HRRN()

void PPRR( vector<data> &input, string filename, int method , int time_slice, string &result ) {
	vector<data> q;  // 誰正在排隊 
	vector<data> pprr;
	sortarrive( input ) ;
	int now = 0;
	int finish = 0 ;
	int k = 0;
	map<int, char> mapping;  // 轉換成字母 
    for (int i = 10; i <= 35; ++i) {
        mapping[i] = 'A' + (i - 10);
    }  // for
	stringstream output;
	while( finish < input.size() ) {
		for( int i = 0 ; i < input.size() ; i++ ) {
			if( now >= input[i].Arrival && input[i].remain > 0 && input[i].putalready != true )  {
				input[i].putalready = true;
				q.push_back( input[i] );
			}  // if
		}  // for
		if( q.empty() ) {  // 如果沒有process可以執行
			now++;
			output << "-";
			continue;
		}  // if
		sortpriority( q );  // 排優先度 
		data temp = q[0];
		q.erase( q.begin() );
		int t = 0;
		while( temp.remain > 0 ) {
			temp.remain--;
			now++;
			t++; 
			if (temp.PID < 10)  output << temp.PID;
            else if (temp.PID >= 10 && temp.PID <= 35)  output << mapping[temp.PID];
			for( int i = 0 ; i < input.size() ; i++ ) {
				if( now >= input[i].Arrival && input[i].remain > 0 && input[i].putalready != true )  {
					input[i].putalready = true;
					q.push_back( input[i] );
				}  // if
			}  // for
			sortpriority( q );  // 排優先度 
			int same = 0;
			for( int i = 0 ; i < q.size() ; i++ ) {  // 看優先度一樣 
				if( q[i].priority == temp.priority ) same++;
			}  // for
			if( !q.empty() && q[0].priority < temp.priority )  {  // 目前做的不是優先度最高的 
				q.push_back( temp );  // 如果任務還未完成，則放回佇列最後面
				break;  //新進來的優先度更高 
			}  // if
			else if ( same > 0 ) {  // 有其他優先度一樣
				 if( t == time_slice && temp.remain != 0 ) {
				 	t = 0;
				 	q.push_back( temp );  // 如果任務還未完成，則放回佇列最後面
				 	break;
				 }   // if
			}  // else
		}  // while
		if( temp.remain == 0 ) {
			temp.turnaround = now - temp.Arrival;
			temp.waiting = temp.turnaround - temp.CPU_Burst;
			pprr.push_back( temp );
			finish++;
		}  // if
	}  // while
//	sortpid( pprr );
    result = output.str();
//    cout << result << "\n";
	input.clear();
	input = pprr;
	if( method == 6 )
		write( pprr , method, result, filename );
}  // PPRR


void ALL( vector<data> input, string filename, int method , int time_slice ) {
	vector<data> temp = input;
	vector<data> temp2 = input;
	vector<data> temp3 = input;
	vector<data> temp4 = input;
	vector<data> temp5 = input;
	vector<data> temp6 = input;
	vector<int> wait[input.size()];
	vector<int> turnaround[input.size()];
	sortpid( input );
	string result = "";
	ofstream file;
	file.open( "out_" + filename + ".txt" );
  	if( !file.is_open() ){
    	cout << "fail\n";
    	return;
  	}  // if
  	file << "All\n"; 
	file << "==        FCFS==\n";
  	FCFS( temp, filename, method, result );
  	file << result << "\n";
  	sortpid( temp );
  	for( int i = 0 ; i < temp.size() ; i++ ) {
  		wait[i].push_back( temp[i].waiting );
  		turnaround[i].push_back( temp[i].turnaround );
	}  // for
  	result.clear();
  	file << "==          RR==\n";
	RR( temp2, filename, method , time_slice, result );
	sortpid( temp2 );
  	file << result << "\n";
  	for( int i = 0 ; i < temp2.size() ; i++ ) {
  		wait[i].push_back( temp2[i].waiting );
  		turnaround[i].push_back( temp2[i].turnaround );
	}  // for
	result.clear();
	file << "==         SJF==\n";
	SJF( temp3, filename, method, result );
	sortpid( temp3 );
	file << result << "\n";
  	for( int i = 0 ; i < temp3.size() ; i++ ) {
  		wait[i].push_back( temp3[i].waiting );
  		turnaround[i].push_back( temp3[i].turnaround );
	}  // for
	result.clear();
	file << "==        SRTF==\n";
	SRTF( temp4, filename, method, result );
	sortpid( temp4 );
	file << result << "\n";
  	for( int i = 0 ; i < temp4.size() ; i++ ) {
  		wait[i].push_back( temp4[i].waiting );
  		turnaround[i].push_back( temp4[i].turnaround );
	}  // for
	result.clear();
	file << "==        HRRN==\n";
	HRRN( temp5, filename, method, result );
	sortpid( temp5 );
	file << result << "\n";
  	for( int i = 0 ; i < temp5.size() ; i++ ) {
  		wait[i].push_back( temp5[i].waiting );
  		turnaround[i].push_back( temp5[i].turnaround );
	}  // for
	result.clear();
	file << "==        PPRR==\n";
	PPRR( temp6, filename, method , time_slice, result );
	sortpid( temp6 );
	file << result << "\n";
  	for( int i = 0 ; i < temp6.size() ; i++ ) {
  		wait[i].push_back( temp6[i].waiting );
  		turnaround[i].push_back( temp6[i].turnaround );
	}  // for
	file << "===========================================================\n\nWaiting Time\n";
	file << "ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR\n";
	file << "===========================================================";
	for( int i = 0 ; i < input.size() ; i++ ) {
		file << "\n" << input[i].PID ;
		for( int j = 0 ; j < wait[i].size() ; j++ ) {
			file << "\t" << wait[i][j];
		}  // for
	}  // for
	file << "\n===========================================================\n\nTurnaround Time\n";
	file << "ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR\n";
	file << "===========================================================";
	for( int i = 0 ; i < input.size() ; i++ ) {
		file << "\n" << input[i].PID ;
		for( int j = 0 ; j < turnaround[i].size() ; j++ ) {
			file << "\t" << turnaround[i][j];
		}  // for
	}  // for
	file << "\n===========================================================\n\n";
}  // ALL

int main() {
	while ( true ) {
		vector<data> input; 
		string filename;
		string result;
		cout << "Please enter File Name (eg.  input1 ) : " ;
		cin >> filename;
		int method = 0, time_slice = 0;
		read( input, method, time_slice, filename );
		if( method == 1 )   FCFS( input, filename, method, result );
		else if( method == 2 )  RR( input, filename, method , time_slice, result );
		else if( method == 3 )  SJF( input, filename, method, result );
		else if( method == 4 )  SRTF( input, filename, method, result );
		else if( method == 5 )  HRRN( input, filename, method, result );
		else if( method == 6 )  PPRR( input, filename, method , time_slice, result );
		else if( method == 7 )  ALL( input, filename, method , time_slice );
		else  return 1;
	}  // while
  

//  else if( method == 4 ){
//    SRTF( input, gant, Turnaround, Waiting );
//  }  // else if

//  else if( method == 6 ){
//    PPRR( input, gant, Turnaround, Waiting, time_slice);
//  }  // else if
//  else if( method == 7 ){
//    ALL(gant, input, Turnaround,  Waiting, filename, time_slice );
//  }  // else if
}  // main
