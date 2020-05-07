/**********************************************
* Joe Gilbert and Jeff Marple
* COMP 2400 
* Project 5: Trees Worse than Whomping Willows
* April 2020
***********************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Student.h"

#define MIN_YEAR 1
#define MAX_YEAR 7

//Enum containing all of the error codes.
enum {
	LOAD_FAILURE,
	SAVE_FAILURE,
	ADD_INVALID_YEAR,
	ADD_INVALID_HOUSE,
	ADD_STUDENT_PRESENT,
	KILL_INVALID_HOUSE,
	KILL_NOTHING_FOUND,
	FIND_INVALID_HOUSE,
	FIND_NOTHING_FOUND,
	POINT_CHANGE_INVALID_HOUSE,
	POINT_CHANGE_NOTHING_FOUND,
	UNKNOWN_COMMAND	
}; 

void handleError(int errorValue);
void help();
int compareStudents(char* last1, char* last2, char* first1, char* first2);
House getHouse(char* houseString);

void load(char* fileName, Student* houses[]); 
void save(char* fileName, Student* houses[]);

void saveData(Student* root, FILE *fp);
void printStudent(Student* student);
void inOrderPrint(Student* root);
void preOrderPrint(Student* root);
void postOrderPrint(Student* root);

Student* addStudent(Student* root, Student* studentToAdd);
Student* findStudent(Student* root, char* firstName, char* lastName);
Student* killStudent(Student** root, char* firstName, char* lastName);
int calculateScores(Student* root);
void clear(Student* house);
  


int main(int argc, char **argv)
{

	int active = 1; //Indicates whether the program is running or not.
	//Initialize roots to NULL.

	Student* houses[HOUSES + 1] = {NULL, NULL, NULL, NULL, NULL};
	
	
	while(active == 1)
	{
		char command[10];
		printf("Enter command: ");
		scanf("%s", command);
	
	
		if (strcmp(command, "help") == 0) //GET HELP
		{
			help();
		}
		
		
		else if(strcmp(command, "load") == 0) //LOAD ROSTER
		{
			char filename[100];
			scanf("%s", filename);
			load(filename, houses);
		}
		
		
		else if(strcmp(command, "save") == 0) //SAVE ROSTER
		{
			char filename[100];
			scanf("%s", filename);
			save(filename, houses);

		}
		
		
		else if(strcmp(command, "clear") == 0) //CLEAR
		{
			for(int i = 0; i < HOUSES+1; ++i)
			{
				clear(houses[i]);//free every node of the list 
				houses[i] = NULL; //reinitialize houses
			}

			printf("All data cleared.\n");
		}
		
		
		else if(strcmp(command, "inorder") == 0) //INORDER PRINT
		{
			printf("Inorder print-out of roster:\n");
			printf("\n");
			
			for(int i = 0; i < HOUSES; ++i)
			{
				
				printf("%s House\n", HOUSE_NAMES[i]);
				printf("\n");
				inOrderPrint(houses[i]);
				printf("\n");
			}
			
			printf("Deceased\n");
			printf("\n");
			inOrderPrint(houses[4]);	
			printf("\n");
		}
		
		
		else if(strcmp(command, "preorder") == 0) //PREORDER PRINT
		{

			printf("Preorder print-out of roster:\n");
			printf("\n");
			
			for(int i = 0; i < HOUSES; ++i)
			{
				
				printf("%s House\n", HOUSE_NAMES[i]);
				printf("\n");
				preOrderPrint(houses[i]);
				printf("\n");
			}
			
			printf("Deceased\n");
			printf("\n");
			preOrderPrint(houses[4]);
			printf("\n");


		}
		
		
		else if(strcmp(command, "postorder") == 0) //POSTORDER PRINT
		{

			printf("Postorder print-out of roster:\n");
			printf("\n");
			
			for(int i = 0; i < HOUSES; ++i)
			{
				printf("%s House\n", HOUSE_NAMES[i]);
				printf("\n");
				postOrderPrint(houses[i]);
				printf("\n");
			}
			printf("Deceased\n");
			printf("\n");
			postOrderPrint(houses[4]);
			printf("\n");
		}
		
		
		else if(strcmp(command, "add") == 0) //ADD STUDENT
		{
			char first[100];
			char last[100];
			int points;
			int year;
			char house[100];
			scanf("%s %s %d %d %s", first, last, &points, &year, house);

			if(year < MIN_YEAR || year > MAX_YEAR) //If the year is invalid, throw an error and free memory.
			{
				handleError(ADD_INVALID_YEAR);
			}
			else
			{

				House home = getHouse(house);
			if(home == -1)
			{
				handleError(ADD_INVALID_HOUSE);
			}
			else
			{
			//Search if the student is in the house already

				Student* searchResult = findStudent(houses[home], first, last);

					if(searchResult) //If the student is found in same house, throw an error and free memory.
					{

						handleError(ADD_STUDENT_PRESENT);

					}
					else //Otherwise, add the student.
					{
						Student* newStudent = (Student*)malloc(sizeof(Student));
						newStudent->first = strdup(first);
						newStudent->last = strdup(last);
						newStudent->points = points;
						newStudent->house = home;
						newStudent->year = year;
						newStudent->left = NULL;
						newStudent->right = NULL;
						houses[home] = addStudent(houses[home], newStudent);
						printf("Added %s %s to roster.\n", newStudent->first, newStudent->last);
					}
				}

			}

		}
		
		
		else if(strcmp(command, "kill") == 0 ) //KILL STUDENT
		{
			//fname, lname, house
			char first[100];
			char last[100];
			char house[100];
			scanf("%s %s %s", first, last, house);
			House home = getHouse(house);
			if(home == -1)
			{
				handleError(KILL_INVALID_HOUSE);
			}
			else
			{
				Student* killedStudent = killStudent(&(houses[home]), first, last);
				if(killedStudent == NULL)
				{
					handleError(KILL_NOTHING_FOUND);
				}
				else
				{
					
					houses[HOUSES] = addStudent(houses[HOUSES], killedStudent);
					printf("Moved %s %s to list of deceased students.\n", killedStudent->first, killedStudent->last);
				}
			}
			
		}
		
		
		else if(strcmp(command, "find") == 0) //FIND STUDENT
		{
			char first[100];
			char last[100];
			char house[100];
			scanf("%s %s %s", first, last, house);
			House home = getHouse(house);
			if(home == -1)
			{
				handleError(FIND_INVALID_HOUSE);
			}
			else
			{
			
				Student* result = findStudent(houses[home], first, last);
				if(result == NULL)
				{
					handleError(FIND_NOTHING_FOUND);
				}
				else
				{
					printStudent(result);
				}
			}
		}
		
		
		else if(strcmp(command, "points") == 0) //ADJUST STUDENT'S POINTS
		{
			char first[100];
			char last[100];
			char house[100];
			int points;
			scanf("%s %s %s %d", first, last, house, &points);
			House home = getHouse(house);
			if(home == -1)
			{
				handleError(POINT_CHANGE_INVALID_HOUSE);
			}
			else
			{
				Student* result = findStudent(houses[home], first, last);
				if(result == NULL)
				{
					handleError(POINT_CHANGE_NOTHING_FOUND);
				}
				else
				{
					result->points += points;
					printf("Points for %s %s changed to %d.\n", result->first, result->last, result->points);
				}
			}
		}
		
		
		else if(strcmp(command, "score") == 0) //CALCULATE SCORES
		{
			printf("Point totals:\n\n");

			for(int i = 0; i < HOUSES; ++i)
			{
				char buffer[100];
				sprintf(buffer, "%s", HOUSE_NAMES[i]);

				strcat(buffer, " House:");
				int houseLength = strlen(HOUSE_NAMES[i]);
				char pointLength[5];
				sprintf(pointLength, "%d", calculateScores(houses[i]));
				int tab = 18 - houseLength + strlen(pointLength);
				printf("%s%*d\n", buffer, tab, calculateScores(houses[i]));
  
			}
			
		}
		
		
		else if(strcmp(command, "quit") == 0) //QUIT PROGRAM
		{
			for(int i = 0; i < HOUSES+1; ++i)
			{
				
				clear(houses[i]);//free every node of the list 
				houses[i] = NULL; //reinitialize houses
			}
			printf("All data cleared.\n");
			active = 0;
		}
		
		
		else //COMMAND NOT RECOGNIZED
		{
			handleError(UNKNOWN_COMMAND);
			printf("%s\n", command);
		}
		
		
	}
	
	return 0;
}




//Prints the help screen.
void help()
{
	printf("help\nPrints this diplay\n\n");
	
	printf("load <filename>\nAdds the contents of a file to the current roster\n\n");
	
	printf("save <filename>\nSaves the current roster to a file\n\n");
	
	printf("clear\nClear the current roster\n\n");
	
	printf("inorder\nPrint out an inorder traversal of the roster for each house\n\n");
	
	printf("preorder\nPrint out a preorder traversal of the roster for each house\n\n");
	
	printf("postorder\nPrint out a postorder traversal of the roster for each house\n\n");
	
	printf("add <firstname> <lastname> <points> <year> <house>\nAdds a student to the roster\n\n");
	
	printf("kill <firstname> <lastname> <house>\nMoves a student to the deceased list\n\n");
	
	printf("find <firstname> <lastname> <house>\nSearches for a student in a house\n\n");
	
	printf("points <firstname> <lastname> <house> <points>\nChanges the points a student has earned by the specified amount\n\n");
	
	printf("score\nLists the point totals for all four houses\n\n");
	
	printf("quit\nQuits the program\n\n");
}





//Helper function to give the correct house based on the input string.
//Also checks if the given house is valid.
House getHouse(char* houseString)
{
	if(strcmp(houseString, HOUSE_NAMES[0]) == 0) //If the house is GRYFFINDOR. 
	{
		return GRYFFINDOR;
	}
	else if(strcmp(houseString, HOUSE_NAMES[1]) == 0) //If the house is RAVENCLAW.
	{
		return RAVENCLAW;
	}
	else if(strcmp(houseString, HOUSE_NAMES[2]) == 0) //If the house is HUFFLEPUFF.
	{
		return HUFFLEPUFF;
	}
	else if(strcmp(houseString, HOUSE_NAMES[3]) == 0) //If the house is SLYTHERIN.
	{
		return SLYTHERIN;
	}
	else //If the house is invalid
	{
		return -1;
	}
}


//Compares student's last names to determine order. If the last names are the same, we use the first names as a tie breaker.
//If student1 comes before student2, then we return negative.
//If student1 comes after student2, we return positive.
//Else, we return 0.
int compareStudents(char* last1, char* last2, char* first1, char* first2)
{
	if(strcmp(last1, last2) != 0)
	{
		return strcmp(last1, last2);
	}
	else
	{
		return strcmp(first1, first2);
	}
}



Student* addStudent(Student* root, Student* studentToAdd)
{
	if(root == NULL)  //If the student we're looking at is NULL, then we add the student here.
	{
		root = studentToAdd;	
	}
	else
	{
		//Else if the student comes before the root, visit the left child.
		int comparisonValue = compareStudents(studentToAdd->last, root->last, studentToAdd->first, root->first);
		if(comparisonValue < 0)
		{
			root->left = addStudent(root->left, studentToAdd);
		}
		//Else if the student comes after the root, visit the right child.
		else if(comparisonValue > 0)
		{
			root->right = addStudent(root->right, studentToAdd);
		}
		//Else, the student already exists.
		else
		{
			return root;
		}
	}	
	return root;
}



//Find the student in the house in which root resides.
Student* findStudent(Student* root, char* firstName, char* lastName)
{
	if(root == NULL) //If the root is NULL, the student isn't in the tree.
	{
		return NULL;
	}
	else
	{
		int comparisonValue = compareStudents(lastName, root->last, firstName, root->first);
		if(comparisonValue == 0) //If the names match, return the root.
		{
			return root;
		}
		else if(comparisonValue < 0) //Look at the left child.
		{
			return findStudent(root->left, firstName, lastName);
		}
		else //Look at the right child.
		{
			return findStudent(root->right, firstName, lastName);
		}
	}
}


//Simple helper function that prints out a student's info.
void printStudent(Student* student)
{

	char* printArray = (char*)malloc(sizeof(char)*100);
	char* firstname = student->first;
	char* lastname = student->last;
	int points = student->points;
	int year = student->year;
	char* home = HOUSE_NAMES[student->house];
	char* space = " ";
	char buffer[7];
	int nameSize = strlen(firstname) + strlen(lastname) + 1;

	sprintf(printArray, "%s", firstname);
	strcat(printArray, space);
	strcat(printArray, lastname);

	for(int i = nameSize; i<25; i++)
	{//space between names and points
		strcat(printArray, space);
	}


	strcat(printArray, "Points: ");
	sprintf(buffer, "%d", points);
	strcat(printArray,  buffer);


	for(int i=strlen(buffer); i<6; i++)
	{//spaces between point value and year
		strcat(printArray, space);
	}

	strcat(printArray, "Year: ");
	sprintf(buffer, "%d", year);
	strcat(printArray, buffer);
	strcat(printArray, space);

	strcat(printArray, "House: ");
	strcat(printArray, home);

	printf("%s\n", printArray);

	free(printArray);
}


//Inorder traversal printing.
void inOrderPrint(Student* root)
{
	if(root != NULL)
	{
		inOrderPrint(root->left);
		printStudent(root);
		inOrderPrint(root->right);
	}	
}

//Preorder traversal printing.
void preOrderPrint(Student* root)
{
	if(root != NULL)
	{
		printStudent(root);
		preOrderPrint(root->left);
		preOrderPrint(root->right);
	}
}

//Postorder traversal printing.
void postOrderPrint(Student* root)
{
	if(root != NULL)
	{
		postOrderPrint(root->left);
		postOrderPrint(root->right);
		printStudent(root);
	}
}



//Given the root of the house, calculates the total points earned for that house. Always called initially with total = 0.
//Returns the total points of the house.   
int calculateScores(Student* root)
{
	if(root != NULL)
	{	
		int leftSum = calculateScores(root->left);
		int rightSum = calculateScores(root->right);
		return root->points + leftSum + rightSum;
	}
	else
	{
		return 0;
	}

}



//Clears the roster for a given house
void clear(Student* house)
{
	if(house != NULL)
	{
		clear(house->left);
		clear(house->right);

		free(house->last);
		free(house->first);
		free(house);

	}
}



//Save the records of all living students in the roster to <filename>. The format of the file should match what is read in by the
//load command. Prints out the students in each house starting with Gryffindor, then Ravenclaw, then 
//Hufflepuff, and finally Slytherin. Each house should be printed in a preorder traversal. That way, the file will match the 
//structure of the trees generated.
void save(char* fileName, Student* houses[])
{

	FILE *fp = fopen(fileName, "w");

	if(fp == NULL) 	//cant save file	
	{	
		fclose(fp);
		handleError(SAVE_FAILURE);
	}

		
	for(int i=0; i<HOUSES; i++)//save data to file from every house NOT including deceased
	{
		//fprintf(fp, HOUSE_NAMES[i]); --not sure exactly how he wants formatted 
 		
		saveData(houses[i], fp);

	}
	

	fclose(fp);
}

void saveData(Student* root, FILE *fp)
{

	if(root != NULL)
	{
		char* firstname = root->first;
		char* lastname = root->last;
		int score = root->points;
		int year = root->year;
		char* home = HOUSE_NAMES[root->house];

		fprintf(fp, "%s %s %d %d %s\n", firstname, lastname, score, year, home);

		saveData(root->left, fp);
		saveData(root->right, fp);
	}


}


//adds all the student records in <filename> to the roster. The file is
//arranged one student per line where each record has the format:
//<firstname> <lastname> <points> <year> <house>
void load(char* fileName, Student* houses[])
{
	char first[100];
	char last[100];
	int points;
	int year;
	char house[100];
	FILE *fp = fopen(fileName, "r");


	//this is essentially a slightly modified add() call, except on input
	//file data instead of input from a linked list
	if (fp == NULL) //file doesnt exist; close file and throw error
	{
		fclose(fp);
		handleError(LOAD_FAILURE);
		return;
	}


	//start getting data from the file
	int line = fscanf(fp, "%s %s %d %d %s", first, last, &points, &year, house);//get first 5 values
	while(line == 5)
	{//Assigns the student struct values based off preformat


		Student* newStudent = (Student*)malloc(sizeof(Student));
		newStudent->left = NULL;
		newStudent->right = NULL;
		newStudent->first = strdup(first);
		newStudent->last = strdup(last);
		newStudent->points = points;
		if(year < MIN_YEAR || year > MAX_YEAR) //If the year is invalid, throw an error and free memory.
		{
			free(newStudent);
			handleError(ADD_INVALID_YEAR);
		}
		else
		{
			newStudent->year = year;
			House home = getHouse(house);
			if(home == -1)
			{
				handleError(ADD_INVALID_HOUSE);
			}
			else
			{
			//Search if the student is in the house already

				Student* searchResult = findStudent(houses[home], newStudent->first, newStudent->last);

				if(searchResult) //If the student is found same house, throw an error and free memory.
				{
					free(newStudent);
					handleError(ADD_STUDENT_PRESENT);

				}
				else //Otherwise, add the student.
				{
					newStudent->house = home;
					houses[home] = addStudent(houses[home], newStudent);
					printf("Added %s %s to roster from file %s.\n", newStudent->first, newStudent->last, fileName);
				}
			}

		}
		line = fscanf(fp, "%s %s %d %d %s", first, last, &points, &year, house);//get next line for EOF check
	}//end while

	fclose(fp);
}


//Kill a student.
Student* killStudent(Student** root, char* firstName, char* lastName)
{
	Student* temp = *root;
	if(temp == NULL) //If the root is NULL, the student isn't in the tree.
	{
		return NULL;
	}
	else
	{
		int comparisonValue = compareStudents(lastName, temp->last, firstName, temp->first);
		if(comparisonValue == 0) //If the names match, return the root. Hard part
		{
			if(temp->left == NULL)
			{
				*root = temp->right;
			}
			else if(temp->right == NULL)
			{
				*root = temp->left;
			}
			else
			{
			//Neither children are NULL.
			//If the leftmost child is NULL,  
				if(temp->right->left == NULL)
				{
					*root = temp->right;
					temp->right->left = temp->left;
				}
				else //If the leftmost child of right subtree has right child, move it up to where the leftmost child of the right subtree was.
				{
					Student* current = temp->right->left;
					Student* parent = temp->right;
					while(current->left != NULL)
					{
						parent = current;
						current = current->left;
						
						
					}
					//Current is leftmost child of right subtree
					//Parent left becomes current right
					//Currents left and right becomes temps left and right
					//Root will become current
					parent->left = current->right;
					current->left = temp->left;
					current->right = temp->right;
					*root = current;
					
				}
			}
			//Chop off temps kids
			//Make them NULL
			temp->left = NULL;
			temp->right = NULL;
			return temp;
		}
		else if(comparisonValue < 0) //Look at the left child.
		{
			return killStudent(&(temp->left), firstName, lastName);
		}
		else //Look at the right child.
		{
			return killStudent(&(temp->right), firstName, lastName);
		}
	}
}



void handleError(int errorValue)  //Generates an error message based on the input and sends it to stderr.
{
	fprintf(stderr, "ERROR: CODE %d\n", errorValue);
	switch(errorValue)
	{
		case LOAD_FAILURE:  //Load failed. File to load cannot be opened.
			fprintf(stderr, "Failed to load file: File to load cannot be opened.\n");
			break;
			
		case SAVE_FAILURE:  //Save failed. File to load cannot be opened.
			fprintf(stderr, "Failed to save file: File to load cannot be opened.\n");
			break;
			
		case ADD_INVALID_YEAR:  //Add failed due to invalid year.
			fprintf(stderr, "Add operation failure: invalid year provided.\n");
			break;
			
		case ADD_INVALID_HOUSE:  //Add failed due to invalid house.
			fprintf(stderr, "Add operation failure: invalid house provided.\n");
			break;
			
		case ADD_STUDENT_PRESENT:  //Add failed due to student already in roster.
			fprintf(stderr, "Add operation failure: student is already present in roster.\n");
			break;
			
		case KILL_INVALID_HOUSE:  //Kill failed due to invalid house.
			fprintf(stderr, "Kill operation failure: invalid house provided.\n");
			break;
			
		case KILL_NOTHING_FOUND:  //Kill failed as student not found.
			fprintf(stderr, "Kill operation failure: provided student not found in roster.\n");
			break;
			
		case FIND_INVALID_HOUSE:  //Find failed due to invalid house.
			fprintf(stderr, "Find operation failure: invalid house provided.\n");
			break;
			
		case FIND_NOTHING_FOUND:  //Find failed. Given student not found in given house.
			fprintf(stderr, "Find operation failure: provided student not found in roster.\n");
			break;
			
		case POINT_CHANGE_INVALID_HOUSE:  //Point change failed due to invalid house.
			fprintf(stderr, "Failed to update points: invalid house provided.\n");
			break;
			
		case POINT_CHANGE_NOTHING_FOUND:  //Point change failed. Given student not found in given house. 
			fprintf(stderr, "Failed to update points: provided student not found in roster.\n");
			break;
			
		case UNKNOWN_COMMAND:  //Unknown command provided.
			fprintf(stderr, "Unrecognized command given: ");
			break;
			
		default:
			fprintf(stderr, "Unidentified error.\n");
			break;
			
	}


}



