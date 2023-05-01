#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

////////////WAITING REVISIONS////////////////
 /**
  * errors
  */

//LLVM IR definitions
#define MAX_EXPR_LEN 1000
#define MAX_OP_LEN 32
int error = 0;


int registerNumber = 1;
int varExist = 0;

//Stack implementation
struct Stack {
    char* items[MAX_EXPR_LEN];
    int top;
};

struct Stack* createStack() {
    struct Stack* stack = (struct Stack*)calloc(1, sizeof(struct Stack));
    stack->top = -1;
    return stack;
}

int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

int isFull(struct Stack* stack) {
    return stack->top == MAX_EXPR_LEN - 1;
}

/**
 * method that adds an item to a stack.
 * @param stack
 * @param item
 */
void push(struct Stack* stack, char* item) {
    if (isFull(stack)) {
        printf("Stack is full\n");
    } else {
        stack->items[++stack->top] = strdup(item);
    }
}

/**
 * method that removes an item from the stack.
 * @param stack
 * @return
 */
char* pop(struct Stack* stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty\n");
        return NULL;
    } else {
        return stack->items[stack->top--];
    }
}


//hashtable implementation
#define TABLE_SIZE 128

typedef struct {
    char *key;
    long long int value;
    int assigned;
} element;

typedef struct {
    element *elements;
    int size;
} table;

table *Hashtable;
table *registerTable;

int hash_function(char *key);

void init_table(table *table);

void insert(table *table, char *key, long long int value);

int is_balanced(char *str);

int is_valid_function(char *str);

int check_function(char *line, char *function_name);

int is_valid_operator(char *line, char *operator);


/**
 * function that checks whether an input string has balanced parantheses or not.
 * @param input str
 * @return balance value
 */

int is_balanced(char *str) {
    int len = strlen(str);
    char stack[len];
    int top = -1;
    for (int i = 0; i < len; i++) {
        if (str[i] == '(') {
            stack[++top] = str[i];
        } else if (str[i] == ')') {
            if (top == -1) {
                return 0;
            } else if (str[i] == ')' && stack[top] == '(') {
                top--;
            } else {
                return 0;
            }
        }
    }
    return (top == -1);
}


/**
 * Hashing function
 * @param key
 * @return hash value
 */

int hash_function(char *key) {
    int hash = 0;
    int length = strlen(key);
    for (int i = 0; i < length; i++) {
        hash = hash + key[i];
    }
    return hash % TABLE_SIZE;
}


/** Function that initializes our hash table
 * @param table
 */


void init_table(table *table) {
    table->elements = (element *) calloc(TABLE_SIZE, sizeof(element));
    table->size = TABLE_SIZE;
    for (int i = 0; i < TABLE_SIZE; i++) {
        table->elements[i].key = NULL;
        table->elements[i].value = 0;
    }
}

/**
 * Function that adds an element to our hash table.
 * @param table hashtable
 * @param key  key to be inserted
 * @param value value to be inserted
 */

void insert(table *table, char *key,  long long int value) {
    int index = hash_function(key);
    int i = index;
    while (table->elements[i].key != NULL && strcmp(table->elements[i].key, "") != 0) {
        if (strcmp(table->elements[i].key, key) == 0) {
            table->elements[i].value = value;
            table->elements[i].assigned = 1;
            return;
        }
        i = (i + 1) % table->size;
        if (i == index) {

            return;
        }
    }
    table->elements[i].key = strdup(key);
    table->elements[i].value = value;
    table->elements[i].assigned = 1;
}


/**
 * Function that fetches a key's value from the table.
 * @param table
 * @param key
 * @return key's value
 */

long long int lookup(table *table, char *key) {
    int index = hash_function(key);
    int i = index;
    while (table->elements[i].key != NULL) {
        if (strcmp(table->elements[i].key, key) == 0) {
            return table->elements[i].value;
        }
        i = (i + 1) % table->size;

        if (i == index) {
            //we have reached the starting index.
            //no variable found, returning a dummy number of 999999 to indicate no existance in AdvCalc++
            error=1;
            return 999999;
        }
    }
    return 999999;
}



typedef enum {
    NONE,
    NUM, //all numbers
    OP, //+,-,&,|,*
    IDENT, //identifiers,initally zero
    FUNC_CALL, //for all functions in the format function(arg1,arg2)
    LPAR,
    RPAR,
    COMMA,
    ASSIGN,
    AND,
    OR

} TokenType;


typedef struct {
    TokenType type;
    char *value; //string value of the token
    char *function_name;   // Name of function
    int register_number; //this register number is only to record an identifier tokens memory adress.

} Token;

Token tokens[257];
int current_index = 0;
int current_token_type;
int *num_tokens = 0;
int old;
int numtoken;
int numofpost;

Token create_token(int type, char *value);


/**
 * function that forms tokens from an input string.
 * i.e the tokenizer / lexer
 * @param input string
 * @param tokens the array that will store the tokens
 * @param num_tokens
 * @return
 */

Token *tokenize(char *input, Token tokens[], int *num_tokens) {

    int i = 0; //current array location,incremented after each process

    char curr_char = input[i];

    while (curr_char != '\0' && curr_char != '%') {

        if (isdigit(curr_char)) {

            char *start = input + i; //start of the number (ex. 5 for 5674)

            while (isdigit(curr_char)) { //current is a digit
                i++;
                curr_char = input[i];
            }

            int length = input + i - start;
            char *token_str = (char *) malloc(length + 1);
            strncpy(token_str, start, length);
            token_str[length] = '\0'; //last character of the number,so the number's tokenized length is one more
            tokens[*num_tokens] = create_token(NUM, token_str);
            (*num_tokens)++;
        }


        else if (isalpha(curr_char)) {

            char *start = input + i;
            while (isalnum(curr_char) && curr_char != ' ') {
                i++;
                curr_char = input[i];
            }

            int length = input + i - start;
            char *token_str = (char *) malloc(length + 1);
            strncpy(token_str, start, length);
            token_str[length] = '\0';

            if (strcmp(token_str, "ls") == 0 || strcmp(token_str, "rs") == 0 ||
                strcmp(token_str, "xor") == 0 || strcmp(token_str, "lr") == 0 ||
                strcmp(token_str, "rr") == 0 || strcmp(token_str, "not") == 0) {
                tokens[*num_tokens] = create_token(FUNC_CALL, token_str);
            }

            else {
                tokens[*num_tokens] = create_token(IDENT, token_str);
                //tokens[*num_tokens].register_number = registerNumber++;
            }

            (*num_tokens)++;
        }

        else if (curr_char == ',') {
            char *token_str = (char *) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] = create_token(COMMA, token_str);
            (*num_tokens)++;
            i++;
            curr_char = input[i];
        }

        else if (curr_char == '=') {
            char *token_str = (char *) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] = create_token(ASSIGN, token_str);
            (*num_tokens)++;
            i++;
            curr_char = input[i];
        }

        else if (curr_char == '+' || curr_char == '-' || curr_char == '*' || curr_char == '/' || curr_char == '%') {
            char *token_str = (char *) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] = create_token(OP, token_str);
            (*num_tokens)++;
            i++;
            curr_char = input[i];
        }

        else if (curr_char == '(') {
            char *token_str = (char *) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] = create_token(LPAR, token_str);
            (*num_tokens)++;
            i++;
            curr_char = input[i];
        }

        else if (curr_char == ')') {
            char *token_str = (char *) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] = create_token(RPAR, token_str);
            (*num_tokens)++;
            i++;
            curr_char = input[i];


        } else if (curr_char == '&') {
            char *token_str = (char *) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] = create_token(AND, token_str);
            (*num_tokens)++;
            i++;
            curr_char = input[i];
        }

        else if (curr_char == '|') {
            char *token_str = (char *) malloc(2);
            token_str[0] = curr_char;
            token_str[1] = '\0';
            tokens[*num_tokens] = create_token(OR, token_str);
            (*num_tokens)++;
            i++;
            curr_char = input[i];
        }

        else {
            i++;
            curr_char = input[i];
        }
    }
    for (int i = 0; i < *num_tokens; i++) {
        numtoken = *num_tokens;
    }

    return tokens;
}


/**
 * function that creates a new token
 * @param type token's type
 * @param value token's value
 * @return Token struct
 */

Token create_token(int type, char *value) {
    Token token;

    token.type = type;
    token.value = value;

    return token;
}


/**
 * Function that decides on the precedence of a token.
 * This method is used to decide our postfix structure.
 * @param opearator
 * @return int precedence
 */

int precedence(char *op) {

    if (strcmp(op, "=") == 0)
        return 7;

    else if (strcmp(op, "(") == 0 || strcmp(op, ")") == 0)
        return 0;

    else if (strcmp(op, ",") == 0)
        return 1;

    else if (strcmp(op, "xor") == 0 || strcmp(op, "not") == 0 || strcmp(op, "ls") == 0 || strcmp(op, "rs") == 0 ||
             strcmp(op, "lr") == 0 || strcmp(op, "rr") == 0)
        return 5;

        //modulo might get mixed up with comment sign.
    else if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0  || strcmp(op, "%") == 0) {
        return 4;
    }
    else if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 3;

    else if (strcmp(op, "&") == 0)
        return 2;

    else if (strcmp(op, "|") == 0)
        return 1;
    else
        return -1;
}


/**
 * checks whether all chars are alphabetic characters
 * @param str input
 * @return whether the string is all alphabetic
 */

int allAlpha(char *str) {
    int i = 0;
    while (str[i]) {
        if (!isalpha(str[i])) {
            return 0;
        }
        i++;
    }
    return 1;
}


/**
 * Function that converts infix expression to postfix
 * @param tokens array of tokens returned by the tokenizer function
 * @param postfix expression
 */

void infix_to_postfix(Token *tokens, Token *postfix) {

    Token stack[MAX_EXPR_LEN];
    int top = -1;
    int i, j;


    for (i = 0, j = 0; i < numtoken; i++) {

        // If the current character is an operand, add it to the postfix expression
        if (tokens[i].type != 4 && (isdigit((int) *(tokens[i].value)) || allAlpha((tokens[i].value)))) {
            postfix[j++].value = (tokens[i].value);

        }

            // If the current character is an operator, add it to the stack
        else if (strcmp(tokens[i].value, "(") == 0) {
            stack[++top].value = (tokens[i].value); // add everything after "(" until you encounter  ")"
        }

        else if (strcmp(tokens[i].value, ")") == 0) {
            // Pop operators off the stack and add them to the postfix expression until a ")" is encountered
            while (top >= 0 && strcmp(stack[top].value, "(") != 0) {
                postfix[j++].value = stack[top--].value;
            }
            // Trash the left parenthesis
            top--;
        }

        else {
            if(tokens[i].type==4 && strcmp(tokens[i+1].value, "(") != 0){
                error=1;
            }

            // Pop operators off the stack and add them to the postfix expression until an operator with lower precedence is encountered
            while (top >= 0 && precedence(stack[top].value) >= precedence(tokens[i].value)) {
                if (strcmp(stack[top].value, ",") == 0) {
                    top--; //skip the comma and get to the function
                    continue;
                }
                postfix[j++].value = stack[top--].value;
            }
            // Push the current operator onto the stack
            stack[++top].value = tokens[i].value;
        }
    }

    // Pop any remaining operators off the stack and add them to the postfix expression

    while (top >= 0) {
        postfix[j++].value = stack[top--].value;
    }

    // Add null terminator to the end of the postfix expression
    postfix[j].value = '\0';
    numofpost = j;
}



/**
 * Function that translates AdvCalc++ language to LLVM IR.
 * This is quite similar to the evaluation function but instead of interpretation it does translation.
 * @param postfix expression to be evaluated
 * @param fp file pointer that points to the output file. This will be our LLVM IR source code.
 */

void postfix_to_ir(Token* postfix,FILE *fp) {

    int dummy = 0;

    varExist = 0;

    struct Stack* stack = createStack();
    //int top = -1;
    int i;


    for (i = 0; i < numofpost; i++) {
        // If the current character is a number, push it onto the stack
        if (isdigit(*postfix[i].value)) {
            push(stack,postfix[i].value);

        }

        else {
            int op1, op2;
            long long int result;
            char op[MAX_OP_LEN] = "";
            int j = 0;
            strcpy(op, postfix[i].value);

            if (strcmp(postfix[i].value, "+") == 0) {

                char* right = pop(stack);
                char*  left = pop(stack);

                fprintf(fp, "\t%%%d = add i32 %s, %s\n", registerNumber++, left, right);
                char reg[20];
                sprintf(reg,"%%%d",registerNumber-1);
                push(stack,reg);
                //fprintf(fp,"ADDED TO THE STKC %s\n",reg);
                //fprintf(fp,"STACK TOP IS %s\n",stack[top]);
                //fprintf(fp,"STACK TOP-1 IS %s\n",stack[top-1]);


            }
            else if (strcmp(postfix[i].value, "-") == 0) {
                char* right = pop(stack);
                char*  left = pop(stack);
                fprintf(fp, "\t%%%d = sub i32 %s, %s\n", registerNumber++, left, right);
                char reg[20];
                //int dummy = registerNumber -1;
                sprintf(reg,"%%%d",registerNumber -1);
                push(stack,reg);


            }
            else if (strcmp(postfix[i].value, "*") == 0){
                char* right = pop(stack);
                char*  left = pop(stack);
                fprintf(fp, "\t%%%d = mul i32 %s, %s\n ", registerNumber++, left, right);
                char reg[20];
                sprintf(reg,"%%%d",registerNumber-1);
                push(stack,reg);


            }
            else if (strcmp(postfix[i].value, "/") == 0) {
                char* right = pop(stack);
                char*  left = pop(stack);
                fprintf(fp, "\t%%%d = sdiv i32 %s, %s\n", registerNumber++, left, right);
                char reg[20];
                sprintf(reg,"%%%d",registerNumber-1);
                push(stack,reg);
            }
            else if (strcmp(postfix[i].value, "%%") == 0) {
                char* right = pop(stack);
                char*  left = pop(stack);
                fprintf(fp, "\t%%%d = srem i32 %s, %s\n", registerNumber++, left, right);
                char reg[20];
                sprintf(reg,"%%%d",registerNumber-1);
                push(stack,reg);
            }
            else if (strcmp(postfix[i].value, "xor") == 0) {
                char* right = pop(stack);
                char*  left = pop(stack);
                fprintf(fp, "\t%%%d = xor i32 %s, %s\n", registerNumber++, left, right);
                char reg[20];
                sprintf(reg,"%%%d",registerNumber-1);
                push(stack,reg);
            }

            else if (strcmp(postfix[i].value, "not") == 0) {
                //char* right = stack[top--];
                char*  left = pop(stack);
                //not?
                fprintf(fp, "\t%%%d = xor i32 %s, -1\n", registerNumber++, left);
                char reg[20];
                sprintf(reg,"%%%d",registerNumber-1);
                push(stack,reg);
            }

            else if (strcmp(postfix[i].value, "ls") == 0) {
                char* right = pop(stack);
                char*  left = pop(stack);
                fprintf(fp, "\t%%%d = shl i32 %s, %s\n", registerNumber++, left, right);
                char reg[20];
                sprintf(reg,"%%%d",registerNumber-1);
                push(stack,reg);
            }

            else if (strcmp(postfix[i].value, "rs") == 0) {
                char* right = pop(stack);
                char*  left = pop(stack);
                fprintf(fp, "\t%%%d = ashr i32 %s, %s\n", registerNumber++, left, right);
                char reg[20];
                sprintf(reg,"%%%d",registerNumber-1);
                push(stack,reg);
            }

            else if (strcmp(postfix[i].value, "lr") == 0) {
                char* right = pop(stack);
                char*  left = pop(stack);
                fprintf(fp, "\t%%%d = rotl i32 %s, %s\n", registerNumber++, left, right);
                char reg[20];
                sprintf(reg,"%%%d",registerNumber-1);
                push(stack,reg);
            }

            else if (strcmp(postfix[i].value, "rr") == 0) {
                char* right = pop(stack);
                char*  left = pop(stack);
                fprintf(fp, "\t%%%d = rotr i32 %s, %s\n", registerNumber++, left, right);
                char reg[20];
                sprintf(reg,"%%%d",registerNumber-1);
                push(stack,reg);
            }


            else if (strcmp(postfix[i].value, "|") == 0) {
                char* right = pop(stack);
                char*  left = pop(stack);
                fprintf(fp, "\t%%%d = or i32 %s, %s\n", registerNumber++, left, right);
                char reg[20];
                sprintf(reg,"%%%d",registerNumber-1);
                push(stack,reg);
            }
            else if (isalpha(*postfix[i].value) && strcmp(postfix[i].value, op) == 0) {
                //if not a function name, this is a variable. Thus fetch the value.
                //result = lookup(Hashtable,op);




                char dum[20];

                //already assigned
                if(lookup(registerTable,postfix[i].value) != 999999){
                    sprintf(dum,"%s%d",postfix[i].value,dummy);
                    dummy++;
                    //postfix[i].value = postfix[i].value+dummy ; dummy+1
                }

                //not assigned
                else {
                    sprintf(dum, "%s", postfix[i].value);
                }


                insert(registerTable,dum,registerNumber);
                long long int regnum = lookup(registerTable,dum);
                fprintf(fp, "\t%%%d = load i32, i32* %%%s\n",registerNumber, postfix[i].value);

                registerNumber++;

                //fprintf(outputFile, "\t%s\n", tokens[k].value);


                //we have to record this register for future calls.
                char reg[20];
                sprintf(reg, "%%%lld", regnum);

                varExist = 1;
                push(stack,reg);

            }
            else if (strcmp(op, ",") == 0) {
                //skip
                continue;

            } else {
                error = 1;
                break;
            }
        } //else ended
    } //for loop ended
    free(stack);
    return;
}



/**
 * Function that evaluates the postfix expression.
 * @param postfix expression
 * @return long long int, result of the expression
 */


long long int evaluate_postfix(Token *postfix) {

    int stack[MAX_EXPR_LEN];
    int top = -1;
    int i;

    for (i = 0; i < numofpost; i++) {
        // If the current character is a number, push it onto the stack
        if (isdigit(*postfix[i].value)) {
            long long int operand = atoi(postfix[i].value);
            stack[++top] = operand;

        } else {
            int op1, op2;
            long long int result;
            char op[32] = ""; //string to store operators,function names or variable names to print out result of that variable
            int j = 0;
            strcpy(op, postfix[i].value);


            //evaluation steps
            if (strcmp(op, "xor") == 0) {

                op1 = (stack[top--]);
                op2 = (stack[top--]);
                result = op1 ^ op2;
                stack[++top] = result;

            } else if (strcmp(op, "not") == 0) {
                op1 = stack[top--];
                result = ~op1;
                stack[++top] = result;

            } else if (strcmp(op, "*") == 0) {
                op1 = stack[top--];
                op2 = stack[top--];
                result = op2 * op1;
                stack[++top] = result;

            }else if (strcmp(op, "/") == 0) {
                op1 = stack[top--];
                op2 = stack[top--];
                result = op2 / op1;
                stack[++top] = result;
            }
            else if (strcmp(op, "%%") == 0) {
                op1 = stack[top--];
                op2 = stack[top--];
                result = op2 % op1;
                stack[++top] = result;
            }

            else if (strcmp(op, "+") == 0) {
                op1 = stack[top--];
                op2 = stack[top--];
                result = op2 + op1;
                stack[++top] = result;

            } else if (strcmp(op, "-") == 0) {
                op1 = stack[top--];
                op2 = stack[top--];
                result = op2 - op1;
                stack[++top] = result;

            } else if (strcmp(op, "&") == 0) {
                op1 = stack[top--];
                op2 = stack[top--];
                result = op2 & op1;
                stack[++top] = result;

            }
            else if (strcmp(op, "|") == 0) {
                op1 = stack[top--];
                op2 = stack[top--];
                result = op2 | op1;
                stack[++top] = result;

            } else if (strcmp(op, "lr") == 0) {
                op1 = stack[top--];
                op2 = stack[top--];
                result = (op2 << op1) | (op2 >> (sizeof(op2) * 8 - op1));
                stack[++top] = result;

            } else if (strcmp(op, "rr") == 0) {
                op1 = stack[top--];
                op2 = stack[top--];
                result = (op2 >> op1) | (op2 << (sizeof(op2) * 8 - op1));
                stack[++top] = result;

            } else if (strcmp(op, "ls") == 0) {
                op1 = stack[top--];
                op2 = stack[top--];
                result = (op2 << op1);
                stack[++top] = result;

            } else if (strcmp(op, "rs") == 0) {
                op1 = stack[top--];
                op2 = stack[top--];
                result = (op2 >> op1);
                stack[++top] = result;

            }
            else if (isalpha(*postfix[i].value) && strcmp(postfix[i].value, op) ==0) {
                //if not a function name, this is a variable. Thus fetch the value.
                result = lookup(Hashtable,op);
                stack[++top] = result;

            } else if (strcmp(op, ",") == 0) {
                //skip
                continue;

            } else {
                error = 1;
                break;
            }
        }
    }
    if (top != 0) {
        error = 1;
    }

    return stack[top];
}


/**
 * Function that strips whitespaces from a string.
 * @param str
 * @return str without whitespaces.
 */

char *trim(char *str) {
    char *end;

    // Trim leading whitespace
    while (isspace((unsigned char) *str)) {
        str++;
    }

    if (*str == 0) { // All spaces?
        return str;
    }

    // Trim trailing whitespace
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char) *end)) {
        end--;
    }

    // Write new null terminator
    *(end + 1) = '\0';
    return str;
}


/**
 * Function that checks whether a function is in the correct form within the input string.
 * For example: xor(3,4) valid and xor((3,4)) invalid.
 * @param str input string
 * @return validness of the function use.
 */
int is_valid_function(char *str) {

    int degree = 0;
    int validness = 0;

    for (int i = 0; i < strlen(str); i++) {
        //increase the degree when you encounter a left paranthesis.
        if (str[i] == '(') {
            degree++;
        }
            //decrease the degree when you encounter a left paranthesis.
        else if (str[i] == ')') {
            degree--;
        }
        else if (str[i] == ',' && degree == 1) {
            //if there is a comma bw 2 first degree parantheses, function syntax is correct.
            validness = 1;
        }
    }
    return validness;
}


/**
 * Function that checks whether the whole input string has a valid use for a particular fuction.
 * This is done by iterating through the whole string.
 * @param line to be iterated
 * @param function_name function name to be checked
 * @return validness of a function's use in the whole string.
 */

int check_function(char *line, char *function_name) {

    char copyline[strlen(line)];
    strcpy(copyline, line);
    char *func_pos = strstr(copyline, function_name);

    int namesize = strlen(function_name);

    int broken_function = 0;

    while (func_pos != NULL) {
        func_pos = strstr(func_pos, function_name);

        if (func_pos != NULL) {
            func_pos = func_pos + namesize; // second part is the value
            int valid_function = is_valid_function(func_pos);

            if (valid_function == 1) {
                func_pos = strstr(func_pos, function_name);
                continue;

            } else {
                broken_function = 1;
                break;
            }
        }
    }

    if (broken_function) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * Function that checks whether the whole input string has a valid use for a particular operator.
 * This is done by iterating through the whole string.
 * for example, 3-4 valid but 3(-)4 is invalid.
 * @param line to be iterated
 * @param op_name operator to be checked
 * @return validness of a operators use in the whole string.
 */

int is_valid_operator(char *line, char *op_name) {

    char copyline[strlen(line)];
    strcpy(copyline, line);
    char *op_pos = strstr(copyline, op_name);

    int namesize = 1;

    int broken_op = 0;

    while (op_pos != NULL) {
        op_pos = strstr(op_pos, op_name);

        if (op_pos != NULL) {
            op_pos--; //include the paranthesis

            if (*(op_pos) == '(' || *(op_pos + 2) == ')') {
                broken_op = 1;
                break;

            } else {
                op_pos += 3;
                op_pos = strstr(op_pos, op_name);
                continue;
            }

        }
    }

    if (broken_op) {
        return 0; //not valid :(
    } else {
        return 1;
    }
}


int main(int argc, char *argv[]) {
    FILE *inputFile,*outputFile;

    char *inputFileName = argv[1];
    char *outputFileName = malloc(strlen(inputFileName) + 5);
    if (outputFileName == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    }
    strcpy(outputFileName, inputFileName);
    char *dot = strrchr(outputFileName, '.');
    if (dot != NULL) {
        *dot = '\0';
    }
    strcat(outputFileName, ".ll");

    inputFile = fopen(inputFileName, "r");
    outputFile= fopen(outputFileName,"w");

    Hashtable = (table *) malloc(sizeof(table));
    init_table(Hashtable);

    //rotl method
    fprintf(outputFile,  "define i32 @rotl(i32 %%x, i32 %%shift_amount) {\n"
                         "%%rs_amount = sub i32 32, %%shift_amount\n"
                         "%%shifted_left = shl i32 %%x, %%shift_amount\n"
                         "%%shifted_right = lshr i32 %%x, %%rs_amount\n"
                         "%%rotated = or i32 %%shifted_left, %%shifted_right\n"
                          "ret i32 %%rotated\n"
                       "}\n\n");

    //rotr method
    fprintf(outputFile, "define i32 @rotr(i32 %%x, i32 %%shift_amount) {\n"
                        "%%ls_amount = sub i32 32, %%shift_amount\n"
                        "%%shifted_right = lshr i32 %%x, %%shift_amount\n"
                        "%%shifted_left = shl i32 %%x, %%ls_amount\n"
                        "%%rotated = or i32 %%shifted_right, %%shifted_left\n"
                        "ret i32 %%rotated\n"
                        "}\n\n");

    fprintf(outputFile, "; ModuleID = 'advcalc2ir'\n");
    fprintf(outputFile, "declare i32 @printf(i8*, ...)\n");
    fprintf(outputFile, "@print.str = constant [4 x i8] c\"%%d\\0A\\00\"\n\n");


    char line[257] = ""; //input line will be stored in here
    //printf(">");

    fprintf(outputFile, "define i32 @main() {\n");
    int lineNum = 1;


    /**
     * var table keeps track of variables so that we do not allocate space for the same variable twice.
     */
    table* varTable;
    varTable = (table *) malloc(sizeof(table));
    init_table(varTable);

    //Memory needs to be allocated beforehand. So we will perform a first overlook of the file in order to obtain variables that we need to allocate memory for.
    while (fgets(line, sizeof(line), inputFile) !=NULL) {
        //ASSIGNMENT CHECK
        char *pos = strchr(line, '=');

        if (pos != NULL) { // if there is an assignment statement

            *pos = '\0'; // replace = with \0
            char *variable = line; // first part is the variable
            char *value = pos + 1; // second part is the value

            variable = trim(variable);

            value = trim(value);
            //if variable is assigned before give error for advcal2

            //reserved keywords cannot be variable names.

            if (!allAlpha(variable)) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("xor", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("ls", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("rs", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("rr", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("lr", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("not", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            }else{
                //No errors so far, we have to collect the variable and allocate space for it.
                //checking whether have we seen this variable before.
                int presence = lookup(varTable,variable);
                if(presence == 999999){
                    //if not encountered before, add it to the variables and allocate space.
                    insert(varTable,variable,100); //dummy value to check presence.
                    fprintf(outputFile, "\t%%%s = alloca i32\n", variable);
                    lineNum++;
                    continue;
                }
            }

        }
    }

    //allocation ends, we free the varTable.
    free(varTable);
    fclose(inputFile);

    //reopen the file after our first scan for variables.
    inputFile = fopen(inputFileName, "r");
    lineNum=1;

    //start taking inputs
    while (fgets(line, sizeof(line), inputFile) !=NULL) {
        //initializing the register table

        registerTable = (table *) malloc(sizeof(table));
        init_table(registerTable);

        lineNum = 1;

        //blankline inputs
        if (strcmp(line, "\n") == 0 || strcmp(line, " \n") == 0 || strcmp(line, "\t\n") == 0) {
            lineNum++;
            continue;

        }

        //erroneous inputs
        //1)empty paranthesis
        char *paranthesis_pos = strstr(line, "()");
        if (paranthesis_pos != NULL) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;
            //line num should be incremented by one
            continue;
        }

            //2)lines starting with operations. i.e unary cases like +1+b
        else if (line[0] == '+' || line[0] == '-' || line[0] == '*' || line[0] == '&' || line[0] == '|') {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;

            continue;
        }

            /*char *commentPos = strchr(line, '%');
            if (commentPos != NULL) {
                *commentPos = '\0'; //trim the expression to the % part included \0
            }
            **/


            //3)unbalanced paranthesis expressions
        else if (!is_balanced(line)) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;
            continue;
        }

            //4) broken function calls. We need to evalute the whole string for all the 5 functions.

        else if (!check_function(line, "xor")) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;

            continue;

        } else if (!check_function(line, "ls")) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;

            continue;

        } else if (!check_function(line, "rs")) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;
            continue;

        } else if (!check_function(line, "rr")) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;
            continue;

        } else if (!check_function(line, "lr")) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;
            continue;
        }



            //5) we won't allow operators inside paranthesis like 3(+)4

        else if (!is_valid_operator(line, "+")) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;
            continue;

        } else if (!is_valid_operator(line, "-")) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;
            continue;

        } else if (!is_valid_operator(line, "&")) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;
            continue;

        } else if (!is_valid_operator(line, "*")) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;
            continue;

        } else if (!is_valid_operator(line, "|")) {
            printf("Error on line %d!\n",lineNum);lineNum++;
            error = 1;
            continue;
        }


        //ASSIGNMENT CHECK
        char *pos = strchr(line, '=');

        if (pos != NULL) { // if there is an assignment statement

            *pos = '\0'; // replace = with \0
            char *variable = line; // first part is the variable
            char *value = pos + 1; // second part is the value

            variable = trim(variable);
            value = trim(value);
            //if variable is assigned before give error for advcal2

            //reserved keywords cannot be variable names.

            if (!allAlpha(variable)) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("xor", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("ls", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("rs", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("rr", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("lr", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            } else if (strcmp("not", variable) == 0) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;

            }

            //No error so far, we can evaluate the right-hand side of the assignment. i.e the expression part.
            Token postfixx[257];
            Token tokens[257];

            int numtok = 0;
            tokenize(value, tokens, &numtok);
            if (error == 1) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;
            }

            infix_to_postfix(tokens, postfixx);

            //If there has been an error in tokenization.
            if (error == 1) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;
            }
            //ıf not CALL LLVM IR TRANSLATOR
            long long int res = evaluate_postfix(postfixx);


            if (error == 1) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;
            }

            postfix_to_ir(postfixx,outputFile);

            if (error == 1) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;
            }

            /**
             * if assignment -> res
             * else -> regnumber
             *
             */

            if(varExist ==1) {
                fprintf(outputFile, "\tstore i32 %%%d, i32* %%%s\n", registerNumber-1, variable);
            }
            else{
                fprintf(outputFile, "\tstore i32 %lld, i32* %%%s\n", res, variable);
            }

            //add the result to the hashtable.
            insert(Hashtable, variable, res);
            lineNum++;
            continue;

        }

            //EXPRESSION EVALUATION
        else { //normal expression input exists. no assignment statement if numtok is one and lookup result is zero(uninit) return error
            Token postfixx[257];
            Token tokens[257];
            int numtok = 0;

            tokenize(line, tokens, &numtok);

            //comment inputs
            if (numtok == 0) {
                //error = 1;
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;

                continue;
            }

            //if there is a single token and it is a variable, fetch the value.
            if (numtok == 1 && tokens[0].type == IDENT) {

                //if it is not initialized, it is an error.
                int result = lookup(Hashtable,tokens[0].value);

                if(result == 999999){
                    printf("Error on line %d!\n",lineNum);lineNum++;
                    error = 1;
                    continue;


                }else{
                    //fetch the register number

                    insert(registerTable,tokens[0].value,registerNumber);
                    fprintf(outputFile, "\t%%%d = load i32, i32* %%%s\n", registerNumber, tokens[0].value);
                    //check again this register increment
                    fprintf(outputFile,"\tcall i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %%%d )\n",registerNumber++);
                    registerNumber++;
                    lineNum++;
                    continue;
                }
            }

            else {

                infix_to_postfix(tokens, postfixx);

                long long int res = evaluate_postfix(postfixx);

                postfix_to_ir(postfixx, outputFile);


                if (!error) {
                    //check again this register increment
                    fprintf(outputFile,"\tcall i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %%%d )\n",registerNumber-1);
                    registerNumber++;
                    lineNum++;
                    continue;
                }else{
                    printf("Error on line %d!\n",lineNum);
                    error = 1;
                    lineNum++;
                    continue;//error occured
                    //added this -irem

                }


            }


            if (error == 1) {
                printf("Error on line %d!\n",lineNum);lineNum++;
                error = 1;
                continue;
            }


            lineNum++;
            continue;
        }
    }
    free(registerTable);
    free(Hashtable);

    //aborting the file creation in an error
    if(error == 1){
	fclose(inputFile);
	fclose(outputFile);
	remove(outputFileName);
	exit(1);
    }

    fprintf(outputFile, "\tret i32 0\n");
    fprintf(outputFile, "}\n");
    fclose(inputFile);
    fclose(outputFile);
}
