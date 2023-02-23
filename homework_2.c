#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <openssl/sha.h>

char* getStringInput(char* prompt)
{
	char* input;
	char buffer[10];
	size_t inputLength = 0;
	
	printf("%s", prompt);
	
	do 
	{
		if(!fgets(buffer, 10, stdin)) // Read in 10 characters
			abort();
		input = realloc(input, inputLength + strlen(buffer) + 1); // Reallaocate memory for the new charatcers read
		strcpy(input + inputLength, buffer); // Copy the contents of buffer into the input
		inputLength += strlen(buffer); // Add the number of characters read into the total input string length
	} 
	while (strlen(buffer) == 10 - 1 && buffer[10 - 2] != '\n'); // Make sure we don't read in more than 10 characters, and make sure we haven't reached the end of a line
    
	return input;
}

int getIntInput(char* prompt)
{
	int convertedInt;
	bool success;
	char buffer[10]; 
	
	do
    {
		errno = 0; // Reset error
		char *lastChar;
        char* input = getStringInput(prompt);
        convertedInt = (int)strtol(input, &lastChar, 10); // Convert base-10 input to integer
		
        if (errno == ERANGE)
        {
            printf("Error! This number is too small or too large\n");
            success = false;
        }
        else if (lastChar == buffer)
        {
			printf("Error! No character was read\n");
            success = false;
        }
        else if (*lastChar && *lastChar != '\n') // Check to see if whole string was read
        {
			printf("Error! Whole string not converted to int\n");
            success = false;
        }
        else
            success = true;
    } 
	while (!success);
	
	return convertedInt;
}

int main()
{
	// Hash the password because apparently i am dumb
	const char* PASSWORD = "ilovecybe366!";
	const unsigned char HASH[SHA_DIGEST_LENGTH];
	SHA1((unsigned char *)PASSWORD, strlen(PASSWORD), (unsigned char *)HASH);
	
	// Check password
	char passwordHash[SHA_DIGEST_LENGTH]; 
	char* password = getStringInput("Enter the password: "); // Read in password
	SHA1((unsigned char *)password, strlen(password), (unsigned char *)passwordHash); // Hash the input
	
	// Check if hash matches
	if (memcmp(passwordHash, HASH, sizeof(HASH)/sizeof(HASH[0])))
		return 1;
	
	// Actual Program
	int key;
	bool encrypt;
	
	encrypt = (bool)getIntInput("Choose an option:\n(0)Decrypt\n(1)Encrypt\nSelection: ");
	if (encrypt)
		inputString = getStringInput("Enter your string to encrypt");
	else
		inputString = getStringInput("Enter your string to decrypt");
	key = getIntInput("Enter your key: ");

	char outputString[strlen(inputString)];

	if (encrypt)
		// Iterate through each character in the String
		for (int i = 0; i < strlen(inputString); i++)
		{
			if (inputString[i] >= 32 && inputString[i] <= 126)
				outputString[i] = (inputString[i] - ' ' + key) % 95 + ' ';
			else
				outputString[i] = inputString[i];
		}
	else // Decrypt
		for (int i = 0; i < strlen(inputString); i++)
		{
			if (inputString[i] >= 32 && inputString[i] <= 126)
				outputString[i] = (inputString[i] - ' ' - key + 95) % 95 + ' ';
			else
				outputString[i] = inputString[i];
		}
	
	printf("Encrypted message: %s", outputString);
}