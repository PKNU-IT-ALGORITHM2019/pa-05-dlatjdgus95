#pragma warning(disable:4996)
#include<stdio.h>
#include<string.h>
#define NWORDS 20000
#define MAX 1200
#define BUFFER_LENGTH 30

typedef struct word {
	char name[BUFFER_LENGTH];
	char class[BUFFER_LENGTH];
	char explain[MAX];        

}Word;
typedef struct tree {
	struct tree *left;
	struct tree *p;
	struct tree *right;
	Word *key;
}Tree;
Word* dict[NWORDS];
int n = 0;
void Insert_BST(Tree **root, Word *key);
Tree *findWord_In_BTS(Tree *x, char word[]);
Tree *Delete_BTS(Tree **root, Tree *node);
Tree *BTS_Successor(Tree *node);
Tree *find_min(Tree *root);
int Delete_words_in_file(Tree **root);


int main(void) {
	FILE *fp = fopen("shuffled_dict.txt", 'r');
	Tree *root = NULL;
	while (!feof(fp)) {
		char str[MAX];
		fgets(str, MAX, fp);
		dict[n] = (Word *)malloc(sizeof(Word));
		char *token = strtok(str, "()");
		strcpy(dict[n]->name, token);
		dict[n]->name[strlen(dict[n]->name) - 1] = '\0';
		token = strtok(NULL, "()");
		strcpy(dict[n]->class, token);
		token = strtok(NULL, "()");
		if (token == NULL) {
			char str[MAX];
			strcpy(str, dict[n]->class);
			strcpy(dict[n]->explain, str + 1);
			strcpy(dict[n]->class, "\0");
		}
		else {
			strcpy(dict[n]->explain, token + 1);
		}
		dict[n]->explain[strlen(dict[n]->explain) - 1] = '\0';
		Insert_BST(&root, dict[n]);
	}
	char command[BUFFER_LENGTH];
	while (1) {
		printf("$ ");
		scanf("%s", command);
		if (strcmp(command, "size") == 0) printf("%d\n", n);
		else if (strcmp(command, "find") == 0) {
			char word[BUFFER_LENGTH];
			scanf("%s", word);
			Tree *tmp = findWord_In_BTS(root, word);
			if (tmp == NULL)printf("Not Found.\n");
			else printf("%s\n", tmp->key->explain);
		}
		else if (strcmp(command, "add") == 0) {
			Word *word = (Word *)malloc(sizeof(Word));
			gets(word->name);
			printf("word: ");
			gets(word->name);
			printf("class: ");
			gets(word->class);
			printf("meaning: ");
			gets(word->explain);
			Insert_BST(&root, word);
		}
		else if (strcmp(command, "delete") == 0) {
			char word[BUFFER_LENGTH];
			scanf("%s", word);
			Tree *tmp = findWord_In_BTS(root, word);
			if (tmp == NULL)printf("Not found.\n");
			else {
				Tree *tmp2 = Delete_BTS(&root, tmp);
				printf("%s Deleted successfully.\n", tmp2->key->name);
				free(tmp2);
			}
		}
		else if (strcmp(command, "deleteall") == 0) {
			int N = Delete_words_in_file(&root);
			printf("%d words were deleted successfully.\n", N);
		}
		else if (strcmp(command, "exit") == 0) break;
		else printf("Error Command.\n");
	}
	fclose(fp);
	getchar();
	return 0;
}

void Insert_BST(Tree **root, Word *key) {
	Tree *x = *root;
	Tree *y = NULL;
	while (x != NULL) {
		y = x;
		if (strcmp(x->key->name, key->name)>0) x = x->left;
		else x = x->right;
	}
	Tree *tmp = (Tree *)malloc(sizeof(Tree));
	tmp->key = key;
	tmp->left = NULL;
	tmp->right = NULL;
	tmp->p = y;
	if (y == NULL) *root = tmp;
	else if (strcmp(y->key->name, key->name)> 0) y->left = tmp;
	else y->right = tmp;
	n++;
	return;
}
Tree *findWord_In_BTS(Tree *x, char word[])
{
	if (x == NULL || strcmp(word, x->key->name) == 0) return x;
	if (strcmp(word, x->key->name) < 0) return findWord_In_BTS(x->left, word);
	else return findWord_In_BTS(x->right, word);
}

Tree *Delete_BTS(Tree **root, Tree *node) {
	Tree *y; 
	if (node->left == NULL || node->right == NULL) y = node;
	else y = BTS_Successor(node);

	Tree *x; 
	if (y->left != NULL) x = y->left;
	else x = y->right;
	if (x != NULL) x->p = y->p;
	if (y->p == NULL) *root = x; 
	else if (y == y->p->left) y->p->left = x;
	else y->p->right = x;

	if (y != node) node->key = y->key;
	n--;
	return y;
}

Tree *BTS_Successor(Tree *node) {
	if (node->right != NULL) return find_min(node->right);
	Tree *p = node->p;
	while (node != NULL && node == p->right) {
		node = p;
		p = node->p;
	}
	return p;
}

Tree *find_min(Tree *root) {
	while (root->left != NULL) {
		root = root->left;
	}
	return root;
}

int Delete_words_in_file(Tree **root)
{
	int N = 0;
	char filename[BUFFER_LENGTH];
	char deleteWords[10000][BUFFER_LENGTH];
	scanf("%s", filename);
	FILE *in_fp = fopen(filename, "r");
	while (fscanf(in_fp, "%s", deleteWords[N]) != EOF) {
		Tree *tmp = findWord_In_BTS(root, deleteWords[N]);
		if (tmp != NULL) {
			Tree *tmp2 = Delete_BTS(root, tmp);
			free(tmp2);
		}
		N++;
	}
	fclose(in_fp);
	return N;
}