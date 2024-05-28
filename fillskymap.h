#ifndef FILLSKYMAP_H
#define FILLSKYMAP_H

#include "calculation.h"
#include "fieldslines.h"

#include <QGraphicsItem>
#include <QPainter>

class fillSkymap : public QObject, public QGraphicsItem
{
    Q_OBJECT

private:
    calculation *equatorialCoordinates;
    fieldsLines *currentFieldsLines;

    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;

    int initialWidth;
    int initialHeight;

    bool isFOVCreated;
    bool isPointsDrawn;
    bool isFITsFileProcessed;

    bool isReadyToChange;

    //1 - showFOV
    //2 - drawPoints
    //3 - showFitsFile
    int action;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    fillSkymap(int whatToDo, int initWidth, int initHeight, fieldsLines *currFieldsLines,  QObject *parent = 0);

    void showFieldOfView(QPainter *painter);
    void showLocalizationArea(QPainter *painter);
    void showMaxAndMinAreaPoints(QPainter *painter);
    void showPointsFromTextFile(QPainter *painter);

    bool checkIfFOVCreated();
    bool checkIfPointsDrawn();
    bool checkIfFITsFileProcessed();

    bool checkDate(int year, int month, int day);
};

#endif // FILLSKYMAP_H
