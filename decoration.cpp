#include "decoration.h"
#include <QGraphicsTextItem>

decoration::decoration(QGraphicsScene *scene)
{
    QFont serifFont("Arial", 14, QFont::Bold);
    //Надпись "Экваториальные координаты"
    QGraphicsTextItem *textItem = new QGraphicsTextItem("Экваториальные координаты");
    textItem->setPos(500, -30);
    textItem->setFont(serifFont);
    //scene->addItem(textItem);
    QPen pen(Qt::black);
    pen.setWidth(3);

    //Надписи шкалы - 19 надписей: 10 на склонение и 9 на прямое восхождение
    QFont serifFont1("Arial", 18);
    QGraphicsTextItem *h0a = new QGraphicsTextItem("0h");
    h0a->setPos(1182, 294);
    h0a->setFont(serifFont1);
    scene->addItem(h0a);
    QGraphicsTextItem *h3 = new QGraphicsTextItem("3h");
    h3->setPos(1030, 294);
    h3->setFont(serifFont1);
    scene->addItem(h3);
    QGraphicsTextItem *h6 = new QGraphicsTextItem("6h");
    h6->setPos(880, 294);
    h6->setFont(serifFont1);
    scene->addItem(h6);
    QGraphicsTextItem *h9 = new QGraphicsTextItem("9h");
    h9->setPos(730, 294);
    h9->setFont(serifFont1);
    scene->addItem(h9);
    QGraphicsTextItem *h12 = new QGraphicsTextItem("12h");
    h12->setPos(575, 294);
    h12->setFont(serifFont1);
    scene->addItem(h12);
    QGraphicsTextItem *h15 = new QGraphicsTextItem("15h");
    h15->setPos(425, 294);
    h15->setFont(serifFont1);
    scene->addItem(h15);
    QGraphicsTextItem *h18 = new QGraphicsTextItem("18h");
    h18->setPos(280, 294);
    h18->setFont(serifFont1);
    scene->addItem(h18);
    QGraphicsTextItem *h21 = new QGraphicsTextItem("21h");
    h21->setPos(130, 294);
    h21->setFont(serifFont1);
    scene->addItem(h21);
    QGraphicsTextItem *h0b = new QGraphicsTextItem("0h");
    h0b->setPos(-15, 294);
    h0b->setFont(serifFont1);
    scene->addItem(h0b);
    QGraphicsTextItem *d0a = new QGraphicsTextItem("0°");
    d0a->setPos(1205, 270);
    d0a->setFont(serifFont1);
    scene->addItem(d0a);
    QGraphicsTextItem *d30a = new QGraphicsTextItem("30°");
    d30a->setPos(1160, 160);
    d30a->setFont(serifFont1);
    scene->addItem(d30a);
    QGraphicsTextItem *d60a = new QGraphicsTextItem("60°");
    d60a->setPos(1020, 50);
    d60a->setFont(serifFont1);
    scene->addItem(d60a);
    QGraphicsTextItem *dm30a = new QGraphicsTextItem("-30°");
    dm30a->setPos(1160, 405);
    dm30a->setFont(serifFont1);
    scene->addItem(dm30a);
    QGraphicsTextItem *dm60a = new QGraphicsTextItem("-60°");
    dm60a->setPos(1020, 515);
    dm60a->setFont(serifFont1);
    scene->addItem(dm60a);
    QGraphicsTextItem *d0b = new QGraphicsTextItem("0°");
    d0b->setPos(-30, 270);
    d0b->setFont(serifFont1);
    scene->addItem(d0b);
    QGraphicsTextItem *d30b = new QGraphicsTextItem("30°");
    d30b->setPos(5, 160);
    d30b->setFont(serifFont1);
    scene->addItem(d30b);
    QGraphicsTextItem *d60b = new QGraphicsTextItem("60°");
    d60b->setPos(145, 50);
    d60b->setFont(serifFont1);
    scene->addItem(d60b);
    QGraphicsTextItem *dm30b = new QGraphicsTextItem("-30°");
    dm30b->setPos(-5, 405);
    dm30b->setFont(serifFont1);
    scene->addItem(dm30b);
    QGraphicsTextItem *dm60b = new QGraphicsTextItem("-60°");
    dm60b->setPos(135, 515);
    dm60b->setFont(serifFont1);
    scene->addItem(dm60b);

    h0a->setZValue(3);
    h3->setZValue(3);
    h6->setZValue(3);
    h9->setZValue(3);
    h12->setZValue(3);
    h15->setZValue(3);
    h18->setZValue(3);
    h21->setZValue(3);
    h0b->setZValue(3);
    d0a->setZValue(3);
    d30a->setZValue(3);
    d60a->setZValue(3);
    dm30a->setZValue(3);
    dm60a->setZValue(3);
    d0b->setZValue(3);
    d30b->setZValue(3);
    d60b->setZValue(3);
    dm30b->setZValue(3);
    dm60b->setZValue(3);
}

decoration::~decoration()
{

}
