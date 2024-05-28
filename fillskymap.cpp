#include "fillskymap.h"
#include <math.h>
#include <fstream>

#include <QDebug>

fillSkymap::fillSkymap(int whatToDo, int initWidth, int initHeight, fieldsLines *currFieldsLines, QObject *parent)
    : initialWidth(initWidth), initialHeight(initHeight), currentFieldsLines(currFieldsLines), QObject(parent), QGraphicsItem()
{
    action = whatToDo;
    isReadyToChange = true;
}

void fillSkymap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    switch (action)
    {
    case 1: showFieldOfView(painter); break;
    case 2: showPointsFromTextFile(painter); break;
    case 3: showLocalizationArea(painter); break;
    default: break;
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void fillSkymap::showFieldOfView(QPainter *painter)
{
    currentFieldsLines->checkDateAndTime(currentFieldsLines);
    QStringList date = currentFieldsLines->getDateStringList();
    QStringList time = currentFieldsLines->getTimeStringList();
    if(date.size() != 3 || time.size() != 3)
    {
        isFOVCreated = false;
        return;
    }
    bool isDateOk, isTimeOk;
    for (int i = 0; i < 3; i++)
    {
        date[i].toInt(&isDateOk);
        time[i].toInt(&isTimeOk);
        if (isDateOk == false || isTimeOk == false)
        {
            isFOVCreated = false;
            return;
        }
    }

    year = date[0].toInt();
    month = date[1].toInt();
    day = date[2].toInt();
    hour = time[0].toInt();
    minute = time[1].toInt();
    second = time[2].toDouble();

    if (checkDate(year, month, day) == false)
    {
        isFOVCreated = false;
        return;
    }
    isFOVCreated = true;
    double auxiliaryTheta, rightAscension, declination, zenithAngle, hourAngle, pointX, pointY;
    double declinationMin = 90., declinationMax = -90., zenithAngleMin, zenithAngleMax;
    double angleThreshold = 100.;

    for(int i=0;i<360;i++)
    {
        declinationMin = 90.;
        declinationMax = -90.;
        for(int j=-89;j<90;j++)
        {
            rightAscension = double(i);// / (180. / M_PI);
            declination = double(j);// / (180. / M_PI);

            equatorialCoordinates = new calculation(year, month, day,
                                                    hour, minute, second, rightAscension, declination);
            rightAscension = equatorialCoordinates->ascensionCurrentCopy();
            declination = equatorialCoordinates->declinationCurrentCopy();

            if(rightAscension > 2. * M_PI)
            {
                rightAscension = rightAscension - ( 2. * M_PI );
            }
            if(rightAscension < 0)
            {
                rightAscension = ( 2. * M_PI ) + rightAscension;
            }
            if(declination > M_PI / 2.)
            {
                declination = M_PI - declination;
            }
            if(declination < -( M_PI / 2. ))
            {
                declination = -M_PI - declination;
            }

            hourAngle = ( equatorialCoordinates->getGMSTDeg() / (180. / M_PI) )
                    + equatorialCoordinates->longitudeCopy() - rightAscension;
            zenithAngle = acos( sin( declination ) * sin( equatorialCoordinates->latitudeCopy() )
                                + cos( declination ) * cos( equatorialCoordinates->latitudeCopy() ) * cos( hourAngle ) );
            zenithAngle = zenithAngle * (180. / M_PI);//Перевели радианы в градусы
            if(zenithAngle < angleThreshold)
            {
                auxiliaryTheta = double(j) / (180. / M_PI);
                for(int m = 0; m < 200; m++)//200 итераций методом Ньютона - компромисс между точностью и быстротой отрисовки точки
                {
                    auxiliaryTheta = auxiliaryTheta - ( 2. * auxiliaryTheta + sin( 2. * auxiliaryTheta )
                                                    - M_PI * sin( double(j) / (180. / M_PI) ) ) / ( 2. + 2. * cos( 2. * auxiliaryTheta ) );
                }
                pointX = ( M_PI - double(i) / (180. / M_PI) ) * ( initialHeight / M_PI ) * cos( auxiliaryTheta )
                        + ( initialWidth / 2. );
                pointY = ( initialHeight / 2. ) * ( 1. - sin( auxiliaryTheta ) );
                QPen myPen(QColor(0,0,255));
                QBrush myBrush(QColor(0,0,255));
                myBrush.setColor(QColor(128,128,128));
                myPen.setColor(QColor(128,128,128));
                painter->setBrush(myBrush);
                painter->setPen(myPen);
                painter->setOpacity(0.1);
                //Поправляем координаты на "-Х", потому что QT рисует точку от левого верхнего угла, а не от центра
                painter->drawEllipse(QRectF(pointX-3, pointY-3, 6, 6));
            }
            if(zenithAngle >= angleThreshold && double(j) < declinationMin)
            {
                declinationMin = double(j);
                zenithAngleMin = zenithAngle;
            }
            if(zenithAngle >= angleThreshold && double(j) > declinationMax)
            {
                declinationMax = double(j);
                zenithAngleMax = zenithAngle;
            }
        }
    }
}

void fillSkymap::showLocalizationArea(QPainter *painter)
{

}

void fillSkymap::showMaxAndMinAreaPoints(QPainter *painter)
{

}

void fillSkymap::showPointsFromTextFile(QPainter *painter)
{
    std::ifstream myPointsFile;
    std::string fileName = ( currentFieldsLines->getFileLink() ).toStdString();
    myPointsFile.open(fileName);
    if(!myPointsFile)
    {
        isPointsDrawn = false;
        return;
    }
    double buffer;
    double auxiliaryTheta, rightAscension, declination, pointX, pointY;
    int counter = 0;
    //while(myPointsFile>>buffer>>buffer
    //        >>buffer>>buffer>>buffer>>buffer>>buffer>>buffer>>buffer>>buffer
    //        >>declination>>rightAscension>>buffer>>buffer>>buffer>>buffer)
    while(myPointsFile>>buffer>>buffer>>buffer>>buffer>>buffer>>buffer>>buffer
            >>buffer>>buffer>>buffer>>buffer>>buffer>>buffer>>buffer>>buffer
            >>declination>>rightAscension>>buffer>>buffer>>buffer>>buffer>>buffer)
    {
        counter++;
        auxiliaryTheta = declination / (180. / M_PI);
        for(int m = 0; m < 2000; m++)//2000 итераций методом Ньютона - компромисс между точностью и быстротой отрисовки точки
        {
            auxiliaryTheta = auxiliaryTheta - ( 2. * auxiliaryTheta + sin( 2. * auxiliaryTheta )
                                                - M_PI * sin( declination / (180. / M_PI) ) ) / ( 2. + 2. * cos( 2. * auxiliaryTheta ) );
        }
        pointX = ( M_PI - rightAscension / (180. / M_PI) ) * ( initialHeight / M_PI ) * cos( auxiliaryTheta ) + ( initialWidth / 2. );
        pointY = ( initialHeight / 2. ) * ( 1. - sin( auxiliaryTheta ) );
        QPen myPen(QColor(0,0,255));
        QBrush myBrush(QColor(0,0,255));
        myPen.setColor(QColor(255,0,0));//Красная точка
        myBrush.setColor(QColor(255,0,0));//Красная точка
        painter->setOpacity(1.);
        painter->setBrush(myBrush);
        painter->setPen(myPen);
        painter->drawEllipse(QRectF(pointX-2., pointY-2., 4., 4.));//Поправили координаты на -4, потому что QT рисует точку от левого верхнего угла, а не от центра
    }
    qDebug() << counter;
    myPointsFile.close();

    /*myPointsFile.open("myTable2.dat");
    while(myPointsFile>>declination>>rightAscension)
    {
        auxiliaryTheta = declination / (180. / M_PI);
        for(int m = 0; m < 2000; m++)//2000 итераций методом Ньютона - компромисс между точностью и быстротой отрисовки точки
        {
            auxiliaryTheta = auxiliaryTheta - ( 2. * auxiliaryTheta + sin( 2. * auxiliaryTheta )
                                                - M_PI * sin( declination / (180. / M_PI) ) ) / ( 2. + 2. * cos( 2. * auxiliaryTheta ) );
        }
        pointX = ( M_PI - rightAscension / (180. / M_PI) ) * ( initialHeight / M_PI ) * cos( auxiliaryTheta ) + ( initialWidth / 2. );
        pointY = ( initialHeight / 2. ) * ( 1. - sin( auxiliaryTheta ) );
        QPen myPen(QColor(0,0,255));
        QBrush myBrush(QColor(0,0,255));
        myPen.setColor(QColor(255,0,0));//Красная точка
        myBrush.setColor(QColor(255,0,0));//Красная точка
        painter->setOpacity(1.);
        painter->setBrush(myBrush);
        painter->setPen(myPen);
        painter->drawEllipse(QRectF(pointX-2., pointY-2., 4., 4.));//Поправили координаты на "-Х", потому что QT рисует точку от левого верхнего угла, а не от центра
    }
    myPointsFile.close();*/
    isPointsDrawn = true;
}

bool fillSkymap::checkIfFOVCreated()
{
    return isFOVCreated;
}

bool fillSkymap::checkIfPointsDrawn()
{
    return isPointsDrawn;
}

bool fillSkymap::checkIfFITsFileProcessed()
{
    //qDebug() << "IS FOV CREATED:\t" << isFOVCreated;
    return isFITsFileProcessed;
}

bool fillSkymap::checkDate(int year, int month, int day)
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

QRectF fillSkymap::boundingRect() const
{
    return QRectF(0,0, initialWidth, initialHeight);
}
