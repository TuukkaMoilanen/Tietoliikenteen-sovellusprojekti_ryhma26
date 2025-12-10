#include <zephyr/kernel.h>
#include <math.h>
#include "confusion.h"
#include "adc.h"
#include "keskipisteet.h"

int CM[6][6]= {0};

static int measurements[6][3] = {
    {1540, 1534, 1834}, // z+
    {1533, 1539, 1235}, // z-
    {1535, 1831, 1536}, // y+
    {1534, 1234, 1535}, // y-
    {1835, 1534, 1536}, // x+
    {1230, 1537, 1535}  // x-
};

void printConfusionMatrix(void)
{
    printk("Confusion matrix:\n");
    printk("     cp1 cp2 cp3 cp4 cp5 cp6\n");
    for (int i = 0; i < 6; i++) {
        printk("cp%d  %d   %d   %d   %d   %d   %d\n",
               i + 1,
               CM[i][0], CM[i][1], CM[i][2],
               CM[i][3], CM[i][4], CM[i][5]);
    }
}

void resetConfusionMatrix(void)
{
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            CM[i][j] = 0;
        }
    }
}

void makeHundredFakeClassifications(void)
{
    printk("Running 100 fake classifications...\n");
    for(int i = 0; i < 100; i++) {
        int trueClass = i % 6;
        makeOneClassificationAndUpdateConfusionMatrix(trueClass);
    }
    printk("Done\n");
}

void classifyRealSamples(int direction)
{
    if (direction < 0 || direction > 5) {
        printk("Invalid direction for real classification!\n");
        return;
    }
    for (int i = 0; i < 100; i++) {
        struct Measurement m = readADCValue();

        int predicted = calculateDistanceToAllCentrePointsAndSelectWinner(
            m.x, m.y, m.z
        );
        CM[direction][predicted]++;
        printk("Sample %d: true=%d predicted=%d\n", i + 1, direction, predicted);
        k_msleep(100);
    }
}

void makeOneClassificationAndUpdateConfusionMatrix(int direction)
{
    int x = measurements[direction][0];
    int y = measurements[direction][1];
    int z = measurements[direction][2];

    int predicted = calculateDistanceToAllCentrePointsAndSelectWinner(x, y, z);

    CM[direction][predicted]++;

    printk("True: cp%d  Predicted: cp%d\n", direction + 1, predicted + 1);
}

int calculateDistanceToAllCentrePointsAndSelectWinner(int x, int y, int z)
{
    float bestDist = 1e6f;
    int bestIndex = 0;

    for (int i = 0; i < 6; i++) {
        float dx = x - CP[i][0];
        float dy = y - CP[i][1];
        float dz = z - CP[i][2];

        float dist = sqrtf(dx * dx + dy * dy + dz * dz);

        if (dist < bestDist) {
            bestDist = dist;
            bestIndex = i;
        }
    }

    return bestIndex;
}
