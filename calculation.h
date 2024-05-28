#ifndef CALCULATION_H
#define CALCULATION_H

#include <QStringList>

/* Объекты этого класса принимают в качестве аргументов дату, время
 * и экваториальные координаты в эпохе J2000. В теле конструктора класса
 * координаты переводятся в эпоху аргументов (даты и времени). Также
 * производится расчет зенитного, азимутального углов для БПСТ */

class calculation
{
private:
    int grigorianDay;//День по Григорианскому календарю
    int grigorianMonth;//Месяц по Григорианскому календарю
    int grigorianYear;//Год по Григорианскому календарю
    int hourUT1;//Часы в UT1
    int minuteUT1;//Минуты в UT1
    double secondUT1;//Секунды в UT1
    double rightAscensionJ2000;//Значение прямого восхождения в эпохе J2000
    double declinationJ2000;//Значение склонения в эпохе J2000

    double latitude;//Широта места наблюдения
    double longitude;//Долгота места наблюдения
    double GMSTDeg;//GMST в градусах

    double azimuthalAngle180;//Азимутальный угол от -180 до 180
    double azimuthalAngle360;//Азимутальный угол от 0 до 360
    double azimuthalAngleBUST;//Азимутальный угол в системе БПСТ
    double zenithAngle;//Зенитный угол
    double altitude;//Высота

    double ascensionCurrent;//Прямое восхождение с поправкой на прецессию и нутацию
    double declinationCurrent;//Склонение с поправкой на прецессию и нутацию

    double dayTimeSeconds;
    int JD;//Юлианская дата

public:
    calculation(int gy, int gm, int gd, int h, int m, double s, double ra, double dec);
    ~calculation();

    double getGMSTDeg();
    double getJD();
    double getAzimuthalAngleBUST();
    double getZenithAngleBUST();
    double latitudeCopy();
    double longitudeCopy();
    double ascensionCurrentCopy();
    double declinationCurrentCopy();
    QString getBUSTCoordinates();
};

#endif // CALCULATION_H
