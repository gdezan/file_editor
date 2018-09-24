#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Função para conferir se um caractere é especial ou não
bool isSpecial(char x) {
    bool answer = true;
    if ((x >= 65 && x <= 90) || (x >= 87 && x <= 122) || (x >= 48 && x <= 57) || x == '\0') {
        answer = false;
    }
    return answer;
}

// Definição do nó
typedef struct node { 
    char* data; // Palavra que ficará no nó
    int index;  // Índice do nó
    struct node* next;
    struct node* prev; 
} Node;

void removeNL(char *input){                             // Função para remover o \n do fgets
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[--len] = '\0';
    }
}

// Função para imprimir a lista
void printList(Node* head) {
    if (head == NULL) return;
    Node* current = head;
    while (current != NULL){
        // Confere se a palavra atual é especial ou não para gerar o espaço antes dela
        if (current->prev != NULL && isSpecial(current->data[0]) == false && strcmp(current->prev->data, "\n") != 0){
            printf(" ");
        }
        printf("%s", current->data);

        current = current->next;
    }
}


// Função para pegar o a palavra apos o comando
// Exemplo: O usuario vai entrar com "i foobar" e a função retorna "foobar"
char* stripWord(char* input) {
    if (input[3] != '\0') removeNL(input);
    
    char* word = (char*) calloc((strlen(input)-1), sizeof(char));
    for (int i = 0; i < strlen(input)-2; i++) {
        word[i] = input[i+2];
    }
    return word;
}


// Função para inserir a palavra após o cursor
void pushAfter(Node* cursor, Node** head, char* value) {
    Node* new = (Node*) malloc(sizeof(Node));
    new->data = (char *) malloc((strlen(value)+1) * sizeof(char)); 
    strcpy(new->data,value);
    
    if (*head == NULL) { // Confere caso a lista não tenha nenhum nó ainda
        new->prev = NULL;
        new->next = NULL; 
        new->index = 0;
        *head = new; 
        return; 
    } 

    Node* current = cursor;  // Pega a posição do cursor para fazer a inserção
    Node* next = current->next;
    current->next = new;
    new->index = current->index; 
    new->next = next;
    new->prev = current;

    if (next != NULL) next->prev = new;
    current = current->next;

    while(current != NULL){ // Renumerando os índices dos nós posteriores ao cursor
        current->index++;
        current = current->next;
    }
}

// Função para adicionar a palavra antes do cursor. Similar a função 'pushAfter'
void pushBefore(Node* cursor, Node** head, char* value) {
    Node* new = (Node*) malloc(sizeof(Node));

    new->data = (char *) malloc((strlen(value)+1) * sizeof(char)); 
    strcpy(new->data,value);
    
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

// Função para substituir uma palavra
void replace(Node* cursor, char* value) {
    Node* current = cursor;
    if (current == NULL){
        return;
    }

    free(current->data);
    current->data = (char *) malloc((strlen(value)+1) * sizeof(char)); 
    strcpy(current->data,value);
}

// Função para encontrar uma palavra e retornar seu índice
void find(Node* cursor, char* value) {
    Node* current = cursor;
    if (current == NULL) {
        return;
    }

    while (strcmp(current->data, value) != 0 ) {
        if (current->next == NULL) break;
        current = current->next;
    }

    if (strcmp(current->data, value) == 0) printf("%d\n", current->index);
}


// Função com métodos para a movimentação do cursor
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

// Função para a remoção de uma palavra
void delete(Node** cursor, Node** head) {
    if (*head == NULL) { 
        return; 
    } 

    Node* current = *cursor;        // Pega o nó anterior e o posterior
    Node* prev = current->prev;     // do cursor para depois uní-los
    Node* next = current->next;     // após a remoção da palavra

    free(current);
    current = *cursor;
    if (prev == NULL) {             // Caso seja a primeira palavra
        next->prev = prev;
        *head = next;
        *cursor = next;
    } else if (next == NULL) {      // Caso seja a última
        prev->next = next;
        *cursor = prev;
    } else {
        next->prev = prev;
        prev->next = next;
        *cursor = next;
    }

    current = current->next;
    while(current != NULL){         // Renumera os índices após a remoção
        current->index--;
        current = current->next;
    }
}


// Função para movimentar o cursor um certo número de passos
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

// Liberação da memória da lista
void freeList(Node* head){
  Node* current = head;
  while( current != NULL ) {
     Node* next = current->next;
     free( current );
     current = next;
  }
}


int main() {
    char fileName[2];               // Pegando o nome do arquivo para
    scanf("%s", fileName);          // lê-lo em seguida
    strcat(fileName, ".ext");
    char* input = (char *) malloc(64 * sizeof(char));
    char command;

    Node* head = NULL;              // Declaração do nó cabeça da lista
    Node* cursor = head;            // Declaração do cursor;
    FILE* fp;
    fp = fopen(fileName, "r");

    // String que irá conter o resultado do fgets (i.e.: linha do arquivo)
    char buff[255];

    // String que irá conter cada palavra da linha por vez (com ajuda da função strtok)                   
    char* token = calloc(63,sizeof(char));

    // Onde ficarão os caracteres especiais antes de serem inseridos na lista    
    char special[2];

    // Onde ficarão as palavras antes dos caracteres especiais
    char before[31];

    // Onde ficarão as palavras depois dos caracteres especiais
    char after[31];

    int i;

    while (fgets(buff, 255, fp)) {  // Lê linhas enquanto ainda existem no arquivo
        token = strtok(buff, " ");      // Função nativa utilizada para separar a linha palavra por palavra
        while(token != NULL){

            strcpy(after, token);

            // O princípio do loop abaixo é pegar uma palavra e separar em duas partes
            // A anterior (before), que são todos os caracteres antes de um caractere especial.
            // E a posterior (after), que são todos os caracters após o fim da anterior
            // Exemplo: Palavra: "foo.bar" -> before: "foo" - after: "bar"
            // A 'before' é, então, enviada para a lista, e o loop ocorre denovo, fazendo o mesmo
            // processo anterior com 'after', separando-a em uma nova 'before' e 'after'.
            // Esse processo se repete até a 'after' não existir mais.
            while (strcmp(after, "") != 0) {    
                i = 0;

                while (i <= strlen(after)) {
                    if (isSpecial(after[i])) break; // Caso encontre um caractere especial, quebra o processo de cópia
                    before[i] = after[i];     // Cópia do começo da palavra para a 'before'
                    i++;
                }

                if (i == 0) {               // Esse caso ocorre quando o primeiro caractere da 'after' já é um
                    before[i] = after[i];   // caractere especial. Ele então é enviada como um nó para a lista
                    i++;
                }

                before[i] = '\0';   // Fechamento da string 'before'


                // OBS: A implementação:
                //
                // moveCursor(&cursor, 'e');
                // pushAfter(cursor, &head, after); 
                //
                // É uma forma de inserir valor no final da lista sem ser necessário criar um função específica para isso.
                // Ela consiste em mover o cursor para o final, e inserir uma palavra após o cursor

                    
                if (strcmp(before, after) == 0) {       // Caso before e after(antes da cisão) sejam iguais
                    pushAfter(cursor, &head, after);    // a palavra inteira pode ser enviada e o loop
                    cursor = head;                      // é encerrado
                    moveCursor(&cursor, 'e');
                    break;
                } else {
                    pushAfter(cursor, &head, before);   // Caso contrário, a 'before' é enviada como um nó
                    cursor = head;
                    moveCursor(&cursor, 'e');

                    special[0] = after[i];              // O caractere especial 'divisor' é enviado como um nó
                    special[1] = '\0';
                    pushAfter(cursor, &head, special);
                    moveCursor(&cursor, 'e');
                    i++;
                    
                    for (int j = i; j <= strlen(after); j++){   // E há a criação da 'after' nova, que são os 
                        after[j-i] = after[j];                  // os caracteres após o caractere 'divisor'
                    }
                }

            }
            token = strtok(NULL, " ");  // Pega a palavra seguinte
        }
    }
    moveCursor(&cursor, 'b');   // Coloca o cursor no início
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
            case 'a':
                pushAfter(cursor, &head, stripWord(input));
                break;
            case 'i':
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
    free(cursor);
    return 0;
}
