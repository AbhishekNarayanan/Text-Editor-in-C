#include<stdio.h>            //including header files    //stdio.h used for input or output operations
#include<string.h>                            //used for strcpy function
#include<stdlib.h>                            //used for system("clear") function
#include<termios.h>                            //used for getch function
#include<unistd.h>                            //used for getch function

#include <stdbool.h>
 
#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])
 
// Alphabet size (# of symbols)
#define ALPHABET_SIZE (26)
 
// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')
 
// trie node
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
 
    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
};
 

struct dll                    //definition of the structure double linked list
{
char s[200];                            //character array
int index;
struct dll*prev,*next;            //pointers to next and previous nodes
};
typedef struct oper{
char op;
char sent[200];
struct oper *next;
struct dll *t;
}stack;


int getch()                            //getch function definition
{
struct termios oldt,
newt;
int ch;
tcgetattr( STDIN_FILENO, &oldt );
newt = oldt;
newt.c_lflag &= ~( ICANON | ECHO );
tcsetattr( STDIN_FILENO, TCSANOW, &newt );
ch = getchar();
tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
return ch;
}


void editcommands(void);                    //function prototypes
void addline(struct dll *temp);
void inp(void);
void printlist(void);
void closer(void);
void edit(void);
void addnode(char t[],struct dll *q);
void delnode(struct dll *p);
void clealist(void);
void editnode(struct dll *p);
void save(void);

struct dll *head;                        //header node declaration
char file[20];

FILE *fp=NULL;                            //file pointer declaration
stack *opstack,*operation,*ops,*ops2,*opstack2;

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode = NULL;
 
    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
 
    if (pNode)
    {
        int i;
 
        pNode->isEndOfWord = false;
 
        for (i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }
 
    return pNode;
}
 
// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;
 
    struct TrieNode *pCrawl = root;
 
    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();
 
        pCrawl = pCrawl->children[index];
    }
 
    // mark last node as leaf
    pCrawl->isEndOfWord = true;
}
 
// Returns true if key presents in trie, else false
bool search(struct TrieNode *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;
 
    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
 
        if (!pCrawl->children[index])
            return false;
 
        pCrawl = pCrawl->children[index];
    }
 
    return (pCrawl != NULL && pCrawl->isEndOfWord);
}
 

struct TrieNode *root;

main()
{
char c;

head=(struct dll*)malloc(sizeof(struct dll));            //header node memory allocation
(head->next)=(head->prev)=NULL;                //initialization
(head->index)=0;
FILE *fp=fopen("dict.txt","r");
    char words[26];
    root=getNode();
    int i=0;
    while(!feof(fp))
    {
        fgets(words,26,fp);
        words[strlen(words)-1]='\0';
        //printf("%s\n",words);
        insert(root, words);
    }
while(1)                //infinite while loop for editing multiple number of tiles
{

system("clear");                        //clearing the screen

//Displaying editor options
printf("\nThis Editor provides the following options \n");
printf("R :opens a file and reads it into a buffer\n   If file doesnot exist creates a new file for editing\n");
printf("E :edit the currently open file\n");
printf("X :closes the current file and allows you to open another file\n");
printf("Q :quit discarding any unsaved changes\n");

c=getch();                            //taking user input
switch(c)                            //testing with switch
{
case 'r' :
case 'R' :
inp();
break;
case 'e' :
case 'E' :
edit();
break;
case 'x' :
case 'X' :
closer();
break;
case 'q' :
case 'Q' :
system("clear");
exit(1);
break;
}
}
}                                //end of main



void addnode(char t[],struct dll *q)        //function to add a new node after a node q
{
struct dll*p=(struct dll*)malloc(sizeof(struct dll));
struct dll *temp=q->next;
strcpy(p->s,t);
p->prev=q;
p->next=q->next;

if((q->next)!=NULL)        //adding the node to the list by manipulating pointers accordingly
{
((q->next)->prev)=p;
while(temp!=NULL)
{
(temp->index)++;        //incrementing the index of the later nodes

temp=temp->next;
}
}
q->next=p;
p->index = q->index + 1;                    //setting the index of the new node
}



void delnode(struct dll *p)                    //function to delete a node
{
struct dll *temp=p->next;
((p->prev->next))=p->next;
if(p->next!=NULL)
{
((p->next)->prev)=p->prev;
while(temp!=NULL)
{
(temp->index)--;        //decrementing the index of the later nodes

temp=temp->next;
}
}
free(p);                        //freeing ht memory of the deleted node
}



void clearlist(void)                        //function to clear the list
{
while(head->next!=NULL)
delnode(head->next);                    //deleting all nodes except head
}



void editnode(struct dll *p)                    //function to edit a line
{
strcpy(operation->sent,p->s);
printf("\nThe original line is :\n%s",p->s);
printf("\nEnter the new line :\n");
gets(p->s);                            //taking the new line input
printf("\nLine edited\n");
}


void printlist(void)            //function to print all the lines stored in the buffer
{
struct dll *temp=head;
system("clear");
while(temp->next!=NULL)
{
temp=temp->next;
printf("%d : ",temp->index);
char ch=temp->s[0];
char words[30];int j=0,i=1;
temp->s[strlen(temp->s)]=' ';
temp->s[strlen(temp->s)+1]='\0';
while(ch!='\0')
{
   if(ch==' ')
   {
      words[j]='\0';

      if(search(root,words)==0)
           printf("%s%s%s ","\e[4m",words,"\e[0m");
      else
           printf("%s ",words);
      j=0;
    }
    else
       words[j++]=ch;
    ch=temp->s[i++];
 }
printf("\n"); 
    
                //printing the lines on the screen
}

}



void closer(void)                //function to close the file orened for editing
{
if(fp==NULL)
return;
fclose(fp);
fp=NULL;
clearlist();                        //the list is also cleared at this point
}



void inp(void)
{
struct dll *buff=head;                        //temporaty variable
char c;
char buf[200];                            //array to store input line

if(fp!=NULL)                            //checking for files opened earlier
{
printf("\nThere is another file open it will be closed\ndo you want to continue ?(Y/N):");
c=getch();
if(c=='n'||c=='N')
return;
else
closer();
}

fflush(stdin);
printf("\nEnter the file name you want to open :");
scanf("%s",file);
getchar();
fflush(stdin);
clearlist();

fp=fopen(file,"r");                        //opening the specified file
if(fp==NULL)                         //checking if the file previously exists
{
printf("\nThe file doesnot exist do you want to create one?(Y/N) :");
c=getchar();
getchar();
if(c=='N'||c=='n')
return;
else
{
fp=fopen(file,"w");                //creating new file
edit();
return;
}
}

if(feof(fp))
return;

while((fgets(buf,201,fp))!=NULL)                //taking input from file
{
addnode(buf,buff);
buff=buff->next;
}
edit();                            //calling the edit function
}

void pushtostack(stack *operation)
{
	if(opstack==NULL)
	{
	opstack=operation;
	}
	else
	{
	operation->next=opstack;
	opstack=operation;
	}
}

void pushtostack2(stack *operation)
{
	if(opstack2==NULL)
	{
	opstack2=operation;
	}
	else
	{
	operation->next=opstack2;
	opstack2=operation;
	}
}

stack* popfromstack()
{
	stack *p=opstack;
	if(opstack!=NULL)
	opstack=opstack->next;
	return p;
}

stack* popfromstack2()
{
	stack *p=opstack2;
	if(opstack2!=NULL)
	opstack2=opstack2->next;
	return p;
}

void printstack()
{
	printf("\n");
	stack *st=opstack;
	while(st!=NULL)
	{
	printf("%c",st->op);
	st=st->next;
	}
	printf("\n");
}

void printstack2()
{
	printf("\n");
	stack *st=opstack2;
	while(st!=NULL)
	{
	printf("%c",st->op);
	st=st->next;
	}
	printf("\n");
}

void edit(void)                            //the edit function
{
	struct dll *temp=head->next;          //pionter used to mark the current position during traversing
	char c,d;

	system("clear");                        //clearing the screen

	if(fp==NULL)                        //checking for files previously open
	{
	printf("\nNo file is open\n");
	return;
	}

	printf("\nThe file contents will be displayed along with the line number\npress any key\n");
	getch();
	system("clear");
	printlist();                            //printing the entire buffered list
	if(temp!=NULL)
	printf("You are at line number %d",temp->index);    //printing the line number of control
	else
	temp=head;

	editcommands();                    //prints the list of commands available

	while(1)                        //infinite loop for multiple command usage
	{
		c=getch();
		operation=(stack*)malloc(sizeof(stack));
		operation->op=c;
		switch(c)                        //switch -->condition checkig
		{
		case 'c' :
		case 'C' :

		editnode(temp);            //edit the current line pointed to by temp
		break;

		case 'p' :
		case 'P' :                    //move to the previous line
		if(temp==head)
		{
		printf("\nFile empty");    //message displayed if list is empty
		break;
		}
		if(temp->prev!=head)
		{
		temp=temp->prev;
		printf("\nYou are at line number %d",temp->index);
		}
		else                //message display if already at first line

		printf("\nalready at first line");
		break;

		case 'n' :
		case 'N' :                    //move to the next line
		if(temp->next!=NULL)
		{
		temp=temp->next;
		printf("\nYou are at line number %d",temp->index);
		}
		else if(temp==head)
		printf("\nFile empty");        //message printed if list is empty
		else
		printf("\nalready at last line");//message printed if already at last line
		break;

		case 'a' :
		case 'A' :             //adding a new line after node ponted by temp
		addline(temp);
		if(temp->next!=NULL)                //addline function takes input and creates a new node via addnode function
		temp=temp->next;
		printlist();
		printf("\nYou are at line number %d",temp->index);
		break;

		case 'h' :
		case 'H' :                    //HELP command displays the list of available commmands
		system("clear");
		editcommands();                //notice that there is no break as after help the entire list is printed
		system("clear");

		case 'v' :
		case 'V' :                    //printing the entire list via printlist function
		printlist();
		printf("\nYou are at line number %d",temp->index);
		break;

		case 'D' :
		case 'd' :                    //deleting a line pointed to by temp
		if(temp==head)                //checking if list is empty
		{
		printf("\nFile empty\n");
		break;
		}
		temp=temp->prev;
		strcpy(operation->sent,temp->next->s);
		delnode(temp->next);            //deleting the node
		printf("\nLine deleted\n");
		printlist();                //printing the list
		if(temp->index)
		printf("\nYou are currently at line number %d",temp->index);
		if(((temp->prev)==NULL)&&(temp->next)!=NULL)
		temp=temp->next;
		else if((temp==head)&&((temp->next)==NULL))
		printf("\nFile empty");        //printing message if list is empty
		break;

		case 'X' :
		case 'x' :                    //exit the editor to main menu

		printf("\nDo you want to save the file before exiting?(y/n) :");

		d=getch();                //warning for saving before exit
		if(d=='y'||d=='Y')
		save();
		closer();
		return;
		break;

		case 's' :
		case 'S' :                    //saving and exitting
		save();
		closer();
		return;
		break;

                case 'f':
                case 'F':
                replace();
                break;

		case 'u':
		case 'U':

		ops=popfromstack();
			if(ops!=NULL)
			{
			pushtostack2(ops);
			char undoop=ops->op;
				switch(undoop)
				{

					case 'c' :
					case 'C' :
						strcpy(temp->s,ops->sent);           //edit the current line pointed to by temp
						break;

					case 'p' :
					case 'P' :if(temp->next!=NULL)
						{
							temp=temp->next;
							printf("\nYou are at line number %d",temp->index);
						}
						else if(temp==head)
							printf("\nFile empty");        //message printed if list is empty
						else
							printf("\nalready at last line");//message printed if already at last line
			
						break;                    //move to the previous line


					case 'n' :
					case 'N' :                    //move to the next line

							if(temp==head)
							{
							printf("\nFile empty");    //message displayed if list is empty
							break;
							}
							if(temp->prev!=head)
							{
							temp=temp->prev;
							printf("\nYou are at line number %d",temp->index);
							}
							else                //message display if already at first line
								printf("\nalready at first line");
			
						break;
					case 'a' :
					case 'A' :             //adding a new line after node ponted by temp
							if(temp==head)                //checking if list is empty
							{
							printf("\nFile empty\n");
							break;
							}
				
							temp=temp->prev;
							delnode(temp->next);            //deleting the node
							printf("\nLine deleted\n");
							printlist();                //printing the list
							if(temp->index)
								printf("\nYou are currently at line number %d",temp->index);
							if(((temp->prev)==NULL)&&(temp->next)!=NULL)
								temp=temp->next;
							else if((temp==head)&&((temp->next)==NULL))
								printf("\nFile empty");        //printing message if list is empty
						break;
					

					case 'D' :
					case 'd' :                    //deleting a line pointed to by temp
							
							addnode(ops->sent,temp);                        //ceatng the new node
							temp=temp->next;
							printlist();
							printf("\nYou are at line number %d",temp->index);
						break;
					}
				}
				else
					printf("cannot undo");
				break;

				
		case 'r':
		case 'R':
		{
		ops2=popfromstack2();
			if(ops2!=NULL)
			{
			char redoop=ops2->op;
				switch(redoop)
				{

					case 'c' :
					case 'C' :
						strcpy(temp->s,ops2->sent);           //edit the current line pointed to by temp
						break;

					case 'n' :
					case 'N' :if(temp->next!=NULL)
						{
							temp=temp->next;
							printf("\nYou are at line number %d",temp->index);
						}
						else if(temp==head)
							printf("\nFile empty");        //message printed if list is empty
						else
							printf("\nalready at last line");//message printed if already at last line
			
						break;                    //move to the previous line


					case 'p' :
					case 'P' :                    //move to the next line

							if(temp==head)
							{
							printf("\nFile empty");    //message displayed if list is empty
							break;
							}
							if(temp->prev!=head)
							{
							temp=temp->prev;
							printf("\nYou are at line number %d",temp->index);
							}
							else                //message display if already at first line
								printf("\nalready at first line");
			
						break;
					case 'd' :
					case 'D' :             //adding a new line after node ponted by temp
							if(temp==head)                //checking if list is empty
							{
							printf("\nFile empty\n");
							break;
							}
				
							temp=temp->prev;
							delnode(temp->next);            //deleting the node
							printf("\nLine deleted\n");
							printlist();                //printing the list
							if(temp->index)
								printf("\nYou are currently at line number %d",temp->index);
							if(((temp->prev)==NULL)&&(temp->next)!=NULL)
								temp=temp->next;
							else if((temp==head)&&((temp->next)==NULL))
								printf("\nFile empty");        //printing message if list is empty
						break;
					

					case 'a' :
					case 'A' :                    //deleting a line pointed to by temp

							addnode(ops2->sent,temp);                        //ceatng the new node
							temp=temp->next;
							printlist();
							printf("\nYou are at line number %d",temp->index);
						break;
					}
				}
				else
					printf("cannot do redo");
				break;
			}
			}
		if(operation->op!='U'&&operation->op!='u'&&operation->op!='s'&&operation->op!='S'&&operation->op!='F'&&operation->op!='f'&&operation->op!='v'&&operation->op!='V'&&operation->op!='h'&&operation->op!='H')
		pushtostack(operation);
	}

}




void addline(struct dll *temp)                    //adding a new line via input from user
{
char buff[200];
printf("\nenter the new line :\n");
gets(buff);
strcpy(operation->sent,buff);                         //taking the new line
addnode(buff,temp);                        //ceatng the new node
}


void save(void)                            //function to save the file
{
struct dll *temp=head->next;
fclose(fp);
fp=fopen(file,"w");                        //opening the file in write mode

while(temp!=NULL)
{
fprintf(fp,"%s%s",temp->s,"\n");                    //writing the linked list contents to file
temp=temp->next;
}

}


void editcommands(void)                        //function to print the list of editer commands
{
printf("\nEditor commands\n");
printf("The edit menu provides the following options \n");
printf("C :edit the current line\n");
printf("F : Find and replace a word\n");
printf("P :move one line up\n");
printf("N :move one line down\n");
printf("D :delete the current line\n");
printf("V :display the contents of the buffer\n");
printf("A :add a line after the line at which you are navigating\n");
printf("S :save changes and exit to main menu\n");
printf("X :exit discarding any changes\n");
printf("H :show the list of commands\n");
printf("U :undo last change made via editing, cursor movement, additions, deletions or display\n");
printf("R :redo the change undone last\n");
getch();
}


char *rep_str(const char *s, const char *old, const char *new1)
{
        char *ret;
        int i, count = 0;
        int newlen = strlen(new1);
        int oldlen = strlen(old);
        for (i = 0; s[i] != '\0'; i++)    
        {
            if (strstr(&s[i], old) == &s[i]) 
            {
                count++;
                i += oldlen - 1;
            }
        }
        ret = (char *)malloc(i + count * (newlen - oldlen));
        if (ret == NULL)
            exit(EXIT_FAILURE);
        i = 0;
        while (*s)
        {
            if (strstr(s, old) == s) //compare the substring with the newstring
            {
                strcpy(&ret[i], new1);
                i += newlen; //adding newlength to the new string
                s += oldlen;//adding the same old length the old string
            }

            else
            ret[i++] = *s++;
        }
        ret[i] = '\0';
        return ret;
}

     

replace()
{	
    	struct dll *temp=head->next;
    	if(temp!=NULL)
			printf("You are at line number %d",temp->index);    //printing the line number of control
		else
		{
			printf("Nothing present in the file");
			return;
		}
		char mystr[100], c[10], d[10];
		//strcpy(mystr,temp->s);

        printf("\nEnter the word to be replaced:\n");

        scanf(" %s",c);

        printf("Enter the new word:\n");

        scanf(" %s",d);
		while(temp!=NULL)
        {

        //printf("Enter a string along with characters to be rep_strd:\n");

        //gets(mystr);
        

        char *newstr = NULL;

     

        //puts(mystr);
        strcpy(mystr,temp->s);
        newstr = rep_str(mystr, c,d);
        //printf("%s\n",temp->s);
        strcpy(temp->s,newstr);
        //printf("%s\n",temp->s);
        //printf("%s\n", newstr);

        free(newstr);
        temp=temp->next;
    	}

        return 0;

}


