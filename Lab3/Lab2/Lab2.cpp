#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <chrono>

using namespace std;

const int degree = 419;

struct LongNumber
{
	unsigned int* value;
	int size;
	string str_value;
public:
	void PrintParameters();
};

LongNumber* GetValue(string str) 
{
	LongNumber *result = new LongNumber;
	result->value = new unsigned int[str.length()];
	for (int i = str.length() - 1; i >= 0; i--)
	{
		char temp = str[i];
		int number = strtol(&temp, NULL, 2);
		result->value[str.length() - 1 - i] = number;
	}
	result->size = str.length();
	result->str_value = str;
	return result;
}

LongNumber* GetValue(unsigned int A[], int size)
{
	LongNumber *result = new LongNumber;
	result->value = A;
	result->size = size;
	return result;
}

void GetStringValue(LongNumber *A)
{
	string str = "";
	for (int i = 0; i < A->size; i++)
	{
		str.insert(0, to_string(A->value[i]));
	}
	A->str_value = str;
}

int GetFirstNotNullBit(unsigned int A[], int size)
{
	int counter = 0;
	while (A[size - 1 - counter] == 0)
	{
		counter++;
	}
	return size - counter - 1;
}

void LongNumber::PrintParameters() // No memory leak
{
	cout << "Parameters: " << endl;
	cout << "Size: " << this->size << endl;
	cout << "String representation: " << endl << this->str_value << endl;
}

void KillZeros(LongNumber *A) // No memory leak
{
	int k = GetFirstNotNullBit(A->value, A->size);
	unsigned int* C = new unsigned int[k + 1];
	for (int i = 0; i < k + 1; i++)
	{
		C[i] = A->value[i];
	}
	delete[] A->value;
	A->value = C;
	A->size = k + 1;
}

void AddZeros(LongNumber *A, int size) // No memory leak
{
	unsigned int *C = new unsigned int[size];
	for (int i = 0; i < A->size; i++)
	{
		C[i] = A->value[i];
	}
	for (int i = A->size; i < size; i++)
	{
		C[i] = 0;
	}
	delete A->value;
	A->value = C;
	A->size = size;
}

LongNumber* GetModule()
{
	string str = "";
	for (int i = 0; i <= degree; i++)
	{
		str.insert(0, "0");
	}
	str[degree - 419] = '1';
	str[degree - 21] = '1';
	str[degree - 14] = '1';
	str[degree - 1] = '1';
	str[degree - 0] = '1';
	LongNumber *result = GetValue(str);
	return result;
}

LongNumber* GetZero()
{
	string str = "";
	for (int i = 0; i < degree; i++)
	{
		str.insert(0, "0");
	}
	LongNumber *result = GetValue(str);
	return result;
}

LongNumber* GetOne()
{
	string str = "";
	for (int i = 0; i < degree; i++)
	{
		str.insert(0, "0");
	}
	str[str.length() - 1] = '1';
	LongNumber* result = GetValue(str);
	return result;
}

LongNumber* CopyLongNumber(LongNumber *A)
{
	LongNumber* B = new LongNumber;
	B->size = A->size;
	B->str_value = A->str_value;
	B->value = A->value;
	return B;
}

int LongCmp(LongNumber *A, LongNumber *B)
{
	int a_len = GetFirstNotNullBit(A->value, A->size);
	int b_len = GetFirstNotNullBit(B->value, B->size);
	if (a_len - b_len > 0) return 1;
	if (a_len - b_len < 0) return -1;
	int i = a_len;
	while (A->value[i] == B->value[i] && i >= 0)
	{
		i--;
	}
	if (i == -1)
	{
		return 0;
	}
	else
	{
		if (A->value[i] > B->value[i]) return 1;
		else return -1;
	}
}

LongNumber *mod = GetModule();

LongNumber* ShiftBitsToHigh(LongNumber *A, int index, bool selfculc)
{
	unsigned int* C = new unsigned int[A->size + index];
	for (int i = A->size + index - 1; i >= index; i--)
	{
		C[i] = A->value[i - index];
	}
	for (int i = 0; i <= index - 1; i++)
	{
		C[i] = 0;
	}
	LongNumber *B = GetValue(C, A->size + index);
	if (selfculc)
	{
		delete A;
		A = NULL;
		return B;
	}
	return B;
}

unsigned int* CopyArray(unsigned int arr[], int size)
{
	unsigned int* temp = new unsigned int[size];
	for (int i = 0; i < size; i++)
	{
		temp[i] = arr[i];
	}
	return temp;
}

unsigned int* LongShiftBitsToHigh(unsigned int A[], int size, int index, bool selfculc)
{
	unsigned int* C = new unsigned int[size + index];
	for (int i = size + index - 1; i >= index; i--)
	{
		C[i] = A[i - index];
	}
	for (int i = 0; i <= index - 1; i++)
	{
		C[i] = 0;
	}
	if (selfculc)
	{
		delete[] A;
		return C;
	}
	return C;
}

unsigned int* AddArray(unsigned int A[], unsigned int B[], int size_A, int size_B, bool selfculc)
{
	unsigned int* C = new unsigned int[size_A];
	for (int i = 0; i < size_B; i++)
	{
		C[i] = (A[i] + B[i]) % 2;
	}
	for (int i = size_B; i < size_A; i++)
	{
		C[i] = A[i];
	}
	if (selfculc)
	{
		free(A);
		return C;
	}
	return C;
}

// ----------------------- OPERATIONS ZONE  -----------------------

LongNumber* Addition(LongNumber *A, LongNumber *B, bool selfculc)
{
	int big = max(A->size, B->size);
	int small = min(A->size, B->size);
	unsigned int* C = new unsigned int[big];
	for (int i = 0; i < small; i++)
	{
		C[i] = (A->value[i] + B->value[i]) % 2;
	}
	if (big == A->size)
	{
		for (int i = small; i < big; i++)
		{
			C[i] = A->value[i];
		}
	}
	else
	{
		for (int i = small; i < big; i++)
		{
			C[i] = B->value[i];
		}
	}
	LongNumber *result = GetValue(C, big);
	return result;
}

LongNumber* TakeModule(LongNumber *A, bool selfculc)
{
	int k = GetFirstNotNullBit(mod->value, mod->size);
	LongNumber *R = CopyLongNumber(A);
	while (R->size >= mod->size)
	{
		LongNumber *C = new LongNumber;
		int t = GetFirstNotNullBit(R->value, R->size);
		C = ShiftBitsToHigh(mod, t - k, false);
		if (LongCmp(R, C) == -1)
		{
			t = t - 1;
			free(C);
			C = ShiftBitsToHigh(mod, t - k, false);
		}
		R = Addition(R, C, true);
		KillZeros(R);
		free(C);
	}
	AddZeros(R, degree);
	if (selfculc)
	{
		free(A);
		return R;
	}
	return R;
}

LongNumber* Multiplication(LongNumber *A, LongNumber *B, bool selfculc)
{
	int sz = 2 * max(A->size, B->size);
	unsigned int* C = new unsigned int[sz];
	fill(&C[0], &C[sz], 0);
	for (int i = 0; i < B->size; i++)
	{
		if (B->value[i] == 1)
		{
			unsigned int* temp = CopyArray(A->value, A->size);
			temp = LongShiftBitsToHigh(temp, B->size, i, true);
			C = AddArray(C, temp, sz, B->size + i, true);
			delete[] temp;
		}
	}
	LongNumber *result = GetValue(C, sz);
	KillZeros(result);
	result = TakeModule(result, true);
	return result;
}

LongNumber* Square(LongNumber *A, bool selfculc)
{
	unsigned int* C = new unsigned int[2 * A->size - 1];
	for (int i = 0; i < A->size - 1; i++)
	{
		C[2 * i] = A->value[i];
		C[2 * i + 1] = 0;
	}
	C[2 * A->size - 2] = A->value[A->size - 1];
	LongNumber *result = GetValue(C, 2 * A->size - 1);
	KillZeros(result);
	result = TakeModule(result, true);
	if (selfculc)
	{
		free(A);
		return result;
	}
	return result;
}

LongNumber* LongPower(LongNumber *A, LongNumber *N, bool selfculc)
{
	LongNumber *C = GetOne();
	LongNumber *temp = CopyLongNumber(A);
	for (int i = 0; i < N->size; i++)
	{
		if (N->value[i] == 1)
		{
			C = Multiplication(C, temp, true);
		}
		temp = Square(temp, true);
	}
	if (selfculc)
	{
		free(A);
		return C;
	}
	return C;
}

LongNumber* Trace(LongNumber *A)
{
	LongNumber *temp = CopyLongNumber(A);
	unsigned int *C = CopyArray(A->value, A->size);
	for (int i = 1; i < degree; i++)
	{
		temp = Square(temp, true);
		C = AddArray(C, temp->value, A->size, temp->size, true);
	}
	free(temp);
	LongNumber *result = GetValue(C, A->size);
	return result;
}

LongNumber* Reversed(LongNumber *A)
{
	LongNumber *temp = Square(A, false);
	LongNumber *result = GetOne();
	for (int i = 1; i < degree; i++)
	{
		result = Multiplication(result, temp, true);
		temp = Square(temp, true);
	}
	free(temp);
	return result;
}

// ----------------------- TEST ZONE  -----------------------

void Test1()
{
	string s_a;
	cout << "Insert number a: " << endl;
	cin >> s_a;

	string s_b;
	cout << "Insert number b: " << endl;
	cin >> s_b;

	string s_c;
	cout << "Insert number c: " << endl;
	cin >> s_c;

	LongNumber *a = GetValue(s_a);
	LongNumber *b = GetValue(s_b);
	LongNumber *c = GetValue(s_c);

	LongNumber *sum1 = Addition(a, b, false);
	LongNumber *leftResult = Multiplication(sum1, c, false);
	LongNumber *mul1 = Multiplication(b, c, false);
	LongNumber *mul2 = Multiplication(c, a, false);
	LongNumber *rightResult = Addition(mul1, mul2, false);

	GetStringValue(leftResult);
	GetStringValue(rightResult);

	leftResult->PrintParameters();
	rightResult->PrintParameters();
}

void Test2()
{
	string s_a;
	cout << "Insert number a: " << endl;
	cin >> s_a;
	LongNumber *a = GetValue(s_a);

	unsigned int *m = new unsigned int[degree];
	fill(&m[0], &m[degree], 1);
	LongNumber *deg = GetValue(m, degree);

	LongNumber *result = LongPower(a, deg, false);
	GetStringValue(result);
	result->PrintParameters();

}

// ----------------------- MAIN ZONE  -----------------------

int main()
{
	//Test1();
	//Test2();
	
	cout << "Module: " << endl;
	mod->PrintParameters();

	auto start = std::chrono::system_clock::now();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end - start;

	end = std::chrono::system_clock::now();
	diff = end - start;
	
	string s_A;
	cout << "Insert number A: " << endl;
	cin >> s_A;

	string s_B;
	cout << "Insert number B: " << endl;
	cin >> s_B;

	string s_N;
	cout << "Insert number N: " << endl;
	cin >> s_N;

	LongNumber *A = GetValue(s_A);
	LongNumber *B = GetValue(s_B);
	LongNumber *N = GetValue(s_N);
	
	cout << endl << "A + B: " << endl;
	start = std::chrono::system_clock::now();
	LongNumber *C = Addition(A, B, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	GetStringValue(C);
	C->PrintParameters();
	cout << "Time: " << diff.count() << " sec" << endl;
	
	cout << endl << "A * B: " << endl;
	start = std::chrono::system_clock::now();
	LongNumber *D = Multiplication(A, B, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	GetStringValue(D);
	D->PrintParameters();
	cout << "Time: " << diff.count() << " sec" << endl;
	
	cout << endl << "A ^ 2: " << endl;
	start = std::chrono::system_clock::now();
	LongNumber *E = Square(A, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	GetStringValue(E);
	E->PrintParameters();
	cout << "Time: " << diff.count() << " sec" << endl;

	cout << endl << "A ^ N: " << endl;
	start = std::chrono::system_clock::now();
	LongNumber *F = LongPower(A, N, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	GetStringValue(F);
	F->PrintParameters();
	cout << "Time: " << diff.count() << " sec" << endl;

	cout << endl << "Trace A: " << endl;
	start = std::chrono::system_clock::now();
	LongNumber *G = Trace(A);
	end = std::chrono::system_clock::now();
	diff = end - start;
	GetStringValue(G);
	G->PrintParameters();
	cout << "Time: " << diff.count() << " sec" << endl;
	
	cout << endl << "A ^ -1: " << endl;
	start = std::chrono::system_clock::now();
	LongNumber *H = Reversed(A);
	end = std::chrono::system_clock::now();
	diff = end - start;
	GetStringValue(H);
	H->PrintParameters();
	cout << "Time: " << diff.count() << " sec" << endl;

}
