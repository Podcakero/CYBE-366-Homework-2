#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <openssl/sha.h>

char* getStringInput(char* prompt, FILE* file)
{
	char* input = NULL; // Set to null so we have memory pre-allocated 
	char buffer[10];
	size_t inputLength = 0;
	
	printf("%s", prompt);
	do 
	{
		if(!fgets(buffer, 10, file)) // Read in 10 characters
		{
			printf("%s\n", "Failed to read in string");
			abort();
		}
		input = realloc(input, inputLength + strlen(buffer) + 1); // Reallaocate memory for the new charatcers read
		strcpy(input + inputLength, buffer); // Copy the contents of buffer into the input
		inputLength += strlen(buffer); // Add the number of characters read into the total input string length
	} 
	while (strlen(buffer) == 10 - 1 && buffer[10 - 2] != '\n'); // Loop until we have read in less than 10 characters and we have not reached a new line
    
	input[strcspn(input, "\n")] = 0; // Remove newline character from input
	if (file != stdin) // Dont close stdin it breaks things
		fclose(file); // close file
	return input;
}

int getIntInput(char* prompt)
{
	int convertedInt;
	char *lastChar;
	
	errno = 0; // Reset error
    char* input = getStringInput(prompt, stdin);
    convertedInt = (int)strtol(input, &lastChar, 10); // Convert base-10 input to integer
	free(input);
		
    if (errno == ERANGE) // Make sure integer is valid size
    {
        printf("Error! This number is too small or too large\n");
        abort();
    }
	
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
	char* password = getStringInput("Enter the password: ", stdin); // Read in password
	SHA1((unsigned char *)password, strlen(password), (unsigned char *)passwordHash); // Hash the input
	
	// Check if hash matches
	if (memcmp(passwordHash, HASH, sizeof(HASH)/sizeof(HASH[0])))
	{
		printf("%s\n", "Incorrect Password");
		abort();
	}
	free(password);
	
	// Actual Program
	bool inputType;
	bool encrypt;
	char* filePath;
	FILE* file;
	char* inputString;
	int key;
	
	inputType = (bool)getIntInput("Choose an option:\n(0)Input Text\n(1)From File\nSelection: ");
	encrypt = (bool)getIntInput("Choose an option:\n(0)Decrypt\n(1)Encrypt\nSelection: ");
	if (inputType) // File input
	{
		filePath = getStringInput("Enter the file path: ", stdin);
		file = fopen(filePath, "r");
		inputString = getStringInput("", file);
	}
	else
		inputString = getStringInput("Enter your string: ", stdin);
	key = getIntInput("Enter your key: ");

	char outputString[strlen(inputString)];
	
	printf("%s\n", inputString);
	printf("%d\n", strlen(inputString));

	if (encrypt)
	{
		// Iterate through each character in the String
		for (int i = 0; i < strlen(inputString); i++)
		{
			if (inputString[i] >= 32 && inputString[i] <= 126)
				outputString[i] = (inputString[i] - ' ' + key) % 95 + ' ';
			else
				outputString[i] = inputString[i];
		}
		printf("Encrypted message: %s\n", outputString);
	}
	else // Decrypt
	{
		for (int i = 0; i < strlen(inputString); i++)
		{
			if (inputString[i] >= 32 && inputString[i] <= 126)
				outputString[i] = (inputString[i] - ' ' - key + 95) % 95 + ' ';
			else
				outputString[i] = inputString[i];
		}
		printf("Decrypted message: %s\n", outputString);
	}
	// Free input string
	free(inputString);
}