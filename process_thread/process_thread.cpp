#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>

using namespace std;

void bubblesort( vector<int> &data ) {
	int n = data.size();
    bool swapped = false;
    for (int i = 0; i < n-1; i++) {
        swapped = false;
        for (int j = 0; j < n-i-1; j++) {
            if (data[j] > data[j + 1]) {
                swap(data[j], data[j + 1]);
                swapped = true;
            }  // if
        }  // for
        
        // �p�G�S���i��洫�A��ܤw�g���ǡA���e�����Ƨ�
        if (!swapped) break;
    }  // for
}  // bubble_sort()


void merge( vector<int> &merged, vector<int> &left, vector<int> &right) {
    merged.resize(left.size() + right.size());  // �T�O�X�֫ᦳ�������Ŷ�
    int i = 0, j = 0, k = 0;
    // �X�־ާ@�A�C����ܥ��k��ӼƲդ��̤p���Ʃ�J�X�᪺֫�Ʋդ�
    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) {
            merged[k++] = left[i++];
        }  // if
		else {
            merged[k++] = right[j++];
        }  // else
    }  // while
    // �N����Ѿl���ƾګ�����X�᪺֫�Ʋդ�
    while (i < left.size()) {
        merged[k++] = left[i++];
    }  // while
    // �N�k��Ѿl����?������X�᪺֫�Ʋդ�
    while (j < right.size()) {
        merged[k++] = right[j++];
    }  // while
}  // merge


bool read( vector<int> &data, string &filename ) {
	bool success = false;
	fstream file;
	int run1 = 0;
	string input = filename + ".txt";

	
	file.open( input.c_str() );

	if( file.is_open() == true ) {  // �T�{�b�P�ӥؿ��U�O�_�s�b�o���ɮ� 

		int number = 0;
 		success = true;
 			
		while (file >> number) {
           	data.push_back(number);
        }  // while
			
		file.close();
	}  // if
     			
	else if ( file.is_open() != true ){ 
     	cout << "Failed to open file.\n" ;
    }  // else 
    		    		

	
	return success;
}  // read()

void write( vector<int> &data, string filename, int sol, double cputime ) {
	ofstream file;
	file.open( filename + "_output" + to_string(sol) + ".txt" );
	file << "Sort :\n";
	for( int i = 0; i < data.size() ; i++ )
    	file << data[i] << "\n";
  	file << "CPU Time: "<< cputime << " s\n";
  	
  	auto now = chrono::system_clock::now();
  	time_t now_c = chrono::system_clock::to_time_t(now);
	tm* now_tm = gmtime(&now_c);
	now_tm->tm_hour = (now_tm->tm_hour + 8) % 24; // �[�W 8 �p�ɡA�ýT�O�ɶ��b24�p�ɤ�

	if (now_tm->tm_hour < 8) // �p�G�p�ɼƤp��8�A�h����[1
		now_tm->tm_mday += 1;

	char buffer[80];
	strftime (buffer, 80, "%Y-%m-%d %H:%M:%S", now_tm );
	file << "Output time: " << buffer << " +08:00\n";

	file.close();
} // write

void method1( vector<int> &data, string filename, int sol ) {
	auto start = chrono::high_resolution_clock::now();
	bubblesort( data );
	auto end = chrono::high_resolution_clock::now();

    // �p�����ɶ�
    chrono::duration<double> duration = end - start;
    double cputime = duration.count();

    //cout << "CPU Time: " << cputime << "\n" ;
    write( data, filename, sol, cputime );
}  // mission1

void method2( vector<int>& data, string filename, int sol, int k ) {
    int num = data.size() / k;  // ����k���@���X��
    int last = data.size() % k;
    vector<vector<int>> temp(k);  // k�� 
    int n = 0;
    int t = 0;
    while (n < k) {
        int put = 0;
        if (n != k - 1) {
        	while (put < num) {
                temp[n].push_back(data[t]);
                t++;
                put++;
            }  // while
        }  // if
		else {
        	while (put < num + last) {  //�ѤU��
                temp[n].push_back(data[t]);
                t++;
                put++;
            }  // while
        }    // else
        n++;
    }  // while

    n = 0;
    data.clear();
    auto start = chrono::high_resolution_clock::now();
    while (n < k) {
        bubblesort(temp[n]);
        n++;
    }  // while

    while (temp.size() > 1) {
        vector<int> merged;
        merge(merged, temp[0], temp[1]);
        temp.erase(temp.begin(), temp.begin() + 2);
        temp.push_back(merged);
    }  // while

    data = temp[0];

    auto end = chrono::high_resolution_clock::now();

    // �p�����ɶ�
    chrono::duration<double> duration = end - start;
    double cputime = duration.count();
    write(data, filename, sol, cputime);
    
} // method2

void method3(int* shared_memory, size_t size, string filename, int sol, int k) {
    // �N shared_memory ���V���O����϶������e�ƻs�� data �o�� vector ��
	vector<int> data(shared_memory, shared_memory + size); // Convert shared memory to vector

    int num = data.size() / k;  // ���� k ���@���X��
    int last = data.size() % k;
    vector<vector<int>> temp(k);  // k ��
    auto start = chrono::high_resolution_clock::now();

    // Create K processes for BubbleSort
    for (int i = 0; i < k; i++) {
        pid_t pid = fork();
        if (pid == 0) { // Child process
            int t = i * num;
            int num_to_sort = (i != k - 1) ? num : num + last;
            for (int j = 0; j < num_to_sort; ++j) {
                temp[i].push_back(data[t++]);
            }  // for
            bubblesort(temp[i]);
            // Write sorted data to shared memory
            for (size_t j = 0; j < temp[i].size(); ++j) {
                shared_memory[i * num + j] = temp[i][j];
            }  // for
            exit(0);
        }  // if
		else if (pid < 0) {  // Fork failed
            cerr << "Fork failed!" << endl;  // error message
            exit(1);
        }  // else if
    }  // for

    // Parent  waits  all children  finish
    for (int i = 0; i < k; i++) {
        wait(NULL);
    }  // for

    // Read sorted data from shared memory
    for (int i = 0; i < k; ++i) {
        int t = i * num;
        int num_to_sort = (i != k - 1) ? num : num + last;
        for (int j = 0; j < num_to_sort; ++j) {
            temp[i].push_back(shared_memory[t++]);
        }  // for
    }  // for

    // Merge sorted arrays
    while (temp.size() > 1) {
        vector<vector<int>> new_merged_data;
        for (size_t i = 0; i < temp.size(); i += 2) {
            if (i + 1 < temp.size()) {
                vector<int> merged;
                merge(merged, temp[i], temp[i + 1]);
                new_merged_data.push_back(merged);
            }  // if
			else {
                new_merged_data.push_back(temp[i]);
            }  // else
        }  // for
        temp = new_merged_data;
    }  // while

    // Assign sorted data to data vector
    data = temp[0];

    auto end = chrono::high_resolution_clock::now();
    // Calculate execution time
    chrono::duration<double> duration = end - start;
    double cputime = duration.count();

    // Write sorted data to file
    write(data, filename, sol, cputime);
}  // method3

void mergeThread(vector<int>& merged, vector<int>& left, vector<int>& right) {
    merge(merged, left, right);
}

void method4(vector<int>& data, string filename, int sol, int k) {
    int num = data.size() / k;  // ����k���@���X��
    int last = data.size() % k;
    vector<vector<int>> temp(k);  // k�� 
    int n = 0;
    int t = 0;
    while (n < k) {
        int put = 0;
        if (n != k - 1) {
            while (put < num) {
                temp[n].push_back(data[t]);
                t++;
                put++;
            }  // while
        }  // if
        else {
            while (put < num + last) {  //�ѤU��
                temp[n].push_back(data[t]);
                t++;
                put++;
            }  // while
        }    // else
        n++;
    }  // while

    n = 0;
    data.clear();
    auto start = chrono::high_resolution_clock::now();

    // �Ы�k�ӽu�{�i��Ƨ�
    vector<thread> threads(k);
    for (int i = 0; i < k; i++) {
        threads[i] = thread(bubblesort, ref(temp[i]));
    }  // for

    // ���ݩҦ��u�{����
    for (auto& th : threads) {
        th.join();
    }  // for

    while (temp.size() > 1) {
        vector<int> merged;
        // �зs��thread 
        thread merge_thread(mergeThread, ref(merged), ref(temp[0]), ref(temp[1]));
        merge_thread.join();
        temp.erase(temp.begin(), temp.begin() + 2);
        temp.push_back(merged);
    }  // while

    data = temp[0];

    auto end = chrono::high_resolution_clock::now();

    // �p�����ɶ�
    chrono::duration<double> duration = end - start;
    double cputime = duration.count();
    write(data, filename, sol, cputime);
}  // method4




int main() {
	while( true ) {
		vector<int> data;  
    	string filename;
    	int k = 0, sol = 0, num;
    	cout << "file name �п�J�ɮצW��\n";
    	cin >> filename ;
    	cout << "cut �п�J�n�����X��\n";
    	cin >> k;
    	if( k == 0 )  k = 1;
    	cout << "which method �п�J��k�s��:(��k1, ��k2,  ��k3, ��k4)\n";
    	cin >> sol;
    	if( read( data, filename ) ) {  // ��r�ɦ����\���} 
//    		cout << data.size() << "\n";
		}  // if
    	else  return 1;
    	if( sol == 1 ) {  // �����ϥ�bubble sort 
    		method1( data, filename, sol );
		}   // if
		else if( sol == 2 ) {
			method2( data, filename, sol, k );
		}  // else if
		else if( sol == 3 ) {
			// Create a shared memory
            int shared_memory_fd = shm_open("/shared_memory", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
            if (shared_memory_fd == -1) {
                cerr << "Failed to create shared memory" << endl;
                return 1;
            }  // if
            // set shared memory size
            if (ftruncate(shared_memory_fd, sizeof(data[0]) * data.size()) == -1) {
                cerr << "Failed to set the size of the shared memory" << endl;
                return 1;
            }  // if

            // Map the shared memory into the address space of the process
            int *shared_memory = (int *)mmap(NULL, sizeof(data[0]) * data.size(), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);
            if (shared_memory == MAP_FAILED) {
                cerr << "Failed to map the shared memory" << endl;
                return 1;
            }  // if

            // Copy data to shared memory
            for (size_t i = 0; i < data.size(); ++i) {
                shared_memory[i] = data[i];
            }  // if

            method3(shared_memory, data.size(), filename, sol, k);

            // Copy data back from shared memory
            for (size_t i = 0; i < data.size(); ++i) {
                data[i] = shared_memory[i];
            }  // if

            // Unmap the shared memory
            if (munmap(shared_memory, sizeof(data[0]) * data.size()) == -1) {
                cerr << "Failed to unmap the shared memory" << endl;
                return 1;
            }  // if

            // Close the shared memory file descriptor
            if (close(shared_memory_fd) == -1) {
                cerr << "Failed to close the shared memory file descriptor" << endl;
                return 1;
            }  // if

            // Unlink the shared memory object
            if (shm_unlink("/shared_memory") == -1) {
                cerr << "Failed to unlink the shared memory object" << endl;
                return 1;
            }  // if
		}  // else if
		else if ( sol == 4 ) {
			method4( data, filename, sol, k );
		}  // else if
	}  // while
 
}  // main()
