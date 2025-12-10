#ifndef CONFUSION_MATRIX_H
#define CONFUSION_MATRIX_H

void printConfusionMatrix(void);
void makeHundredFakeClassifications(void);
void makeOneClassificationAndUpdateConfusionMatrix(int direction);
void classifyRealSample(int direction);
int calculateDistanceToAllCentrePointsAndSelectWinner(int x, int y, int z);
void resetConfusionMatrix(void);

#endif
