#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <openssl/sha.h>

const unsigned char* password = "ilovecybe366!";
const unsigned char HASH[SHA_DIGEST_LENGTH];
SHA(password, strlen(password), HASH);

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
	char name[40];
    printf("What's your name? ");
    if (fgets(name, 40, stdin))
    {
        name[strcspn(name, "\n")] = 0;
        printf("Hello %s!\n", name);
    }
	
	// Check password
	char password[50];
	char passwordHash[1024]; // 1KiB for SHA1 Hash

	printf("Enter the password: ");
	if (!fgets(password, 50, stdin))
	{
		return 1;
	}
	
	password[strcspn(password, "\n")] = 0; // Remove new line from password
	printf("%s\n", password);
	SHA1((unsigned char *)password, strlen(password), (unsigned char *)passwordHash);
	printf("%s\n", passwordHash);
	printf("%s\n", HASH);
	
	for (int i=0; i<SHA_DIGEST_LENGTH; i++)
		printf("%02x", hash[i]);
	putchar('\n');
	
	// Check if hash matches
	if (strcmp(passwordHash, HASH) != 0)
	{
		abort();
	}
	
	// Actual Program
	int key;
	int encrypt;
	char inputString[1024]; // Limit to 1KiB
	
	printf("Choose an option:\n");
	printf("(0)Decrypt\n(1)Encrypt\n");
	printf("Selection: ");
	
	char temp[2];
	fgets(temp, 2, stdin);
	encrypt = atoi(temp);
	
	printf("Enter your message: ");
    if (!fgets(inputString, 500, stdin))
	{
        return 1;
	}
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