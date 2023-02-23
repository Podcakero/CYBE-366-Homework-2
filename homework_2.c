#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <openssl/sha.h>

const char *HASH = "e4226064565f0c1e6c01e1d7506406f3b892453c"; // SHA1 Hash of Password

long getIntInput(char* prompt)
{
	int output;
	char buffer[10]; 
	int success;
	
	do
    {
        printf("%s", prompt);
        if (!fgets(buffer, 10, stdin))
        {
            printf("Error! Failed reading input");
            return 1;
        }

        
        char *endPointer;
        errno = 0; 
        output = (int)strtol(buffer, &endPointer, 10);
        if (errno == ERANGE)
        {
            printf("Error! This number is too small or too large\n");
            success = 0;
        }
        else if (endPointer == buffer)
        {
			printf("Error! No character was read\n");
            success = 0;
        }
        else if (*endPointer && *endPointer != '\n')
        {
			printf("Error! Whole string not converted to int\n");
            success = 0;
        }
        else
        {
            success = 1;
        }
    } while (!success);
	
	return output;
}

bool getBoolInput(char* prompt)
{
	int output;
	char buffer[2];
	int success;
	
	do
    {
        printf("%s", prompt);
        if (!fgets(buffer, 2, stdin))
        {
            printf("Error! Failed reading input");
            return 1;
        }

        char *endPointer;
        errno = 0;
        output = (int)strtol(buffer, &endPointer, 10);
        if (errno == ERANGE)
        {
            printf("Error! This number is too small or too large\n");
            success = 0;
        }
        else if (endPointer == buffer)
        {
			printf("Error! No character was read\n");
            success = 0;
        }
        else if (*endPointer && *endPointer != '\n')
        {
			printf("Error! Whole string not converted to int\n");
            success = 0;
        }
        else
        {
            success = 1;
        }
    } while (!success); // repeat until we got a valid number
	
	return (bool)output;
}

int main()
{
	// Check password
	unsigned char* password;
	unsigned char* passwordHash;
	
	printf("Enter the password: ");
	fgets(password, 50, stdin);
	printf("%s", password);
	password[strcspn(password, "\n")] = 0; // Remove new line from password
	SHA1(password, strlen(password), passwordHash);
	
	printf("%s", passwordHash);
	
	// Check if hash matches
	if (strcmp(passwordHash, HASH) != 0)
		abort();
	
	// Actual Program
	int key;
	bool encrypt;
	char* inputString;
	
	printf("Choose an option:\n");
	printf("(0)Decrypt\n(1)Encrypt\n");
	printf("Selection: ");
	
	char* temp;
	fgets(temp, 2, stdin);
	encrypt = atoi(temp);
	
	printf("Enter your message: ");
    if (fgets(inputString, 500, stdin))
        inputString[strcspn(inputString, "\n")] = 0;
	
	key = getIntInput("Enter your key: ");

	char outputString[strlen(inputString)];

	if (encrypt)
	{
		// Iterate through each character in the String
		for (int i = 0; i < strlen(inputString); i++)
		{
			if (inputString[i] >= 32 && inputString[i] <= 126)
			{
				outputString[i] = (inputString[i] - ' ' + key) % 95 + ' ';
			}
			else
			{
				outputString[i] = inputString[i];
			}
		}
	}
	else // Decrypt
	{
		for (int i = 0; i < strlen(inputString); i++)
		{
			if (inputString[i] >= 32 && inputString[i] <= 126)
			{
				outputString[i] = (inputString[i] - ' ' - key + 95) % 95 + ' ';
			}
			else
			{
				outputString[i] = inputString[i];
			}
		}
	}
	
	printf("Encrypted message: %s", outputString);
}