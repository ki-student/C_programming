#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_ENTRIES 100

struct Entry {
    char name[30];
    char phone[20];
    char memo[40];
};// Entry 구조체를 통해 data.txt에 들어갈 배열의 크기 초기화

// 전화번호 데이터를 로드하여 엔트리 배열에 저장하는 함수
int load(struct Entry entries[], int minEntries) {
    FILE *file = fopen("data.txt", "r");	// 읽기 모드
    if (file == NULL) {
        printf("NULL\n");
        return 0;
    }// 파일이 비어있으면 NULL 출력

    int count = 0;				// 엔트리 수 계산
    char line[MIN_ENTRIES];			// 배열의 개수 초기화
    while (fgets(line, sizeof(line), file) != NULL) {	// 한 줄씩 읽으며 조건에 충족하는지 확인
	 if (count == minEntries) {
        	 printf("END\n");
       		 break;
   	 }// 파일의 끝에 도달하면 break
        char *name = strtok(line, ":");
        char *phone = strtok(NULL, ":");
        char *memo = strtok(NULL, ":");
	// ':'을 구분자로 사용하여 name, phone, memo를 순차적으로 저장
        if (name != NULL && phone != NULL && memo != NULL) {
            strcpy(entries[count].name, name);
            strcpy(entries[count].phone, phone);
            strcpy(entries[count].memo, memo);
            count++;
        }// 유효한 값인 경우에만 저장 후 count 증가
    }
    fclose(file);
    return count;
    // 파일을 닫은 후 엔트리 개수 반환
}

// 특정 단어로 검색하여 일치하는 엔트리를 출력하는 함수
void search(struct Entry entries[], int count, const char *word) {
    int matchcount = 0;
	// 일치하는 항목의 개수 확인
    for (int i = 0; i < count; i++) {
        if (strstr(entries[i].name, word) != NULL || strstr(entries[i].phone, word) != NULL ||
            strstr(entries[i].memo, word) != NULL) {
            printf("%d %s %s %s\n", matchcount + 1, entries[i].name, entries[i].phone, entries[i].memo);
            matchcount++;
        }// 검색된 word를 통해 strstr함수로 찾아 해당 내용이 존재하면 출력 후 개수 증가
    }// 엔트리들을 순회하며 검색

    if (matchcount > 0) {
        printf("match found.\n");	// 개수가 1개라도 찾았다는 메시지 출력
    } else {
        printf("no match found.\n");	// 존재하지 않으면 찾기 못했다는 메시지 출력
    }
}

// 새로운 엔트리를 추가하는 함수
void add(struct Entry entries[], int *count, const char *name, const char *phone, const char *memo)
{
   if (*count == MIN_ENTRIES) {
        printf("Full\n");
        return;
    }// 파일의 끝에 도달하면 해당 메시지 출력

    strcpy(entries[*count].name, name);
    strcpy(entries[*count].phone, phone);
    strcpy(entries[*count].memo, memo);
    (*count)++;
    // 새롭게 추가된 엔트리를 strcpy 함수를 통해 위치에 맞게 추가 후 개수 증가
}

// 특정 엔트리를 삭제하는 함수
void delete(struct Entry entries[], int *count, const char* name) {
    int indices[MIN_ENTRIES];	// 삭제할 엔트리의 인덱스를 저장
    int indexcount = 0;		// 현재 개수

    // 이름에 해당하는 엔트리 인덱스 찾기
    for (int i = 0; i < *count; i++) {
        if (strstr(entries[i].name, name) != NULL) {
            indices[indexcount] = i;
            indexcount++;
        }// 이름이 일치하는 경우 indices 배열에 해당 인덱스 저장 후 현재 개수 증가
    }

    if (indexcount == 0) {
        printf("Entry not found.\n");
        return;
    }// 엔트리가 없는 경우


    for (int i = 0; i < indexcount; i++) {
        int index = indices[i];
        printf("%d %s %s %s\n", i + 1, entries[index].name, entries[index].phone, entries[index].memo);
    }// 삭제할 엔트리 출력

    printf("Which one? ");
    int selected;
    scanf("%d", &selected);
    // 삭제할 엔트리 선택

    if (selected > 0 && selected <= indexcount) {	// 유효한 범위 내에 있는지 확인
        int index = indices[selected - 1];		// 선택된 indices배열 index 변수에 저장

        // 선택된 엔트리 삭제
        for (int i = index; i < *count - 1; i++) {
            strcpy(entries[i].name, entries[i + 1].name);
            strcpy(entries[i].phone, entries[i + 1].phone);
            strcpy(entries[i].memo, entries[i + 1].memo);
        }// 선택된 인덱스부터 남아있는 엔트리들의 개수까지 한 칸씩 앞으로 옮김

        (*count)--;			// 현재 개수를 1만큼 줄임
        printf("Entry deleted.\n");

        // data.txt 파일에서 엔트리 삭제하기
        FILE* file = fopen("data.txt", "w");	// 쓰기 모드
        if (file == NULL) {
            printf("NULL\n");
        } else {
            for (int i = 0; i < *count; i++) {
                fprintf(file, "%s:%s:%s\n", entries[i].name, entries[i].phone, entries[i].memo);
            }// 파일이 비어있지 않다면 출력
            fclose(file);
        }
    } else {
        printf("Entry error.\n");
    }// 삭제할 엔트리가 범위 내에 없는 것이라면 해당 메시지 출력
}


// 모든 엔트리를 리스트 형태로 출력하는 함수
void list(struct Entry entries[], int count) {
    for(int i = 0; i < count; i++) {
        printf("%d %s %s %s\n", i + 1, entries[i].name, entries[i].phone, entries[i].memo);
    }// count 안에 포함되는 엔트리들을 출력
}

// qsort에서 사용될 비교 함수
int compare(const void *a, const void *b) {
    const struct Entry *entryA = (const struct Entry *)a;
    const struct Entry *entryB = (const struct Entry *)b;
    return strcmp(entryA->name, entryB->name);
}// 반환 값이 음수이면 A가 더 작은 순서, 양수이면 반대
 // 오름차순 정렬 수행

// Entry에 존재하는지 여부 파악하는 함수
int exists(const struct Entry entries[], int count, const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].name, name) == 0) {
            return 1;  // 해당 이름이 일치한다면 이미 존재하는 엔트리
        }
    }
    return 0;  // 존재하지 않는 엔트리
}

int main(int argc, char *argv[]) {
    struct Entry entries[MIN_ENTRIES];
    int count = load(entries, MIN_ENTRIES);

    if (argc == 1) {
        printf("type: ./tel [-adl] words\n");		// 명령어가 없는 경우 해당 메시지 출력
    } else {
        if (strcmp(argv[1], "-a") == 0 && argc == 5) {	// '-a'와 4개의 값이 주어진 경우
            printf("%s %s %s\n", argv[2], argv[3], argv[4]);	// 추가할 엔트리 확인
            printf("add? [Y/N]: ");
            char response;
            scanf("%c", &response);

            if (response == 'Y' || response == 'y') {
		if(exists(entries, count, argv[2])){
		    printf("Already exists.\n");
		}// 이미 해당 이름이 존재하는 경우
		else{
               	    add(entries, &count, argv[2], argv[3], argv[4]);	// add 함수 호출
                    printf("Entry added.\n");
		// 새로운 이름일 경우
                // data.txt 파일에 추가된 엔트리 저장하기
                FILE *file = fopen("data.txt", "a");
                if (file == NULL) {
                    printf("NULL\n");
                } else {
                    fprintf(file, "%s:%s:%s\n", argv[2], argv[3], argv[4]);
                    fclose(file);
                }
		}
            } else{
                printf("Entry not added.\n");
            }// 'Y' 또는 'y'가 아닌 다른 것을 입력했을 경우 해당 메시지 출력 후 종료
        } else if (strcmp(argv[1], "-d") == 0 && argc == 3) {	// '-d'와 1개의 값이 주어진 경우
            delete(entries, &count, argv[2]);			// delete 함수 호출
        } else if (strcmp(argv[1], "-l") == 0 && argc == 2) {	// '-l'인 경우
            qsort(entries, count, sizeof(struct Entry), compare);
	    // 사전순으로 엔트리 정렬
            list(entries, count);				// list 함수 호출
        } else { // 옵션이 없는 경우
            char *word = argv[1];
            search(entries, count, word);			// search 함수 호출
        }
    }
    return 0;
}
