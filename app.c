#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

// Node declaration

typedef struct Node
{

    char *word;
    int length;
    struct Node *next;
    struct Node *prev;

} Node;

// Create a new node

Node *createNode(char *word, int length)
{

    Node *newNode = (Node *)malloc(sizeof(Node));

    newNode->word = word;
    newNode->length = length;
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

// Insert after current Node

void insertAfterNode(Node *currentNode, char *word, int length, Node **last)
{

    // Create the new node
    Node *newNode = createNode(word, length);
    newNode->next = currentNode->next;
    currentNode->next = newNode;
    newNode->prev = currentNode;

    if (newNode->next != NULL)
    {

        newNode->next->prev = newNode;
    }
    else
    {

        *last = newNode;
    }
}

// To delete the given node

void deleteCurrentNode(Node *currentNode, Node **head)
{

    if (*head == NULL)
    {

        return;
    }
    else if (currentNode == *head)
    {

        *head = NULL;
    }
    else
    {

        currentNode->prev->next = currentNode->next;

        if (currentNode->next != NULL)
        {

            currentNode->next->prev = currentNode->prev;
        }
    }

    //free(currentNode);
}

// To convert the given file data to the list representation

Node *covertStringToDoublyLinkedList(char *fileData, int *nodeCount, Node **last)
{

    // Create a dummy node

    Node *head = createNode("Dummy", 0);

    Node *curr = head;
    head->next = NULL;

    for (int i = 0; fileData[i] != '\0'; i++)
    {

        int end = i;

        // find the next white space

        while (fileData[end] != '\0' && fileData[end] != ' ')
        {

            end++;
        }

        // Create a new Node

        char *word = (char *)calloc(end - i + 1, sizeof(char));

        for (int j = i; j < end; j++)
        {

            char curr[1] = {fileData[j]};
            strcat(word, curr);
        }

        Node *newNode = createNode(word, end - i);

        *last = newNode;

        curr->next = newNode;
        newNode->prev = curr;
        curr = curr->next;

        *nodeCount += 1;

        i = fileData[end] == '\0' ? end - 1 : end;
    }

    if (head->next != NULL)
    {

        head->next->prev = NULL;
    }

    return head->next;
}

// Convert the given doubly linked list to a string.

char *covertDoublyLinkedListToString(Node *head, int totalLength)
{

    char *res = (char *)calloc(totalLength, sizeof(char));

    while (head != NULL)
    {

        strcat(res, head->word);
        strcat(res, " ");
        head = head->next;
    }

    return res;
}

// To calculate the size of the file

int getFileSize(char *filename)
{

    struct stat st;
    stat(filename, &st);

    return st.st_size;
}

// To read the file data

char *readFile(char *filename, int fileSize)
{

    // Open the file

    FILE *fp = fopen(filename, "r");

    // Result Buffer

    char *res = (char *)calloc(fileSize, sizeof(char));

    // Read the file

    if (fp == NULL)
    {

        printf("Unable to open the file.");
        exit(0);
    }
    else
    {

        while (!feof(fp))
        {

            char *buff = (char *)calloc(100, sizeof(char));

            fread(buff, sizeof(char), 100, fp);
            strcat(res, buff);

            //free(buff);
        }
    }

    // Close the file

    fclose(fp);

    return res;
}

// Print the user view

void printUserView(char *fileData, int cursorPos, char *currChar, Node *activeNode)
{

    system("clear");

    printf("Current cursor position: %d\n", cursorPos);
    printf("Current character: %c\n", *currChar);
    printf("Current word: %s\n\n", activeNode->word);

    printf("Current file content:\n\n");
    printf("%s", fileData);

    printf("\n\n");

    printf("Available Options\n\n");
    printf("F - To move the cursor forward \n");
    printf("B - To move the cursor backward \n");
    printf("S - To move the cursor to the start \n");
    printf("E - To move the cursor to the end \n");
    printf("I - To insert new characters \n");
    printf("E - To delete the current character \n");
    printf("Q - To search if the query word is present in the file or not \n");
    printf("U - To undo the last move \n");
    printf("P - To save the changes \n\n");

    printf("Press any other key to exit \n\n");
}

// check if the operation is valid or not

int isValidOperation(char operation)
{

    int result = operation == 'F' || operation == 'f' ||
                 operation == 'B' || operation == 'b' ||
                 operation == 'S' || operation == 's' ||
                 operation == 'E' || operation == 'e' ||
                 operation == 'I' || operation == 'i' ||
                 operation == 'D' || operation == 'd' ||
                 operation == 'Q' || operation == 'q' ||
                 operation == 'U' || operation == 'u' ||
                 operation == 'P' || operation == 'p';

    return result;
}

// Move cursor forward

void moveCusrorForward(int *currPos, Node **activeNode, char *currChar)
{

    *currPos = *currPos + 1;

    if (*currPos == (*activeNode)->length)
    {

        if ((*activeNode)->next != NULL)
        {

            *activeNode = (*activeNode)->next;
            *currPos = 0;
            *currChar = (*activeNode)->word[0];
        }
        else
        {

            *currPos = *currPos - 1;
        }
    }
    else
    {

        *currChar = (*activeNode)->word[*currPos];
    }
}

// Move cursor backward

void moveCusrorBackward(int *currPos, Node **activeNode, char *currChar)
{

    *currPos -= 1;

    if (*currPos < 0 && (*activeNode)->length)
    {

        if ((*activeNode)->prev != NULL)
        {

            *activeNode = (*activeNode)->prev;
            *currPos = (*activeNode)->length - 1;
            *currChar = (*activeNode)->word[*currPos];
        }
        else
        {

            *currPos = *currPos + 1;
        }
    }
    else
    {

        *currChar = (*activeNode)->word[*currPos];
    }
}

// Move cursor to the start

void moveCusrorStart(int *currPos, Node **activeNode, char *currChar, Node *head)
{

    *currPos = 0;
    *activeNode = head;
    *currChar = (*activeNode)->word[0];
}

// move cursor to the end

void moveCusrorEnd(int *currPos, Node **activeNode, char *currChar, Node *last)
{

    *activeNode = last;
    *currPos = last->length - 1;
    *currChar = (*activeNode)->word[*currPos];
}

// To search the query word

void searchWord(Node *head)
{

    char *query = (char *)calloc(1000, sizeof(char));

    system("clear");

    printf("\nEnter the query string: \n");
    scanf("%s", query);

    int flag = 0;

    while (head != NULL)
    {

        if (!strcmp(head->word, query))
        {

            system("clear");
            printf("\n\nThe word exists in the file...\n");
            printf("\nEnter c to continue...\n");
            scanf("%s", query);
            flag = 1;
            break;
        }

        head = head->next;
    }

    if (!flag)
    {

        system("clear");
        printf("\n\nThe word does not exist in the file...\n");
        printf("\nEnter c to continue...\n");
        scanf("%s", query);
    }

    //free(query);
}

// Write to file

void writeToFile(char *filename, char *fileData)
{

    // Open the file

    FILE *fp = fopen(filename, "w");

    if (fp != NULL)
    {

        fileData[0] = 'Y';

        fprintf(fp, "%s", fileData);

        system("cat abc.txt");

        int n;
        scanf("%d", &n);
    }
    else
    {

        printf("\nUnable to open the file\n");
        exit(0);
    }

    // Close the file

    fclose(fp);
}

// To insert character(s) at current position

void insert(Node **activeNode, char *currChar, int *currPos, Node **last)
{

    char *item = (char *)calloc(1000, sizeof(char));
    system("clear");
    printf("Enter the word to be inserted:\n");
    scanf("%s", &item);
    int a = strlen(item);
    for (int i = (*activeNode)->length; i > *currPos; i--)
    {

        (*activeNode)->word[i + 1] = (*activeNode)->word[i];
    }

    (*activeNode)->length = (*activeNode)->length + 1;

    if((*activeNode)->length >= *currPos) {

        *currPos = *currPos + 1;

    }
    // insertAfterNode(*activeNode,item,a,last);

    if ((*activeNode)->length >= 0)
    {

        Node *tmp = *activeNode;

        if ((*activeNode)->next != NULL)
        {

            *activeNode = (*activeNode)->next;
            *currPos = (*activeNode)->length + 1;
        }
        else
        {

            *activeNode = (*activeNode)->prev;
            *currPos = 0;
        }

        insertAfterNode(tmp, item, a, last);
    }

    // if(*activeNode != NULL) {

    //     *currChar = (*activeNode)->word[*currPos];

    // }
}

// To delete the current character

void deleteCharacter(Node **activeNode, int *currPos, char *currChar, Node **head)
{

    if (*activeNode == NULL)
        return;

    for (int i = *currPos; i < (*activeNode)->length; i++)
    {

        (*activeNode)->word[i] = (*activeNode)->word[i + 1];
    }

    (*activeNode)->length = (*activeNode)->length - 1;

    if ((*activeNode)->length <= *currPos)
    {

        *currPos = *currPos - 1;
    }

    if ((*activeNode)->length <= 0)
    {

        Node *tmp = *activeNode;

        if ((*activeNode)->prev != NULL)
        {

            *activeNode = (*activeNode)->prev;
            *currPos = (*activeNode)->length - 1;
        }
        else
        {

            *activeNode = (*activeNode)->next;
            *currPos = 0;
        }

        deleteCurrentNode(tmp, head);
    }

    if (*activeNode != NULL)
    {

        *currChar = (*activeNode)->word[*currPos];
    }
}

// Perform the given task

void performTask(char opcode, char *fileData, int *currPos, Node **activeNode, char *currChar, Node *head, Node *last, char *filename)
{

    switch (opcode)
    {

    case 'f':
    case 'F':
        moveCusrorForward(currPos, activeNode, currChar);
        break;
    case 'b':
    case 'B':
        moveCusrorBackward(currPos, activeNode, currChar);
        break;
    case 's':
    case 'S':
        moveCusrorStart(currPos, activeNode, currChar, head);
        break;
    case 'e':
    case 'E':
        moveCusrorEnd(currPos, activeNode, currChar, last);
        break;
    case 'i':
    case 'I':
        insert(activeNode, currChar, currPos, &last);
        break;
    case 'd':
    case 'D':
        deleteCharacter(activeNode, currPos, currChar, &head);
        break;
    case 'q':
    case 'Q':
        searchWord(head);
        break;
    case 'u':
    case 'U':
        break;
    case 'p':
    case 'P':
        writeToFile(filename, fileData);
        break;
    default:
        printf("Invalid Operation, Exiting...");
    }
}

// Driver code

int main()
{
    // Set the file name

    char filename[] = "abc.txt";

    // Open the file and read te saved data

    int *fileSize = (int *)calloc(1, sizeof(int));
    *fileSize = getFileSize(filename);

    char *fileData = readFile(filename, *fileSize);

    // Convert the data to a doubly LinkedList

    int nodeCount = 0;
    Node *last;
    Node *head = covertStringToDoublyLinkedList(fileData, &nodeCount, &last);
    Node *activeNode = head;
    char currChar = head->word[0];

    int *currPos = (int *)calloc(1, sizeof(int));

    // Program begins

    char currenOperation = ' ';
    do
    {

        printUserView(fileData, *currPos, &currChar, activeNode);
        scanf(" %c", &currenOperation);
        performTask(currenOperation, fileData, currPos, &activeNode, &currChar, head, last, filename);
        fileData = covertDoublyLinkedListToString(head, *fileSize);

    } while (isValidOperation(currenOperation));

    // Ending screen

    system("clear");
    printf("\n\nThanks for using! \n");
    printf("Hope to see you soon :) \n");
    printf("Bye! \n\n");

    return 0;
}