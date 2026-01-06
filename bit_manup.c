#include <stdio.h>
//Template
#if 0
int main() {
	int n;
	printf("Number: ");
	scanf("%d", &n);
}
#endif

int XOR_N(int n) {
	switch(n & 3) {
		case 0:
			return n;
		case 1:
			return 1;
		case 2:
			return n+1;
		case 3:
			return 0;
	}
	return 0;
}

void read_array(int *arr, int n) {
	printf("Enter elements: ");
	for(int i=0; i<n; i++)
		scanf("%d", arr+i);
}

void print_array(int *arr, int n) {
	printf("Elements: ");
	for(int i=0; i<n; i++)
		printf("%d", *(arr+i));
}

#if 0
//Find number is even or odd
int main() {
	int n;
	printf("Number: ");
	scanf("%d", &n);

	if(n & 1) printf("Odd\n");
	else printf("Even\n");
	return 0;
}
#endif

#if 0
//Find number is power of 2 or not
int main() {
	int n;
	printf("Number: ");
	scanf("%d", &n);

	if((n > 0) && (n & (n-1)) == 0) printf("Power of 2\n");
	else printf("Not a power of 2\n");
	return 0;
}
#endif

#if 0
//Count number of set bits in a integer
int main() {
	int n;
	printf("Number: ");
	scanf("%d", &n);

	int count = 0;
	while(n) {
		count++;
		n = n & (n-1);
	}
	printf("Set bits: %d\n", count);
}
#endif

#if 0
//XOR from 1 - n
int main() {
	int n;
	printf("Number: ");
	scanf("%d", &n);

	printf("Xor of %d is %d\n", n, XOR_N(n));
	return 0;
}
#endif

#if 0
//Find only non-repeating element in an array where every other element appears twice
int main() {
	int n;
	printf("Enter no.of elements: ");
	scanf("%d", &n);

	int arr[n];
	read_array(arr, n);

	int unique = 0;
	for(int i=0; i<n; i++)
		unique ^= arr[i];

	printf("Unique element: %d\n", unique);

	return 0;
}
#endif

#if 0
//Find longest series of 1 in binary
int main() {
	int n;
	printf("Number: ");
	scanf("%d", &n);

	int count = 0;
	while (n) {
		n = n & (n<<1);
		count++;
	}

	printf("Longest 1s = %d\n", count);
	return 0;
}
#endif

#if 0
//Find longest series of 0 in binary
int main() {
	int n;
	printf("Number: ");
	scanf("%d", &n);

	int num = ~n;
	int count = 0;
	while (num) {
		num = num & (num<<1);
		count++;
	}

	printf("Longest 0s = %d\n", count);
	return 0;
}
#endif

#if 0
//Set a range of bits in number
int main() {
	int n;
	printf("Number: ");
	scanf("%d", &n);

	int L,R;
	printf("Enter Range [L-R] : ");
	scanf("%d %d", &L, &R);

	if(L < R) {
		printf("Wrong range\n");
		return 0;
	}

	int mask = ( 1 << (L - R + 1)) - 1;
	mask = mask << R;
	n = n | mask;

	printf("Number = %d\n", n);
	return 0;
}
#endif

#if 1
//Clear a range of bits in number
int main() {
	int n;
	printf("Number: ");
	scanf("%d", &n);

	int L,R;
	printf("Enter Range [L-R] : ");
	scanf("%d %d", &L, &R);

	if(L < R || L > 31 || R < 0) {
		printf("Wrong range\n");
		return 0;
	}

	int mask = ( 1 << (L - R + 1)) - 1;
	mask = mask << R;
	n = n & ~mask;

	printf("Number = %d\n", n);
	return 0;
}
#endif

