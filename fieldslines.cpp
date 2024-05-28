#include "fieldslines.h"

#include <QListWidget>

fieldsLines::fieldsLines()
{
    QFont serifFont("Arial", 14, QFont::Bold);
    //Поля ввода даты и времени
    dateField = new QLineEdit ("");
    dateField->resize(80, 20);
    dateField->move(0, 67);
    dateField->setStyleSheet("QLineEdit { background: cyan;}");
    dateField->show();
    timeField = new QLineEdit ("");
    timeField->resize(80, 20);
    timeField->move(170, 35);
    timeField->setStyleSheet("QLineEdit { background: cyan;}");
    timeField->show();
    dateFieldLine = new QListWidget();
    new QListWidgetItem("Введите дату в формате\nГГГГ-ММ-ДД\nГГГГ/ММ/ДД\nГГГГ:ММ:ДД", dateFieldLine);
    dateFieldLine->resize(163, 67);
    dateFieldLine->setStyleSheet("background-color: gainsboro");
    dateFieldLine->move(0, 0);
    dateFieldLine->setSelectionMode(QAbstractItemView::NoSelection);
    timeFieldLine = new QListWidget();
    new QListWidgetItem("Введите время в формате\n          ЧЧ:ММ:СС (UTC)", timeFieldLine);
    timeFieldLine->resize(173, 35);
    timeFieldLine->setStyleSheet("background-color: gainsboro");
    timeFieldLine->move(170, 0);
    timeFieldLine->setSelectionMode(QAbstractItemView::NoSelection);
    //Поле ввода ссылки на fits-файл или файл для отрисовки точек
    fileLinkField = new QLineEdit ("");
    fileLinkField->resize(300, 20);
    fileLinkField->move(0, 670);
    fileLinkField->setStyleSheet("QLineEdit { background: cyan;}");
    fileLinkField->show();
    //Поле для номеров столбцов ra и dec в файле с набором координат
    raDecColumnsField = new QLineEdit ("");
    raDecColumnsField->resize(40, 20);
    raDecColumnsField->move(310, 670);
    raDecColumnsField->setStyleSheet("QLineEdit { background: cyan;}");
    raDecColumnsField->show();

    //Информационная строка для экваториальных координат
    equatorialInfoListWidget = new QListWidget();
    new QListWidgetItem("Экваториальные координаты:\nПр.восх\tСклонение", equatorialInfoListWidget);
    equatorialInfoListWidget->resize(200, 35);
    equatorialInfoListWidget->setStyleSheet("background-color: gainsboro");
    equatorialInfoListWidget->move(350,0);
    equatorialInfoListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    //Выделение цветом поля вывода экваториальных координат
    equatorialDataListWidget = new QListWidget();
    equatorialDataListWidgetItem = new QListWidgetItem("", equatorialDataListWidget);
    equatorialDataListWidget->resize(200, 20);
    equatorialDataListWidget->setStyleSheet("background-color: cyan");
    equatorialDataListWidget->move(350,35);
    //Информационная строка для координат БПСТ
    BUSTCoordInfoListWidget = new QListWidget();
    new QListWidgetItem("Координаты ПСТ:\nАстр.азимут\tАзимут ПСТ\tЗен. Угол", BUSTCoordInfoListWidget);
    BUSTCoordInfoListWidget->resize(240, 35);
    BUSTCoordInfoListWidget->setStyleSheet("background-color: gainsboro");
    BUSTCoordInfoListWidget->move(557,0);
    BUSTCoordInfoListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    //Выделение цветом поля вывода координат БПСТ
    BUSTCoordDataListWidget = new QListWidget();
    BUSTCoordDataListWidgetItem = new QListWidgetItem("", BUSTCoordDataListWidget);
    BUSTCoordDataListWidget->resize(240, 20);
    BUSTCoordDataListWidget->setStyleSheet("background-color: cyan");
    BUSTCoordDataListWidget->move(557,35);
    //Информационная строка для Юлианской даты
    JDInfoListWidget = new QListWidget();
    new QListWidgetItem("Юлианская дата:", JDInfoListWidget);
    JDInfoListWidget->resize(130, 20);
    JDInfoListWidget->setStyleSheet("background-color: gainsboro");
    JDInfoListWidget->move(804,0);
    JDInfoListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    //Выделение цветом поля вывода Юлианской даты
    JDDataListWidget = new QListWidget();
    JDDataListWidgetItem = new QListWidgetItem("", JDDataListWidget);
    JDDataListWidget->resize(130, 20);
    JDDataListWidget->setStyleSheet("background-color: cyan");
    JDDataListWidget->move(804,20);
    //Информационная строка для GMST
    GMSTInfoListWidget = new QListWidget();
    new QListWidgetItem("GMST, °:", GMSTInfoListWidget);
    GMSTInfoListWidget->resize(70, 20);
    GMSTInfoListWidget->setStyleSheet("background-color: gainsboro");
    GMSTInfoListWidget->move(941,0);
    GMSTInfoListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    //Выделение цветом поля вывода GMST
    GMSTDataListWidget = new QListWidget();
    GMSTDataListWidgetItem = new QListWidgetItem("", GMSTDataListWidget);
    GMSTDataListWidget->resize(70, 20);
    GMSTDataListWidget->setStyleSheet("background-color: cyan");
    GMSTDataListWidget->move(941,20);

    //Информационная строка для fits-файла или файла для отрисовки точек
    fileInfoListWidget = new QListWidget();
    new QListWidgetItem("Ссылка на FITS файл или файл с набором координат", fileInfoListWidget);
    fileInfoListWidget->resize(335, 20);
    fileInfoListWidget->setStyleSheet("background-color: gainsboro");
    fileInfoListWidget->move(941,0);
    fileInfoListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    //Информационная строка для номеров столбцов ra и dec в файле с набором координат
    raDecColumnsInfoListWidget = new QListWidget();
    new QListWidgetItem("Номера столбцов RA и Dec:", raDecColumnsInfoListWidget);
    raDecColumnsInfoListWidget->resize(180, 20);
    raDecColumnsInfoListWidget->setStyleSheet("background-color: gainsboro");
    raDecColumnsInfoListWidget->move(941,0);
    raDecColumnsInfoListWidget->setSelectionMode(QAbstractItemView::NoSelection);

    //Информационная строка для площади БПСТ (3 плоскости)
    BUSTGeometricAreaInfoListWidget = new QListWidget();
    new QListWidgetItem("Площадь БПСТ (3 пл.):", BUSTGeometricAreaInfoListWidget);
    BUSTGeometricAreaInfoListWidget->resize(150, 20);
    BUSTGeometricAreaInfoListWidget->setStyleSheet("background-color: gainsboro");
    BUSTGeometricAreaInfoListWidget->move(1017,0);
    BUSTGeometricAreaInfoListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    //Выделение цветом поля вывода площади БПСТ (3 плоскости)
    BUSTGeometricAreaDataListWidget = new QListWidget();
    BUSTGeometricAreaDataListWidgetItem = new QListWidgetItem("", BUSTGeometricAreaDataListWidget);
    BUSTGeometricAreaDataListWidget->resize(70, 20);
    BUSTGeometricAreaDataListWidget->setStyleSheet("background-color: cyan");
    BUSTGeometricAreaDataListWidget->move(1017,20);
}

void fieldsLines::setParent(QWidget *parent)
{
    dateField->setParent(parent);
    timeField->setParent(parent);
    fileLinkField->setParent(parent);
    raDecColumnsField->setParent(parent);

    dateFieldLine->setParent(parent);
    timeFieldLine->setParent(parent);

    equatorialInfoListWidget->setParent(parent);
    equatorialDataListWidget->setParent(parent);
    BUSTCoordInfoListWidget->setParent(parent);
    BUSTCoordDataListWidget->setParent(parent);
    JDInfoListWidget->setParent(parent);
    JDDataListWidget->setParent(parent);
    GMSTInfoListWidget->setParent(parent);
    GMSTDataListWidget->setParent(parent);
    BUSTGeometricAreaInfoListWidget->setParent(parent);
    BUSTGeometricAreaDataListWidget->setParent(parent);
    fileInfoListWidget->setParent(parent);
    raDecColumnsInfoListWidget->setParent(parent);
}

void fieldsLines::removeSelection()
{
    equatorialDataListWidgetItem->setSelected(false);
    BUSTCoordDataListWidgetItem->setSelected(false);
    JDDataListWidgetItem->setSelected(false);
    GMSTDataListWidgetItem->setSelected(false);
}

void fieldsLines::outOfSkyMap()
{
    equatorialDataListWidgetItem->setText("Вне области неб.сферы!");
    BUSTCoordDataListWidgetItem->setText("");
}

void fieldsLines::fillEquatorialData(QString Coordinates)
{
    equatorialDataListWidgetItem->setText(Coordinates);
}

void fieldsLines::fillBUSTCoordinates(QString BUSTCoordinates)
{
    BUSTCoordDataListWidgetItem->setText(BUSTCoordinates);
}

void fieldsLines::fillJD_GMST(QString JD, QString GMST)
{
    JDDataListWidgetItem->setText(JD);
    GMSTDataListWidgetItem->setText(GMST);
}

void fieldsLines::fillBUSTArea(QString BUSTArea)
{
    BUSTGeometricAreaDataListWidgetItem->setText(BUSTArea);
}

QString fieldsLines::extractDate()
{
    return dateField->text();
}

QString fieldsLines::extractTime()
{
    return timeField->text();
}

void fieldsLines::checkDateAndTime(fieldsLines *FL)
{
    QString dateString = FL->extractDate();
    QString timeString = FL->extractTime();
    char separators[5] = " -/:";
    for (int i=0; i<4; i++)
    {
        timeStringList = timeString.split(separators[i]);
        if(timeStringList.size() != 3) { timeStringList.clear(); } else { break; }
    }
    for (int i=0; i<4; i++)
    {
        dateStringList = dateString.split(separators[i]);
        if(dateStringList.size() != 3) { dateStringList.clear(); } else { break; }
    }
    //if(timeStringList.size() != 3 || dateStringList.size() != 3) {}
    //{ advices->wrongFormat(); }
}

QStringList fieldsLines::getDateStringList()
{
    return dateStringList;
}

QStringList fieldsLines::getTimeStringList()
{
    return timeStringList;
}

void fieldsLines::moveFileLink(int windowHeight)
{
    fileLinkField->move(4, windowHeight-20);
    raDecColumnsField->move(480, windowHeight-20);
    fileInfoListWidget->move(4, windowHeight-40);
    raDecColumnsInfoListWidget->move(300, windowHeight-20);
}

QString fieldsLines::getFileLink()
{
    return fileLinkField->text();
}

fieldsLines::~fieldsLines()
{

}
