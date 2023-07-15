#include "heapsort.h"
#include "customer.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

int swap(FILE *file, int size, int i, int j)
{
    customer temp_i;
    customer temp_j;

    if (fseek(file, i * size, SEEK_SET) != 0)
    {
        perror("Error: fseek failed");
        return 0;
    }

    if (fread(&temp_i, size, 1, file) != 1)
    {
        perror("Error: fread failed");
        return 0;
    }

    if (fseek(file, j * size, SEEK_SET) != 0)
    {
        perror("Error: fseek failed");
        return 0;
    }

    if (fread(&temp_j, size, 1, file) != 1)
    {
        perror("Error: fread failed");
        return 0;
    }

    if (fseek(file, i * size, SEEK_SET) != 0)
    {
        perror("Error: fseek failed");
        return 0;
    }

    if (fwrite(&temp_j, size, 1, file) != 1)
    {
        perror("Error: fwrite failed");
        return 0;
    }
    fflush(file);

    if (fseek(file, j * size, SEEK_SET) != 0)
    {
        perror("Error: fseek failed");
        return 0;
    }

    if (fwrite(&temp_i, size, 1, file) != 1)
    {
        perror("Error: fwrite failed");
        return 0;
    }
    fflush(file);

    return 1;
}

int heapify(FILE *file, int size, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    customer largestElement;
    customer leftElement;
    customer rightElement;

    if (fseek(file, largest * size, SEEK_SET) != 0)
    {
        perror("Error: fseek failed");
        return 0;
    }

    if (fread(&largestElement, size, 1, file) != 1)
    {
        perror("Error: fread failed");
        return 0;
    }

    if (left < n)
    {
        if (fseek(file, left * size, SEEK_SET) != 0)
        {
            perror("Error: fseek failed");
            return 0;
        }

        if (fread(&leftElement, size, 1, file) != 1)
        {
            perror("Error: fread failed");
            return 0;
        }
        if ((leftElement.loyalty == largestElement.loyalty && strncmp(leftElement.name, largestElement.name, CUSTOMER_NAME_MAX) > 0) ||
            (leftElement.loyalty > largestElement.loyalty))
        {
            largestElement = leftElement;
            largest = left;
        }
    }

    if (right < n)
    {
        if (fseek(file, right * size, SEEK_SET) != 0)
        {
            perror("Error: fseek failed");
            return 0;
        }

        if (fread(&rightElement, size, 1, file) != 1)
        {
            perror("Error: fread failed");
            return 0;
        }
        if ((rightElement.loyalty > largestElement.loyalty && rightElement.loyalty > leftElement.loyalty) || (rightElement.loyalty == largestElement.loyalty && strncmp(rightElement.name, largestElement.name, CUSTOMER_NAME_MAX) > 0))
        {
            largestElement = rightElement;
            largest = right;
        }
    }

    if (largest != i)
    {
        if (swap(file, size, i, largest) != 1)
        {
            return 0;
        }
        heapify(file, size, n, largest);
    }

    return 1;
}

int heap_sort(FILE *file, int size, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        if (heapify(file, size, n, i) != 1)
        {
            return 0;
        }
    }

    for (int i = n - 1; i >= 0; i--)
    {
        if (swap(file, size, 0, i) != 1)
        {
            return 0;
        }
        if (heapify(file, size, i, 0) != 1)
        {
            return 0;
        }
    }

    return 1;
}

int heapsort(const char *filename)
{
    FILE *file = fopen(filename, "rb+");
    if (file == NULL)
    {
        perror("File does not exist.");
        return 0;
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        perror("Error: fseek failed.");
        return 0;
    }
    long numrecords = ftell(file) / sizeof(customer);

    if (heap_sort(file, sizeof(customer), numrecords) != 1)
    {
        return 0;
    }

    if (fclose(file) != 0)
    {
        perror("Error: fclose failed.");
        return 0;
    }
    return 1;
}