#include <stdio.h>
#include <string.h>
#include <time.h>

int compare_dates (char date[])
{
	time_t ti = time(NULL);
	struct tm tm = *localtime(&ti);
	char *t;
	t=strtok(date, "-");
	int d=atoi(t);
	if (d < (tm.tm_year+1900))
		return 1;
	if (d > (tm.tm_year+1900))
		return 0;
	t=strtok(NULL, "-");
	d=atoi(t);
	if (d < (tm.tm_mon+1))
		return 1;
	if (d > (tm.tm_mon+1))
		return 0;
	t=strtok(NULL, "");
	d=atoi(t);
	if (d < tm.tm_mday)
		return 1;
	return 0;
}

void borrowBook()
{
	time_t ti = time(NULL);
	struct tm tm = *localtime(&ti);
	int flag=0;
	char bookID[100], userID[100], date[100], line[100], copy[100], *t ;
	gets(bookID);
	printf("Enter UsedID : ");
	gets(userID);
	printf("Enter Book Accession Number : ");
	gets(bookID);
	sprintf(date, "%d-%d-%d", tm.tm_year + 1900+ (tm.tm_mon + 7)/12, (tm.tm_mon+6)%12+1, tm.tm_mday);
	
	FILE *orgfile=fopen("Users.csv","r");
	FILE *replacer= fopen("replace.txt", "w");
	fgets(line, 100, orgfile);
	fprintf(replacer, "%s", line);
	while(fgets(line, 100, orgfile))
	{
		strcpy(copy, line);
		t=strtok(copy, ",");
		if(strcmp(userID, t)!=0)
			fprintf(replacer, "%s", line);
		else
		{
			fprintf(replacer, "%s,%s,%s(%s)\n", userID, strtok(NULL, ","), bookID, date);
			t=strtok(NULL, "\n");
			if(strcmp("None", t)==0)
				flag++;
			else
			{
				printf("This book could not be borrowed as user has not yet returned the previously borrowed book");
				fclose(orgfile);
				fclose(replacer);
				remove("replace.txt");
				return ;
			}
		}
	}
	fclose(orgfile);
	fclose(replacer);
	
	orgfile=fopen("Books.csv","r");
	replacer= fopen("replace.csv", "w");
	fgets(line, 100, orgfile);
	fprintf(replacer, "%s", line);
	while(fgets(line, 100, orgfile))
	{
		strcpy(copy, line);
		t=strtok(copy, ",");
		if(strcmp(bookID, t)!=0)
			fprintf(replacer, "%s", line);
		else
		{
			fprintf(replacer, "%s,%s,%s,%s (%s)\n", bookID, strtok(NULL, ","), strtok(NULL, ","), userID, date);
			t=strtok(NULL, "\n");
			if(strcmp("None", t)==0)
				flag++;
			else
			{
				printf("This book couldnot be borrowed as someone has already borrowed it");
				fclose(orgfile);
				fclose(replacer);
				remove("replace.csv");
				return;
			}
		}
	}
	fclose(orgfile);
	fclose(replacer);
	
	if (flag==2)
	{
		remove("Users.csv");
		rename("replace.txt", "Users.csv");
		remove("Books.csv");
		rename("replace.csv", "Books.csv");
		printf("Book borrowed. Please return it by %s", date);
	}
	else
	{
		printf("Book couldnot be borrowed as either the book or the user has not been registered");
		remove("replace.txt");
		remove("replace.csv");
	}
}

void returnBook()
{
	int flag=0;
	char bookID[100], userID[100], line[100], copy[100], *t ;
	gets(bookID);
	printf("Enter UsedID : ");
	gets(userID);
	
	FILE *orgfile=fopen("Users.csv","r");
	FILE *replacer= fopen("replace.txt", "w");
	fgets(line, 100, orgfile);
	fprintf(replacer, "%s", line);
	while(fgets(line, 100, orgfile))
	{
		strcpy(copy, line);
		t=strtok(copy, ",");
		if(strcmp(userID, t)!=0)
			fprintf(replacer, "%s", line);
		else
		{
			fprintf(replacer, "%s,%s,None\n", userID, strtok(NULL, ","));
			t=strtok(NULL, " (");
			strcpy(bookID,t);
		}
	}
	fclose(orgfile);
	fclose(replacer);
	
	orgfile=fopen("Books.csv","r");
	replacer= fopen("replace.csv", "w");
	fgets(line, 100, orgfile);
	fprintf(replacer, "%s", line);
	while(fgets(line, 100, orgfile))
	{
		strcpy(copy, line);
		t=strtok(copy, ",");
		if(strcmp(bookID, t)!=0)
			fprintf(replacer, "%s", line);
		else
		{
			fprintf(replacer, "%s,%s,", bookID, strtok(NULL, ",")); 
			t=strtok(NULL, ",");
			fprintf(replacer, "%s,None\n", t);
			printf("Please return it on shelf %s", t);
			strtok(NULL, "(");
			t=strtok(NULL, ")");
			if (compare_dates(t)==1)
				printf("\nAlso pay late fine at the counter");
		}	
	}
	fclose(orgfile);
	fclose(replacer);
	
	/*remove("Users.csv");
	rename("replace.txt", "Users.csv");
	remove("Books.csv");
	rename("replace.csv", "Books.csv");*/
}

void addBook()
{
	FILE *file= fopen("Books.csv", "a+");
	char ID[100], name[100], line[100], c=',';
	gets(ID);
	printf("Enter Book Accession Number : ");
	gets(ID);
	printf("Enter Name : ");
	gets(name);
	strncat(ID, &c, 1);
	strcat(ID, name);
	printf("Enter Shelf where you will keep it : ");
	gets(name);
	fseek(file, 0, SEEK_SET);
	fgets(line, 100, file);
	while(fgets(line, 100, file))
	{
		strtok(line, ",");
		strtok(NULL, ",");
		char *shelf=strtok(NULL, ",");
		if (strcmp(name, shelf)==0)
		{
			printf("Shelf already in use. New book could not be added");
			fclose(file);
			return ;
		}
	}
	fprintf(file, "%s,%s,None\n", ID, name);
	printf("Book added");
	fclose(file);
}

void addUser()
{
	char ID[100], name[100];
	gets(ID);
	FILE *file = fopen("Users.csv", "a");
	printf("Enter User ID : ");
	scanf("%s", ID);
	printf("Enter Full Name : ");
	scanf("%s", name);
	fprintf(file, "%s,%s,None\n", ID, name);
	printf("User Added");
	fclose(file);
}

void removeBook()
{
	char ID[100], line[100], copy[100], *t ;
	gets(ID);
	printf("Enter Book Accession Number : ");
	gets(ID);
	FILE *orgfile=fopen("Books.csv","r");
	FILE *replacer= fopen("replace.txt", "w");
	fgets(line, 100, orgfile);
	fprintf(replacer, "%s", line);
	while(fgets(line, 100, orgfile))
	{
		strcpy(copy, line);
		t=strtok(copy, ",");
		if(strcmp(ID, t)!=0)
			fprintf(replacer, "%s", line);
		else
		{
			strtok(NULL, ",");
			strtok(NULL, ",");
			t=strtok(NULL, "\n");
			if(strcmp("None", t)==0)
				printf("Book removed");
			else
			{
				printf("Book could not be removed as it has been borrowed");
				fclose(orgfile);
				fclose(replacer);
				remove("replace.txt");
				return;
			}
		}
	}
	fclose(orgfile);
	fclose(replacer);
	remove("Books.csv");
	rename("replace.txt", "Books.csv");
}

void removeUser()
{
	char ID[100], line[100], copy[100], *t ;
	gets(ID);
	printf("Enter UserID : ");
	gets(ID);
	FILE *orgfile=fopen("Users.csv","r");
	FILE *replacer= fopen("replace.txt", "w");
	fgets(line, 100, orgfile);
	fprintf(replacer, "%s", line);
	while(fgets(line, 100, orgfile))
	{
		strcpy(copy, line);
		t=strtok(copy, ",");
		if(strcmp(ID, t)!=0)
			fprintf(replacer, "%s", line);
		else
		{
			strtok(NULL, ",");
			t=strtok(NULL, "\n");
			if(strcmp("None", t)==0)
				printf("User removed");
			else
			{
				printf("User could not be removed as borrowed book has not been returned");
				fclose(orgfile);
				fclose(replacer);
				remove("replace.txt");
				return ;
			}
		}
	}
	fclose(orgfile);
	fclose(replacer);
	remove("Users.csv");
	rename("replace.txt", "Users.csv");
}

int main(void)
{
	printf("Enter 1 to borrow a book ");
	printf("\nEnter 2 to return a book");
	printf("\nEnter 3 to add a new book");
	printf("\nEnter 4 to add a new user");
	printf("\nEnter 5 to remove a book");
	printf("\nEnter 6 to remove a user");
	printf("\nEnter your choice : ");
	int ch;
	scanf("%d", &ch);
	if (ch==1)
		borrowBook();
	else if (ch==2)
		returnBook();
	else if (ch==3)
		addBook();
	else if (ch==4)
		addUser();
	else if (ch==5)
		removeBook();
	else if (ch==6)
		removeUser();
	else
		printf("Invalid Input");
	printf("\nT");
	printf("hanks for using this program made by S");
	printf("anskriti");
	return 0;
}