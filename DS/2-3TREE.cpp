#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <stack>
#include <cstdlib> // atoi, system
using namespace std;
class Data{
	public:
	int graduate;  // �W�Ǧ~�ײ��~�ͼ�
	string sn;  // �ǮզW��
	string dn;  // ��t�W��
	string dayornight;  // �鶡�A�i�קO
	string level;  // ���ŧO
	int num;  // ��ƽs�� 
};

class Tree23{
	public:
	vector<Data> same[3];
	int count; // ��ȼƶq
	Tree23 *child[3];
	Tree23 *p;
	
	Tree23() {
        same[0].clear();
        same[1].clear();
        same[2].clear();
        count = 1;
        child[0] = NULL;
        child[1] = NULL;
        child[2] = NULL;
        p = NULL;

    }  // Tree23()
};
typedef Tree23 *ptr;
stack<ptr> parent ;  // �O����`�I
class tree23{
	public:
	ptr Root; 
	 
	// �s�W�`�I 
	ptr neww( Data data ) {
	    ptr root;
	    root = new Tree23;
		root->same[0].push_back( data );
	    root->child[0] = NULL;
	    root->child[1] = NULL;
	    root->child[2] = NULL;
	    root->count = 1;
	    root->p = NULL;
	 
	    return root;
	}  // neww()
			
    void split( ptr &node , ptr &head) {
    	// �Ȧs ��̤j�M�̤p�ƻs�U�� 
    	node->count--;
		ptr t = new Tree23;
		t->same[0].push_back( node->same[0][0] ) ;
		t->same[0] = node->same[0];
		t->child[0] = node->child[0] ;
		t->child[2] = node->child[1] ;
		t->child[1] = NULL ;
		t->count = 1;
		t->p = NULL ; 
					
						
		ptr t2 = new Tree23;
		t2->same[0].push_back( node->same[2][0] ) ;
		t2->same[0] = node->same[2];
		t2->child[0] = node->p ;
		t2->child[1] = NULL ;
		t2->child[2] = node->child[2] ;
		t2->p = NULL ;
		t2->count = 1;
		ptr new_root = NULL;
		//�S�����`�I
	    // �b�ڪ���m �إ߷s���� 
		if( parent.size() == 0 ) { 
			new_root = new Tree23 ;
			new_root->count = 1;
			new_root->same[0].push_back( node->same[1][0] );
			new_root->same[0] = node->same[1];
			// ��p�ı��W�h 
			new_root->child[0] = t ;
			new_root->child[2] = t2 ;
			new_root->child[1] = NULL ;	
			new_root->p = NULL ;
			
			// ���n�F��^�h	
			node = new_root ;
			head = node;
			
			return;	
		}  // if
		
		// �����`�I ���ݲ{�b�i�Ӫ��`�I�O�b���������� 
		else {
			new_root = parent.top();
			// ���亡�F�n���� 
			if( node == parent.top()->child[0] ) {  
				// ���� 
				if( parent.top()->count == 1 ) {
					vector<Data> d = parent.top()->same[0];
					parent.top()->same[0].clear();
					parent.top()->same[0] = node->same[1];
					parent.top()->same[1] = d;
				}  // if
				else{
					vector<Data> d = parent.top()->same[0];
					vector<Data> dd = parent.top()->same[1];
					parent.top()->same[0].clear();
					parent.top()->same[1].clear();
					parent.top()->same[0] = node->same[1];
					parent.top()->same[1] = d;
					parent.top()->same[2] = dd;
				}  // else

				// ���`�I���p�ĬO�X�� 
				if( parent.top()->child[1] == NULL ){ 
					new_root->child[0] = t;
					new_root->child[1] = t2;										
				}  // if
				else { 	
					new_root->p = new_root->child[1];
					new_root->child[0] = t;
					new_root->child[1] = t2;											 
				}  // else
															 
			}  //  if
			
			// �O�������F�ҥH�����Ӥ��� 
			else if( node == parent.top()->child[1] ) {
				// �񤤶� 
				// ���`�I���p�Ĥ@�w�T��

				vector<Data> dd = parent.top()->same[1];
				parent.top()->same[1].clear();
				parent.top()->same[1] = node->same[1];
				parent.top()->same[2] = dd;
  					
				new_root->p = t2;
				new_root->child[1] = t;
							 				
			}  // else if
			// �O�k�亡�F�ҥH�k��Ӥ��� 
			else if( node == parent.top()->child[2] ) {
				// �n���W�񪺸`�I���ӭn�񪨪������Ӧ�m 
				if( parent.top()->count == 1 )
					parent.top()->same[1] = node->same[1];
				else
					parent.top()->same[2] = node->same[1];
				// ���`�I���p�ĬO�X�� 
				if( parent.top()->child[1] == NULL ) { 
					new_root->child[1] = t;
					new_root->child[2] = t2;										
				}  // if
				else { 	
					new_root->p = t;
					new_root->child[2] = t2;												 
				}  // else

			}  // else if
			
			new_root->count++;
			// �{�b�o�Ӥ��`�I�B�z�n�F�ҥH�n������ 
			parent.pop();
			// �ݲ{�b�٬O������ �O���ܭn�~�����
			if( new_root->count == 3 )	{
				split( new_root ,head );	
			}  // if									
				
		}  // else 

    }  // Spilt()
    
	void insert( Data data, ptr &root, ptr &head ) {  
	    if( root == NULL ) {
	    	root = neww( data );
	    	return;
		}  // if
        	
	    if( root->count == 1 ) {  
	    	if( root->child[0] == NULL && root->child[1] == NULL && root->child[2] == NULL ) {
	    		if( data.sn == root->same[0][0].sn ) {
	    			root->same[0].push_back( data );
				}  // if
	    		else if ( data.sn < root->same[0][0].sn ) { 
	    			root->same[1] = root->same[0];
					root->same[0].clear();
					root->same[0].push_back( data );
					root->count = 2;
				}  // else if
				else  {  
					root->same[1].push_back( data );
					root->count = 2;
				}  // else
	    		return;
			}  // if
			// �٦��p���~�򻼰j
	    	else {
	    		
		    	if( data.sn < root->same[0][0].sn ) {
		    		parent.push( root );
		    		insert( data, root->child[0] , head);
				}  // if
		            
				else if( data.sn > root->same[0][0].sn ) {
					parent.push( root );
					insert( data, root->child[2] , head);
				}  // else if
					
		        else {
		            root->same[0].push_back( data );
		            return;
		        }  // else	
			}  // if

	        return;
	    }  // if
		else if( root->count == 2 ) {
			// �쩳�F ����i�h�n���� 
			if( root->child[0] == NULL && root->child[1] == NULL && root->child[2] == NULL ) {
				if( data.sn == root->same[0][0].sn ) {  // �ۦP���
		       		root->same[0].push_back( data );
		        	return;
		        }  // if
		        else if( data.sn == root->same[1][0].sn ) {  // �ۦP���
		        	root->same[1].push_back( data );
		        	return;
		        }  // else if
		        else if( data.sn < root->same[0][0].sn ) {
		        	vector<Data> d = root->same[0];
		        	root->same[0].clear();
		        	root->same[0].push_back( data );
		        	vector<Data> d2 = root->same[1];
		        	root->same[1].clear();
		        	root->same[1] = d;
		        	root->same[2].clear();
		        	root->same[2] = d2;
		        	root->count++;
				}  // else if
				else if( data.sn < root->same[1][0].sn ) {
		        	vector<Data> d2 = root->same[1];
		        	root->same[1].clear();
		        	root->same[1].push_back( data );
		        	root->same[2].clear();
		        	root->same[2] = d2;

		        	root->count++;
				}  // else if
				else if( data.sn > root->same[1][0].sn ) {
		        	root->same[2].push_back( data );
		        	root->count++;
				}  // else if
				
			}  // if
			else {  

				if( data.sn == root->same[0][0].sn ) {  // �ۦP���
		           root->same[0].push_back( data );
		           return;
		        }  // if
		        else if( data.sn == root->same[1][0].sn ) {  // �ۦP���
		           root->same[1].push_back( data );
		           return;
		        }  // else if
		        // �٨S�쩳�~�򻼰j 
				else if( data.sn < root->same[0][0].sn ) {
					parent.push( root );
					insert( data, root->child[0] , head);
				}  // else if
				else if( data.sn < root->same[1][0].sn ) {
					parent.push( root );
					insert( data, root->child[1] , head);
				}  // else if
					
				else if( data.sn > root->same[1][0].sn ) {
					parent.push( root );
					insert( data, root->child[2] , head);
				}  // else if
					
			}  // else
			
	    }  // else if
	    
	    // ���F 
	    if( root->count == 3 ) {
	    	split( root, head );
		}  // if
	    
	    return;
	}  // Insert

	int Height( ptr root ) {
	    if( root == NULL ) {
	        return 0;
	    }  // if
	    
	    else {
	        int left = Height( root->child[0] );
	        int mid = Height( root->child[1] );
	        int right = Height( root->child[2] );
	        return 1 + max( max( left, mid ), right );
	    }  // else
	}  // Height()

	int countNodes( ptr root) {
	    if( root == NULL ) {
	        return 0;
	    }  // if
	    else {
	        int count = 1;
	        count += countNodes( root->child[0] );
	        count += countNodes( root->child[2] );
	        
	        // ���T�Ӹ`�I 
	        if ( root->child[0] != NULL && root->child[1] != NULL && root->child[2] != NULL ) {
	            count += countNodes( root->child[1] );
	        }  // if
	        return count;
	    }  // else
	    
	}  // countNodes()

};


class NodeA {
	// AVL tree Node
    public:
    vector<Data> same;
    int height = 0 ;
    NodeA *next = NULL ;
    NodeA *left = NULL ;
    NodeA *right = NULL ;
};
typedef NodeA *Avl;
class AvlTree {
    public :		
	int get_height( Avl kaku ) {
		// �D���`�I���� 
        if( kaku == NULL ) return 0;
        return kaku->height;
    }  // get_height()
    
    int get_max( int left, int right ) { 
    	if ( left > right ) return left ;
    	return right ;
	}  // get_max()
	
	int get_balance( Avl temp, int left, int right ) {
		// ���ūY�� 
		if ( temp == NULL ) return 0 ;
		else return left - right ;
	}  // get_balance()

    Avl right_rotate( Avl kaku ) {
    	// �k�� 
        Avl a = kaku ;
        Avl b = kaku->left ;
        a->left = b->right ;
        b->right = a ;
        a->height = max( get_height( a->left ), get_height( a->right ) ) + 1 ;
        b->height = max( get_height( b->left ), get_height( b->right ) ) + 1 ;
        return b ;
    }  // right_rotate()

    Avl left_rotate( Avl kaku ) {
    	// ���� 
        Avl a = kaku ;
        Avl b = kaku->right ;
        a->right = b->left ;
        b->left = a ;
        a->height = max( get_height( a->left ), get_height( a->right ) ) + 1 ;
        b->height = max( get_height( b->left ), get_height( b->right ) ) + 1 ;
        return b ;
    }  // left_rotate()
    
    Avl check_balance( int balance, Data data, Avl my_avl ) {
    	// �ˬd���ūY�� -> ���� 
        if( balance >= 2 && data.graduate < my_avl->left->same[0].graduate ) {
        	// LL 
            return right_rotate( my_avl );
        }  // if
        else if( balance <= -2 && data.graduate > my_avl->right->same[0].graduate ) {
        	// RR
            return left_rotate( my_avl );
        }  // else if
        else if( balance >= 2 && data.graduate > my_avl->left->same[0].graduate ) {
        	// LR
        	my_avl->left = left_rotate( my_avl->left ) ;
            return right_rotate( my_avl );
        }  // else if
        else if( balance <= -2 && data.graduate < my_avl->right->same[0].graduate ) {
        	// RL
            my_avl->right = right_rotate( my_avl->right ) ;
            return left_rotate( my_avl );
        }  // else if
		return NULL ;	// �S��������� 
	}  // check_balance()
    
    Avl insert_avl( Avl my_avl, Data data ) { 
    	if ( my_avl == NULL ) {
    		// ��� �s�W�`�I 
    		my_avl = new NodeA ;
    		my_avl->same.push_back( data );
            my_avl->height = 1 ;
            return my_avl ;
		} // if
		else if ( data.graduate < my_avl->same[0].graduate )  my_avl->left = insert_avl( my_avl->left, data ) ; // ��Ƥ�`�I�p ���� 
		else if ( data.graduate > my_avl->same[0].graduate )  my_avl->right = insert_avl( my_avl->right, data ) ; // ��Ƥ�`�I�j ���k 
		else {
			my_avl->same.push_back( data );
		}  // else

		my_avl->height = get_max( get_height( my_avl->left ), get_height( my_avl->right ) ) + 1 ;
		// �P�_�O�_�ݭn���� 
        Avl b = check_balance( get_balance( my_avl, get_height( my_avl->left ), get_height( my_avl->right ) ), data, my_avl ) ;
        if ( b != NULL ) return b ;
		return my_avl ;
	}  // insert_avl()
	
	void built_avl( vector<Data> data, Avl &z ) {
		// build AVL tree
		for( int i = 0 ; i < data.size() ; i++ ) {
          z = insert_avl( z, data[i] ) ;
		}  // for
	}  // built_avl()
	
	int my_height ( Avl wing ) {
    	// �p��� 
    	Avl temp ;
    	temp = wing ;
    	int left = 0, right = 0, h = 0 ;
    	
    	if ( temp != NULL ) {
    		h++ ;
    		left = my_height( temp->left ) ; // ���j 
    		right = my_height( temp->right ) ; // ���j 
		} // if
		
		if ( left > right ) return left + h ; // �^�ǳ̤j�����| 
		else if ( left <= right ) return right + h ;
	}  // my_height()
	
	int my_node( Avl wing ) {
    	// �p��`�I�� 
    	int left = 0, right = 0, h = 0 ;
    	if ( wing == NULL ) return 0 ;
    	if ( wing != NULL ) {
    		h++ ;
    		left = my_node( wing->left ) ; // ���j 
    		right = my_node( wing->right ) ; // ���j 
		} // if
		
		return right + left + 1 ;
	}  // my_node()
	
	void print_result( Avl z ) {
		// �L�X���G 
		cout << "Tree height = " << my_height( z ) << endl ;
		cout << "Number of nodes = " << my_node ( z ) << endl ;
		Avl temp = z ;
		int x = 1 ;
		for( int i = 0 ; i < temp->same.size() ; i++ ) {
			cout << x << ": [ " << temp->same[i].num << " ] " << temp->same[i].sn << ", " << temp->same[i].dn << ", ";
			cout << temp->same[i].dayornight << ", " << temp->same[i].level << ", " << temp->same[i].graduate << "\n";
			x++;
		}  // for
	}  // print_result()
	void searchavl( Avl tree, int &x, int &k, int &j ) {
		if( tree == NULL || k <= 0 ) 
			return;
		
		searchavl( tree->right, x, k, j );
		if( k > 0 ) {
			for( int i = 0 ; i < tree->same.size() ; i++ ) {
				cout << x << ": [ " << tree->same[i].num << " ] " << tree->same[i].sn << ", " << tree->same[i].dn << ", ";
				cout << tree->same[i].dayornight << ", " << tree->same[i].level << ", " << tree->same[i].graduate << "\n";
				x++;
				k--;
			}  // for
			
		}  // if

//		if( k <= 0 )  return;
		searchavl( tree->left, x, k, j );
		 
	}  // search()
	/* 
	void print( Avl tree, int k, vector<Data> data ) {
		stack<int> num;
		search( tree, num );
		int i = 0;
		int x = 0;
		while( i < k ) {
			if( num.size() == 0 )  break;
			cout << x+1 << ": [ " << data[num.top()-1].num << " ] " << data[num.top()-1].sn << ", " << data[num.top()-1].dn << ", ";
			cout << data[num.top()-1].dayornight << ", " << data[num.top()-1].level << ", " << data[num.top()-1].graduate << "\n";
			x++;
			int now = num.top()-1;
			num.pop();
			if( num.size() > 0 && data[now].graduate != data[num.top()-1].graduate )
				i++;
			
		}  // while
		
	}  // print()
	*/ 
};

bool read( vector<Data> &data, string &filename ) {
	bool success = false;
	fstream file;
	int run1 = 0;
	
	string input;
	while ( run1 == 0 ) {
		cout << "\n" << "Input a file number ([0] Quit): ";
    	cin >> filename;
    	if ( filename.size() == 1 && filename[0] == '0' )  break;
    	else {
    		input = "\0" ;
			input = input + "input";
			input = input + filename;
			input = input + ".txt";
        		
			file.open( input.c_str() );
		}  // else
		
		if( file.is_open() == true ) {  // �T�{�b�P�ӥؿ��U�O�_�s�b�o���ɮ� 
			success = true;
			run1++;
			string line;
			getline( file, line );  // ���D 
			getline( file, line );  // ���D 
			getline( file, line );  // ���D 
			int n = 1;
			while ( getline( file, line ) ) {  // ���s�U�Ҧ����Χ����F�� 
				int tabb = 0;
				string graduateee = "\0";
				string t = "\0";
				Data temp;
				for( int i = 0 ; i < line.size() ; i++ ) {
					if( line[i] == '\t' )  tabb++;
					if( tabb == 1 && line[i] != '\t' ) {
						temp.sn = temp.sn + line[i];
					} // if 
					if( tabb == 3 && line[i] != '\t' ) {
						temp.dn = temp.dn + line[i];
					}  // if
					if( tabb == 4 && line[i] != '\t' ) {
						temp.dayornight = temp.dayornight + line[i];
					}  // if
					if( tabb == 5 && line[i] != '\t' )  {
						temp.level = temp.level +line[i];
					}  // if
					if ( tabb == 8 && line[i] != '\t' ) {  // ���~�ͼ� 
						if ( line[i] != ',' ) {
							if ( line[i] != '\"' ) {
								graduateee = graduateee + line[i] ;
							}  // if
						}  // if
					}  // if
				}  // for
				temp.graduate = atoi( graduateee.c_str() );
				temp.num = n;  // ��ƽs�� 
				n++;
				data.push_back( temp );
				
			}  // while
			//cout << data.size();
			file.close();
		}  // if
     			
		else if ( file.is_open() != true ){ 
     		cout << "\n" << "###" << input << " does not exist! ###" << "\n";  // ���s�b 
    	}  // else 
    		    		
	}  // while
	return success;
	
}  // read()
void bubblesort( vector<Data> &data ){
	for( int i = data.size()-1 ; i > 0 ; i-- ) {  
		for( int j = 0 ; j < i ; j++ ) {
			if( data[j+1].sn > data[j].sn ) {
				swap( data[j], data[j+1] );	
			}  // if
		}  // for
	}  // for
}  // bubblesort()
int main() {
	int run = 0;
	vector<Data> data;
	tree23 tree;
	AvlTree avl;
	Avl treeavl;
    while ( run == 0 ) {
    	
    	bool functionisnum = true;
		string function;
		int command = 0;
		
		string filename;
    	cout << "\n" << "*** Search Tree Utilities **" << "\n" << "* 0. QUIT                  *" << "\n";
    	cout << "* 1. Build 2-3 tree        *" << "\n" ;
		cout << "* 2. Build AVL tree        *" << "\n";
		cout << "* 3. Top-K search on AVL tree *" << "\n";
		cout << "****************************" << "\n";
		cout << "\n" << "Input a choice( 0, 1, 2, 3 ):";
		cin >> function;	  
		for (int f = 0; f < function.size(); f++) {  // �P�_Ū�J���r��O�_���Ʀr 
        	int temp = (int)function[f];
        	if (temp >= 48 && temp <= 57) {
        		continue;
        	}  // if
        	else  functionisnum = false;
    	}  // for
    
    	if ( functionisnum )  command = atoi(function.c_str());  // �r����int
    	if ( functionisnum && command == 0 )  run++;
    	
    	else if ( functionisnum && command == 1 ) {
    		data.clear();
 			if( read( data, filename ) )  {
				tree.Root = NULL;

				for ( int i = 0 ; i < data.size() ; i++ ) {
					while( !parent.empty() ) parent.pop();
					tree.insert( data[i], tree.Root , tree.Root);
				}  // for
				cout << "2-3 tree height = " << tree.Height( tree.Root ) << "\n";
				cout << "Number of node = " << tree.countNodes( tree.Root ) << "\n";
				int number = 1;
				vector<Data> sort;
				for( int i = 0 ; i < 2 ; i++ ) {
					for( int j = 0 ; j < tree.Root->same[i].size() ; j++ )
						sort.push_back( tree.Root->same[i][j] );	
				}  // for
				bubblesort( sort );
				for( int j = 0 ; j < sort.size() ; j++ ) {
					cout << number << ": [ " << sort[j].num << " ] " << sort[j].sn << ", " << sort[j].dn << ", ";
					cout << sort[j].dayornight << ", " << sort[j].level << ", " << sort[j].graduate << "\n";
					number++;	
				}  // for
			
			}  //if

	    }  // else if
		else if ( functionisnum && command == 2 ) {
			if ( data.size() != 0 ) {
				treeavl = NULL;
	
				avl.built_avl( data, treeavl ) ;  // �� AVL tree 
    			avl.print_result( treeavl ) ;  // �L�X���G 
				
			}  // if
			else  cout << "### Choose 1 first. ###\n";

		}  // else if
		
		
		else if ( functionisnum && command == 3 ) {
			if ( data.size() != 0 ) {
					cout << "\nEnter the value of K in [" << data[0].num << "," << data.size() << "]: ";
					string k;
					cin >> k;
					bool isnum = true;
					int kk = 0;	  
					for (int f = 0; f < k.size(); f++) {  // �P�_Ū�J���r��O�_���Ʀr 
			        	int temp = (int)k[f];
			        	if (temp >= 48 && temp <= 57) {
			        		continue;
			        	}  // if
			        	else  isnum = false;
			    	}  // for
			    	
			    	if ( isnum != false )  {
						kk = atoi(k.c_str());  // �r����int
						cout << "\n";
						int x = 1;
						int j = 0;
						if ( kk >= 1 && kk <= data.size() ) {
							avl.searchavl( treeavl, x, kk, j );
						}  // if
			    	}  // if
				} // if
				else  cout << "### Choose 2 first. ###\n";
		}  // else if
		
		else  cout << "\nthis command does not exist";

	}  // while
}  // main

