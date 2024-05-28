#ifndef FIELDSLINES_H
#define FIELDSLINES_H

#include <QListWidgetItem>
#include <QLineEdit>

class fieldsLines
{
private:
    QLineEdit *dateField;
    QLineEdit *timeField;
    QLineEdit *fileLinkField;
    QLineEdit *raDecColumnsField;

    QListWidgetItem *equatorialDataListWidgetItem;
    QListWidgetItem *BUSTCoordDataListWidgetItem;
    QListWidgetItem *JDDataListWidgetItem;
    QListWidgetItem *GMSTDataListWidgetItem;
    QListWidgetItem *BUSTGeometricAreaDataListWidgetItem;

    QListWidget *dateFieldLine;
    QListWidget *timeFieldLine;

    QListWidget *equatorialInfoListWidget;
    QListWidget *equatorialDataListWidget;
    QListWidget *BUSTCoordInfoListWidget;
    QListWidget *BUSTCoordDataListWidget;
    QListWidget *JDInfoListWidget;
    QListWidget *JDDataListWidget;
    QListWidget *GMSTInfoListWidget;
    QListWidget *GMSTDataListWidget;
    QListWidget *BUSTGeometricAreaInfoListWidget;
    QListWidget *BUSTGeometricAreaDataListWidget;
    QListWidget *fileInfoListWidget;
    QListWidget *raDecColumnsInfoListWidget;

    QStringList dateStringList;
    QStringList timeStringList;

    QString extractDate();
    QString extractTime();

public:
    fieldsLines();
    ~fieldsLines();

    void setParent(QWidget *parent);

    void removeSelection();
    void outOfSkyMap();
    void fillEquatorialData(QString Coordinates);
    void fillBUSTCoordinates(QString BUSTCoordinates);
    void fillJD_GMST(QString JD, QString GMST);
    void fillBUSTArea(QString BUSTArea);
    void moveFileLink(int windowHeight);
    void checkDateAndTime(fieldsLines *FL);

    QStringList getDateStringList();
    QStringList getTimeStringList();
    QString getFileLink();
};

#endif // FIELDSLINES_H
