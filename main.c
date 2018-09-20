#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void removeNL(char *input){                             // Função para remover o \n do fgets
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[--len] = '\0';
    }
}

bool isSpecial(char x) {
    bool answer = true;
    if ((x >= 65 && x <= 90) || (x >= 87 && x <= 122) || (x >= 48 && x <= 57)) {
        answer = false;
    }
    return answer;
}

typedef struct node { 
    char* data;
    int index; 
    struct node* next; // Pointer to next node in DLL 
    struct node* prev; // Pointer to previous node in DLL 
} Node;

void showList(Node* head, Node* cursor) {
    if (head == NULL) return;
    Node* current = head;
    while (current != NULL){
        if (current->prev != NULL && isSpecial(current->data[0]) == false) {
            printf(" "
            );
        }
        if (current->index == cursor->index) {
            printf("/%s/", current->data);
        } else {
            printf("/%s/", current->data);
        }

        current = current->next;
    }
    printf("\n\nCursor index: %d\n", cursor->index);
}

void printList(Node* head) {
    if (head == NULL) return;
    Node* current = head;
    while (current != NULL){
        if (current->prev != NULL && isSpecial(current->data[0]) == false) {
            printf(" "
            );
        }
        printf("%s", current->data);

        current = current->next;
    }
}

char* stripWord(char* input) {
    if (input[3] != '\0') removeNL(input);
    
    char* word = (char*) calloc((strlen(input)-1), sizeof(char));
    for (int i = 0; i < strlen(input)-2; i++) {
        word[i] = input[i+2];
    }
    return word;
}

void pushAfter(Node* cursor, Node** head, char* value) {
    Node* new = (Node*) malloc(sizeof(Node));

    new->data = (char *) malloc(strlen(value) * sizeof(char)); 
    new->data = value;
    
    if (*head == NULL) { 
        new->prev = NULL;
        new->next = NULL; 
        new->index = 0;
        *head = new; 
        return; 
    } 

    Node* current = cursor;
    Node* next = current->next;
    current->next = new;
    new->index = current->index; 
    new->next = next;
    new->prev = current;

    if (next != NULL) next->prev = new;
    current = current->next;

    while(current != NULL){
        current->index++;
        current = current->next;
    }
}

void pushBefore(Node* cursor, Node** head, char* value) {
    Node* new = (Node*) malloc(sizeof(Node));

    new->data = (char *) malloc(strlen(value) * sizeof(char)); 
    new->data = value;
    
    if (*head == NULL) { 
        new->prev = NULL;
        new->next = NULL; 
        new->index = 0;
        *head = new; 
        return; 
    } 

    Node* current = cursor;
    Node* prev = current->prev;
    current->prev = new;
    new->index = current->index; 
    new->prev = prev;
    new->next = current;

    if (prev != NULL) {
        prev->next = new;
    } else {
        *head = new;
    }

    while(current != NULL){
        current->index++;
        current = current->next;
    }
}

void replace(Node* cursor, char* value) {
    Node* current = cursor;
    if (current == NULL){
        return;
    }

    free(current->data);
    current->data = (char *) malloc(strlen(value) * sizeof(char)); 
    current->data = value;
}

void find(Node* cursor, char* value) {
    Node* current = cursor;
    if (current == NULL) {
        return;
    }

    while (strcmp(current->data, value) != 0 ) {
        if (current->next == NULL) break;
        current = current->next;
    }

    if (strcmp(current->data, value) == 0) printf("\nINDEX: %d\n", current->index);
}

void moveCursor(Node** cursor, char dir) {
    switch(dir){
        case 'n':
            if ((*cursor)->next != NULL) (*cursor) = (*cursor)->next;
            break;
        case 'p':
            if ((*cursor)->prev != NULL) (*cursor) = (*cursor)->prev;
            break;
        case 'b':
            while((*cursor)->prev != NULL) (*cursor) = (*cursor)->prev;
            break;
        case 'e':
            while((*cursor)->next != NULL) (*cursor) = (*cursor)->next;
            break;
    }
}

void delete(Node** cursor, Node** head) {
    if (*head == NULL) { 
        return; 
    } 

    Node* current = *cursor;
    Node* prev = current->prev;
    Node* next = current->next;

    free(current);
    current = *cursor;
    if (prev == NULL) {
        next->prev = prev;
        *head = next;
        *cursor = next;
    } else if (next == NULL) {
        prev->next = next;
    } else {
        next->prev = prev;
        prev->next = next;
        *cursor = prev;
    }

    current = current->next;
    while(current != NULL){
        current->index--;
        current = current->next;
    }
}

void stepCursor(Node** cursor, int steps) {
    Node* current = *cursor;

    if (steps >= 0) {
        for (int i = 0; i < steps; i++){
            if (current->next != NULL) {
                current = current->next;
            }
        }
    } else {
        for (int i = 0; i < abs(steps); i++){
            if (current->prev != NULL) {
                current = current->prev;
            }
        }
    }


    *cursor = current;
}

void freeList(Node* head){
  Node* current = head;
  while( current != NULL ) {
     Node* next = current->next;
     free( current );
     current = next;
  }
}


int main() {
    char fileName[2];
    scanf("%s", fileName);
    strcat(fileName, ".ext");
    char* input = (char *) malloc(64 * sizeof(char));
    char command;

    Node* head = (Node*)malloc(sizeof(Node));
    Node* cursor = head;
    FILE* fp;
    fp = fopen(fileName, "r");

    char buff[255];
    char* token = calloc(63,sizeof(char));

    while(fgets(buff, 255, fp)){
      token = strtok(buff, " ");
      while(token != NULL){
        moveCursor(&cursor, 'e');
        pushAfter(cursor, &head, token);
        cursor = head;
        showList(head, cursor);
        token = strtok(NULL, " ");

      }
      getchar();
    }
    printf("\n");
    getchar();
    moveCursor(&cursor, 'b');
    delete(&cursor, &head);
    fclose(fp);
    while(input[0] != 's') {
        if (head != NULL && head->next == NULL && head->prev == NULL) cursor = head;
        fgets(input, 64, stdin);

        command = input[0];
        switch(command) {
            case 'n':
            case 'p':
            case 'b':
            case 'e':
                moveCursor(&cursor, command);
                break;
            case 'i':
                pushAfter(cursor, &head, stripWord(input));
                break;
            case 'a':
                pushBefore(cursor, &head, stripWord(input));
                break;
            case 'r':
                replace(cursor, stripWord(input));
                break;
            case 'f':
                find(cursor, stripWord(input));
                break;
            case 'd':
                delete(&cursor, &head);
                break;
            case 'g':
                stepCursor(&cursor, atoi(stripWord(input)));
                break;
        }
    }
    printList(head);
    freeList(head);
    freeList(cursor);
    return 0;
}
