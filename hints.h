#ifndef HINTS_H
#define HINTS_H

#include <QGraphicsScene>
#include <QLabel>
#include <QListWidget>
#include <QTimer>

class hints
{
private:
    QGraphicsScene *currentScene;
    QWidget *currentWindow;
    //Выпрыгивающая подсказка - мышка
    QLabel *mouseLabel;
    //Выпрыгивающая подсказка "Ctrl+C"
    QListWidget *ctrlCListWidget;
    //Выпрыгивающее сообщение о неверном вводе
    QListWidget *wrongFormatListWidget;
    //Выпрыгивающая просьба подождать
    QListWidget *waitPleaseListWidget;
    //Выпрыгивающее сообщение об отсутствии файла
    QListWidget *fileNotFoundListWidget;

    //Таймер
    QTimer *timerForWait;

    QFont *serifFont;

public:
    hints(QWidget *parent, QGraphicsScene *curScene);

    void mouseHint();
    void ctrlCHint();
    void wrongFormat();

    void startTimer();
    void stopTimer();
    void showCtrlCMouse();
    void hideCtrlCMouse();
    void showPleaseWait();
    void hidePleaseWait();
    void showWrongFormat();
    void hideWrongFormat();
};

#endif // HINTS_H
