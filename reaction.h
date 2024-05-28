#ifndef REACTION_H
#define REACTION_H

#include "fieldslines.h"
#include "hints.h"
#include "calculation.h"
#include "BUSTArea.h"

#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>

class reaction
{
private:
    QWidget *currentWindow;
    QGraphicsView *currentView;
    QGraphicsEllipseItem *currentAnchorPoint;
    QGraphicsEllipseItem *currentMyPoint;
    QFont *serifFont;
    QGraphicsScene *currentScene;

    fieldsLines *currentFieldsLines;
    hints *currentHints;
    calculation *getCoordinates;
    BUSTArea *BUSTGeometricArea;

    QTimer *timerOfHints;

    //Размер окна без шапки (рамки)
    QSize currentWindowSizeWithoutFrame;

    int initialSceneWidthCopy;
    int initialSceneHeightCopy;

    //Параметры зума
    double scaleControl = 1.;
    double scaleFactor = 1.15;
    int scaleStep = 0;

public:
    reaction(QWidget *parent, QGraphicsView *view, QGraphicsScene *currScene, fieldsLines *currFL,
             QGraphicsEllipseItem *currAnchorPoint, QGraphicsEllipseItem *currMyPoint,
             int initialSceneHeight, int initialSceneWidth, hints *currHints, QTimer *timerForHints);

    void drawMyPoint(double pointX, double pointY, double pointSize);

    bool checkDate(int year, int month, int day);

    //События кнопок мыши
    virtual void mousePressEvent(QMouseEvent *mousePress);
    virtual void wheelEvent(QWheelEvent *wheel);
    virtual void resizeEvent(QResizeEvent *resize);
};

#endif // REACTION_H
