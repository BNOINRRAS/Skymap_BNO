#include "calculation.h"
#include <math.h>

calculation::calculation(int gy, int gm, int gd, int h, int m, double s, double ra, double dec) :
    grigorianYear(gy), grigorianMonth(gm), grigorianDay(gd),
    hourUT1(h), minuteUT1(m), secondUT1(s),
    rightAscensionJ2000(ra), declinationJ2000(dec)
{
    //Переменные, не являющиеся полями класса
    double ascensionNutation;//Нутация по Пр. восх.
    double declinationNutation;//Нутация по склонению
    double ascensionPrecession;//Прецессия по Пр. восх.
    double declinationPrecession;//Прецессия по склонению
    double hourAngle;//Часовой угол

    double GMST;//Гринвичское среднее звёздное время
    double d;//Интервал времени от эпохи T0 до эпохи T в средних солнечных сутках
    double t;//Интервал времени от эпохи T0 до эпохи T в юлианских столетиях по 36525 средних солнечных суток
    double v1, v2, v3, v4, v5;//Суммируемые члены в формуле вычисления Гринвичского Среднего Звездного Времени

    double nutationInLongitude;//Нутация в долготе
    double nutationInObliquity;//Нутация в наклоне
    double meanObliquityOfTheEcliptic;//Средний наклон эклиптики к экватору
    double trueObliquityOfTheEcliptic;//Истинный наклон эклиптики к экватору

    double X2000, Y2000, Z2000;//X, Y, Z в прямоугольной системе координат в момент J2000
    double xCurrent, yCurrent, zCurrent;//X, Y, Z в прямоугольной системе координат в момент события

    double raWithNutation;//Прямое восхождение с поправкой на нутацию
    double decWithNutation;//Склонение с поправкой на нутацию
    //double par1;//Средняя аномалия Луны
    double par2;//Средняя аномалия Солнца
    double par3;//Средний аргумент широты Луны
    double par4;//Разность средних долгот Луны и Солнца
    double par5;//Средняя долгота восходящего узла орбиты Луны на эклиптике

    double n11, n12, n13, n21, n22, n23, n31, n32, n33;//Члены матрицы нутации

    double raWithPrecession;//Прямое восхождение с поправкой на прецессию
    double decWithPrecession;//Склонение с поправкой на прецессию

    double coeff1, coeff2, coeff3;//Коэффициенты в матрице прецессии - ζ, Z, θ
    double p11, p12, p13, p21, p22, p23, p31, p32, p33;//Члены матрицы прецессии

    //ВЫЧИСЛЕНИЕ ЮЛИАНСКОЙ ДАТЫ
    double var1, var2, var3;//Переменные для промежуточных вычислений
    var1 = floor(( 14. - (double)grigorianMonth ) / 12.);
    var2 = (double)grigorianYear + 4800. - var1;
    var3 = (double)grigorianMonth + ( 12. * var1 ) - 3.;
    JD = (double)grigorianDay
            + floor( ( ( 153. * var3 ) + 2. ) / 5. )
            + ( 365. * var2 )
            + floor( var2 / 4. )
            - floor( var2 / 100. )
            + floor( var2 / 400. )
            - 32045.;//Википедия, стр. "Юлианская дата"

    //ВЫЧИСЛЕНИЕ GMST В ВЫБРАННЫЙ МОМЕНТ ВРЕМЕНИ
    d = JD - 0.5 - 2451545.0;//J2000//Вычли 0.5, потому что в 00:00 UT1 наступает половина Юлианского дня
    t = d / 36525.;
    v1 = 6. * 3600. + 41. * 60. + 50.54841;
    v2 = 8640184.812866 * t;
    v3 = 0.093104 * t * t;
    v4 = -0.0000062 * t * t * t;

    dayTimeSeconds = ( hourUT1 * 3600. ) + ( minuteUT1 * 60. ) + secondUT1;

    v5 = dayTimeSeconds * 1.00273790935;//Время с поправкой для перехода к Гринвичскому Времени
    GMST = v1 + v2 + v3 + v4;//Получили GMST в 00:00 UT1 на Гринвичском меридиане
    GMST = GMST + v5;//Получили GMST на выбранный момент времени
    while (GMST > 86400.)
    {
        GMST = GMST - 86400.;
    }
    while (GMST <= 0)
    {
        GMST = GMST + 86400.;
    }
    GMST = GMST / 3600.;//Перевели в часы
    GMSTDeg = GMST * 15.;//Перевели часы в градусы
    //ЗАДАЕТСЯ ПРЯМОЕ ВОСХОЖДЕНИЕ
    rightAscensionJ2000 = rightAscensionJ2000 / (180./M_PI);//Перевод в радианы
    //ЗАДАЕТСЯ СКЛОНЕНИЕ
    declinationJ2000 = declinationJ2000 / (180./M_PI);//Перевод в радианы
    latitude = 43.271438;//Примерное положение ПСТ
    latitude = latitude / (180./M_PI);
    longitude = 42.695409;//Примерное положение ПСТ 42 41 43.5
    longitude = longitude / (180./M_PI);

    //Расчеты ведутся с помощью перехода к прямоугольной системе координат
    //Источник "МЕТОДИЧЕСКИЕ УКАЗАНИЯ. СПУТНИКИ ЗЕМЛИ ИСКУССТВЕННЫЕ. ОСНОВНЫЕ СИСТЕМЫ КООРДИНАТ ДЛЯ БАЛЛИСТИЧЕСКОГО ОБЕСПЕЧЕНИЯ ПОЛЕТОВ И МЕТОДИКА РАСЧЕТА ЗВЕЗДНОГО ВРЕМЕНИ" 1989г. (1990г.)
    //РАСЧЕТ НУТАЦИИ
    //Средняя аномалия Луны
    //par1 = ( 485866.733 / 3600. ) / ( 180. / M_PI )
    //        + ( ( 1717915922.633 / 3600. ) / ( 180. / M_PI ) ) * t
    //        + ( ( 31.310 / 3600. ) / ( 180. / M_PI ) ) * t * t
    //        + ( ( 0.064 / 3600. ) / ( 180. / M_PI ) ) * t * t * t;
    //Средняя аномалия Солнца
    par2 = ( 1287099.804 / 3600. ) / ( 180. / M_PI )
            + ( ( 129596581.224 / 3600. ) / ( 180. / M_PI ) ) * t
            - ( ( 0.577 / 3600. ) / ( 180. / M_PI ) ) * t * t
            - ( ( 0.012 / 3600. ) / ( 180. / M_PI ) ) * t * t * t;
    //Средний аргумент широты Луны
    par3 = ( 335778.877 / 3600. ) / ( 180. / M_PI )
            + ( ( 1739527263.137 / 3600. ) / ( 180. / M_PI ) ) * t
            - ( ( 13.257 / 3600. ) / ( 180. / M_PI ) ) * t * t
            + ( ( 0.011 / 3600. ) / ( 180. / M_PI ) ) * t * t * t;
    //Разность средних долгот Луны и Солнца
    par4 = ( 1072261.307 / 3600. ) / ( 180. / M_PI )
            + ( ( 1602961601.328 / 3600. ) / ( 180. / M_PI ) ) * t
            - ( ( 6.891 / 3600. ) / ( 180. / M_PI ) ) * t * t
            + ( ( 0.019 / 3600. ) / ( 180. / M_PI ) ) * t * t * t;
    //Средняя долгота восходящего узла орбиты Луны на эклиптике
    par5 = ( 450160.280 / 3600. ) / ( 180. / M_PI )
            - ( ( 6962890.539 / 3600. ) / ( 180. / M_PI ) ) * t
            + ( ( 7.455 / 3600. ) / ( 180. / M_PI ) ) * t * t
            + ( ( 0.008 / 3600. ) / ( 180. / M_PI ) ) * t * t * t;
    //Расчет с точностью 0'',1
    //Нутация в долготе
    nutationInLongitude = ( ( -17.1996 / 3600. ) / ( 180. / M_PI ) ) * sin( par5 )
            + ( ( 0.2062 / 3600. ) / ( 180. / M_PI ) ) * sin( 2. * par5 )
            - ( ( 1.3187 / 3600. ) / ( 180. / M_PI ) ) * sin( 2. * ( par3 - par4 + par5 ) )
            + ( ( 0.1426 / 3600. ) / ( 180. / M_PI ) ) * sin( par2 )
            - ( ( 0.2274 / 3600. ) / ( 180. / M_PI ) ) * sin( 2. * ( par3 + par5 ) );

    //Нутация в наклоне
    nutationInObliquity = ( ( 9.2025 / 3600. ) / ( 180. / M_PI ) ) * cos( par5 )
            + ( ( 0.5736 / 3600. ) / ( 180. / M_PI ) ) * cos( 2. * ( par3 - par4 + par5 ) )
            + ( ( 0.0977 / 3600. ) / ( 180. / M_PI ) ) * cos( 2. * ( par3 + par4 ) );

    //Средний наклон эклиптики к экватору
    meanObliquityOfTheEcliptic = ( 84381.448 / 3600. ) / ( 180. / M_PI )
            - ( ( 46.815 / 3600. ) / ( 180. / M_PI ) ) * t
            - ( ( 0.00059 / 3600. ) / ( 180. / M_PI ) ) * t * t
            + ( ( 0.001813 / 3600. ) / ( 180. / M_PI ) ) * t * t * t;

    //Истинный наклон эклиптики к экватору
    trueObliquityOfTheEcliptic = meanObliquityOfTheEcliptic + nutationInObliquity;

    n11 = cos( nutationInLongitude );
    n12 = -sin( nutationInLongitude ) * cos( meanObliquityOfTheEcliptic );
    n13 = -sin( nutationInLongitude ) * sin( meanObliquityOfTheEcliptic );
    n21 = sin( nutationInLongitude ) * cos( trueObliquityOfTheEcliptic );
    n22 = cos( nutationInLongitude ) * cos( trueObliquityOfTheEcliptic ) * cos( meanObliquityOfTheEcliptic ) + sin( trueObliquityOfTheEcliptic ) * sin( meanObliquityOfTheEcliptic );
    n23 = cos( nutationInLongitude ) * cos( trueObliquityOfTheEcliptic ) * sin( meanObliquityOfTheEcliptic ) - sin( trueObliquityOfTheEcliptic ) * cos( meanObliquityOfTheEcliptic );
    n31 = sin( nutationInLongitude ) * sin( trueObliquityOfTheEcliptic );
    n32 = cos( nutationInLongitude ) * sin( trueObliquityOfTheEcliptic ) * cos( meanObliquityOfTheEcliptic ) - cos( trueObliquityOfTheEcliptic ) * sin( meanObliquityOfTheEcliptic );
    n33 = cos( nutationInLongitude ) * sin( trueObliquityOfTheEcliptic ) * sin( meanObliquityOfTheEcliptic ) + cos( trueObliquityOfTheEcliptic ) * cos( meanObliquityOfTheEcliptic );

    X2000 = cos(rightAscensionJ2000) * cos(declinationJ2000);
    Y2000 = sin(rightAscensionJ2000) * cos(declinationJ2000);
    Z2000 = sin(declinationJ2000);

    xCurrent = X2000 * n11 + Y2000 * n12 + Z2000 * n13;
    yCurrent = X2000 * n21 + Y2000 * n22 + Z2000 * n23;
    zCurrent = X2000 * n31 + Y2000 * n32 + Z2000 * n33;

    raWithNutation = atan2(yCurrent, xCurrent);
    if(raWithNutation < 0)
    {
        raWithNutation = raWithNutation + ( 2. * M_PI );
    }
    if(raWithNutation > 2. * M_PI)
    {
        raWithNutation = raWithNutation - ( 2. * M_PI );
    }

    decWithNutation = asin(zCurrent);

    ascensionNutation = raWithNutation - rightAscensionJ2000;
    declinationNutation = decWithNutation - declinationJ2000;

    //РАСЧЕТ ПРЕЦЕССИИ
    coeff1 = ( ( 2306.2181 / 3600. ) / ( 180. / M_PI ) ) * t
            + ( ( 0.30188 / 3600. ) / ( 180. / M_PI ) ) * t * t
            + ( ( 0.017998 / 3600. ) / ( 180. / M_PI ) ) * t * t * t;//ζ
    coeff2 = ( ( 2306.2181 / 3600. ) / ( 180. / M_PI ) ) * t
            + ( ( 1.09468 / 3600. ) / ( 180. / M_PI ) ) * t * t
            + ( ( 0.018203 / 3600. ) / ( 180. / M_PI ) ) * t * t * t;//Z
    coeff3 = ( ( 2004.3109 / 3600. ) / ( 180. / M_PI ) ) * t
            - ( ( 0.42665 / 3600. ) / ( 180. / M_PI ) ) * t * t
            - ( ( 0.041833 / 3600. ) / ( 180. / M_PI ) ) * t * t * t;//θ
    p11 = cos(coeff1) * cos(coeff2) * cos(coeff3) - sin(coeff1) * sin(coeff2);
    p12 = -sin(coeff1) * cos(coeff2) * cos(coeff3) - cos(coeff1) * sin(coeff2);
    p13 = -cos(coeff2) * sin(coeff3);
    p21 = cos(coeff1) * sin(coeff2) * cos(coeff3) + sin(coeff1) * cos(coeff2);
    p22 = -sin(coeff1) * sin(coeff2) * cos(coeff3) + cos(coeff1) * cos(coeff2);
    p23 = -sin(coeff2) * sin(coeff3);
    p31 = cos(coeff1) * sin(coeff3);
    p32 = -sin(coeff1) * sin(coeff3);
    p33 = cos(coeff3);

    X2000 = cos(rightAscensionJ2000) * cos(declinationJ2000);
    Y2000 = sin(rightAscensionJ2000) * cos(declinationJ2000);
    Z2000 = sin(declinationJ2000);

    xCurrent = X2000 * p11 + Y2000 * p12 + Z2000 * p13;
    yCurrent = X2000 * p21 + Y2000 * p22 + Z2000 * p23;
    zCurrent = X2000 * p31 + Y2000 * p32 + Z2000 * p33;

    raWithPrecession = atan2(yCurrent, xCurrent);
    if(raWithPrecession < 0)
    {
        raWithPrecession = raWithPrecession + ( 2. * M_PI );
    }
    if(raWithPrecession > 2. * M_PI)
    {
        raWithPrecession = raWithPrecession - ( 2. * M_PI );
    }

    decWithPrecession = asin(zCurrent);

    ascensionPrecession = raWithPrecession - rightAscensionJ2000;
    declinationPrecession = decWithPrecession - declinationJ2000;

    GMSTDeg = GMSTDeg + nutationInLongitude * (180./M_PI) * cos(trueObliquityOfTheEcliptic);//Добавили поправку за нутацию для GMST. Расчет окончен - получили GTST (Greenwich True Sidereal Time)
    if (GMSTDeg > 360. )
    {
        GMSTDeg = GMSTDeg - 360.;
    }
    if (GMSTDeg < 0)
    {
        GMSTDeg = GMSTDeg + 360.;
    }

    ascensionCurrent = rightAscensionJ2000 + ascensionPrecession + ascensionNutation;//Добавили поправки за прецессию и нутацию
    declinationCurrent = declinationJ2000 + declinationPrecession + declinationNutation;//Добавили поправки за прецессию и нутацию

    //Последняя стадия расчета координат - получаем зенитный и азимутальный угол
    hourAngle = ( GMSTDeg / (180./M_PI) ) + longitude - ascensionCurrent;

    zenithAngle = acos( sin( declinationCurrent ) * sin( latitude ) + cos( declinationCurrent ) * cos( latitude ) * cos( hourAngle ) );

    azimuthalAngle180 = atan( ( sin(hourAngle) ) * cos(declinationCurrent)
                           / ( ( sin(latitude) * cos(hourAngle) * cos(declinationCurrent) ) - ( sin(declinationCurrent) * cos(latitude) ) ) );

    if( ( (sin(latitude) * cos(hourAngle) * cos(declinationCurrent) ) - ( sin(declinationCurrent) * cos(latitude) ) ) > 0 )
    {
        if ( azimuthalAngle180 < 0 )
        {
            azimuthalAngle180 = azimuthalAngle180 + ( 2. * M_PI );
        }
    }
    else
    {
        azimuthalAngle180 = azimuthalAngle180 + M_PI;
    }

    if ( azimuthalAngle180 > M_PI )
    {
        azimuthalAngle180 = azimuthalAngle180 - ( 2. * M_PI );
    }
    //ПЕРЕВОД В СИСТЕМУ ПСТ
    azimuthalAngle360 = azimuthalAngle180;
    if(azimuthalAngle180 < 0)
    {
        azimuthalAngle360 = ( 2. * M_PI ) + azimuthalAngle180;
    }
    azimuthalAngleBUST = ( 421.5 * ( M_PI / 180. ) ) - azimuthalAngle360;//Угол между направлением на север и направлением fi=270 равен 28.5 градуса
    if(azimuthalAngleBUST > ( 2. * M_PI ))
    {
        azimuthalAngleBUST = azimuthalAngleBUST - ( 2. * M_PI );
    }
    altitude = ( M_PI / 2. ) - zenithAngle;
}

//Деструктор
calculation::~calculation()
{
    delete &grigorianDay;
    delete &grigorianMonth;
    delete &grigorianYear;
    delete &hourUT1;
    delete &minuteUT1;
    delete &secondUT1;
    delete &rightAscensionJ2000;
    delete &declinationJ2000;
    delete &azimuthalAngle180;
    delete &azimuthalAngle360;
    delete &azimuthalAngleBUST;
    delete &zenithAngle;
    delete &altitude;
    delete &ascensionCurrent;
    delete &declinationCurrent;
}

double calculation::getGMSTDeg()
{
    return GMSTDeg;
}

double calculation::getJD()
{
    return double(JD) + (dayTimeSeconds/(24.*3600.) - 0.5);
}

double calculation::getAzimuthalAngleBUST()
{
    return azimuthalAngleBUST;
}

double calculation::getZenithAngleBUST()
{
    return zenithAngle;
}

double calculation::longitudeCopy()
{
    return longitude;
}

double calculation::latitudeCopy()
{
    return latitude;
}

double calculation::ascensionCurrentCopy()
{
    return ascensionCurrent;
}

double calculation::declinationCurrentCopy()
{
    return declinationCurrent;
}

QString calculation::getBUSTCoordinates()
{
    QString stringAzimuthalAngle360 = QString::number(azimuthalAngle360 * 180./M_PI);
    QString stringAzimuthalAngleBUST = QString::number(azimuthalAngleBUST * 180./M_PI);
    QString stringZenithAngle = QString::number(zenithAngle * 180./M_PI);
    QString divide = "\t";
    return stringAzimuthalAngle360 + divide + stringAzimuthalAngleBUST + divide + stringZenithAngle;
}
