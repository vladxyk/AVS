#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

int matrix(int M, int N, int K)
{
	int S, i, j;
	int **a, **b, **c;
	
	//if (N1 != M2){
	//	cout << "error!" << endl;
	//	return 0;
	//}


	a = new int*[M];
	for (i = 0; i < M; i++){
		a[i] = new int[N];
	}
//cout << "RAND_MAX = " << RAND_MAX << endl;
	for (i = 0; i < M; i++) {
		for (j = 0; j < N; j++) {
			a[i][j] = rand();
		}
	}
/*	
	for (int i = 0; i < M1; i++){
    	for (int j = 0; j < N1; j++){
      		cout << a[i][j] << " ";
		}
    	cout << "\n";
	}
*/

	b = new int*[N];
	for (i = 0; i < N; i++){
		b[i] = new int[K];
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < K; j++) {
			b[i][j] = rand();
		}
	}
/*
	for (int i = 0; i < M2; i++){
    	for (int j = 0; j < N2; j++){
      		cout << b[i][j] << " ";
		}
    	cout << "\n";
	}
*/

	c = new int*[M];
  	for (int i = 0; i < M; i++){
    	c[i] = new int[K];
	}

    for (int i = 0; i < N; i++){
		for (int j = 0; j < K; j++){
      		c[i][j] = 0;
      		for (int k = 0; k < N; k++)
        		c[i][j] += a[i][k] * b[k][j];
    	}
  	}
/*	
	cout << "dgemm" << "\n";
	for (int i = 0; i < M; i++){
    	for (int j = 0; j < K; j++){
      		cout << c[i][j] << " ";
		}
    	cout << "\n";
	}
*/

	return 0;
}

int main(int argc, char* argv[])
{
	clock_t t;
	cout << "enter total number of tests: ";
	int q, i;
	cin >> q;
	int M, N, K;
	float avt = 0;
	//float AE;
	//float RE;
	float *arr = new float[q];
	float *AE = new float[q];
	float *RE = new float[q];
	float *tp = new float[q];
{
M = 1000;
N = 1000;
//M2 = 1000;
K = 1000;
}

	for (i = 1; i < (q+1); i++){
		t = clock();
	/*
	cout<<"M1="; cin>>M1;
	cout<<"N1="; cin>>N1;
	cout<<"M2="; cin>>M2;
	cout<<"N2="; cin>>N2;
	*/
		matrix(M, N, K);
		t = clock() - t;
		cout << "clock ticks = " << t << ", seconds = " << ((float)t) / CLOCKS_PER_SEC << "\n";
		arr[i] = ((float)t)/ CLOCKS_PER_SEC;
		avt = avt + (((float)t)/ CLOCKS_PER_SEC);
	}

	avt = avt / q;
	cout << "\naverage time = " << avt << "\n\n";
	int q1 = 1;
	float AES = 0;
	float IC = M * K * (N + (N - 1));
	while (q1 <= q){
		//cout << avt << endl;
		//cout << arr[q1] << endl;
		AE[q1] = abs(avt - arr[q1]);
		RE[q1] = (AE[q1] / avt) * 100;
		tp[q1] = (IC/arr[q1]);
		cout << "AE[" << q1 << "] = " << AE[q1] << "	" << "RE[" << q1 << "] = " << RE[q1] << "%" << "	" << "TP[" << q1 << "] = " << tp[q1] << endl;
		AES = AES + pow(AE[q1], 2);
		q1++;
	}


	float D = AES / q;
	float b = sqrt(D);
	float re = (b / avt) * 100;
	cout <<"\nD = " << D << "	" << "ϭ = " << b << endl;
	float TP = IC/avt;
	cout << "TP = " << TP << " IPS" << endl;
	cout << "AE = " << b << " " << "RE = " << re << endl;
	ofstream fout(argv[1]); 

    fout << "PModel;Task;OpType;Opt;LNum;Timer;InsCount;AvTime;AbsErr;RelErr;Time;TaskPerf\n";
    fout << "AMD Ryzen 3 3200U with Radeon Vega Mobile;dgemm;int;none;10;clock()"; 
	fout << "  ;" << IC;
	fout << "  ;" << avt;
	fout << "  ;" << b;
	fout << "  ;" << re;
	fout << "  ;";
	fout << "  ;" << TP << endl;
	int q3=10;
	while (q3>0){
	fout << "AMD Ryzen 3 3200U with Radeon Vega Mobile;dgemm;int;none;10;clock()";
	fout << "  ;" << IC;
	fout << "  ;" << avt;
	fout << " ;" << AE[q3];
	fout << " ;" << RE[q3];
	fout << " ;" << arr[q3];
	fout << "  ;" << tp[q3] << endl;
	q3--;
	}
	fout.close();

delete [] arr;
return 0;
}
/*
void AE(float arr[i]){
	cout << arr[i] << endl;
}
*/
