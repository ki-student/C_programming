#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000
#define MAXWORDS 100

// compare 함수를 qsort()에서 사용하기 위한 함수 형식
int compare(const void* a, const void* b) {
    return strcmp(*(char**)a, *(char**)b);
}

int compare_numeric(const void* a, const void* b) {
    int num1 = atoi(*(char**)a);
    int num2 = atoi(*(char**)b);		// 아스키를 int형으로 변환
    return num1 - num2;				// 비교한 결과 return
    						// numeric정렬과 사전순 정렬 결정
}

int main(int argc, char *argv[]) {
    char *words[MAXWORDS];
    int count, reverse, numeric;
    count = reverse = numeric = 0;

    // 옵션 처리
    while (argc > 1 && argv[1][0] == '-') {	// '-n' 여부 파악 후 numeric값 1로 저장
        switch (argv[1][1]) {
            case 'n':
                numeric = 1;
                break;
            default:
                printf("error\n");
                return 1;
        }
        argc--;
        argv++;
    }

    // 문자열 저장
    char line[MAXLINE];				// line배열 생성 후 문자열을 저장
    while (fgets(line, MAXLINE, stdin)) {	// fgets함수를 사용하여 문자열을 입력받음
	    if(count == MAXWORDS){		// 배열이 꽉 차면 error
		    printf("error\n");
		    return 1;
	    }
	    line[strcspn(line, "\r\n")] = '\0';	// 문자열이 나타나는 곳을 받아 line배열에 저장
	    words[count++] = strdup(line);	// words배열에 line을 동적 할당함
    }

    if (numeric) {				// '-n' 여부 확인
        qsort(words, count, sizeof(char*), compare_numeric);
	// words 배열을 compare_numeric을 사용하여 정렬
	// count는 배열 words에 저장된 문자열 개수
	// sizeof(char*)는 words배열의 원소인 포인터의 크기
	// words배열의 원소들을 숫자의 크기를 기준으로 정렬
    } else {
        qsort(words, count, sizeof(char*), compare);
    }

    printf("After sort\n");
    if (reverse) {				// numeric 정렬
        for (int i = count - 1; i >= 0; i--) {
            printf("%s\n", words[i]);
        }
    } else {					// 사전순 정렬
        for (int i = 0; i < count; i++) {
            printf("%s\n", words[i]);
        }
    }

    // 동적 할당한 메모리 해제
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }

    return 0;
}
