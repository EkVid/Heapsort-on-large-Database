#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    char name[44];
    unsigned int loyalty;
} customer;

void generateRandomName(char *name, int maxLength)
{
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int length = rand() % maxLength + 1;

    for (int i = 0; i < length; i++)
    {
        int index = rand() % (sizeof(charset) - 1);
        name[i] = charset[index];
    }

    name[length] = '\0';
}

void generateRandomCustomers(customer *customers, size_t numRecords)
{
    srand(time(NULL));

    for (size_t i = 0; i < numRecords; i++)
    {
        generateRandomName(customers[i].name, sizeof(customers[i].name) - 1);
        customers[i].loyalty = rand() % 1000 + 1;
    }
}

int main()
{
    FILE *file = fopen("test1.dat", "wb");
    if (file == NULL)
    {
        perror("Failed to create the sample file.");
        return 1;
    }

    const size_t numRecords = 90000; // Change this to whatever number of records needed
    customer customers[numRecords];

    generateRandomCustomers(customers, numRecords);

    fwrite(customers, sizeof(customer), numRecords, file);

    fclose(file);

    printf("Sample test file created successfully.\n");

    return 0;
}
