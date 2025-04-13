/*convert infix to postfix w Dijkstra's algo -> evaluate postfix 
Removes the need to worry about parentheses, etc. */

//Infix to Postfix - Shunting-Yard algorithm

/*Summary of the Rules

    If the incoming symbols is an operand, print it..

    If the incoming symbol is a left parenthesis, push it on the stack.

    If the incoming symbol is a right parenthesis: discard the right parenthesis, pop and print the stack symbols until you see a left parenthesis. Pop the left parenthesis and discard it.

    If the incoming symbol is an operator and the stack is empty or contains a left parenthesis on top, push the incoming operator onto the stack.

    If the incoming symbol is an operator and has either higher precedence than the operator on the top of the stack, or has the same precedence as the operator on the top of the stack and is right associative, or if the stack is empty, or if the top of the stack is "(" (a floor) -- push it on the stack.

    If the incoming symbol is an operator and has either lower precedence than the operator on the top of the stack, or has the same precedence as the operator on the top of the stack and is left associative -- continue to pop the stack until this is not true. Then, push the incoming operator.

    At the end of the expression, pop and print all operators on the stack. (No parentheses should remain.)
*/

//Infix to Postfix using Array Stack
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 128
int is_operator(char c);
int precedence(char op);
char get_matching_open(char close);
void InToPost(char source[], char target[]);
int eval(char postfix[]);
int top=-1;

int main()
{
    char infix[128];
    char postfix[128] = ""; // Initialize as empty string
    
    printf("Enter your infix expression: ");
    fgets(infix, sizeof(infix), stdin); // Safer than gets()
    infix[strcspn(infix, "\n")] = '\0'; // Remove newline
	/*    
	1) strcspn(infix, "\n") - This finds the position of the first newline character (\n) in the string infix. 
	2) The function strcspn returns the number of characters before the first occurrence of any character in the search set ("\n" in this case).
    3) infix[...] = '\0' - This assigns a null terminator (\0) at the position found by strcspn, effectively truncating the string at that point.
	*/
    
    InToPost(infix, postfix);
    printf("\nThe corresponding postfix expression is: %s\n", postfix);
	
	int result=eval(postfix);
    printf("Final result is: %d",result);
	
    getchar();
    return 0;
}

int is_operator(char c) 
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

int precedence(char op) 
{
    switch (op) 
	{
        case '^': return 4;
        case '*': return 3;
        case '/': return 3;
        case '+': return 2;
        case '-': return 2;
        default:  return 0; // For brackets
    }
}

// Define matching pairs (opening -> closing)
char get_matching_open(char close) 
{
    switch (close) 
	{
        case ')': return '(';
        case '}': return '{';
        case ']': return '[';
        default:  return '\0';
    }
}

void InToPost(char infix[],char postfix[])
{
	char stack[MAX_SIZE];
	int i=0,j=0;
	char temp;
	
	for (int i=0;infix[i]!='\0';i++){
		char token =infix[i];
		
		//Operand (alphanumeric)
		if(isdigit(token))
		{
			postfix[j++]=token;
		}
		
		//Opening braces -> push to stack
		else if(token=='('||token=='{'||token=='[')
		{
			stack[++top]=token;	
		}
		
		//Closing bracket -> pop until matching open
		else if(token==')'||token=='}'||token==']')
		{
			while(top>=0 && stack[top]!=get_matching_open(token))
			{
				postfix[j++]=stack[top--];
			}
			if (top<0)
			{
				printf("Mismatched brackets");
				exit(1);
			}
			top--;
		}
		else if(is_operator(token))
		{
			while(top>=0 && precedence(stack[top])>=precedence(token))
			{
				
				postfix[j++]=stack[top--];
			}
			postfix[j++]=' ';
			stack[++top]=token; 
		}
	}
	
	while (top>=0)
	{
		if(stack[top]=='('||stack[top]=='{'||stack[top]=='[')
			{
				printf("\n unclosed brackets");
				exit(1);
			}
			postfix[j++]=stack[top--];
	}
	postfix[j]='\0';
}

//Evaluating postfix expressions
int eval(char postfix[])
{
	int stack[MAX_SIZE];
	int top=-1;
	int i;
	int temp=0;
	stack[0]=0;

    for (int i = 0; postfix[i] != '\0'; i++) {
        char token = postfix[i];
		printf(" %d ",stack[top]);
        
        // Use whitespace to our advantage
        if (token == ' '){
			temp=0;
			++top;
		}
		
		if(isdigit(token))
		{
			temp=(temp*10)+(token-'0');
			stack[++top]=temp;
			--top;
		}
		
		else if(is_operator(token))
		{
			++top;
			int num2=stack[top--];
			int num1=stack[top--];
			
			switch(token) { // Apply operation
                case '+': stack[++top] = num1 + num2; --top; break;
                case '-': stack[++top] = num1 - num2; --top; break;
                case '*': stack[++top] = num1 * num2; --top; break;
                case '/': 
					if(num2==0)
					{
						printf("Error Div by Zero");
						exit(1);
					}
					stack[++top] = num1 / num2; --top;
					break;
                case '^': stack[++top] = pow(num1, num2); --top; break; // Needs <math.h>
                default: 
                    printf("Error: Unknown operator '%c'\n", postfix[i]);
                    exit(1);
			}
		}
	}
	return stack[++top];
}