#include <stdio.h>
#include "heapsort.c"

void printFileContents(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("Failed to open the file.");
        return;
    }

    customer record;
    size_t numRecords = 0;

    while (fread(&record, sizeof(customer), 1, file) == 1)
    {
        printf("Name: %s, Loyalty: %u\n", record.name, record.loyalty);
        numRecords++;
    }

    fclose(file);

    printf("Total records: %zu\n", numRecords);
}

int main()
{
    int value = heapsort("test1.dat");

    const char *filename = "test1.dat";

    printf("return value: %d\n", value);

    printFileContents(filename);

    return 0;
}
