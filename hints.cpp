#include "hints.h"

#include <QPixmap>

hints::hints(QWidget *parent, QGraphicsScene *curScene) : currentWindow(parent), currentScene(curScene)
{
    serifFont = new QFont("Arial", 16, QFont::Bold);
    timerForWait = new QTimer();
    timerForWait->setInterval(1);
    //Выпрыгивающая просьба подождать
    waitPleaseListWidget = new QListWidget();
    waitPleaseListWidget->setParent(currentWindow);
    QListWidgetItem *waitPListWidgetItem = new QListWidgetItem("Подождите..", waitPleaseListWidget);
    waitPListWidgetItem->setFont(*serifFont);
    waitPleaseListWidget->resize(180, 30);
    waitPleaseListWidget->setStyleSheet("background-color: white");
    waitPleaseListWidget->setStyleSheet("border-style: outset");
    waitPleaseListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    waitPleaseListWidget->hide();

    //Выпрыгивающая подсказка - мышка
    QPixmap mousePixmap(":images/mousecontrollericon.png");
    mouseLabel = new QLabel();
    mouseLabel->setParent(currentWindow);
    mouseLabel->setPixmap(mousePixmap);
    mouseLabel->setScaledContents(true);
    mouseLabel->resize(120, 130);
    mouseLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    mouseLabel->move(currentScene->width()-100, currentScene->height()-127);
    mouseLabel->hide();

    //Выпрыгивающая подсказка "Ctrl+C"
    ctrlCListWidget = new QListWidget();
    ctrlCListWidget->setParent(currentWindow);
    new QListWidgetItem("Ctrl+C", ctrlCListWidget);
    ctrlCListWidget->resize(50, 20);
    ctrlCListWidget->setStyleSheet("background-color: white");
    ctrlCListWidget->setStyleSheet("border-style: outset");
    ctrlCListWidget->move(1250,81);
    ctrlCListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ctrlCListWidget->hide();

    //Cообщение о неверном вводе даты и времени
    wrongFormatListWidget = new QListWidget();
    wrongFormatListWidget->setParent(currentWindow);
    QListWidgetItem *wfListWidgetItem = new QListWidgetItem("Введите значения даты и времени!", wrongFormatListWidget);
    wfListWidgetItem->setFont(*serifFont);
    wrongFormatListWidget->resize(400, 40);
    wrongFormatListWidget->setStyleSheet("background-color: white");
    wrongFormatListWidget->setStyleSheet("border-style: outset");
    wrongFormatListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    wrongFormatListWidget->hide();

    //Cообщение об отсутствии файла
    fileNotFoundListWidget = new QListWidget();
    fileNotFoundListWidget->setParent(currentWindow);
    QListWidgetItem *fnfListWidgetItem = new QListWidgetItem("Нет файла!", fileNotFoundListWidget);
    fnfListWidgetItem->setFont(*serifFont);
    fileNotFoundListWidget->resize(400, 40);
    fileNotFoundListWidget->setStyleSheet("background-color: white");
    fileNotFoundListWidget->setStyleSheet("border-style: outset");
    fileNotFoundListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    fileNotFoundListWidget->hide();
}

void hints::showPleaseWait()
{
    waitPleaseListWidget->move(currentWindow->size().width()/2.-160.,currentWindow->size().height()/2.-20.);
    waitPleaseListWidget->show();
}

void hints::hidePleaseWait()
{
    waitPleaseListWidget->hide();
}

void hints::startTimer()
{
    timerForWait->start();
}

void hints::stopTimer()
{
    timerForWait->stop();
}

void hints::showCtrlCMouse()
{
    ctrlCListWidget->move(currentWindow->width()-60, 4);
    mouseLabel->move(currentWindow->width()-100, currentWindow->height()-127);
    ctrlCListWidget->show();
    mouseLabel->show();
}

void hints::hideCtrlCMouse()
{
    ctrlCListWidget->hide();
    mouseLabel->hide();
}

void hints::showWrongFormat()
{
    wrongFormatListWidget->move(currentWindow->size().width()/2.-160.,currentWindow->size().height()/2.-20.);
    wrongFormatListWidget->show();
}

void hints::hideWrongFormat()
{
    wrongFormatListWidget->hide();
}
