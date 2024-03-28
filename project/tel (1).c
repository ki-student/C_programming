#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <locale.h>

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
        printw("NULL\n");
	refresh();
        return 0;
    }// 파일이 비어있으면 NULL 출력

    int count = 0;				// 엔트리 수 계산
    char line[MIN_ENTRIES];			// 배열의 개수 초기화
    while (fgets(line, sizeof(line), file) != NULL) {	// 한 줄씩 읽으며 조건에 충족하는지 확인
	 if (count == minEntries) {
        	 printw("END\n");
		 refresh();
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
            printw("%d %s %s %s\n", matchcount + 1, entries[i].name, entries[i].phone, entries[i].memo);
	    refresh(); 
            matchcount++;
        }// 검색된 word를 통해 strstr함수로 찾아 해당 내용이 존재하면 출력 후 개수 증가
    }// 엔트리들을 순회하며 검색

    if (matchcount > 0) {
	start_color();			// 해당 조건 만족하면 글자색 변경
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        printw("match found.\n");	// 개수가 1개라도, 찾았다는 메시지 출력
        attroff(COLOR_PAIR(1));
	refresh();
    } else {
	start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        printw("no match found.\n");	// 존재하지 않으면 찾지 못했다는 메시지 출력
        attroff(COLOR_PAIR(1));
	refresh();
    }
}

// 새로운 엔트리를 추가하는 함수
void add(struct Entry entries[], int *count, const char *name, const char *phone, const char *memo)
{
   if (*count == MIN_ENTRIES) {
        printw("Full\n");
	refresh();
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
	start_color();				// 조건 만족 시 글자색 변경
        init_pair(1, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        printw("Entry not founded.\n");		// 찾고자 하는 엔트리가 없으면 해당 메시지 출력
        attroff(COLOR_PAIR(1));
	refresh();
        return;
    }// 엔트리가 없는 경우


    for (int i = 0; i < indexcount; i++) {
        int index = indices[i];
        printw("%d %s %s %s\n", i + 1, entries[index].name, entries[index].phone, entries[index].memo);
    }// 삭제할 엔트리 출력
    refresh();
    printw("Which one? ");
    refresh();
    int selected;
    scanw("%d", &selected);
    clear();
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
	start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        printw("Entry deleted.\n");
        attroff(COLOR_PAIR(1));
	refresh();

        // data.txt 파일에서 엔트리 삭제하기
        FILE* file = fopen("data.txt", "w");	// 쓰기 모드
        if (file == NULL) {
            printw("NULL\n");
	    refresh();
        } else {
            for (int i = 0; i < *count; i++) {
                fprintf(file, "%s:%s:%s\n", entries[i].name, entries[i].phone, entries[i].memo);
            }// 파일이 비어있지 않다면 출력
            fclose(file);
        }
    } else {
	start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        attron(COLOR_PAIR(1));
        printw("Entry error.\n");
        attroff(COLOR_PAIR(1));
	refresh();
    }// 삭제할 엔트리가 범위 내에 없는 것이라면 해당 메시지 출력
}


// 모든 엔트리를 리스트 형태로 출력하는 함수
void list(struct Entry entries[], int count) {
    for(int i = 0; i < count; i++) {
        printw("%d %s %s %s\n", i + 1, entries[i].name, entries[i].phone, entries[i].memo);
    }// count 안에 포함되는 엔트리들을 출력
    refresh();
}

// qsort에서 사용될 비교 함수
int compare(const void *a, const void *b) {
    const struct Entry *entryA = (const struct Entry *)a;
    const struct Entry *entryB = (const struct Entry *)b;
    return strcmp(entryA->name, entryB->name);
}// 반환 값이 음수이면 A가 더 작은 순서, 양수이면 반대
 // 오름차순 정렬 수행

// Entry에 존재하는지 여부 파악하는 함수
int exists(const struct Entry entries[], int count, const char *phone) {
    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].phone, phone) == 0) {
            return 1;  // 해당 번호가 일치한다면 이미 존재하는 엔트리
        }
    }
    return 0;  // 존재하지 않는 엔트리
}

int main() {
    struct Entry entries[MIN_ENTRIES];
    int count = load(entries, MIN_ENTRIES);
    setlocale(LC_ALL, "ko_KR.utf-8");		// 한글 사용을 위해 로케일 설정
    initscr();  // NCURSES 초기화
    cbreak();   // 버퍼를 없애 입력을 즉시 받음
    keypad(stdscr, TRUE);  // 화살표 키나 함수 키 입력 가능

    int choice, h, w;		// 선택 및 가로세로 길이에 대한 변수
    WINDOW *ui;			// 메뉴 UI표시할 구조체 생성
    h = 8, w = 30;

    while (1) {
        clear();		// 화면 초기화
	ui = newwin(h, w, 0, 0);	// 새로운 창 (0, 0)에 생성
	refresh();			// 새로고침
	box(ui, '*', '*');		// 상자 생성
        mvwprintw(ui, 0, 12, "Menu");
        mvwprintw(ui, 1, 2, "1. Search");
        mvwprintw(ui, 2, 2, "2. Add Entry");
        mvwprintw(ui, 3, 2, "3. Delete Entry");
        mvwprintw(ui, 4, 2, "4. List Entries");
        mvwprintw(ui, 5, 2, "5. Quit");
        mvwprintw(ui, 6, 2, "Enter your choice: ");
	// mvwprintw함수를 사용하여 메뉴 항목과 그에 따른 메시지 출력
	move(6, 21);			// 커서 이동
        wrefresh(ui);			// ui상자 새로고침
        refresh();

	scanw("%d", &choice);		// 메뉴 번호를 입력받음
	clear();


        if (choice == 1) {		// 검색 키워드 입력 후 search함수 호출
            char word[30];
            printw("Enter the search keyword: ");
            refresh();
            scanw("%s", word);
            search(entries, count, word);
        } else if (choice == 2) {	// 이름 전화번호 메모 순으로 입력받은 후 존재 여부 파악
            char name[30], phone[20], memo[40];
            printw("Enter to add(name phone memo): ");
            refresh();
            scanw("%s %s %s", name, phone, memo);
	    if(exists(entries, count, phone)){
		    start_color();
	            init_pair(1, COLOR_RED, COLOR_BLACK);
	            attron(COLOR_PAIR(1));
	            printw("Already exists.\n");	// 존재하면 해당 메시지 출력
	            attroff(COLOR_PAIR(1));
	    }else{
		    add(entries, &count, name, phone, memo);
		    start_color();
	            init_pair(1, COLOR_GREEN, COLOR_BLACK);
	            attron(COLOR_PAIR(1));
	            printw("Entry added.\n");
	            attroff(COLOR_PAIR(1));

		    FILE *file = fopen("data.txt", "a");
		    if(file == NULL){
			    printw("NULL\n");
		    }else{
			    fprintf(file, "%s:%s:%s\n", name, phone, memo);
		    }fclose(file);
	    }// 존재하지 않으면 엔트리 추가 후 data.txt에 추가
	    refresh();
        } else if (choice == 3) {	// 삭제할 이름 입력받은 후 delete함수 호출
            char name[30];
            printw("Enter the name to delete: ");
            refresh();
            scanw("%s", name);
            delete(entries, &count, name);
        } else if (choice == 4) {	// 현재 전화번호부에 있는 엔트리 사전순으로 출력
	    qsort(entries, count, sizeof(struct Entry), compare);	// 사전순 정렬
            list(entries, count);
        } else if (choice == 5) {	// 종료
            break;
        } else {			// 5번이 아닌 choice 입력 시 해당 메시지 출력
	    start_color();
	    init_pair(1, COLOR_RED, COLOR_BLACK);
	    attron(COLOR_PAIR(1));
            printw("Index error. Please try again.\n");
	    attroff(COLOR_PAIR(1));
        }
	attron(A_BLINK | A_BOLD);
	printw("press any key to exit.");
	attroff(A_BLINK | A_BOLD);
	// attr함수를 사용하여 해당 메시지 굵기와 깜박임 설정
	refresh();
        getch();			// 메뉴 선택 화면으로 돌아감
    }

    endwin();  // NCURSES 종료

    return 0;
}
