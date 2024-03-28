#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXWORDS 1000
#define MAXLEN 100

int compare(const void* a, const void* b) {
    return strcmp(*(char**)a, *(char**)b);		// 비교 후 사전순 정렬한 값 return
}

int compare_reverse(const void* a, const void* b) {
    return strcmp(*(char**)b, *(char**)a);		// 비교 후 사전순 정렬을 내림차순
}

int main(int argc, char *argv[]) {
    char* words[MAXWORDS];
    int i, j, count = 0, reverse = 0;

    while (argc > 1 && argv[1][0] == '-') {		// -r여부 파악
        if (argv[1][1] == 'r')
            reverse = 1;
        else {
            printf("error");
            return 1;
        }
        argc--;						// 요소 감소
        argv++;						// 다음 요소를 가리킴
    }

    while (argc > 1 && count < MAXWORDS) {		// command-line arguments의 단어 출력 
        words[count++] = strdup(argv[1]);		// 단어 추출 후 words배열에 저장
        argc--;
        argv++;
    }

    if (reverse)					// '-r' check
        qsort(words, count, sizeof(char*), compare_reverse);
    else
        qsort(words, count, sizeof(char*), compare);

    for (i = 0; i < count; i++)				// 요소의 개수만큼 출력
        printf("%s ", words[i]);
    putchar('\n');

    for (i = 0; i < count; i++)				// 할당된 메모리값 해제
        free(words[i]);

    return 0;
}
