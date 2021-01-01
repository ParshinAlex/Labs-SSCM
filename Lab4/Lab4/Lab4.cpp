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
	for (int i = 0; i < str.length(); i++)
	{
		char temp = str[i];
		int number = strtol(&temp, NULL, 2);
		result->value[i] = number;
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
		str.insert(str.length(), to_string(A->value[i]));
	}
	A->str_value = str;
}

int GetFirstNotNullBit(unsigned int A[], int size)
{
	int counter = 0;
	while (A[counter] == 0)
	{
		counter++;
	}
	return counter;
}

void LongNumber::PrintParameters() // No memory leak
{
	cout << "Parameters: " << endl;
	cout << "Size: " << this->size << endl;
	cout << "String representation: " << endl << this->str_value << endl;
}

LongNumber* GetModule()
{
	string str = "";
	unsigned int *arr = new unsigned int[degree + 1];
	fill(&arr[0], &arr[degree], 0);
	arr[0] = 1;
	arr[degree - 21] = 1;
	arr[degree - 14] = 1;
	arr[degree - 1] = 1;
	arr[degree] = 1;
	LongNumber *result = GetValue(arr, degree + 1);
	GetStringValue(result);
	return result;
}

LongNumber *mod = GetModule();

LongNumber* GetZero()
{
	unsigned int* temp = new unsigned int[degree];
	fill(&temp[0], &temp[degree], 0);
	LongNumber* result = GetValue(temp, degree);
	return result;
}

LongNumber* GetOne()
{
	unsigned int* temp = new unsigned int[degree];
	fill(&temp[0], &temp[degree], 1);
	LongNumber* result = GetValue(temp, degree);
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
	if (a_len - b_len > 0) return -1;
	if (a_len - b_len < 0) return 1;
	int i = a_len;
	while (A->value[i] == B->value[i] && i < A->size)
	{
		i++;
	}
	if (i == A->size)
	{
		return 0;
	}
	else
	{
		if (A->value[i] > B->value[i]) return 1;
		else return -1;
	}
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

unsigned int* MoveLeft(unsigned int A[], int size, int index, bool selfculc)
{
	unsigned int *buffer = new unsigned int[index];
	unsigned int* temp = CopyArray(A, size);
	for (int i = 0; i < index; i++)
	{
		buffer[i] = A[i];
	}
	for (int i = 0; i < size - index; i++)
	{
		temp[i] = A[i + index];
	}
	for (int i = 0; i < index; i++)
	{
		temp[size - index + i] = buffer[i];
	}
	delete[] buffer;
	if (selfculc)
	{
		delete[] A;
		return temp;
	}
	return temp;
}

unsigned int* MoveRight(unsigned int A[], int size, int index, bool selfculc)
{
	unsigned int* temp = CopyArray(A, size);
	rotate(temp, &temp[size] - index, &temp[size]);
	if (selfculc)
	{
		delete[] A;
		return temp;
	}
	return temp;
}

int* GetDegs(int amount)
{
	int *degs = new int[amount];
	int n = 2 * degree + 1;
	degs[0] = 1;
	for (int i = 1; i < amount; i++)
	{
		degs[i] = (degs[i - 1] * 2) % n;
	}
	return degs;
}

int** CreateMatrix()
{
	int* degs = GetDegs(degree);
	int **matrix;
	int n = (2 * degree + 1);
	matrix = new int* [degree];
	for (int i = 0; i < degree; i++)
	{
		matrix[i] = new int[degree];
	}
	for (int i = 0; i < degree; i++)
	{
		for (int j = 0; j < degree; j++)
		{
			matrix[i][j] = 0;
			if ((degs[i] + degs[j]) % n == 1) matrix[i][j] = 1;
			if ((degs[i] % n) == ((1 + degs[j]) % n)) matrix[i][j] = 1;
			if ((degs[j] % n) == ((1 + degs[i]) % n)) matrix[i][j] = 1;
			if ((-degs[i] - degs[j]) % n == -n + 1) matrix[i][j] = 1;
		}
	}
	return matrix;
}

void ShowMatrix(int** matrix)
{
	for (int i = 0; i < degree; i++)
	{
		for (int j = 0; j < degree; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

int** matrix = CreateMatrix();

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

unsigned int Trace(LongNumber *A)
{
	int counter = 0;
	for (int i = 0; i < A->size; i++)
	{
		if (A->value[i] == 1) counter++;
	}
	if (counter % 2 == 1) return 1;
	else return 0;
}

LongNumber* Square(LongNumber *A, bool selfculc)
{
	unsigned int* arr = MoveRight(A->value, A->size, 1, false);
	LongNumber *temp = GetValue(arr, A->size);
	if (selfculc)
	{
		free(A);
		return temp;
	}
	return temp;
}

unsigned int* MatrixMul(unsigned int A[])
{
	unsigned int* C = new unsigned int[degree];
	for (int i = 0; i < degree; i++)
	{
		int sum = 0;
		for (int j = 0; j < degree; j++)
		{
			sum = (sum + A[j] * matrix[j][i]) % 2;
		}
		C[i] = sum;
	}
	return C;
}

unsigned int VectorMul(unsigned int A[], unsigned int B[])
{
	unsigned int result = 0;
	for (int i = 0; i < degree; i++)
	{
		result = (result + (A[i] * B[i])) % 2;
	}
	return result;
}

LongNumber* Multiplication(LongNumber *A, LongNumber *B, bool selfculc)
{
	unsigned int* C = new unsigned int[degree];
	unsigned int* a_temp = CopyArray(A->value, A->size);
	unsigned int* b_temp = CopyArray(B->value, B->size);
	unsigned int *t = MatrixMul(a_temp);
	C[0] = VectorMul(t, b_temp);
	free(t);
	for (int i = 1; i < degree; i++)
	{
		a_temp = MoveLeft(a_temp, degree, 1, true);
		b_temp = MoveLeft(b_temp, degree, 1, true);
		unsigned int *temp = MatrixMul(a_temp);
		C[i] = VectorMul(temp, b_temp);
		free(temp);
	}
	LongNumber *result = GetValue(C, degree);
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
	for (int i = N->size - 1; i >= 0; i--)
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

int GetSize(int num)
{
	int k = 0;
	while ((1 << k) <= num) k++;
	return k;
}

unsigned int* GetBinNumber(int num)
{
	int k = GetSize(num);
	unsigned int* result = new unsigned int[k];
	for (int i = 0; num > 0; i++)
	{
		result[i] = num % 2;
		num = num / 2;
	}
	unsigned int* temp = CopyArray(result, k);
	for (int i = 0; i < k; i++)
	{
		temp[i] = result[k - 1 - i];
	}
	delete[] result;
	return temp;
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
	
	//cout << "Module: " << endl;
	//mod->PrintParameters();

	//Test1();
	//Test2();

	auto start = std::chrono::system_clock::now();
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end - start;

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

	cout << endl << "A ^ 2: " << endl;
	start = std::chrono::system_clock::now();
	LongNumber *D = Square(A, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	GetStringValue(D);
	D->PrintParameters();
	cout << "Time: " << diff.count() << " sec" << endl;

	start = std::chrono::system_clock::now();
	unsigned int tr = Trace(A);
	end = std::chrono::system_clock::now();
	diff = end - start;
	cout << "Trace: " << endl << tr << endl;
	cout << "Time: " << diff.count() << " sec" << endl;

	cout << endl << "A * B: " << endl;
	start = std::chrono::system_clock::now();
	LongNumber *E = Multiplication(A, B, false);
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

	cout << endl << "A ^ -1: " << endl;
	start = std::chrono::system_clock::now();
	LongNumber *H = Reversed(A);
	end = std::chrono::system_clock::now();
	diff = end - start;
	GetStringValue(H);
	H->PrintParameters();
	cout << "Time: " << diff.count() << " sec" << endl;
}
