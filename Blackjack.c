/*
Author: Nathan Dunne
Date: 20/12/2016 -> 21/12/2016
Function: Execute a game of Blackjack using Linked Lists.

Any feedback / criticism is welcome and appreciated. 


NOTE: system("cls"); is used in this program to clear the console text. It may not be portable to other OS's other than W10.
It may need to be revised to become portable.
*/

#include <stdio.h>
#include <stdlib.h>
#include "deckConstraints.h"
#define FALSE 0
#define TRUE 1

// This struct is a template for a linked list node that also contains the data that makes up the concept of a "card" 
struct Node
{
	Suit suit;
	Rank rank;
	int value;
	struct Node* next;
};

// This union is a template to allow a player to either use their player number or a given name.
typedef union player
{
	char name[21];
	int playerNum;
}Player;

int count(struct Node* list); // Returns the amount of nodes in a given linked list
int getChips(); // Returns chip value from file.
int getHandValue(struct Node* temp, int checkAce); // Returns the value of a hand

int winCheck(int playerValue, int computerValue, union player playerID, int checkAce, int betAmount, int checkStand);

	/* 
		This winCheck() function is unwieldy - it is unnecessarily complex as it handles too many arguments and does too many things, 
		thus it has become the defining drawback of the program's design.
								
		I need to learn how to better concisely handle large logic problems such as this, breaking it down into more components.
								
		I have the option of making all the functions be more modular by taking in more arguments themselves,
		but I believe that's getting into the area of triple indirection with the pointers and nodes as I pass
		them down through functions, experimentation of which I, currently, do not have time for. 
								
		Needs work. Would functions passed into functions with pointers help here?
	*/


void printList(struct Node* temp); // Prints the given linked list
void printPlayerIdentity(union player player); // Prints the player ID, will be 1 if the player doesn't change it.
void fillDeck(struct Node** deck);     // Fills the deck with the required cards.
void shuffleDeck(struct Node** deckHead);    // shuffles the deck.
void writeChips(int chips); //Write chip value to file
void transferCards(struct Node** origin, struct Node** destination, int amount); // Transfers an amount of cards from origin to destination.

char* getSuitName(int i); // Returns text represenation of suit
char* getRankName(int i); // Returns text represenation of rank

struct Node* insertCard(struct Node* head, int x, int y, int z); // Returns created node given a head, rank suit and value.


int main()
{
	// The linked lists begin as empty.
	struct Node* deckHead = NULL; // The head node of the deck.
	struct Node* playerHead = NULL; // The head node of the player hand.
	struct Node* computerHead = NULL; // The head node of the computer hand.
	struct Node* discardHead = NULL; // The head node of the discard pile.
	
	union player playerOne;
	playerOne.playerNum = 1; // By default the player will be referred to as Player 1 unless changed.
 	
	fillDeck(&deckHead); // Fill the deck list with 52 nodes or "cards".
	shuffleDeck(&deckHead); // Shuffles the deck randomly.
	
	
	// Mandatory seasons greetings will execute once at the beginning of the program.
	printf("\n Hello World, Merry Christmas and a Happy New Year! \n\n \t Welcome to Blackjack at the C Casino.\n\n");
	
	
	char selection;
	
	// Setting loop accessibility. The whole design here needs a lot of work. 
	int menu = TRUE;
	int game = TRUE;
	int round = TRUE;
	int roundStart = FALSE;
	
	int betAmount = 0; //Initialising bet amount.
	
	int maxRounds = 5; // Max rounds before the deck needs to be re-assembled.
								
	while(menu == TRUE) 
	{	
		printPlayerIdentity(playerOne);	// Player identity is printed, at this point it's always going to be "Player 1"
		
		printf(", your chip amount is: %d \n", getChips());
		
			if(getChips() <= 0)
			{
				printf("\n\nYou're out of chips! Resetting to 2000.");
				writeChips(2000);
				printf("\n Your chip amount is now: %d \n\n", getChips());
			}
		
		printf("\n Please Enter: \n");
		printf("\n A to: Play Blackjack");
		printf("\n B to: Configure Player Name.");
   		printf("\n C to: Reset Chips to 2000.");
    	printf("\n D to: Exit.\n\n");
    		
	   	scanf(" %c", &selection);
		   	
	   	switch(selection)
	   	{
		    case 'A' :
		    
			printf("\n Now playing Blackjack. \n");
		    game = TRUE;
			maxRounds = 5;	
			
		    	while(game == TRUE)
		    	{
					round = TRUE;
					roundStart = FALSE;
			    	while((round == TRUE) && maxRounds>0)
			    	{
			    		system("cls");
								
							/*
							  After some research this is what I decided on using to clear the screen. The portability of this function is questionable and I understand that.
							  If any problem arises it's most likely related to this.
							*/
							
			    			if(roundStart == FALSE)	
			    			{
								printf("\n Now playing Blackjack. \n");
								
								printf("\n How much would you like to bet? \n");
								printf("\n A win will double the bet amount, a loss will remove the chips. \n\n ");
								
								scanf(" %d", &betAmount);
								system("cls");
									
								writeChips(getChips() - betAmount);
								
								// Deal the initial two cards to player and computer
					    		transferCards(&deckHead,  &playerHead, 2); 
								transferCards(&deckHead,  &computerHead, 2);
								
								// Testing for blackjack. Needs work. 
								round = winCheck(getHandValue(playerHead, TRUE), getHandValue(computerHead, TRUE), playerOne, TRUE, betAmount, FALSE); 
							
								roundStart = TRUE;
							}
								 
						printf("\n Computer Hand Value: %d \n", getHandValue(computerHead, FALSE));
						printList(computerHead);
								
								
						printf("\n ");
						printPlayerIdentity(playerOne);
						printf(" Hand Value: %d  \n",getHandValue(playerHead, FALSE));
						printList(playerHead);
								
						printf("\n Bet Amount: %d chips\n\n", betAmount);
						
						

							
						
							if(round == TRUE)	
							{	
								printf("\n Please Enter: \n");
								printf("\n A to: Hit");
								printf("\n B to: Stand\n");
										
								scanf(" %c", &selection);
									
					        	switch(selection)
				   				{
				   					case 'A' :
				   						system("cls");
									    transferCards(&deckHead,  &playerHead, 1);
									    printf("\n Computer Hand Value: %d \n", getHandValue(computerHead, FALSE));
										printList(computerHead);
									
									
										printf("\n ");
										printPlayerIdentity(playerOne);
										printf(" Hand Value: %d  \n",getHandValue(playerHead, FALSE));
										printList(playerHead);
										
									    round = winCheck(getHandValue(playerHead, FALSE), getHandValue(computerHead, FALSE), playerOne, FALSE, betAmount, FALSE);
									    break;
									        
									case 'B' :
									    printf(" \n Standing");
									    										    	
									    while(round == TRUE)
									    {	
											system("cls");
									    	transferCards(&deckHead,  &computerHead, 1);
										    printf("\n Computer Hand Value: %d \n", getHandValue(computerHead, FALSE));
											printList(computerHead);
									
									
											printf("\n ");
											printPlayerIdentity(playerOne);
											printf(" Hand Value: %d  \n",getHandValue(playerHead, FALSE));
											printList(playerHead);
											
									    	round = winCheck(getHandValue(playerHead, FALSE), getHandValue(computerHead, FALSE), playerOne, FALSE, betAmount, TRUE);
										}
										
										
									    break;   
											 
									default :
									    printf("\nInvalid selection, please enter capital letters.\n" );
									    printf("Press any key to continue. \n" );
									    getch();
								}
		   					}
		   					
		   					if(round == FALSE)
		   					{
		   						// Transfer the cards in the hands into the discard pile
		   						transferCards(&playerHead,  &discardHead, count(playerHead));
								transferCards(&computerHead,  &discardHead, count(computerHead));
								
								maxRounds--;
								
								if( maxRounds >0)
								{
									printf("\nYour chip amount is: %d \n", getChips());
									printf("\n\nWould you like to play another round? Y/N\n");
									scanf(" %c", &selection);
									
									if((selection == 'Y') || (selection == 'y'))
									{
										round = TRUE;
										roundStart = FALSE;
									}
									else if((selection == 'N') || (selection == 'n'))
									{
										system("cls");
										printf("\nReturning to main menu...\n\n");
										game = FALSE;
									}
								}
								else
								{
									printf("\n\nMax rounds reached. Returning to main menu.\n\n");
									game = FALSE;
								}
							}

		   			}
		   				
	   			}
	   			
	   			// Transfer the cards in the discard pile back into the deck for re-use
	   			transferCards(&discardHead,  &deckHead, count(discardHead));
	   				
		        break;
		        
		    case 'B' :
		    	system("cls");
		    	printf("\n Please Enter your name (No spaces, Max 20 characters): \n"); // Needs work to make the size variable.
		    	scanf("%s", playerOne.name);
				printf("\n Thanks, %s. \n", playerOne.name);
		        break;
		         
			case 'C' :
				system("cls");
		        printf("\n Resetting Chips... \n\n" );
				writeChips(2000);
		        break;
		        
		    case 'D' :
		    	menu = FALSE;
		        break;    
		    default :
		         printf("Invalid selection\n" );
 		}
 	}

	
	printf("\nExiting. Thanks for playing! Press any key to end." );
	
	getch();
    return(0);
}

int getChips()
{
	FILE * pFile;
	int number;
	
	pFile = fopen ("chips.txt","r");
	
	if (pFile!=NULL)
	{
		fscanf(pFile, "%d", &number);
		fclose (pFile);
	}
	
	return number;
}

int count(struct Node* list)
{
    struct Node *temp = list;
    int length = 0;
    
    while(temp!=NULL)
    {
        length++;
        temp=temp->next;
    }
    
	return length;
}

// Dissapointed in this, the combination of lazy design and time constraints has greatly damaged the generality of the program here. 
// By it's design, it can't even check against all BlackJack win/loss conditions even if it tried as Ace needs to both be checked as 1 and 10 at all times.
int winCheck(int playerValue, int computerValue, union player playerID, int checkAce, int betAmount, int checkStand)
{
	if(computerValue == 21 && checkAce == TRUE)
	{
		printf("\nBlackjack! The computer wins.\n");
		return FALSE;
	}
	else if(playerValue == 21 && checkAce == TRUE)
	{
		printf("\nBlackjack! ");
		printPlayerIdentity(playerID);
		printf(" wins.\n");
		printf("\nYou have won: %d chips", betAmount*2);
		writeChips((getChips()) + (betAmount*2) + betAmount);
		return FALSE;
	}
	else if( (computerValue != 21)  && (checkAce == TRUE))
	{	
		return TRUE;
	}
	else if( (playerValue != 21)  && (checkAce == TRUE))
	{		
		return TRUE;
	}
	
	if(playerValue > 21)
	{
		printf("\nBust! The computer wins.");
		printf("\n You have lost: %d chips.", betAmount);
		return FALSE;
	}
	else if((checkStand == TRUE) && (computerValue >= playerValue)  && (computerValue <= 21))
	{
		printf("\nThe computer wins.");
		printf("\nYou have lost: %d chips.", betAmount);
		return FALSE;
	}
	else if(computerValue > 21)
	{
		printf("\nThe computer busted! You win!.");
		printf("\nYou have won: %d chips.\n", betAmount*2);
		
		writeChips((getChips()) + (betAmount*2) + betAmount);
		return FALSE;
	}
	else
	{
		return TRUE;
	}
		
}

void writeChips(int chips)
{
	FILE *fp;

   fp = fopen("chips.txt", "w+");
   fprintf(fp, "%d", chips);
   fclose(fp);
}


// For transfering the cards I've passed in two pointers to pointers and used double indirection to access and re-write the head values behind the parameters.
void transferCards(struct Node** origin, struct Node** destination, int amount)
{
	struct Node* temp = *origin; // Storing the head node of the origin.
	struct Node* limbo = NULL; // Creating a go between for our nodes.
	
	int i=0;
	
	for(i=0;i<amount;i++)
	{
	    limbo = temp->next; //  limbo head node equals Temp/origin second node
	    temp->next = *destination; // Temp second node equals to head node of destination
	    *destination = temp;     // destination head node will equal temp head node
	    temp = limbo; // Temp head node equals limbo head node
	    
	    *origin = temp;  // origin head node equals temp head node
	}    
}


void fillDeck(struct Node** deck)
{
    int i,j,k, value = 0;
    
    /*
	    This loops through both the suit and rank enumerations to fill a standard deck of
	    52 cards with 4 different suits and 13 ranks for each suit.
    */
    for(i=0, j=Hearts; i<=INITIALDECKSIZE, j<=Diamonds; j++)
   	{	
  		for(k=Ace; k<=King; i++, k++) 		
  		{
			value = k;
			
			if(value == 11 || value == 12 || value == 13)
 			{
 				value=10;
			}
			
 			*deck = insertCard(*deck, j, k, value);
			// Inserting  the suit and rank of 52 nodes into the deck linked list

  		}
   	}
}


/*
	 I didn't include recursion for the savingsInvestment question in the exam where it was appropriate to do so, so I wanted to implement it in my project somewhere.
	 This printList method recursively prints a string representation of the Rank and Suit enum elements of each node in a given linked list.
*/

void printList(struct Node* temp)
{
	
	if(temp== NULL)
	{
		return; // I don't want to print out the elements of an empty list or when there are no more nodes remaining in the list.
	}
	else
	{
		printf(" %s", getRankName(temp->rank)); // Print out the string representation of the rank enum
		printf(" of %s", getSuitName(temp->suit)); // Print out the string representation of the suit enum
		printf("\n");
		
		printList(temp->next); // Pass in the address to the next node for the next call.
	}
	
}

void printPlayerIdentity(union player playerID)
{
	if(playerID.playerNum == 1)
	{
		printf("Player %d", playerID.playerNum);
	}
	else
	{
		printf("%s", playerID.name);
	}
}

/*
	 To shuffle the deck I've collected the addresses of each node in the deck in an array of void pointers, 
	 randomised that array and then re-threaded the linked list using those randomised addresses.
 */
 
void shuffleDeck(struct Node** deckHead)
{
    int positionInDeck, randomPosition, i = 0;     
   	int deckSize = count(*deckHead); // Getting the size of the deck
   	
    void *deckNodePointers[deckSize]; // This temporary array of type void and size returned by count() will store the addresses of each node in the deck. 
    
	struct Node* temp = *deckHead;


	/* 	I want to include the address of the head node in my array and then loop through the remainder of the list.
		To do this I've seperated the assignment of the address stored in the head node from the loop and then incremented 
		i by 1 to reconsile the offset.
	*/
	
	deckNodePointers[i] =temp;
	i+=1;
	
	
	// Collecting the remaining addresses
	while(temp != NULL)
	{	
		deckNodePointers[i] =temp->next;
		temp = temp->next;
		i++;
	}
	
	   	srand (time(NULL)); // Seeding rand with NULL time
	   	
	// Randomising all the addresses in the array
	for(positionInDeck = 0;positionInDeck<deckSize; positionInDeck++) 
    { 
      randomPosition = rand()%deckSize; // Gets  random position within the deck
      temp = deckNodePointers[positionInDeck];   // Creates a temp copy of the position to be changed.
      deckNodePointers[positionInDeck] = deckNodePointers[randomPosition]; // The first position is equal to the random position.
      deckNodePointers[randomPosition] = temp;  // The random position is equal to the stored temp position.
    } 
    
   	i=0;
 	
 	// Assigning the head node of deck
   	*deckHead = deckNodePointers[i];
   	i+=1;
   	
   	// Re-assigning temp, as it has been modified while collecting addresses.
   	temp = *deckHead;
   	
   	// Re-threading the linked list with the rest of the new randomised addresses.
   	while(temp != NULL)
	{	
		temp->next = deckNodePointers[i] ; // Assigns the address contained in the node to the i'th index of the array.
		temp = temp->next;
		i++;
	}
	
}

int getHandValue(struct Node* temp, int checkAce)
{
	int	handValue=0;
	
	while(temp != NULL)
	{
		int checkStr = strcmp(getRankName(temp->rank), "Ace");
		
			// strcmp returns a value of 0 if true. Why does it not return 1? Needs work.
			if((checkStr == 0) && (checkAce == TRUE))
			{
				handValue+= temp->value + 10;
			}
			else
			{
				handValue+= temp->value;
			}
		
		temp = temp->next;
		
	}

	return handValue;
}

char* getSuitName(suit)
{
   switch (suit)
   {
      case Diamonds: return "Diamonds";
      case Clubs: return "Clubs";
      case Hearts: return "Hearts";
      case Spades: return "Spades";
      default: return "Error in card suit/name"; 
   }
}

char* getRankName(rank)
{
   switch (rank)
   {
   	  case Ace: return "Ace";
      case Two: return "Two";
      case Three: return "Three";
      case Four: return "Four";
      case Five: return "Five";
      case Six: return "Six";
      case Seven: return "Seven";
      case Eight: return "Eight";
      case Nine: return "Nine";
      case Ten: return "Ten";
      case Jack: return "Jack";
      case Queen: return "Queen";
      case King: return "King";
      default: return "Error in card rank/name"; 

   }
}

struct Node* insertCard(struct Node* head, int x, int y, int z)
{	
	/*
	 I have learned that in C++ you have to cast the return value of malloc to Node*, 
	 however in C, it is automatically promoted by the compiler from type void.
	 
	 I have decided to include the cast syntax below anyway.
	*/
	
	struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
	
	temp->suit = x; 
	temp->rank = y;
	temp->value = z;
	temp->next = head; // address of next node is now head.
	
	head = temp; // head node of the deck = temp.
	
	return head;
}
