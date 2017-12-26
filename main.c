#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIALCAPACITY 100
#define BYTE 8
#define BYTES 6

typedef __int64_t int_t;

void newLine()
{
    printf("\n");
}

typedef struct vector
{
    int_t *buf;
    size_t size, capacity;
} vector;

vector newVector()
{
    vector data;

    data.size = 0;
    data.capacity = INITIALCAPACITY;
    data.buf = calloc(data.capacity, sizeof(int_t));

    return data;
}

void append(vector *data, int_t value)
{
    if (data->capacity > data->size)
    {
        data->buf[data->size] = value;
        data->size++;
        return;
    }

    data->capacity *= 2;
    data->capacity++;
    int_t *tempBuf = calloc(data->capacity, sizeof(int_t));
    memcpy(tempBuf, data->buf, data->size * sizeof(int_t));
    free(data->buf);
    data->buf = tempBuf;
    data->buf[data->size] = value;
    data->size++;
}

void move(vector *dst, vector *src)
{
    free(dst->buf);
    dst->size = src->size;
    dst->capacity = src->capacity;
    dst->buf = src->buf;

    src->size = 0;
    src->buf = calloc(src->capacity, sizeof(int_t));
}

void print(const vector data)
{
    for (size_t i = 0; i < data.size; i++)
        printf("%ld ", data.buf[i]);
    newLine();
}

int isPrime(int_t value)
{
    if (value == 1)
        return 0;
    if (value == 2)
        return 1;

    int_t border = (int_t)ceil(sqrt(value));

    for (int_t i = 2; i <= border; i++)
        if (!(value % i))
            return 0;

    return 1;
}

void generateDataFile(const char *name, vector data)
{
    FILE *dataFile = fopen(name, "wb");

    for (size_t i = 0; i < data.size; i++)
        for (int byteNumber = 0; byteNumber < BYTES; byteNumber++)
        {
            unsigned char byte = (unsigned char)(data.buf[i] >> byteNumber * BYTE);
            //printf("%X ", byte);
            fprintf(dataFile, "%c", byte);
        }

    //newLine();

    fclose(dataFile);
}

int_t readNextValue(FILE *dataFile)
{
    unsigned char buf;
    int counter = 0;
    int_t temp = 0;

    while (fscanf(dataFile, "%c", &buf) != EOF)
    {
        int_t shift = buf;
        temp += shift << counter * BYTE;
        if (++counter == BYTES)
            return temp;
    }

    return EOF;
}

vector readDataFile(const char *name)
{
    vector data = newVector();
    int_t value;

    FILE *dataFile = fopen(name, "rb");
    if (!dataFile)
        return data;

    while ((value = readNextValue(dataFile)) != EOF)
        append(&data, value);

    fclose(dataFile);

    return data;
}

int_t processChain(vector *chain, int_t value)
{
    append(chain, value);
    //print(*chain);

    return value;
}

void electChain(vector *chosen, vector *devastated)
{
    if (devastated->size > chosen->size || devastated->size == chosen->size && devastated->buf[0] > chosen->buf[0])
        move(chosen, devastated);
    devastated->size = 0;
    //print(*chosen);
}

vector findBestPrimeChain(const char *name)
{
    vector bestChain = newVector(),
           curChain = newVector();
    int_t value,
          lastPrime = 0;

    FILE *dataFile = fopen(name, "rb");
    if (!dataFile)
    {
        free(curChain.buf);
        return bestChain;
    }

    while ((value = readNextValue(dataFile)) != EOF)
        if (isPrime(value))
        {
            if (value > lastPrime)
                lastPrime = processChain(&curChain, value);
            else
            {
                electChain(&bestChain, &curChain);
                lastPrime = processChain(&curChain, value);
            }
        }

    electChain(&bestChain, &curChain);

    fclose(dataFile);

    free(curChain.buf);
    return bestChain;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Wrong arguments!\n");
        return 1;
    }

    int printData = 0;
    if (argc == 3 && argv[2][0] == 'p')
        printData = 1;

    if (printData)
    {
        vector myData = readDataFile(argv[1]);
        print(myData);
        free(myData.buf);
    }

    vector result = findBestPrimeChain(argv[1]);
    print(result);
    free(result.buf);

    return 0;
}