#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cmath>

bool flip_coin = true;
const unsigned int MAX_COIN_FLIPS = 2213; //Prime numbers only
const unsigned int PRECISION = 1000;
const unsigned int NUM_FLIPS_PER_SECOND=10000;
unsigned int COIN_VALUES[MAX_COIN_FLIPS];

const unsigned int INITIALIZATION_QUANTITY = 113;
float MEAN_VALUES[INITIALIZATION_QUANTITY];
float STANDARD_DEVIATION = 0;
float GLOBAL_MEAN = 0;

template<typename T>
float calculateMean(T values[], unsigned int size)
{
    T sum = 0;
    for(unsigned int i = 0; i < size; ++i)
        sum += values[i];

    return (float) sum / size;
}

template<typename T>
float calculateStandardDeviation(T values[], unsigned int size)
{
    T sum = 0;
    float mean = calculateMean(values, size);
    for(unsigned int i = 0; i < size; ++i)
    {
        float temp = values[i] - mean;
        sum += (temp*temp);
    }

    return sqrt((sum/(float)size));
}

template<typename T>
T findMin(T values[], unsigned int  size)
{
    T result = 0;
    if(size == 0) return result;
    result = values[0];

    for(unsigned int i = 1; i < size; ++i)
        if(values[1] < result) result = values[1];

    return result;
}

template<typename T>
T findMax(T values[], unsigned int  size)
{
    T result = 0;
    if(size == 0) return result;
    result = values[0];

    for(unsigned int i = 1; i < size; ++i)
        if(values[1] > result) result = values[1];

    return result;
}

void * coin_flipper(void * arg)
{
    unsigned int ctr = 0;
    unsigned int meanCtr = 0;
    float localDeviation = 0;
    bool initialized = false;

    unsigned int sleepTime = (unsigned int)((1.0 * 1000 * 1000) / NUM_FLIPS_PER_SECOND);
    printf("Flipping %u coins per second. Sleeping for %uus\n", NUM_FLIPS_PER_SECOND, sleepTime);
    while(flip_coin)
    {
        unsigned int coin_flip = rand() % PRECISION;

        COIN_VALUES[ctr] = coin_flip;

        ++ctr;
        if(initialized)
            usleep(sleepTime);

        if(ctr == MAX_COIN_FLIPS)
        {
            float mean = calculateMean(COIN_VALUES, MAX_COIN_FLIPS);
            MEAN_VALUES[meanCtr] = mean;
            ++meanCtr;

            if(initialized == false)
                printf("%d\n", INITIALIZATION_QUANTITY - meanCtr);

            if(meanCtr == INITIALIZATION_QUANTITY)
            {
                localDeviation = calculateStandardDeviation(MEAN_VALUES, INITIALIZATION_QUANTITY);
                meanCtr = 0;
                if(initialized == false) {
                    GLOBAL_MEAN = calculateMean(MEAN_VALUES, INITIALIZATION_QUANTITY);
                    STANDARD_DEVIATION = localDeviation;
                    float minMean = findMin(MEAN_VALUES, INITIALIZATION_QUANTITY);
                    float maxMean = findMax(MEAN_VALUES, INITIALIZATION_QUANTITY);
                    printf("Initialized global mean - dev(min,max): %5.5f - %5.5f(%f,%f)\n", GLOBAL_MEAN, STANDARD_DEVIATION, minMean, maxMean);
                    initialized = true;
                }
            }

            if(initialized && abs(mean - GLOBAL_MEAN) > 2 * STANDARD_DEVIATION)
            {
                //printf("Current Mean(Dev): %5.5f %5.5f\r", mean, localDeviation);
                //fflush(stdout);
                printf("Outlier Mean(dev): %5.5f %5.5f\n", mean, localDeviation);
            }
            ctr = 0;
        }
    }
}

int main(int argc, char * argv[])
{
    srand(time(NULL));

    pthread_t coinThread;
    pthread_create(&coinThread, 0, &coin_flipper, 0);

    pthread_join(coinThread, 0);

    return 1;
}
