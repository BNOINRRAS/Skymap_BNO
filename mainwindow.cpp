#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScroller>
#include <QGraphicsItem>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      indentation(0), initialSceneWidth(1200 + 2 * indentation), initialSceneHeight(600 + 2 * indentation)
{
    ui->setupUi(this);

    view = new QGraphicsView(this);
    scene = new QGraphicsScene(0,0,initialSceneWidth,initialSceneHeight, this);
    view->setScene(scene);
    view->setGeometry(x(),y(),initialSceneWidth,initialSceneHeight);
    view->show();

    //Оформляем окно с помощью конструктора windowDecoration - координаты небесной сферы и заголовок
    decorations = new decoration(scene);
    //Добавляем информационные поля и поля ввода с помощью конструктора fieldsLines
    fieldsAndLines = new fieldsLines();
    fieldsAndLines->setParent(this);
    //Подсказки
    advices = new hints(this, scene);

    //Управление мышью - скроллинг правой кнопкой мыши, с помощью перетаскивания
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScroller::grabGesture(view, QScroller::RightMouseButtonGesture);

    //Изображения пустой проекции Мольвейде
    //Прозрачный фон
    QPixmap emptyDecorationTransparent = QPixmap(":images/ProjectionOnly.png");
    emptyDecorationTransparent = emptyDecorationTransparent.scaled(initialSceneWidth-2.*indentation, initialSceneHeight-2.*indentation, Qt::KeepAspectRatio);
    QGraphicsPixmapItem *EDTItem = scene->addPixmap(emptyDecorationTransparent);
    EDTItem->setPos( ( initialSceneWidth - EDTItem->boundingRect().width() ) /2.,
                     ( initialSceneHeight - EDTItem->boundingRect().height() ) /2.);
    //EDTItem->hide();
    //Бежевый фон
    QPixmap emptyDecorationBeige = QPixmap(":images/ProjectionOnly2.png");
    emptyDecorationBeige = emptyDecorationBeige.scaled(initialSceneWidth-2.*indentation, initialSceneHeight-2.*indentation, Qt::KeepAspectRatio);
    QGraphicsPixmapItem *EDBItem = scene->addPixmap(emptyDecorationBeige);
    EDBItem->setPos( ( initialSceneWidth - EDBItem->boundingRect().width() ) /2., ( initialSceneHeight - EDBItem->boundingRect().height() ) /2.);
    EDBItem->show();
    //Белый фон
    QPixmap emptyDecorationWhite = QPixmap(":images/ProjectionOnly3.png");
    emptyDecorationWhite = emptyDecorationWhite.scaled(initialSceneWidth-2.*indentation, initialSceneHeight-2.*indentation, Qt::KeepAspectRatio);
    QGraphicsPixmapItem *EDWItem = scene->addPixmap(emptyDecorationWhite);
    EDWItem->setPos( ( initialSceneWidth - EDWItem->boundingRect().width() ) /2., ( initialSceneHeight - EDWItem->boundingRect().height() ) /2.);
    EDWItem->hide();

    //Прозрачный фон
    view->setStyleSheet("QGraphicsView{background-color:transparent;}");

    //Добавление двух точек: точка для привязки координат и точка, которая будет отрисовываться по клику мыши
    anchorPoint = scene->addEllipse(0-1, 0-1, 1*1.0, 1*1.0, QPen(Qt::white), QBrush(Qt::white));//Позиция [0, 0]
    anchorPoint->hide();
    myPoint = scene->addEllipse(1-8, 1-8, 8*1.0, 8*1.0, QPen(Qt::red), QBrush(Qt::red));
    myPoint->setZValue(2);
    myPoint->hide();

    //Таймер для подсказок
    timerForCtrlCMouse = new QTimer();
    timerForCtrlCMouse->setInterval(2500);
    connect(timerForCtrlCMouse, SIGNAL(timeout()), this, SLOT(hideCtrlCMouseHintSLOT()));
    connect(timerForCtrlCMouse, SIGNAL(timeout()), this, SLOT(hideWrongDateTimeSLOT()));
    //Таймер для просьбы подождать
    timerForWaitPlease = new QTimer();
    timerForWaitPlease->setInterval(1);

    //Кнопки
    calculateFOV = new QPushButton("&Рассчитать поле зрения", this);
    calculateFOV->
            setStyleSheet("QPushButton{background-color:#F0E68C;border: 0px;}QPushButton:pressed{background-color:#E0FFFF;border:0px;}");
    calculateFOV->setGeometry(QRect(QPoint(0, 90), QSize(244, 26)));

    showFOV = new QPushButton("&Скрыть", this);
    showFOV->
            setStyleSheet("QPushButton{background-color:#00FF00;border: 0px;}QPushButton:pressed{background-color:#E0FFFF;border:0px;}");
    showFOV->setGeometry(QRect(QPoint(200, 90), QSize(100, 26)));

    drawMyPoints = new QPushButton("&Нарисовать набор точек", this);
    drawMyPoints->
            setStyleSheet("QPushButton{background-color:#F0E68C;border: 0px;}QPushButton:pressed{background-color:#E0FFFF;border:0px;}");
    drawMyPoints->setGeometry(QRect(QPoint(0, 120), QSize(244, 26)));

    showMyPoints = new QPushButton("&Скрыть", this);
    showMyPoints->
            setStyleSheet("QPushButton{background-color:#00FF00;border: 0px;}QPushButton:pressed{background-color:#E0FFFF;border:0px;}");
    showMyPoints->setGeometry(QRect(QPoint(200, 120), QSize(100, 26)));

    calculateFITSLocalization = new QPushButton("&Нарисовать 90% контуры из FITS файла", this);
    calculateFITSLocalization->
            setStyleSheet("QPushButton{background-color:#F0E68C;border: 0px;}QPushButton:pressed{background-color:#E0FFFF;border:0px;}");
    calculateFITSLocalization->setGeometry(QRect(QPoint(0, 150), QSize(264, 26)));

    connect(calculateFOV, SIGNAL(clicked()), this, SLOT(calculateFieldOfViewSLOT()));
    connect(showFOV, SIGNAL(clicked()), this, SLOT(showFieldOfViewSLOT()));
    //connect(calculateFOV, SIGNAL(clicked()), this, SLOT(waitProcessFitsFileSLOT()));
    connect(drawMyPoints, SIGNAL(clicked()), this, SLOT(drawMyPointsSLOT()));
    connect(showMyPoints, SIGNAL(clicked()), this, SLOT(showMyPointsSLOT()));

    FOVCreated = false;
    FOVShown = false;
    isFOVPngCreated = false;

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//Скроллинг осуществляется правой кнопкой мыши, с помощью перетаскивания
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//Скроллинг осуществляется правой кнопкой мыши, с помощью перетаскивания
    QScroller::grabGesture(view, QScroller::RightMouseButtonGesture);//Скроллинг осуществляется правой кнопкой мыши, с помощью перетаскивания
    view->viewport()->installEventFilter(this);//Отключили скроллинг мышью
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    //Объект для реакции на воздействие пользователя
    response = new reaction(this, view, scene, fieldsAndLines, anchorPoint, myPoint, initialSceneHeight, initialSceneWidth,
                            advices, timerForCtrlCMouse);

    isDateTimeHasChanged = true;
}

void MainWindow::mousePressEvent(QMouseEvent *eventMouse)
{
    response->mousePressEvent(eventMouse);
}

void MainWindow::wheelEvent(QWheelEvent *eventWheel)
{
    response->wheelEvent(eventWheel);
}

void MainWindow::resizeEvent(QResizeEvent *eventResize)
{
    response->resizeEvent(eventResize);
}

QRectF MainWindow::windowSizeWithFrame()
{
    return this->frameGeometry();
}

QSize MainWindow::windowSizeWithoutFrame()
{
    return view->size();
}

QPointF MainWindow::mapToScene()
{
    return view->mapToScene(QCursor::pos());
}

QPointF MainWindow::mapFromScene()
{
    return anchorPoint->mapFromScene(pos());
}

int MainWindow::initialSceneHeightCopy()
{
    return initialSceneHeight;
}

int MainWindow::initialSceneWidthCopy()
{
    return initialSceneWidth;
}

int MainWindow::currentSceneHeightCopy()
{
    return scene->height();
}

int MainWindow::currentSceneWidthCopy()
{
    return scene->width();
}

void MainWindow::removeMyPoint()
{
    scene->removeItem(myPoint);
}

void MainWindow::calculateFieldOfViewSLOT()
{
    fieldsAndLines->checkDateAndTime(fieldsAndLines);
    QStringList currentDate = fieldsAndLines->getDateStringList();
    QStringList currentTime = fieldsAndLines->getTimeStringList();
    //myPoint->hide();
    fillSMFOV = new fillSkymap(1, initialSceneWidth, initialSceneHeight, fieldsAndLines, this);
    if (currentDate.size() == previousDate.size() && currentDate.size() == 3
            && currentTime.size() == previousTime.size() && currentTime.size() == 3)
    {
        for (int i = 0; i < 3; i++)
        {
            if (currentDate[i] != previousDate[i]
                    || currentTime[i] != previousTime[i])
            {
                isDateTimeHasChanged = true;
            }
        }
    }

    if (isDateTimeHasChanged == true)
    {
        advices->showPleaseWait();
        scene->addItem(fillSMFOV);
        this->repaint();
        FOVCreated = fillSMFOV->checkIfFOVCreated();

        if(FOVCreated == false)
        {
            advices->showWrongFormat();
            timerForCtrlCMouse->start();
            scene->removeItem(fillSMFOV);
        }
        else
        {
            if (isFOVPngCreated == true)
            {
                filledFOVSkymapItem->hide();
            }
            scene->setSceneRect(-30,-30,1260, 660);

            img = new QImage(5040, 2640,QImage::Format_ARGB32_Premultiplied);
            img->fill(Qt::transparent);
            p = new QPainter(img);
            scene->render(p);
            p->end();
            img->save("FieldOfViewBUST.png");

            scene->setSceneRect(0,0,1200, 600);

            fillSMFOV->hide();
            filledFOVSkymap = QPixmap("FieldOfViewBUST.png");
            filledFOVSkymap = filledFOVSkymap.scaled(initialSceneWidth+2.*30., initialSceneHeight+2.*30., Qt::KeepAspectRatio);
            filledFOVSkymapItem = scene->addPixmap(filledFOVSkymap);
            filledFOVSkymapItem->setPos( ( initialSceneWidth - filledFOVSkymapItem->boundingRect().width() ) /2.,
                                         ( initialSceneHeight - filledFOVSkymapItem->boundingRect().height() ) /2.);
            filledFOVSkymapItem->show();
            showFOV->setText("Скрыть");
            isFOVPngCreated = true;
            FOVShown = true;
        }

        advices->hidePleaseWait();
        previousDate = fieldsAndLines->getDateStringList();
        previousTime = fieldsAndLines->getTimeStringList();
        isDateTimeHasChanged = false;
    }
}

void MainWindow::showFieldOfViewSLOT()
{
    if (FOVCreated == true && FOVShown == false)
    {
        //fillSMFOV->show();
        filledFOVSkymapItem->show();
        FOVShown = true;
        showFOV->setText("Скрыть");
    }
    else if (FOVCreated == true && FOVShown == true)
    {
        //fillSMFOV->hide();
        filledFOVSkymapItem->hide();
        FOVShown = false;
        showFOV->setText("Показать");
    }
}

void MainWindow::drawMyPointsSLOT()
{
    fillMyPoints = new fillSkymap(2, initialSceneWidth, initialSceneHeight, fieldsAndLines);
    scene->addItem(fillMyPoints);
    bool isPointsDrawn = fillMyPoints->checkIfPointsDrawn();
    //fillMyPoints->hide();

    if(isPointsDrawn)
    {
        fillMyPoints->show();
        scene->setSceneRect(-30, -30, 1260, 660);

        QImage img(5040, 2640,QImage::Format_ARGB32_Premultiplied);
        img.fill(Qt::transparent);
        QPainter p(&img);
        scene->render(&p);
        p.end();
        img.save("MyPoints.png");

        scene->setSceneRect(0, 0, 1200, 600);

        fillMyPoints->hide();
        filledMyPointsSkymap = QPixmap("MyPoints.png");
        filledMyPointsSkymap = filledMyPointsSkymap.scaled(initialSceneWidth+2.*30., initialSceneHeight+2.*30., Qt::KeepAspectRatio);
        filledMyPointsSkymapItem = scene->addPixmap(filledMyPointsSkymap);
        filledMyPointsSkymapItem->setPos( ( initialSceneWidth - filledMyPointsSkymapItem->boundingRect().width() ) / 2.,
                                          ( initialSceneHeight - filledMyPointsSkymapItem->boundingRect().height() ) /2.);
        filledMyPointsSkymapItem->show();
        showMyPoints->setText("Скрыть");
        isMyPointsShown = true;
    }
    else
    {

    }
}

void MainWindow::showMyPointsSLOT()
{
    if (isMyPointsShown == false)
    {
        //fillSMFOV->show();
        filledMyPointsSkymapItem->show();
        isMyPointsShown = true;
        showMyPoints->setText("Скрыть");
    }
    else
    {
        //fillSMFOV->hide();
        filledMyPointsSkymapItem->hide();
        isMyPointsShown = false;
        showMyPoints->setText("Показать");
    }
}

void MainWindow::hideCtrlCMouseHintSLOT()
{
    advices->hideCtrlCMouse();
    advices->stopTimer();
}

void MainWindow::hideWaitHintSLOT()
{
    advices->hidePleaseWait();
    advices->stopTimer();

    fieldsAndLines->removeSelection();
}

void MainWindow::waitProcessFitsFileSLOT()
{

}

void MainWindow::hideWrongDateTimeSLOT()
{
    advices->hideWrongFormat();
    advices->stopTimer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Фильтр для отключения реакции на скроллинг мышью
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == view->viewport() && event->type() == QEvent::Wheel)
    {
        return true;
    }
    return false;
}
