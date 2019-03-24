/**
 * File        : Board.c
 * Description : C program that uses linked
 *               list data struct to store moves
 *               being made by two players in a tick tack toe game
 * Version     : 1.00
 * @Author     : Namo Najem © 2019
 **/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


// defining our struct so each node contains an int(move made) and pointers to the next and previous nodes
typedef struct node
{
  int data;
  struct node * prev;
  struct node * next;
} node1;

//initializing our functions and paramaters
void append(struct node **, int);
void print_board(struct node *, char ** board);
bool gamewon(char* board);
void load(char** board);
void save(char* board);
void emptyList(struct node** list);



int main(void)
{
  //initializing our linked list and our board
  node1 *list = malloc(sizeof(node1));

  char *board = "123456789"; //board holds game state before any move is made, any move made is appended to the board via the print_board function


  //variables to hold the names of our two players followed by scanf for input
  char player1[15] = "p1";
  char player2[15] = "p2";

  printf("Player1 Enter Name:\n\n");
  scanf("%s",player1);

  printf("Player2 Enter Name:\n\n");
  scanf("%s",player2);

  printf("New game will commence! you can load and save at any time by typing save or load\n\n");
  //this flag is swapped everytime a move(append) is successfully made, swapping turns between players
  bool playeroneflag = 1;


  print_board(list, &board); //first time board is printed before game commences
  while (gamewon(board)) { //checks if game is won, if not continue
    char* c = malloc(10); //variable to hold our input


    //flag determines whose turn it is and prompts
    if (playeroneflag){
      printf("%s turn:\n\n",player1);
    }
    else {printf("%s turn:\n\n",player2);}




    //input is taken
    scanf("%s",c);

    //save flag
    if (strcmp(c,"save") == 0) {
      save(board);

      print_board(list, &board);
    }



    //load flag
    else if (strcmp(c,"load") == 0) {
      load(&board);
      emptyList(&list);

      //after new board is loaded and the linked list is emptied, this repopulates our list with the turns loaded from the save file
      for (size_t i = 0; i < 9; i++) {
        if (board[i] == 'X' || board[i] == 'O') {
          append(&list, i+1);
        }
      }
      print_board(list, &board);
    }





    //undo flag
    else if (strcmp(c,"undo") == 0) {
      node1 *temp = list; //temporary variables so list can remain our head
      node1 *temp2;
      if (list->next == NULL) { //flag to check if there is a move to be undone
        printf("Error, no move to undo\n\n");
      }
      else{ //itterates until second to last node, then frees the next node
        while(temp->next->next !=NULL)
        {
          temp=temp->next;
        }
        temp2=temp->next;
        temp->next=NULL;
        free(temp2);
        strcpy(board, "123456789"); //reseting the board, print_board populates it again with all valid turns

        print_board(list, &board);
      }
    }



    // if non of the flags above are triggered this verifies input for move to be made
    else if (isdigit(c[0]) && c[1] == NULL) {
      int j = atoi(c); //converting input into int
      if (board[j-1] != 'X' && board[j-1] != 'O') { //checking if move has already been made
        //if so move gets appended and flag gets switched
        append(&list,j);
        playeroneflag = !playeroneflag;
        print_board(list, &board);
      }
      //if move isnt valid we prompt
      else {printf("Please choose an unselected square\n\n");
      print_board(list, &board);}
    }


    //if no flag is triggered thus far input is incorrect
  else {printf("Wrong input, please enter a number between 1 and 9 or load or save\n\n");}
  }
//end of while loop that dictates game turn itterations


  //prompts the winner of the game
  if (player1) {
    printf("Congratulations %s! You Won!\n",player1);
  }
  else{printf("Congratulations %s! You Won!\n",player2);}

  return 0;
}


//append function almost identical to that from the practicals
void append(struct node ** list, int squares)
{
  struct node *temp, *current = *list;
  //if list is empty create first node
  if(*list == NULL)
  {
    *list = (struct node *) malloc(sizeof(struct node));
    (*list) -> prev = NULL;
    (*list) -> data = squares;
    (*list) -> next = NULL;
  }
  //if list isnt empty, create a new node and have the previous node point to it
  else
  {
    while(current -> next != NULL)
    {
      current = current -> next;
    }
    temp = (struct node *) malloc(sizeof(struct node));
    temp -> data = squares;
    temp -> next = NULL;
    temp -> prev = current;
    current -> next = temp;
  }
}

//save function for our board, only saves what moves are made to a text file, no order
void save(char* board)
{
  char* file = malloc(10);
  printf("Enter save file name including extension ie:save1.txt\n");
  scanf("%s",file);
  FILE *fileID = fopen(file,"w");
  fprintf(fileID,"%s\n",board);
  fclose(fileID);
}


//load function for our board, loads a string from a text file and sets the game board to equal it,
// the rest of the load happens in the main method because said moves must be appended to the linked list aswell
void load(char** board)
{

  strcpy(*board, "123456789");
  char* file = malloc(10);
  char* txt = malloc(10);
  printf("Enter save state name including extension ie:save1.txt\n");
  scanf("%s",txt);
  FILE *fileID = fopen(txt, "r");
  if (fileID == NULL) {
    printf("Error, no save file with that name\n");
    return;
  }
  fgets(file, 10, fileID);
  strcpy(*board,file);
  fclose(fileID);

}


//functioin that takes the game board in its current state and checks if game is won horizontally, vertically, or diagonally respectively
bool gamewon(char* board)
{
  for (int i = 0; i <= 6; i+=3) {
    if (board[i] == board[i+1] && board[i] == board[i+2] || board[i] == board[i+3] && board[i] == board[i+6]) { //returns false if game is won against the horizontal or vetical axis
      return false;
    }
  }
  if (board[0] == board[4] && board[0] == board[8] || board[2] == board[4] && board[2] == board[6]) { //returns false if diagonal axis
    return false;
  }
  return true;
}


//function that empties the list, this is called every time a load is performed
void emptyList(struct node** list)
{
  node1* prev = *list;
  while (prev->next != NULL)
  {
    *list = (*list)->next;
    free(prev);
    prev = *list;
  }
}


//funtion responsible for both printing the board and appending moves to the board from linked list
void print_board(struct node * current, char ** board)
{
  char move[2] = "XO"; //decides whether to put an X or an O, consistent as player1 will always be the X
  int XOflag = 0;

  while (current->next != NULL) {
    current = current->next;
    int i = current->data - 1;
    (*board)[i] = move[XOflag];
    XOflag = !XOflag;
  }

  //prints board in a 3x3 grid
  printf("\n");
  for (int i = 0; i < 9; i+=3) {
    printf("%c %c %c\n", (*board)[i], (*board)[i+1], (*board)[i+2]);
  }
    printf("\n");
}
