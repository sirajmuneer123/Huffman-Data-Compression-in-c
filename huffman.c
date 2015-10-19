#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 1000

struct code{
	char ch;
	char *pat;
	struct code *next;
};

struct node{
	char ch;
	int count;
	struct node *next;
	struct node *left;
	struct node *right;
};

int length(struct node *head);
void printCode(struct code *head);
void printList(struct node *head);
void sortFreq(struct node **href);
void buildTree(struct node **href);
char *copy(char *s,int index,char ch);
char *encoding(char *msg,struct code *code);
char *decoding(struct node *head,char *emsg);
char *returnCode(struct code *node,char m);
struct node *frequency(struct node *head, char ch);
struct node *combineNode(struct node *leastTwo);
void push(struct code **cref,char *pat,char ch);
void sortedInsert(struct node **href,struct node *newnode);
void spliting(struct node *head,struct node **aref,struct node **bref);
void assignCodes(struct node *node,struct code **cref,char *pat,int l);


int main()
{
	char *ch;
	char pat[MAX];
	char line[MAX];
	char *eMessage,*dMessage;
	struct node *head = NULL;
	struct code *code = NULL;

	printf("Enter the string: ");
	fgets(line,sizeof(line),stdin);
	line[strlen(line)-1] = '\0';

	for(ch=line;*ch;++ch)		
		head = frequency(head,*ch);

	printf("Determining Relative Frequencies.\n");
	printList(head);

	sortFreq(&head);

	printf("\n\nSorting based on the count  \n");
	printList(head);

	buildTree(&head);

	printf("\n\nAssigning codes to the Characters\n");
	assignCodes(head,&code,pat,0);
	printCode(code);

	eMessage = encoding(line,code);
	printf("\nmessage after encoding : %s\n",eMessage);

	dMessage = decoding(head,eMessage);
	printf("message after Decoding: %s\n\n",dMessage);
	return;
}

struct node *frequency(struct node *head, char ch)
{
	if(head == NULL){
		head = malloc(sizeof(struct node));
		head->ch = ch;
		head->count = 1;
		head->next = NULL;
		head->left = NULL;
		head->right = NULL;
	}
	else if(head->ch == ch)
		head->count += 1;
	else
		head->next = frequency(head->next,ch);

	return head;
}

void printList(struct node *head)
{
	while(head != NULL){
		printf("\n%c : %d ",head->ch,head->count);
		head = head->next;
	}
}

void sortFreq(struct node **href)	
{
	struct node *current,*sortlist,*nextNode;

	sortlist=NULL;
	current=*href;

	while(current!=NULL){
		nextNode = current->next;
		sortedInsert(&sortlist,current);
		current=nextNode;
	}

	*href=sortlist;
}
		
void sortedInsert(struct node **href,struct node *newnode)
{
	struct node *current,*prev_node;

	current=*href;
	prev_node=NULL;

	while(current!=NULL){
		if( (current->count > newnode->count) || ((current->count == newnode->count) && (current->ch > newnode->ch)) )
			break;
		prev_node=current;
		current=current->next;
	}

	if(prev_node == NULL){			
			newnode->next=*href;
			*href=newnode;
	}
	else{							
		newnode->next = current;
		prev_node->next = newnode;
	}
}

void buildTree(struct node **href)
{
	struct node *leastTwo,*theRest;
	struct node *newNode;

	while(length(*href) > 1){
		spliting(*href,&leastTwo,&theRest);
		newNode = combineNode(leastTwo);
		sortedInsert(&theRest,newNode); 
        	*href=theRest;
	}
}

int length(struct node *head)
{
	int len = 0;

	while(head != NULL){
		len++;
		head = head->next;
	}
	return len;
}

void spliting(struct node *head,struct node **aref,struct node **bref)
{
	*aref = head;
	*bref = (*aref)->next->next;
	(*aref)->next->next = NULL;
}

struct node *combineNode(struct node *leastTwo)
{
	struct node *newNode = malloc(sizeof(struct node));

	newNode->count = leastTwo->count + leastTwo->next->count;
	newNode->ch = '~';
	newNode->next = NULL;
	newNode->left = leastTwo;
	newNode->right = leastTwo->next;

	leastTwo->next->next = NULL;

	return newNode;
}

void assignCodes(struct node *node,struct code **cref,char *pat,int l)
{
	if( (node->left == NULL) || (node->right == NULL) )
		push(cref,copy(pat,l,'\0'),node->ch);
	else{
		assignCodes(node->left,cref,copy(pat,l,'0'),l+1);
		assignCodes(node->right,cref,copy(pat,l,'1'),l+1);
	}
	
}

void push(struct code **cref,char *pat,char ch)
{
	struct code *newNode = malloc(sizeof(struct code));

	newNode->pat = strdup(pat);
	newNode->ch = ch;
	newNode->next = *cref;
	*cref = newNode;
}

char *copy(char *s,int index,char ch)
{
	*(s+index) = ch;
	return s;
}

void printCode(struct code *head)
{
	while(head != NULL){
		printf("\n%c : %s",head->ch,head->pat);
		head = head->next;
	}
}


char *encoding(char *msg,struct code *node)
{
	char *m;
	char emsg[MAX]={'\0'};
	char *p;

	for(m=msg;*m;++m){
		strcat(emsg,returnCode(node,*m));
	
	}
	return strdup(emsg);
}

char *returnCode(struct code *node,char m)
{
	while(node != NULL){
		if(node->ch == m)
			return node->pat;
		node = node->next;
	}
}

char *decoding(struct node *head,char *emsg)
{
	char *m;
	char *outp;
	char output[MAX];
	struct node *node;

	node = head;
	outp = output;
	for(m=emsg;*m;++m){
		if(*m == '0')
			node = node->left;
		else
			node = node->right;

		if( (node->left == NULL) || (node->right == NULL) ){
			*outp++ = node->ch;
			node = head;
		}
	}
	*outp = '\0';

	return strdup(output);
}
