#include "BUSTArea.h"

#include <fstream>
#include "math.h"

BUSTArea::BUSTArea()
{
    int firstWord;
    std::string secondWord;
    std::ifstream areaFile;
    areaFile.open("TablesArea.txt");
    if(!areaFile) { }//Здесь должно быть сообщение об ошибке чтения файла с площадями
    for(int p = 0; p < 8; p++)
    {
        areaFile >> firstWord;
        areaFile >> secondWord;
        if(secondWord == "плоскостей")
        {
            if(firstWord == p + 1)
            {
                for(int i = 0; i < 180; i++)//Строки
                {
                    for(int j = 0; j < 360; j++)//Столбцы
                    {
                        areaFile >> BPSTArea[p][i][j];
                    }
                }
            }
        }
    }
    //for(int p = 0; p < 8; p++) { qDebug() << BPSTAreaMin[p] << BPSTAreaMax[p]; }
    areaFile.close();
}

double BUSTArea::getArea(double azimuthalAngleBUST, double zenithAngleBUST)
{
    int zenithAngle = round(zenithAngleBUST * 180./M_PI);
    int azimuthalAngle = round(azimuthalAngleBUST * 180./M_PI);
    return BPSTArea[2][zenithAngle][azimuthalAngle];
}
