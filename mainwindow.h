#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "decoration.h"
#include "fieldslines.h"
#include "hints.h"
#include "fillskymap.h"
#include "reaction.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    //Оформляем окно с помощью конструктора windowDecoration - координаты небесной сферы и заголовок
    decoration *decorations;
    //Добавляем информационные поля и поля ввода с помощью конструктора fieldsLines
    fieldsLines *fieldsAndLines;
    //Подсказки
    hints *advices;
    //Заполнение небесной сферы данными
    fillSkymap *fillSMFOV;
    fillSkymap *fillMyPoints;

    bool isFOVButtonPressed;

    //Форма окна
    int indentation;
    int initialSceneWidth;
    int initialSceneHeight;

    //View и scene
    QGraphicsView *view;
    QGraphicsScene *scene;

    //Точка левого нижнего угла и отрисовывающаяся по клику мыши точка
    QGraphicsEllipseItem *anchorPoint;
    QGraphicsEllipseItem *myPoint;

    //Кнопки
    QPushButton *showFOV;
    QPushButton *showMyPoints;
    QPushButton *calculateFOV;
    QPushButton *drawMyPoints;
    QPushButton *calculateFITSLocalization;

    //Данные полей
    QStringList previousDate;
    QStringList previousTime;
    QString fitsFile;
    QString textFile;

    bool isDateTimeHasChanged;

    //Проверка того, что объекты созданы и показаны на экране
    bool FOVCreated;
    bool FOVShown;
    bool isFOVPngCreated;
    bool isMyPointsShown;

    //Объект, реагирующий на воздействие пользователя
    reaction *response;

    //Таймер для подсказок
    QTimer *timerForCtrlCMouse;
    //Таймер для просьбы подождать
    QTimer *timerForWaitPlease;

    //Создаем хранилище изображений
    QImage *img;
    QPainter *p;

    QPixmap filledFOVSkymap;
    QPixmap filledMyPointsSkymap;
    QGraphicsPixmapItem *filledFOVSkymapItem;
    QGraphicsPixmapItem *filledMyPointsSkymapItem;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void mousePressEvent(QMouseEvent *mousePress);
    virtual void wheelEvent(QWheelEvent *wheel);
    virtual void resizeEvent(QResizeEvent *resize);

    //Фильтр для отключения реакции на скроллинг мышью
    bool eventFilter(QObject *object, QEvent *event);

    QRectF windowSizeWithFrame();
    QSize windowSizeWithoutFrame();
    QPointF mapToScene();
    QPointF mapFromScene();
    int initialSceneHeightCopy();
    int initialSceneWidthCopy();
    int currentSceneHeightCopy();
    int currentSceneWidthCopy();
    void removeMyPoint();
    void drawMyPoint(double pointX, double pointY, double pointSize);

public slots:
    void calculateFieldOfViewSLOT();
    void showFieldOfViewSLOT();
    void drawMyPointsSLOT();
    void showMyPointsSLOT();
    void hideCtrlCMouseHintSLOT();
    void hideWaitHintSLOT();
    void waitProcessFitsFileSLOT();
    void hideWrongDateTimeSLOT();
};
#endif // MAINWINDOW_H
