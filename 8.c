#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int count = 0;
	double sum, avg;
	sum = avg = 0.0;

	if(argc > 1){				// 요소가 하나라도 있을 경우
		for(int i = 1; i < argc; i++){
			sum += atof(argv[i]);	// 아스키 코드를 실수형으로 변환
			count++;		// 배열의 갯수 count
		}
		avg = sum / (double)count;
		printf("sum: %f\n", sum);
		printf("average: %f\n", avg);
	}
	return 0;
}
