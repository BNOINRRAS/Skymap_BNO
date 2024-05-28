#ifndef BUSTAREA_H
#define BUSTAREA_H

class BUSTArea
{
private:
    double BPSTArea[8][180][360];

public:
    BUSTArea();
    double getArea(double azimuthalAngleBUST, double zenithAngleBUST);
};

#endif // BUSTAREA_H
