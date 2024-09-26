# include <iostream> // cin,cout,endl
# include <cstdlib> // atoi, system
# include <algorithm>
# include <stdio.h>
# include <iomanip>
# include <fstream>
# include <vector> 
# include <string>
# include <string.h>
# include <map>
# include <queue>
# include <stack>
# include <algorithm>
# include <sstream>


using namespace std ;

struct Data{
	public:
	char ID1[10] ;  
	char ID2[10] ; 
	float weight ;  // 權重 
};

struct Receive{
	public:
	string ID ;  
	float weight ;  // 權重 
};

struct Node{
	public:
	int dfsNum = 0 ;
	vector<string> travel ;
	Node * next = NULL ;
};

typedef Node * Ptr ;

bool compareStruct(const Receive& a, const Receive& b) {
	    return a.ID < b.ID;
}  // compareStruct

bool compareWeight(const Receive& a, const Receive& b) {
	    return a.weight < b.weight;
}  // compareStruct

class Readfile{
	private:
	void weight( float &number ) {
	    
	    
	    bool Input = false;
    
	    while ( !Input ) {
	        cout << "Input a real number in (0,1]:";
	        cin >> number;
	        
	        if ( cin.fail() || number <= 0.0 || number > 1.0 ) {
	            cout << "\n### It is NOT in (0,1] ###\n" ;
	            cin.clear();              // 清除錯誤
            	cin.ignore(1000, '\n');
	        }  // if
			else  Input = true;

	    }  // while

	}  // weight
	
	public:
	vector<Data> readbin( Data data, string &filename, float &number ) {
		// 讀 .bin 檔 
		fstream file ;
		int count ;
		vector<Data> d ;
		Data temp ;
		string input = "pairs" + filename + ".bin" ;
		file.open( input.c_str(), fstream::in | fstream::binary ) ; // 開檔案 
		if( file.is_open() == true ) {
			file.seekg( 0, file.end ) ;  //取得文件中第 0個到最後一個 
			count = file.tellg() / sizeof( data ) ;  // 來計算檔案大小 
			file.seekg( 0, file.beg ) ; // 找檔案的開頭 
			
			for( int i = 0 ; i < count ; i++ ) {
				file.read( (char*)&data, sizeof(data) ) ;
				// 從 file 中讀取 data 所指向的記憶體位置開始的 sizeof(data) 個位元組，並將其存儲在 data 中
				strcpy( temp.ID1, data.ID1 ) ;
                strcpy( temp.ID2, data.ID2 ) ;
				temp.weight = data.weight ;
				if( temp.weight <= number )
					d.push_back( temp ) ;
			}  // for
			
		}  // if
		file.close() ; // 關檔案 
		return d ;
	}  // readbin
	
	bool binisexist( vector<Data> &d, string &filename, float &number ) {
		weight( number );
		fstream file ;
		string input = "\0" ;
		Data data ;
		bool success = true ;
		cout << "\n" << "Input a file number ([0] Quit): " ;
	    cin >> filename ;
	    if ( filename.size() == 1 && filename[0] == '0' )  return false ; // [0] Quit 
	    else {
			input = "pairs" + filename + ".bin" ;	
			file.open( input.c_str(), fstream::in | fstream::binary ) ; // 以二進制模式打開檔案 
		}  // else
		
		if( file.is_open() != true ) {
			// 開 .bin 檔失敗 
			cout << "### " << input << " does not exist! ###\n\n"; 
			success = false ;
		}  // if
		else  d = readbin( data, filename, number ) ;
		file.close() ;
		
		return success ;
	}  // binisexist
	
};

class Adjacencylist{

	public:
	
	void createlist( vector<Data> data, map< string, vector<Receive> > &mainlist, string filename, float &number ) {
		for( int i = 0 ; i < data.size() ; i++ ) {
			Receive temp;
			temp.ID = string(data[i].ID2);
			temp.weight = data[i].weight;
			mainlist[string(data[i].ID1)].push_back( temp );
			
			temp.ID = string(data[i].ID1);
			mainlist[string(data[i].ID2)].push_back( temp );
		}  // for
		
		print( mainlist, filename, number );
	}  // createlist
	
	void runall( vector<Data> &data, string filename, map< string, vector<Receive> > &mainlist, float &number ) {  // 計算總共有多少ID 
	
		for( int i = 0 ; i < data.size() ; i++ ) {
			auto find = mainlist.find( string(data[i].ID1) );  // 自動轉換型別 
			if( find != mainlist.end() )  continue;
			else  mainlist[string(data[i].ID1)] = vector<Receive>();  // 不存在，所以放進mainlist
			
			find = mainlist.find( string(data[i].ID2) );
			if( find != mainlist.end() )  continue;
			else  mainlist[string(data[i].ID2)] = vector<Receive>();  // 不存在，所以放進mainlist
		}  // for

		createlist( data, mainlist, filename, number );
	}  // runall
	
	void print( map< string, vector<Receive> > &mainlist, string filename, float &number ) {
		if ( mainlist.size() == 0 ) return ;
		stringstream ss;
    	ss << number;
    	string str = ss.str();
		string name = "pairs" + filename + "_" ;
		name = name + str + ".adj";
		fstream file;
		file.open( name.c_str(), ios::out );
		file <<  "<<< There are " << mainlist.size() << " IDs in total. >>>\n";
		cout << endl <<  "<<< There are " << mainlist.size() << " IDs in total. >>>\n";
		int n = 1;
		int node = 0;

		for ( auto it = mainlist.begin(); it != mainlist.end(); ++it ) {
		    file << "[ " << setw(2) << n << "] " << it->first << ": \n";
		    sort( it->second.begin(), it->second.end(), compareStruct );
		    for( int i = 0 ; i < it->second.size() ; i++ ) {
		    	node++;
		    	file << "\t(" << setw(2) << i+1 << ") " << it->second[i].ID << ",  " << setw(4) << it->second[i].weight ;
		    	if( (i+1)%10 == 0 && i != 0 )  file << "\n";
		    	if( i == it->second.size()-1 )  file << "\n";
			}  // for
			n++;
		}  // for
		
		file << "<<< There are " << node << " nodes in total. >>>";
		cout << endl << "<<< There are " << node << " nodes in total. >>>\n";
		file.close();
	}  // print
};

class ConnectionCount{
	public:	
	void count( map< string, vector<Receive> > mainlist, string filename, float st ) {
		Ptr head = NULL ; 
		Ptr ctc = new Node ;
		int n = 0 ;
		vector<string> v ; // 堆疊用 
		map< string, string > tra ; // 紀錄已走過的 
		for ( auto it = mainlist.begin(); it != mainlist.end(); ++it ) {
		    v.push_back( it->first ) ;
		    ctc->travel.push_back( string( it->first ) ) ;
		    tra[it->first] = it->first ;
		    bool putIn = false ;
		    while ( ! v.empty() ) {
		    	string top = v[v.size()-1] ; // getTop
		    	auto find = mainlist.find(top) ;
		    	bool untraved = false ; 
		    	// 是否還有未走過的 
			    for( int i = 0 ; i < find->second.size() ; i++ ) {
			    	auto f = tra.find( find->second[i].ID ) ;
			    	if( f != tra.end() )  continue;
			        else  {
			        	// 找到一個就可以跳出迴圈 
			        	untraved = true ;
			        	tra[find->second[i].ID] = find->second[i].ID ;
			        	v.push_back( string(find->second[i].ID) ) ;
					    ctc->travel.push_back( string(find->second[i].ID) ) ;
					    putIn = true ;
					    break ;
					} // else
			    } // for
			    
			    // 沒有未走過的 
			    if ( ! untraved ) v.pop_back() ;
		    } // while
		    
		    if ( putIn ) {
		    	ctc->dfsNum = ctc->travel.size() ;
		    	sort( ctc->travel.begin(), ctc->travel.end() ) ;
		    	orderInsert( head, ctc, n ) ;
		    	ctc = new Node ;
			} // if
			else {
				// 沒有路徑不加入head 
				delete ctc ;
				ctc = new Node ;
			} // else 
	    } // for 
	    print( st, n, head, filename ) ;
	} // count()
	
	string floatToString(float value) {
        std::ostringstream oss ;
        oss << value ;  
        return oss.str() ;  
    } // floatToString()
	
	void print( float st, int m , Ptr head, string filename ) {
		// 寫檔 
		string name = "pairs" + filename + "_" + floatToString(st) + ".cc" ;
		fstream file ;
		file.open( name.c_str(), ios::out ) ;
	    file <<  "<<< There are " << m << " connected components in total. >>>" ;
		cout << endl <<  "<<< There are " << m << " connected components in total. >>>" << endl ;
	    Ptr ctc = head ;
	    int n = 1 ;
	    while ( ctc != NULL ) {
	    	file << endl << "{" ;
	    	if ( n < 10 ) file << " " ;
	    	file << n << "} " << "Connected Component: size = " << ctc->dfsNum << endl ;
	    	cout << "{" ;
	    	if ( n < 10 ) cout << " " ;
	    	cout << n << "} " << "Connected Component: size = " << ctc->dfsNum << endl ;
	    	for ( int i = 0 ; i < ctc->travel.size() ; i++ ) {
		        file << " 	" << "(" ;
		        if ( i+1 < 10 ) file << " " ;
		        if ( i+1 < 100 ) file << " " ;
			    file << i+1 << ")" << " " << ctc->travel[i] ;
                if( i != 0 && (i+1)%8 == 0 )  file << endl ;	
		    }  // for
	    	ctc = ctc->next ;
			n++ ;	
		} // while	
		
		file << endl ;
	    file.close() ;	
	}  // print
	
	void orderInsert( Ptr &head, Ptr &temp, int &n ) {
		// 排序 + insert 
		if ( head == NULL ) {
			head = temp ;
			n++ ;
			return ;
		}  // if 
		else if ( head->next == NULL ) {
			if ( head->dfsNum > temp->dfsNum ) head->next = temp ;
			else if ( head->dfsNum == temp->dfsNum ) {
			    if ( head->travel[0] > temp->travel[0] ) {
			    	head->next = temp ;
				} // if
				else {
					temp->next = head ;
			    	head = temp ;
				} // else	
			} // else if 
			else {
			    temp->next = head ;
			    head = temp ;
			}  // else
			n++ ;
			return ;
		}  // else if
		
		if ( head->dfsNum < temp->dfsNum ) {
			temp->next = head ;
			head = temp ;
			n++ ;
	        return ;
		}  // if
		else if ( head->dfsNum == temp->dfsNum ) {
			if ( head->travel[0] > temp->travel[0] ) {
				temp->next = head->next ;
			    head->next = temp ;
			} // if
			else {
				temp->next = head ;
			    head = temp ;
			} // else 	
			n++ ;
	        return ;
		}  // if
		
		Ptr cur = head ;
		while ( cur->next != NULL ) {
			if ( cur->next->dfsNum < temp->dfsNum ) {	
				temp->next = cur->next ;
				cur->next = temp ;
				n++ ;
				return ;
			}  // if
			else if ( cur->next->dfsNum == temp->dfsNum ) {
				while ( cur->next != NULL && cur->next->travel[0] > temp->travel[0] ) cur = cur->next ;
				temp->next = cur->next ;
				cur->next = temp ;
				n++ ;
				return ;
			}  // else if
			
			cur = cur->next ;
		}  // while
		n++ ;
		cur->next = temp ;
	}  // oderInser
};

class Dijkstra {
	public:
	void printselect( map< string, vector<Receive> > &mainlist ) {
		int i = 1;
		for ( auto it = mainlist.begin(); it != mainlist.end(); ++it ) {
		    cout <<  it->first << " ";
		    if( i%8 == 0 )  cout << "\n";
		    i++;
		}  // for
	} // printselect
	string select( map< string, vector<Receive> > &mainlist ) {
		string id;
		cout << "\nInput a student ID [0: exit] :  ";
		cin >> id;
		bool in = true;
		while( in == true ) {
			auto find = mainlist.find( id );  // 自動轉換型別 
			if( find != mainlist.end() )  in = false;
			else if ( id == "0" ) in = false;
			else {
				cout << "\n### the student id does not exist! ###\n";
				printselect( mainlist );
				cout << "\nInput a student ID [0: exit] :  ";
				cin >> id;  // 不存在，讀新的
			}  // else
		}  // while

		return id;
	}  // select

	void shortestpath( map< string, vector<Receive> > &mainlist, string filename, float &number ) {
		printselect( mainlist );
		string id = select( mainlist );
		if( id == "0" )  return;
		while( id != "0" ) {
			// 先找連通數 
			vector<string> v ; // 堆疊用 
			vector<Receive> temp;
			map< string, vector<Receive> > path ; // 紀錄有誰 
			map< string, string > tra ; // 走過
			auto find = mainlist.find( id );
			temp = mainlist[id];
			path[id] = vector<Receive>(temp.begin(), temp.end());
			tra[id] = id ;
			v.push_back( id );
			int c = 0 ;
			while ( ! v.empty() ) {
			    string top = v[v.size()-1] ; // getTop
			    auto find = mainlist.find(top) ;
			    bool untraved = false ; 
			    // 是否還有未走過的 
				for( int i = 0 ; i < find->second.size() ; i++ ) {
				    auto f = tra.find( find->second[i].ID ) ;
				    if( f != tra.end() )  continue;
				    else  {
				        // 找到一個就可以跳出迴圈 
				        untraved = true ;
				        tra[find->second[i].ID] = find->second[i].ID ;
				        v.push_back( string(find->second[i].ID) ) ;
				        temp = mainlist[string(find->second[i].ID)];
				        path[string(find->second[i].ID)] = vector<Receive>(temp.begin(), temp.end());
						break ;
					}  // else
				}  // for
				    
				// 沒有未走過的 
				if ( ! untraved ) v.pop_back() ;
			}  // while

			// 初始化 edgeWeight100000.0
			float edgeWeight[path.size()][path.size()];
			for (int i = 0; i < path.size(); i++) {
		        for (int j = 0; j < path.size(); j++) {
		            edgeWeight[i][j] = 100000.0;
		        }  // for
		    }  // for
		    
		    // 將 path 轉換為 edgeWeight 
		    for ( auto it = path.begin(); it != path.end(); ++it ) {			    
			    for( int i = 0 ; i < it->second.size() ; i++ ) {
			    	edgeWeight[getIndex(it->first, path)][getIndex(it->second[i].ID, path)] = it->second[i].weight ;
				}  // for
			}  // for
			int size = path.size(); // 點的總數量
		    float weight[size]; // 存從起點到每個頂點的最小權重
		    bool vertexSet[size]; // 紀錄點是否已被加入集合中
		    string vertex = id; // 現在設為起點id 
		    float cheapest;
			vector <Receive> load;
			load.clear();
		    // 初始化 weight 和 vertexSet
		    for (int i = 0; i < size; i++) {
		        weight[i] = 100000.0; // 初始化為無窮大
		        vertexSet[i] = false;
		    }  // for
		
		    weight[getIndex(id, path)] = 0; // 起點的權重為 0
		
		    do {
		        vertexSet[getIndex(vertex, path)] = true; // 將當前點加入集合中
		
		        // 更新與當前頂點相鄰的頂點的權重
		        for (int u = 0; u < size; u++) {
		            if ( !vertexSet[u] ) {
		                // 更新權重，取最小值
		                weight[u] = min(weight[u], weight[getIndex(vertex, path)] + edgeWeight[getIndex(vertex, path)][u]);
		            }  // if
		        }  // for
		
		        cheapest = 100000.0;
		
		        // 找出集合外具有最小權重的頂點
		        for (int u = 0; u < size; u++) {
		            if (!vertexSet[u] && weight[u] < cheapest) {
		                vertex = getName(u, path);
		                cheapest = weight[u];
		            }  // if
		        }  // for
		    } while (cheapest < 100000.0 );
        	// 輸出最短路徑
        	
        	int num = 0;
        	for ( auto it = path.begin(); it != path.end(); ++it ) {			    
			    Receive temp;
			    temp.ID = it->first;
			    temp.weight = weight[num];
			    load.push_back( temp );
				num++;	
			}  // for
        	print( load, filename, number);

			//  可重複輸入學號 
			printselect( mainlist );
			id = select( mainlist );	    	
		}  // while
	}  // shortestpath
	int getIndex(string name, map<string, vector<Receive>>& path) {
	    int index = 0;
	    for (const auto& kv : path) {
	        if (kv.first == name) {
	            return index;
	        }  // if
	        index++;
	    }  // fpr
	    return -1; // 找不到
	}  // getIndex
	
	string getName(int index, map<string, vector<Receive>>& path) {
	    int i = 0;
	    for (const auto& kv : path) {
	        if (i == index) {
	            return kv.first;
	        }  // if
	        i++;
	    }  // for
	    return ""; // 找不到
	}  // getName
	
	void print( vector <Receive> load, string filename, float &number ) {
//		sort( load.begin(), load.end(), compareStruct );
		sort( load.begin(), load.end(), compareWeight );
	    stringstream ss;
    	ss << number;
    	string str = ss.str();
		string name = "pairs" + filename + "_" ;
		name = name + str + ".ds";
		ofstream file( name, ios::app );
		if ( file.is_open() ) {
			for( int i = 0 ; i < load.size() ; i++ ) {
				if( i == 0 ) {
		    		file << "\norigin: " << load[i].ID ;
				}  // if
				else {
					file << "(" << setw(2) << i << ") \t" << load[i].ID << ", " << load[i].weight << "\t"; 
				}  // else
				if( i%8 == 0 || i == load.size()-1 )  file << "\n";
			}  // for

		    
		}  // if
	}  // print
	
};

int main() {
	int run = 0;
	int size = 0 ;
    int command = 0 ;
    float num = 0.0 ;
    vector<Data> data ;
    string filename ;
    Readfile r ;
	Adjacencylist a ;
	ConnectionCount c ;
	Dijkstra d;
	float number = 0.0 ;
	map< string, vector<Receive> > list;
	bool one = false;
    while ( run == 0 ) {
    	bool functionisnum = true;
		string function;
		int command = 0;
    	cout << endl << "******* Graph data applications ******" ;
        cout << endl << "* [Any other key: QUIT]              *" ;
        cout << endl << "* 0. Create adjacency lists          *" ;
        cout << endl << "* 1. Build connected components      *" ;
        cout << endl << "* 2. Find shortest paths by Dijkstra *" ;
        cout << endl << "************************************** *" ;
        cout << endl << "Input a choice(0, 1, 2) [Any other key: QUIT]:" ;
	    cin >> function ;	  
		for ( int f = 0; f < function.size(); f++ ) {  // 判斷讀入的字串是否為數字 
        	int temp = (int)function[f] ;
        	if (temp >= 48 && temp <= 57) {
        		continue ;
        	}  // if
        	else  functionisnum = false ;
    	}  // for
    
    	if ( functionisnum )  command = atoi(function.c_str()) ;  // 字串轉int
    	if ( functionisnum && command == 0 )  {
    		data.clear() ;
    		list.clear() ;
    		filename = "" ;
    		number = 0.0;
    		one = false;
	    	if ( r.binisexist( data, filename, number ) ) {
				a.runall( data, filename, list, number ) ;
			}  // if
    	}  // if
    	
    	else if ( functionisnum && command == 1 ) {
	    	if ( data.size() != 0 ) { 
	    		one = true;
	    	    c.count( list, filename, number ) ;		    
	        }  // if
	        else  cout << "### There is no graph and choose 0 first. ###" << endl ;

	    }  // else if
		else if ( functionisnum && command == 2 ) {
			if ( data.size() != 0 && one ) { 
				d.shortestpath( list, filename, number );
	        }  // if
	        else if ( data.size() != 0 && !one )  cout << "### Choose 1 to find connected components. ###" << endl ;
	        else  cout << "### There is no graph and choose 0 first. ###" << endl ;
		}  // else if

		else  run++;
	}  // while
		
	system( "pause" ) ;
  	return 0 ;
}  // main
