//  배열   리스트를   사용한   단어   빈도
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <time.h>
#define MAX_FNAME           100
#define MAX_WORDS       10000
#define MAX_WORD_SIZE   100
#define TRUE            1
#define FALSE           0
//  데이터 형식 
typedef struct {
char    word[MAX_WORD_SIZE];                 //  키필드 
int     count;
} element;
//  디스크   파일로부터   한   개   단어를   읽어   배열   word에   저장
//  성공적으로   읽었으면   TRUE,  그렇지   않으면(파일   끝) FALSE  리턴 
int getword(FILE *fd, char *word)
{
char    ch; int    i;
do {
ch = getc(fd);
if (ch == EOF) return FALSE;
} while (!isalpha(ch)); //  첫번째   알파벳   문자가   나올때까지   읽는다. 
i = 0;
do {    //  단어   1개를   읽는다.
ch = tolower(ch);   //  소문자로   변환 
word[i++] = ch;
ch = getc(fd);
} while (isalpha(ch));  //  알파벳   문자이면   계속   읽는다. 
word[i] = '\0';
return TRUE; }
// wlist  배열에   단어가   없는   경우   추가, 이미   있는   경우   빈도(count)만   갱신 
int update_wlist(int *max_words, element wlist[], char *word)
{
int     i;
// word가   wlist에   들어   있는지   찾아서   count  증가 
for (i = 0; i < *max_words; i++) {
if (strcmp(wlist[i].word, word) == 0) { wlist[i].count++;
return wlist[i].count; }

}
if (i >= MAX_WORDS) {
fprintf(stderr, "Error: too many words ...\n"); exit(1);
}
//  새   word인   경우   wlist에   추가 
strcpy(wlist[i].word, word); wlist[i].count = 1;
(*max_words)++; return wlist[i].count;
}
//  배열   리스트를   사용하는   단어   빈도   계산 
void main()
{
FILE        *fd;
element     wordlist[MAX_WORDS]; char        fname[MAX_FNAME]; char        w[MAX_WORD_SIZE];
int         flag;
int         wnum, cnt;
int         i;
clock_t     start, finish; double     duration;
printf("파일   이름: "); scanf("%s", fname);
if( (fd = fopen(fname,"r")) == NULL ) { fprintf(stderr, "파일을   열수없습니다.\n"); return;
}
wnum = 0; cnt = 0;
start = clock();    //  시간   측정   시작 
do {
flag = getword(fd, w);   //  단어   1개   읽기 
if (flag == FALSE)
break;
cnt = update_wlist(&wnum, wordlist, w); // wordlist update //        printf(" %s(%d)", w, cnt);
} while(1);
finish = clock();   //  시간   측정   종료
duration = (double)(finish - start) / CLOCKS_PER_SEC; for (i = 0; i < wnum; i++) {
printf("%s %d ", wordlist[i].word, wordlist[i].count); }
printf("\n%.6f  초입니다.\n", duration); }