#include "reaction.h"

#include <math.h>
#include <QListWidget>

#include <QDebug>

reaction::reaction(QWidget *parent, QGraphicsView *view, QGraphicsScene *currScene, fieldsLines *currFL,
                   QGraphicsEllipseItem *currAnchorPoint, QGraphicsEllipseItem *currMyPoint,
                   int initialSceneHeight, int initialSceneWidth, hints *currHints, QTimer *timerForHints)
                    : currentWindow(parent), currentView(view), currentScene(currScene), currentFieldsLines(currFL),
                      currentAnchorPoint(currAnchorPoint), currentMyPoint(currMyPoint),
                    initialSceneHeightCopy(initialSceneHeight), initialSceneWidthCopy(initialSceneWidth),
                    currentHints(currHints), timerOfHints(timerForHints)
{
    serifFont = new QFont("Arial", 14, QFont::Bold);
}

void reaction::mousePressEvent(QMouseEvent *mousePress)
{
    currentHints->showCtrlCMouse();
    timerOfHints->start();
    QRectF currentWindowSizeWithFrame = currentWindow->frameGeometry();
    QSize currentWindowSizeWithoutFrame = currentView->size();
    double HeaderSize = currentWindowSizeWithFrame.height() - currentWindowSizeWithoutFrame.height();
    double pointX = 99., pointY = 99.;
    double pointSize = 8.;
    //"Географические" координаты в проекции Мольвейде и вспомогательная переменная
    double geographicalLatitude;
    double geographicalLongitude;
    double auxiliaryTheta;
    //Координаты в экваториальной системе
    double equatorialRA;
    double equatorialDec;
    if (mousePress->button() == Qt::LeftButton)
    {
        //Получим координаты клика мыши по сцене и нарисуем в этом месте точку
        //Координаты внутри сцены внури всей доступной области (прямоугольника, внутри которого помещаются все дисплеи, если их больше 1)
        QPointF pToScene = currentView->mapToScene(QCursor::pos());
        QPointF sceneTopLeftCorner = currentAnchorPoint->mapFromScene(currentWindow->pos());
        pointX = pToScene.x() - pow(scaleFactor, -scaleStep) * sceneTopLeftCorner.x();
        pointY = pToScene.y() - pow(scaleFactor, -scaleStep) * sceneTopLeftCorner.y() - pow(scaleFactor, -scaleStep) * HeaderSize;//Нашли координаты
        //Пересчет координат клика мыши в географические координаты
        auxiliaryTheta = asin( 1. - ( pointY / ( initialSceneHeightCopy / 2. ) ) );
        geographicalLatitude = asin( ( 2. * auxiliaryTheta + sin( 2. * auxiliaryTheta ) ) / M_PI );
        geographicalLongitude = - M_PI * ( ( initialSceneWidthCopy / 2. ) - pointX ) / ( 2. * ( initialSceneHeightCopy / 2. ) * cos( auxiliaryTheta ) );
        //Пересчет географических координат в экваториальные
        equatorialRA = M_PI - geographicalLongitude;
        equatorialDec = geographicalLatitude;
        //Поля вывода координат
        QString Xcoord;
        QString Ycoord;
        QString Divide;
        QString Coordinates;
        currentScene->removeItem(currentMyPoint);
        currentFieldsLines->checkDateAndTime(currentFieldsLines);

        QStringList date = currentFieldsLines->getDateStringList();
        QStringList time = currentFieldsLines->getTimeStringList();
        if(equatorialRA * 180./M_PI > 360. || equatorialRA * 180./M_PI < 0 || equatorialDec * 180./M_PI > 90. || equatorialDec * 180./M_PI < -90.)
        {
            currentFieldsLines->outOfSkyMap();
            currentScene->removeItem(currentMyPoint);
        }
        else if(date.size() != 3 || time.size() != 3)
        {
            currentHints->showWrongFormat();
            currentScene->removeItem(currentMyPoint);
            return;
        }
        else
        {
            bool isDateOk, isTimeOk;
            for (int i = 0; i < 3; i++)
            {
                date[i].toInt(&isDateOk);
                time[i].toInt(&isTimeOk);
                if (isDateOk == false || isTimeOk == false)
                {
                    currentHints->showWrongFormat();
                    currentScene->removeItem(currentMyPoint);
                    return;
                }
            }
            if (checkDate(date[0].toInt(), date[1].toInt(), date[2].toInt()) == false)
            {
                currentHints->showWrongFormat();
                currentScene->removeItem(currentMyPoint);
                return;
            }
            pointSize = 8./pow((scaleFactor-0.1), scaleStep);
            drawMyPoint(pointX, pointY, pointSize);
            Xcoord = QString::number(equatorialRA * 180./M_PI);
            Ycoord = QString::number(equatorialDec * 180./M_PI);
            Divide = "\t";
            Coordinates = Xcoord + Divide + Ycoord;
            currentFieldsLines->fillEquatorialData(Coordinates);
            equatorialRA = equatorialRA * 180./M_PI;//Перевод в градусы
            equatorialDec = equatorialDec * 180./M_PI;//Перевод в градусы

            getCoordinates = new calculation(date[0].toInt(), date[1].toInt(), date[2].toInt(),
                                            time[0].toInt(), time[1].toInt(), time[2].toDouble(),
                                            equatorialRA, equatorialDec);
            currentFieldsLines->fillBUSTCoordinates(getCoordinates->getBUSTCoordinates());
            currentFieldsLines->fillJD_GMST(QString::number(getCoordinates->getJD(), 'f', 6),
                                            QString::number(getCoordinates->getGMSTDeg()));
            BUSTGeometricArea = new BUSTArea();//Временно пусть создается здесь
            currentFieldsLines->fillBUSTArea(QString::number(BUSTGeometricArea->getArea(getCoordinates->getZenithAngleBUST(),
                                                                              getCoordinates->getAzimuthalAngleBUST())));
        }
    }
    //Отмена выделения полей
    currentFieldsLines->removeSelection();
    //Подсказка
    //advices->showCtrlCMouse();
    //advices->startTimer();
}

void reaction::wheelEvent(QWheelEvent *wheel)
{
    currentHints->showCtrlCMouse();
    timerOfHints->start();
    // Scale the view / do the zoom
    if(wheel->delta() > 0)
    {
        // Zoom in
        currentView->scale(scaleFactor, scaleFactor);
        scaleControl = scaleControl * scaleFactor;
        scaleStep++;
    }
    else
    {
        // Zooming out
        if(scaleControl > 1.14999)
        {
            currentView->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
            scaleControl = scaleControl / scaleFactor;
            scaleStep--;
        }
    }
}

void reaction::resizeEvent(QResizeEvent *resize)
{
    currentHints->showCtrlCMouse();
    timerOfHints->start();
    //Изображение мышки
    //label->move(currentWindow->width()-100, currentWindow->height()-127);
    currentWindowSizeWithoutFrame = currentWindow->size();
    currentView->resize(currentWindowSizeWithoutFrame.width(), currentWindowSizeWithoutFrame.height());
    currentFieldsLines->moveFileLink(currentWindow->height());
}

void reaction::drawMyPoint(double pointX, double pointY, double pointSize)
{
    currentMyPoint = currentScene->addEllipse(pointX-(pointSize/2.), pointY-(pointSize/2.), pointSize*1.0, pointSize*1.0, QPen(Qt::red), QBrush(Qt::red));
    currentMyPoint->setZValue(2);
}

bool reaction::checkDate(int year, int month, int day)
{
    if (month == 2)
    {
        if ( ( year%4 == 0 && year%100 != 0 ) || year%400 == 0)
        {
            if (day < 1 || day > 29)
            {
                return false;
            }
        }
        else
        {
            if (day < 1 || day > 28)
            {
                return false;
            }
        }
    }
    else if (month == 1 || month == 3 || month == 5 || month == 7
             || month == 8 || month == 10 || month == 12)
    {
        if (day < 1 || day > 31)
        {
            return false;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day < 1 || day > 30)
        {
            return false;
        }
    }
    else if (month < 1 || month > 12)
    {
        return false;
    }
    return true;
}
