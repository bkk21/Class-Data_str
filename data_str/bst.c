//  이진   탐색   트리를   사용한   단어   빈도
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <time.h>
#define MAX_FNAME 100
#define MAX_WORDS 10000
#define MAX_WORD_SIZE 100
#define TRUE 1
#define FALSE 0
//  데이터   형식
typedef struct {
    char      word[MAX_WORD_SIZE]; //단어
    int     count;
} element;

//  노드의   구조
typedef struct TreeNode {
    element key;
    struct TreeNode* left, * right;
} TreeNode;

//  만약    e1 < e2 ->  음수   반환   (시스템에   따라: -1) 
//  만약    e1 == e2 -> 0   반환
//  만약 e1 > e2 ->  양수 반환 (시스템에 따라: 1)
int compare(element e1, element e2) {
    return strcmp(e1.word, e2.word); //두 단어를 비교해서 값을 반환 
}

//  디스크   파일로부터   한   개   단어를   읽어   배열   word에   저장
//  성공적으로   읽었으면   TRUE,  그렇지   않으면(파일   끝) FALSE  리턴 
int getword(FILE* fd, char* word) {
    char ch;
    int i;

    do {
        ch = getc(fd);
        if (ch == EOF)
            return FALSE;
    } while (!isalpha(ch)); //  첫번째   알파벳   문자가   나올때까지   읽는다. 

    i = 0;
    do {    //  단어   1개를   읽는다.
        ch = tolower(ch);   //  소문자로   변환 
        word[i++] = ch;
        ch = getc(fd);
    } while (isalpha(ch));  //  알파벳   문자이면   계속   읽는다. 
    
    word[i] = '\0';

    return TRUE;
}

TreeNode* new_node(element elem) {
    TreeNode* T;

    T = (TreeNode*)malloc(sizeof(TreeNode));

    T->key = elem;
    T->key.count = 1; // 1로 초기화해서 중복 상황 대비
    T->left = NULL;
    T->right = NULL;

    return T;

}

// Binary Search Tree에   삽입하려는   단어가   없는   경우   추가, 이미   있는   경우에는   빈도(count)만   증가 
TreeNode* update_BST(TreeNode* root, element key)
{
    //  교재   프로그램   insert_node()  참고하여   작성
    if (root == NULL)
        return new_node(key); // 트리가 공백이면 새로운 노드를 반환

    else if (compare(key, root->key) == 0) // 이미 저장되어 있으면 count만 증가 
        root->key.count++;

    else if (compare(key, root->key) < 0) // 작으면 왼쪽으로 탐색 후 저장
        root->left = update_BST((root->left), key);

    else
        root->right = update_BST((root->right), key); //크면 오른쪽 탐색 후 저장 

    return root; // 루트 포인터 반환
}

// inorder traversal
void inorder(TreeNode* t) {
    //  방문한   노드의   단어와   count  출력 
    if (t != NULL) {
        inorder(t->left);
        printf("%s %d ", t->key.word, t->key.count);
        inorder(t->right);
    }

}

//  노드   개수   세기
int get_node_count(TreeNode* node) {
    int count = 0; //0으로 초기화

    //node가 비어있지 않으면 루트 노드 + 왼쪽 자식 노드 + 오른쪽 자식 노드
    if (node != NULL)
        count = 1 + get_node_count(node->left) + get_node_count(node->right);

    return count; //count값 반환 
}

//  최대값   return 
int max(int a, int b) {
    return (a > b) ? a : b;
}

//  트리   높이   계산
int get_height(TreeNode* node) {
    int height = 0;

    //노드가 비어있지 않으면 루트 노트 + 왼쪽 자식 노드와 오른 쪽 자식 노드의 높이 중 더 큰 값 
    if (node != NULL)
        height = 1 + max(get_height(node->left), get_height(node->right));

    return height; // height 값 반환
}

// Binary Search Tree를   사용하여   단어   빈도   파악 
void main()
{
   FILE* fd;
   element e;
   TreeNode* root = NULL;
   TreeNode* tmp;
   char fname[MAX_FNAME];
   char w[MAX_WORD_SIZE]; // 읽어들인 단어
   int flag; // 파일 끝인지 여부
   clock_t start, finish;
   double duration;

   printf("파일 이름: ");
   scanf("%s", fname);
   if ((fd = fopen(fname, "r")) == NULL) {
      fprintf(stderr, "파일을 열수없습니다.\n");
      return;
   }
   start = clock(); // 시간 측정 시작
   do {

      flag = getword(fd, w);

      if (flag == FALSE)
         break;
      strcpy(e.word, w);
      root = update_BST(root, e);

   } while (1);

   finish = clock(); // 시간 측정 종료
   duration = (double)(finish - start) / CLOCKS_PER_SEC; // 수행 시간 계산
   inorder(root); //트리 탐색 후 출력
   printf("\n노드 개수 = %d\n", get_node_count(root)); // 노드 개수 출력
   printf("트리 높이 = %d\n\n", get_height(root)); // 트리 높이 출력
   printf("%.6f 초입니다.\n", duration); // 수행시간 출력
   fclose(fd); //파일 닫기
}
