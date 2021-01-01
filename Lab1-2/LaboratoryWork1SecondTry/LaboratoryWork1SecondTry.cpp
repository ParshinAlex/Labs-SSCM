#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <chrono>

using namespace std;

// ----------------------- CONST ZONE -----------------------

const unsigned int len = 1024, max_len = 2049;

// ----------------------- CONVERTATION ZONE  -----------------------

unsigned int* CreateConstant(unsigned int number, unsigned int w)
{
	unsigned int* A = new unsigned int[max_len];
	fill(&A[0], &A[max_len], 0);
	A[0] = number & ((1i64 << w) - 1);
	return A;
}

// Function that gets value of the first non-null bit of a number 
int GetFirstNotNullBit(unsigned int A[max_len])
{
	int counter = 0, temp = max_len;
	while (A[max_len - 1 - counter] == 0)
	{
		counter++;
	}
	return max_len - counter - 1;
}

void CopyArray(unsigned int A[max_len], unsigned int B[max_len])
{
	for (int i = 0; i < max_len; i++)
	{
		B[i] = A[i];
	}
}

void PrintArray(unsigned int A[max_len])
{
	cout << endl;
	for (int i = GetFirstNotNullBit(A); i >= 0; i--)
	{
		cout << A[i];
	}
	cout << endl;
}

// Function that converts hex string into big number
unsigned int* ConvertHexToBigNumber(string s, int w)
{
	int i = 0, counter = 0;
	unsigned int *A = new unsigned int[max_len];
	fill(&A[0], &A[max_len], 0);
	unsigned int num = 0;
	string sub = "", temp = s;
	if (w >= 4)
	{
		while (temp.size() % (w / 4) != 0)
		{
			temp.insert(0, "0");
		}

		while (temp.size() != 0)
		{
			sub = temp.substr(temp.size() - (w / 4), w / 4);
			temp.erase(temp.size() - (w / 4), w / 4);
			sscanf_s(sub.c_str(), "%x", &num);
			A[i] = num;
			i++;
		}
		return A;
	}
	if (w == 1)
	{
		int calc = 0;
		for (int i = s.length() - 1; i >= 0; i--)
		{
			char temp = s[i];
			int number = strtol(&temp, NULL, 16);
			for (int j = 0; j < 4; j++)
			{
				A[calc] = ((number >> j) & 1);
				calc++;
			}
		}
		return A;
	}
	return A;
}

// Function that converts int 2^w to hex
string ConvertIntToHex(unsigned int n, int w)
{
    stringstream ss;
    string result;
    ss << hex << n;
    result = ss.str();
    while (result.length() != (w / 4))
    {
        result.insert(0, "0");
    }
    return result;
}

// Function that converts binary number to hex
string ConvertBinNumberToHex(unsigned int A[max_len])
{
	stringstream ss;
	string result, temp;
	int A_len = GetFirstNotNullBit(A) + 1;
	int value = 0;
	while (A_len % 4 != 0)
	{
		A_len++;
	}
	for (int i = 0; i < A_len; i = i + 4)
	{
		value = 0;
		for (int j = 0; j <= 3; j++)
		{
			value = value + (A[i+j] * (1 << j));
		}
		ss << hex << value;
		temp = ss.str();
		result.insert(0, temp);
		ss.str("");
	}
	while (result[0] == '0')
	{
		result.erase(0, 1);
	}
	return result;
}

// Function that coonverts decimal 2^w number to hex
string ConvertBigNumberToHex(unsigned int A[max_len], int w)
{
	if (w >= 4)
	{
		int counter = 0;
		string temp = "", result = "";
		int A_len = GetFirstNotNullBit(A);
		while (counter != A_len + 1)
		{
			temp = ConvertIntToHex(A[counter], w);
			result.insert(0, temp);
			counter++;
		}
		while (result[0] == '0')
		{
			result.erase(0, 1);
		}
		if (result == "") result = "0";
		return result;
	}
	if (w == 1)
	{
		return ConvertBinNumberToHex(A);
	}
	return "0";
}

// ----------------------- OPERATIONS ZONE  -----------------------

// Function calculating A + B
unsigned int* LongAdd(unsigned int A[max_len], unsigned int B[max_len], unsigned int w, bool SelfCulc)
{
	unsigned int carry = 0;
	const unsigned long long base = 1i64 << w;
	unsigned long long temp = 0;
	unsigned int* C = new unsigned int[max_len];
	fill(&C[0], &C[max_len], 0);
	for (int i = 0; i < max_len; i++)
	{
		temp = (unsigned long long)A[i] + B[i] + carry;
		C[i] = temp & (base - 1);
		carry = temp >> w;
	}
	if (SelfCulc)
	{
		delete[]A;
		A = NULL;
		return C;
	}
	return C;
}

// Function comparing A and B
int LongCmp(unsigned int A[max_len], unsigned int B[max_len])
{
	int i = max_len - 1;
	while (A[i] == B[i] && i >= 0)
	{
		i--;
	}
	if (i == -1) return 0;
	else
	{
		if (A[i] > B[i]) return 1;
		else return -1;
	}
}

// Function calculating A - B
unsigned int* LongSub(unsigned int A[max_len], unsigned int B[max_len], unsigned int w, bool SelfCulc)
{
	int borrow = 0, temp = 0;
	unsigned int* C = new unsigned int[max_len];
	fill(&C[0], &C[max_len], 0);
	int check = LongCmp(A, B);
	if (check == -1)
	{
		if (SelfCulc)
		{
			delete[]A;
			A = NULL;
			return C;
		}
		return C;
	}
	else
	{
		int A_len = GetFirstNotNullBit(A);
		for (int i = 0; i < A_len + 1; i++)
		{
			temp = A[i] - B[i] - borrow;
			if (temp >= 0)
			{
				C[i] = temp;
				borrow = 0;
			}
			else
			{
				C[i] = (1i64 << w) + temp;
				borrow = 1;
			}
		}
		if (SelfCulc)
		{
			delete[]A;
			A = NULL;
			return C;
		}
		return C;
	}
}

// ----------------------- Section of A * B -----------------------

// Function calculating A * b, where is b - is one digit
unsigned int* LongMulOneDigit(unsigned int A[max_len], unsigned int b, unsigned int w, bool SelfCulc)
{
	unsigned int carry = 0;
	unsigned int *C = new unsigned int[max_len];
	fill(&C[0], &C[max_len], 0);
	int A_len = GetFirstNotNullBit(A);
	for (int i = 0; i < A_len + 1; i++)
	{
		unsigned long long temp = (unsigned long long)A[i] * b + carry;
		C[i] = temp & ((1i64 << w) - 1);
		carry = temp >> w;
	}
	if (A_len == max_len)
	{
		if (SelfCulc)
		{
			delete[]A;
			A = NULL;
			return C;
		}
		return C;
	}
	else
	{
		C[A_len + 1] = carry;
		if (SelfCulc)
		{
			delete[]A;
			A = NULL;
			return C;
		}
		return C;
	}
}

// Function that moves *index* bits to higher places
void LongShiftBitsToHigh(unsigned int A[max_len], int index)
{
	for (int i = max_len - 1 - index; i >= 0 ; i--)
	{
		A[i + index] = A[i];
	}
	for (int i = 0; i <= index - 1; i++)
	{
		A[i] = 0;
	}
}

// Function that moves *index* bits to lower places
void LongShiftBitsToLow(unsigned int A[max_len], int index)
{
	for (int i = index; i <= max_len - 1; i++)
	{
		A[i - index] = A[i];
	}
	for (int i = max_len - index; i <= max_len - 1; i++)
	{
		A[i] = 0;
	}
}

// Function calculating A * B
unsigned int* LongMul(unsigned int A[max_len], unsigned int B[max_len], unsigned int w, bool SelfCulc)
{
	unsigned int* C = new unsigned int[max_len];
	fill(&C[0], &C[max_len], 0);
	for (int i = 0; i < max_len; i++)
	{
		unsigned int* temp = LongMulOneDigit(A, B[i], w, false);
		LongShiftBitsToHigh(temp, i);
		C = LongAdd(C, temp, w, true);
		delete[]temp;
		temp = NULL;
	}
	if (SelfCulc)
	{
		delete[]A;
		A = NULL;
		return C;
	}
	return C;
}

// ----------------------- Section of A / B -----------------------

unsigned int** LongDivMod(string s_A, string s_B)
{
	unsigned int* A_bin = new unsigned int[max_len];
	unsigned int* B_bin = new unsigned int[max_len];
	unsigned int* C_bin = new unsigned int[max_len];
	unsigned int* R = new unsigned int[max_len];
	unsigned int* Q = new unsigned int[max_len];
	unsigned int** result = new unsigned int*[2];
	fill(&Q[0], &Q[max_len], 0);
	A_bin = ConvertHexToBigNumber(s_A, 1);
	B_bin = ConvertHexToBigNumber(s_B, 1);
	int k = GetFirstNotNullBit(B_bin);
	CopyArray(A_bin, R);
	while (LongCmp(R, B_bin) == 1 || LongCmp(R, B_bin) == 0)
	{
		int t = GetFirstNotNullBit(R);
		CopyArray(B_bin, C_bin);
		LongShiftBitsToHigh(C_bin, t - k);
		if (LongCmp(R, C_bin) == -1)
		{
			t = t - 1;
			CopyArray(B_bin, C_bin);
			LongShiftBitsToHigh(C_bin, t - k);
		}
		R = LongSub(R, C_bin, 1, true);
		Q[t - k] = 1;
	}
	result[0] = Q;
	result[1] = R;
	delete[]A_bin, delete[]B_bin, delete[]C_bin;
	A_bin = NULL, B_bin = NULL, C_bin = NULL;
	return result;
}

unsigned int* LongSquare(unsigned int A[max_len], unsigned int w, bool SelfCulc)
{
	//unsigned int* temp = new unsigned int[max_len];
	unsigned int* result = new unsigned int[max_len];
	//CopyArray(A, temp);
	//CopyArray(LongMul(A, temp, w), result);
	result = LongMul(A, A, w, false);
	//delete[]temp;
	if (SelfCulc)
	{
		delete[]A;
		A = NULL;
		return result;
	}
	//temp = NULL;
	return result;
}

unsigned int* LongPower(unsigned int A[max_len], unsigned int B[max_len], unsigned int w, bool SelfCulc)
{
	unsigned int* C = CreateConstant(1, w);
	int B_len = GetFirstNotNullBit(B);
	for (int i = 0; i < B_len + 1; i++)
	{
		if (B[i] == 1)
		{
			//CopyArray(LongMul(C, A, w), C);
			C = LongMul(C, A, w, true);
		}
		A = LongSquare(A, w, true);
		//CopyArray(LongSquare(A, w), A);
	}
	if (SelfCulc)
	{
		delete[]A;
		A = NULL;
		return C;
	}
	return C;
}

// ------------------------- LAB 2  -------------------------

string LongGcd(string s_A, string s_B, unsigned int w)
{
	unsigned int* A_bin = ConvertHexToBigNumber(s_A, 1);
	unsigned int* B_bin = ConvertHexToBigNumber(s_B, 1);
	unsigned int* zero = new unsigned int[max_len];
	fill(&zero[0], &zero[max_len], 0);

	if (LongCmp(A_bin, B_bin) == 0)
	{
		delete[]A_bin, delete[]B_bin, delete[]zero;
		A_bin = NULL, B_bin = NULL, zero = NULL;
		return s_A;
	}
	if (LongCmp(A_bin, zero) == 0)
	{
		delete[]A_bin, delete[]B_bin, delete[]zero;
		A_bin = NULL, B_bin = NULL, zero = NULL;
		return s_B;
	}
	if (LongCmp(B_bin, zero) == 0)
	{
		delete[]A_bin, delete[]B_bin, delete[]zero;
		A_bin = NULL, B_bin = NULL, zero = NULL;
		return s_A;
	}

	unsigned int* d = CreateConstant(1, 1);
	unsigned int* temp = new unsigned int[max_len];
	string result = "";
	
	while ((LongCmp(A_bin, zero) != 0) && (LongCmp(B_bin, zero) != 0))
	{
		while ((A_bin[0] == 0) && (B_bin[0] == 0))
		{
			LongShiftBitsToLow(A_bin, 1);
			LongShiftBitsToLow(B_bin, 1);
			LongShiftBitsToHigh(d, 1);
		}
		while ((A_bin[0] == 0) && (LongCmp(A_bin, zero) != 0))
		{
			LongShiftBitsToLow(A_bin, 1);
		}
		while ((B_bin[0] == 0) && (LongCmp(B_bin, zero) != 0))
		{
			LongShiftBitsToLow(B_bin, 1);
		}
		if (LongCmp(A_bin, B_bin) == 1)
		{
			A_bin = LongSub(A_bin, B_bin, 1, true);
		}
		else
		{
			B_bin = LongSub(B_bin, A_bin, 1, true);
		}
	}
	d = LongMul(d, A_bin, 1, true);
	result = ConvertBinNumberToHex(d);
	delete[]A_bin, delete[]B_bin, delete[]temp, delete[]zero, delete[]d;
	A_bin = NULL, B_bin = NULL, temp = NULL, zero = NULL, d = NULL;
	return result;
}

string LongLcm(string s_A, string s_B, int w)
{
	string g = LongGcd(s_A, s_B, w);
	unsigned int* A_decimal = ConvertHexToBigNumber(s_A, w);
	unsigned int* B_decimal = ConvertHexToBigNumber(s_B, w);
	unsigned int* gcd = ConvertHexToBigNumber(g, w);
	if (g == "1")
	{
		A_decimal = LongMul(A_decimal, B_decimal, w, true);
		string res = ConvertBigNumberToHex(A_decimal, w);
		delete[]A_decimal, delete[]B_decimal, delete[]gcd;
		A_decimal = NULL, B_decimal = NULL, gcd = NULL;
		return res;
	}
	else
	{
		unsigned int* temp = new unsigned int[max_len];
		temp = LongMul(A_decimal, B_decimal, w, false);
		unsigned int* lcm = LongDivMod(ConvertBigNumberToHex(temp, w), g)[0];
		string res = ConvertBigNumberToHex(lcm, 1);
		delete[]A_decimal, delete[]B_decimal, delete[]gcd, delete[]temp, delete[]lcm;
		A_decimal = NULL, B_decimal = NULL, gcd = NULL, temp = NULL, lcm = NULL;
		return res;
	}
}

string BarrettReduction(string s_x, string s_n, unsigned int w)
{
	unsigned int *x = ConvertHexToBigNumber(s_x, 1);
	unsigned int *n = ConvertHexToBigNumber(s_n, 1);

	int k = GetFirstNotNullBit(n);
	unsigned int* beta = CreateConstant(1, 1);
	LongShiftBitsToHigh(beta, 2 * k);
	string s_beta = ConvertBinNumberToHex(beta);
	delete[]beta;
	beta = NULL;
	unsigned int **temp = LongDivMod(s_beta, s_n);
	unsigned int *mu = temp[0];
	
	unsigned int* q = new unsigned int[max_len];
	CopyArray(x, q);
	LongShiftBitsToLow(q, k - 1);
	q = LongMul(q, mu, 1, true);
	LongShiftBitsToLow(q, k + 1);
	unsigned int *help = LongMul(q, n, 1, false);
	unsigned int *r = LongSub(x, help, 1, false);
	while (LongCmp(r, n) >= 0)
	{
		r = LongSub(r, n, 1, true);
	}
	delete[]x, delete[]n, delete[]help, delete[]q, delete[]temp[0], delete[]temp[1], delete[]temp;
	help = NULL, x = NULL, n = NULL, q = NULL, temp = NULL;
	string answer = ConvertBinNumberToHex(r);
	delete[]r;
	return answer;
}

unsigned int* LongAddMod(unsigned int A[max_len], unsigned int B[max_len], unsigned int N[max_len], int w, bool SelfCulc)
{
	unsigned int *ad = LongAdd(A, B, w, false);
	string s_result = BarrettReduction(ConvertBigNumberToHex(ad, w), ConvertBigNumberToHex(N, w), w);
	delete[]ad;
	ad = NULL;
	if (SelfCulc)
	{
		delete[]A;
		A = NULL;
		return ConvertHexToBigNumber(s_result, w);
	}
	return ConvertHexToBigNumber(s_result, w);
}

unsigned int* LongSubMod(unsigned int A[max_len], unsigned int B[max_len], unsigned int N[max_len], int w, bool SelfCulc)
{
	unsigned int *sub = LongSub(A, B, w, false);
	string s_result = BarrettReduction(ConvertBigNumberToHex(sub, w), ConvertBigNumberToHex(N, w), w);
	delete[]sub;
	sub = NULL;
	if (SelfCulc)
	{
		delete[]A;
		A = NULL;
		return ConvertHexToBigNumber(s_result, w);
	}
	return ConvertHexToBigNumber(s_result, w);
}

unsigned int* LongMulMod(unsigned int A[max_len], unsigned int B[max_len], unsigned int N[max_len], int w, bool SelfCulc)
{
	unsigned int *mul = LongMul(A, B, w, false);
	string s_result = BarrettReduction(ConvertBigNumberToHex(mul, w), ConvertBigNumberToHex(N, w), w);
	delete[]mul;
	mul = NULL;
	if (SelfCulc)
	{
		delete[]A;
		A = NULL;
		return ConvertHexToBigNumber(s_result, w);
	}
	return ConvertHexToBigNumber(s_result, w);
}

unsigned int* LongSquareMod(unsigned int A[max_len], unsigned int N[max_len], int w, bool SelfCulc)
{
	unsigned int *sq = LongSquare(A, w, false);
	string s_result = BarrettReduction(ConvertBigNumberToHex(sq, w), ConvertBigNumberToHex(N, w), w);
	delete[]sq;
	sq = NULL;
	if (SelfCulc)
	{
		delete[]A;
		A = NULL;
		return ConvertHexToBigNumber(s_result, w);
	}
	return ConvertHexToBigNumber(s_result, w);
}

unsigned int* LongPowerMod(unsigned int A[max_len], unsigned int B[max_len], unsigned int N[max_len], int w, bool SelfCulc)
{
	unsigned int* C = CreateConstant(1, w);
	int B_len = GetFirstNotNullBit(B);
	for (int i = 0; i < B_len + 1; i++)
	{
		if (B[i] == 1)
		{
			//CopyArray(LongMulMod(C, A, N, w), C);
			C = LongMulMod(C, A, N, w, true);
		}
		//CopyArray(LongSquareMod(A, N, w), A);
		A = LongSquareMod(A, N, w, true);
	}
	if (SelfCulc)
	{
		delete[]A;
		A = NULL;
		return C;
	}
	return C;
}

// ----------------------- MAIN ZONE  -----------------------

int main()
{
	// LAB-1 Section --------------------------------------------------------------


	
	/*int i = 0;
	const unsigned int base = 32, bin_base = 1;

	int A_decimal_len = 0, A_bin_len = 0;
	int B_decimal_len = 0, B_bin_len = 0;
	int C_decimal_len = 0, C_bin_len = 0;
	int D_decimal_len = 0, D_bin_len = 0;
	int E_decimal_len = 0, E_bin_len = 0;
	int F_decimal_len = 0, F_bin_len = 0;
	int N_decimal_len = 0, N_bin_len = 0;
	

	unsigned int *A_decimal, *A_bin;
	unsigned int *B_decimal, *B_bin;
	unsigned int *C_decimal, *C_bin;
	unsigned int *D_decimal, *D_bin;
	unsigned int *E_decimal, *E_bin;
	unsigned int *F_decimal, *F_bin;
	unsigned int *N_decimal, N_bin;
	unsigned int *R, *Q;
	unsigned int** division_result;
	
	string s_A, s_A_bin_converted, s_A_decimal_converted;
	string s_B;
	string s_C;
	string s_D;
	string s_E;
	string s_R;
	string s_Q;
	string s_F;
	string s_N;
	
	cout << "Enter number A: " << endl;
	cin >> s_A;

	cout << "Enter number B: " << endl;
	cin >> s_B;

	cout << "Enter number N: " << endl;
	cin >> s_N;

	A_decimal = ConvertHexToBigNumber(s_A, base);
	A_decimal_len = GetFirstNotNullBit(A_decimal);

	B_decimal = ConvertHexToBigNumber(s_B, base);
	B_bin = ConvertHexToBigNumber(s_B, 1);
	B_decimal_len = GetFirstNotNullBit(B_decimal);

	N_decimal = ConvertHexToBigNumber(s_N, base);
	N_decimal_len = GetFirstNotNullBit(N_decimal);

	

	auto start = std::chrono::system_clock::now();
	C_decimal = LongAdd(A_decimal, B_decimal, base, false);
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end - start;
	C_decimal_len = GetFirstNotNullBit(C_decimal);
	s_C = ConvertBigNumberToHex(C_decimal, base);
	cout << "A + B: " << endl << s_C << endl;
	cout << "Time: " << diff.count() << " sec" << endl;
	
	start = std::chrono::system_clock::now();
	D_decimal = LongSub(A_decimal, B_decimal, base, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	D_decimal_len = GetFirstNotNullBit(D_decimal);
	s_D = ConvertBigNumberToHex(D_decimal, base);
	cout << "A - B: " << endl << s_D << endl;
	cout << "Time: " << diff.count() << " sec" << endl;

	start = std::chrono::system_clock::now();
	E_decimal = LongMul(A_decimal, B_decimal, base, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	E_decimal_len = GetFirstNotNullBit(E_decimal);
	s_E = ConvertBigNumberToHex(E_decimal, base);
	cout << "A * B: " << endl << s_E << endl;
	cout << "Time: " << diff.count() << " sec" << endl;

	start = std::chrono::system_clock::now();
	division_result = LongDivMod(s_A, s_B);
	end = std::chrono::system_clock::now();
	diff = end - start;
	Q = division_result[0];
	R = division_result[1];
	s_Q = ConvertBinNumberToHex(Q);
	s_R = ConvertBinNumberToHex(R);
	cout << "A / B: " << endl << s_Q << endl;
	cout << "A mod B: " << endl << s_R << endl;
	cout << "Time: " << diff.count() << " sec" << endl;
	
	B_bin = ConvertHexToBigNumber(s_B, bin_base);
	start = std::chrono::system_clock::now();
	F_decimal = LongPower(A_decimal, B_bin, base, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	F_decimal_len = GetFirstNotNullBit(F_decimal);
	s_F = ConvertBigNumberToHex(F_decimal, base);
	cout << "A ^ B: " << endl << s_F << endl;
	cout << "Time: " << diff.count() << " sec" << endl;*/
	

	// TEST-1: A * (B * N) = (A * B) * N

	/*unsigned int *temp_1, *temp_2, *temp_3, *temp_4;
	temp_1 = LongMul(A_decimal, B_decimal, base, false);
	temp_2 = LongMul(temp_1, N_decimal, base, false);
	temp_3 = LongMul(B_decimal, N_decimal, base, false);
	temp_4 = LongMul(temp_3, A_decimal, base, false);

	string res_1 = ConvertBigNumberToHex(temp_2, base);
	string res_2 = ConvertBigNumberToHex(temp_4, base);

	cout << "(A * B) * N: " << endl << res_1 << endl;
	cout << "A * (B * N): " << endl << res_2 << endl;*/

	// TEST-2: CONVERTATION NUMBER A FROM HEX TO DECIMAL AND BINARY - AND BACK

	//// Decimal System
	//A_decimal = ConvertHexToBigNumber(s_A, 32);
	//A_decimal_len = GetFirstNotNullBit(A_decimal);
	//cout << "This number in decimal: " << endl;
	//for (int i = A_decimal_len; i >= 0; i--)
	//{
	//	cout << A_decimal[i];
	//}
	//cout << endl;

	//// Converting decimal number to hex
	//s_A_decimal_converted = ConvertBigNumberToHex(A_decimal, 32);
	//cout << "A converted from decimal 2^32 to hex: " << endl;
	//cout << s_A_decimal_converted << endl;

	//// Binary System
	//A_bin = ConvertHexToBigNumber(s_A, 1);
	//A_bin_len = GetFirstNotNullBit(A_bin);
	//cout << "This number in binary: " << endl;
	//for (int i = A_bin_len - 1; i >= 0; i--)
	//{
	//	cout << A_bin[i];
	//}
	//cout << endl;

	//// Converting binary number to hex
	//s_A_bin_converted = ConvertBigNumberToHex(A_bin, 1);
	//cout << "A converted from bin to hex: " << endl;
	//cout << s_A_bin_converted << endl;

	

	
	
	// LAB-2 Section --------------------------------------------------------------

	

	int i = 0;
	const unsigned int base = 32, bin_base = 1;

	unsigned int *A_decimal, *B_decimal, *N_decimal;
	string s_A, s_B, s_N;

	cout << "Enter number A: " << endl;
	cin >> s_A;
	A_decimal = ConvertHexToBigNumber(s_A, base);

	cout << "Enter number B: " << endl;
	cin >> s_B;
	B_decimal = ConvertHexToBigNumber(s_B, base);

	cout << "Enter number N: " << endl;
	cin >> s_N;
	N_decimal = ConvertHexToBigNumber(s_N, base);

	unsigned int *B_bin = ConvertHexToBigNumber(s_B, bin_base);
	
	
	auto start = std::chrono::system_clock::now();
	string s_d = LongGcd(s_A, s_B, base);
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end - start;
	cout << "gcd(A, B): " << endl << s_d << endl;
	cout << "Time: " << diff.count() << " sec" << endl;

	start = std::chrono::system_clock::now();
	string lcm = LongLcm(s_A, s_B, base);
	end = std::chrono::system_clock::now();
	diff = end - start;
	cout << "lcm(A, B):" << endl << lcm << endl;
	cout << "Time: " << diff.count() << " sec" << endl;

	unsigned int *abAdd, *abSub, *abMul, *aSq, *abPower;
	string sAbAdd, sAbSub, sAbMul, sASq, sAbPower;

	start = std::chrono::system_clock::now();
	abAdd = LongAddMod(A_decimal, B_decimal, N_decimal, base, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	sAbAdd = ConvertBigNumberToHex(abAdd, base);
	cout << "A + B mod N: " << endl << sAbAdd << endl;
	cout << "Time: " << diff.count() << " sec" << endl;

	start = std::chrono::system_clock::now();
	abSub = LongSubMod(A_decimal, B_decimal, N_decimal, base, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	sAbSub = ConvertBigNumberToHex(abSub, base);
	cout << "A - B mod N: " << endl << sAbSub << endl;
	cout << "Time: " << diff.count() << " sec" << endl;

	start = std::chrono::system_clock::now();
	abMul = LongMulMod(A_decimal, B_decimal, N_decimal, base, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	sAbMul = ConvertBigNumberToHex(abMul, base);
	cout << "A * B mod N: " << endl << sAbMul << endl;
	cout << "Time: " << diff.count() << " sec" << endl;

	start = std::chrono::system_clock::now();
	aSq = LongSquareMod(A_decimal, N_decimal, base, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	sASq = ConvertBigNumberToHex(aSq, base);
	cout << "A ^ 2 mod N: " << endl << sASq << endl;
	cout << "Time: " << diff.count() << " sec" << endl;
	
	start = std::chrono::system_clock::now();
	abPower = LongPowerMod(A_decimal, B_bin, N_decimal, base, false);
	end = std::chrono::system_clock::now();
	diff = end - start;
	sAbPower = ConvertBigNumberToHex(abPower, base);
	cout << "A ^ B mod N: " << endl << sAbPower << endl;
	cout << "Time: " << diff.count() << " sec" << endl;
	
	delete[]abAdd, delete[]abSub, delete[]abMul, delete[]aSq, delete[]abPower;;
	abAdd = NULL, abSub = NULL, abMul = NULL, aSq = NULL, abPower = NULL;
	
	// TEST-1: (A+B)*C = C*(A+B) = A*C + B*C

	/*string s_C;
	unsigned int *sumAB, *C_decimal;

	cout << "Enter number C: " << endl;
	cin >> s_C;
	C_decimal = ConvertHexToBigNumber(s_C, base);

	sumAB = LongAdd(A_decimal, B_decimal, base, false);
	cout << "(A + B) * C mod N: " << endl << ConvertBigNumberToHex(LongMulMod(C_decimal, sumAB, N_decimal, base, false), base) << endl;
	cout << "C * (A + B) mod N: " << endl << ConvertBigNumberToHex(LongMulMod(sumAB, C_decimal, N_decimal, base, false), base) << endl;
	cout << "(A * C + B * C) mod N: " << endl << ConvertBigNumberToHex(LongAddMod(LongMul(A_decimal, C_decimal, base, false), LongMul(B_decimal, C_decimal, base, false), N_decimal, base, false), base) << endl;
	*/
	// TEST-2: A * N = A + A + ... + A mod m

	/*int num = 15;
	unsigned int *test = CreateConstant(num, base);
	unsigned int *result1 = LongMulMod(A_decimal, test, N_decimal, base, false);
	unsigned int *result2 = new unsigned int[max_len];
	CopyArray(A_decimal, result2);
	for (int i = 1; i < num; i++)
	{
		result2 = LongAddMod(result2, A_decimal, N_decimal, base, true);
	}
	cout << "num * A: " << endl << ConvertBigNumberToHex(result1, base) << endl;
	cout << "Sum A num times: " << endl << ConvertBigNumberToHex(result2, base) << endl;*/
	
	// TEST-3: A^PHI(N) = 1 mod N
	
	
	/*unsigned int* test2 = LongPower(A_decimal, N_decimal, base, false);
	unsigned int* one = CreateConstant(1, base);
	unsigned int* phi = LongSub(N_decimal, one, base, false);
	string phi_str = ConvertBigNumberToHex(phi, base);
	unsigned int* bin_phi = ConvertHexToBigNumber(phi_str, bin_base);
	unsigned int* test2 = LongPowerMod(A_decimal, bin_phi, N_decimal, base, false);
	string test2str = ConvertBigNumberToHex(test2, base);
	string result3 = BarrettReduction(test2str, s_N, base);
	cout << "A^PHI(N) mod N: " << endl << result3 << endl;*/
	
	
	
}