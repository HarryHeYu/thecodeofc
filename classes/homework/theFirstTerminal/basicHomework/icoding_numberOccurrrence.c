// 出现次数（Number Occurrences）
// 修改如下程序，使其打印⼀份列表，
// 显示出每个数字在数中出现的次数。
// 输出范例：
// Enter a number: 41271092
// Digit:	0	1	2	3	4	5	
// 6	7	8	9
// Occurrences:	1	2	2	0	1	
// 0	0	1	0	1
#include<stdio.h>
int main(){
    long long int digit;
    int digits[20];
    int occurrences[10]={0};
    printf("Enter a number:");
    scanf("%lld",&digit);
    if(digit<0){
        printf("Number must be positive");
        return 0;
    }
    if(digit==0){
        occurrences[0]=1;
    }
    while(digit>0){
        occurrences[digit%10]++;
        digit/=10;
    }
    printf("\nDigit:	0	1	2	3	4	5	6	7	8	9\n");
    printf("Occurrences:	%d	%d	%d	%d	%d	%d	%d	%d	%d	%d\n",occurrences[0],occurrences[1],occurrences[2],occurrences[3],occurrences[4],occurrences[5],occurrences[6],occurrences[7],occurrences[8],occurrences[9]);
    return 0;
}
//以下是csdn上找到的代码：
// #define _CRT_SECURE_NO_WARNINGS 1
// #include <stdio.h>

// int main() {
// 	int digit, digit_count[10] = { 0 };
// 	long num;
// 	printf("Enter a number: ");
// 	scanf("%ld", &num);
// 	if (num == 0) {
// 		digit_count[0] = 1;
// 	}
// 	while (num != 0) {
// 		digit = num % 10;
// 		digit_count[digit]++;
// 		num = num / 10;
// 	}
// 	printf("\nDigit:      ");
// 	for (int i = 0; i < 10; i++) {
// 		printf("%d", i);
// 	}
// 	printf("\nOccurrences:");
// 	for (int i = 0; i < 10; i++) {
// 		printf("%d", digit_count[i]);
// 	}
// 	return 0;
// }