// Function Grapher.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#include <ctime>
//#include <cstdlib>
//#include <cstdio>
#include <conio.h>
#include <windows.h>
#include <math.h>

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

void changeColor(int color)
{
    color = color % 256;
    SetConsoleTextAttribute(hOut, color);
}


class Screen
{
    struct Pixel
    {
        char Char;
        short fontColor;
        short backgroundColor;
    };
private:
    unsigned int width;
    unsigned int high;
    Pixel** pixel2Dpointer;
public:
    //creating Screen
    Screen(const unsigned int width, const unsigned int high)
    {
        this->width = width;
        this->high = high;
       
        pixel2Dpointer = new Pixel*[width];

        for (unsigned int x = 0; x < width; x++)
        {
            pixel2Dpointer[x] = new Pixel[high];
        }

        for (unsigned int y = 0; y < high; y++)
        {
            for (unsigned int x = 0; x < width; x++)
            {

                pixel2Dpointer[x][y].Char = '.';
                pixel2Dpointer[x][y].fontColor = 0;
                pixel2Dpointer[x][y].fontColor = 1;
            }
        }
    }

    Screen()
    {
        *this = Screen(30, 10);
    }

    void view()
    {
        std::cout << std::endl <<"Screen high: " << this->high << " Screen width : " << this->width << std::endl;

        std::cout << std::endl;
        for (unsigned int y = 0; y < high; y++)
        {
            for (unsigned int x = 0; x < width; x++)
            {
                changeColor(pixel2Dpointer[x][y].backgroundColor * 16 + pixel2Dpointer[x][y].fontColor);
                std::cout << pixel2Dpointer[x][y].Char;
                changeColor(15);//Standard Console Color
            }
            std::cout << std::endl;
        }
    }
    //changing screen

    void colorPixel(const char ch, const short backgroundColor, const short fontColor, const unsigned int width, const unsigned int high)
    {
        pixel2Dpointer[width][high].Char = ch;
        pixel2Dpointer[width][high].backgroundColor = backgroundColor;
        pixel2Dpointer[width][high].fontColor = fontColor;
    }

    //getters
    unsigned int getWidth()
    {
        return width;
    }

    unsigned int getHigh()
    {
        return high;
    }

};
const unsigned int maxNumOfPoints = 41000;
class DynamicBoard
{
private:
    struct Point
    {
        float x;
        float y;
    };
    Screen* screen;
    Point pointTab[maxNumOfPoints];
    Point leftDownCorner;
    Point rightUpCorner;
    int pointTabSize;

    float modulo(float x,const float mod)
    {
        while (x > mod)
        {
            x-=mod;
        }
        return x;
    }
public:
    //konstruktory

    DynamicBoard()
    {
        pointTabSize = 0;

        for (int i = 0; i < maxNumOfPoints; i++)
        {
            pointTab[i].x = 0;
            pointTab[i].y = 0;
        }

        leftDownCorner.x = -1;
        leftDownCorner.y = -1;

        rightUpCorner.x = 4;
        rightUpCorner.y = 1;
        screen = new Screen(250, 100);
    }
    //wyswietlanie i nawigacja
    /*
    void updateBoard()
    {
        float pixelWidth = (rightUpCorner.x - leftDownCorner.x) / screen->getWidth();
        float pixelHigh = (rightUpCorner.y - leftDownCorner.y) / screen->getHigh();

        for (unsigned int scrX = 0; scrX < screen->getWidth(); scrX++)
        {
            for (unsigned int scrY = 0; scrY < screen->getHigh(); scrY++)
            {
                //std::cout << scrX * screen->getHigh() + scrY << std::endl;
                int howManyPointsInPixel = 0;
                for (int i = 0; i < maxNumOfPoints; i++)
                {
                    if (pointTab[i].x >= leftDownCorner.x + scrX * pixelWidth &&pointTab[i].x <= leftDownCorner.x + (scrX + 1) * pixelWidth && -pointTab[i].y >= leftDownCorner.y + scrY * pixelHigh &&
                        pointTab[i].x <= leftDownCorner.x + (scrX + 1) * pixelWidth && -pointTab[i].y <= leftDownCorner.y + (scrY + 1) * pixelHigh)
                    {
                        howManyPointsInPixel++;
                    }
                    if ((0 >= leftDownCorner.x + scrX * pixelWidth && 0 <= leftDownCorner.x + (scrX + 1) * pixelWidth)
                        || (0 >= leftDownCorner.y + scrY * pixelHigh && 0 <= leftDownCorner.y + (scrY + 1) * pixelHigh))
                        screen->colorPixel('X', 1, 1, scrX, scrY);
                    else
                        screen->colorPixel('X', 2, 2, scrX, scrY);

                    if (howManyPointsInPixel > 0)
                        screen->colorPixel('X', 15, 15, scrX, scrY);
                }
            }
        }
    }
    */
    void updateBoard()
    {
        float pixelWidth = (rightUpCorner.x - leftDownCorner.x) / screen->getWidth();
        float pixelHigh = (rightUpCorner.y - leftDownCorner.y) / screen->getHigh();

        for (unsigned int scrX = 0; scrX < screen->getWidth(); scrX++)
        {
            for (unsigned int scrY = 0; scrY < screen->getHigh(); scrY++)
            {
                if ((0 >= leftDownCorner.x + scrX * pixelWidth && 0 <= leftDownCorner.x + (scrX + 1) * pixelWidth)
                    || (0 >= leftDownCorner.y + scrY * pixelHigh && 0 <= leftDownCorner.y + (scrY + 1) * pixelHigh))
                    screen->colorPixel('X', 1, 1, scrX, scrY);
                else
                    screen->colorPixel('X', 2, 2, scrX, scrY);
            }
        }
        for (int i = 0; i < maxNumOfPoints; i++)
        {
            int x = (pointTab[i].x - leftDownCorner.x) / pixelWidth;
            int y = (pointTab[i].y - leftDownCorner.y) / pixelHigh;
            y = screen->getHigh()-y-1;//flipping y axis to change it from screen coordinates to cartesian coordinates
            if (x < screen->getWidth() && x >= 0 && y < screen->getHigh() && y >= 0)
                screen->colorPixel('X', 15, 15, x, y);
        }
    }
    void changeBoardCapturingProcedure()
    {
        std::cout << std::endl;
        std::cout << "LeftDown Corner X:" << std::endl;
        std::cin >> leftDownCorner.x;
        std::cout << "UpRight Corner X:" << std::endl;
        std::cin >> rightUpCorner.x;
        std::cout << "LeftDown Corner Y:" << std::endl;
        std::cin >> rightUpCorner.y;
        std::cout << "UpRight Corner Y:" << std::endl;
        std::cin >> leftDownCorner.y;
        rightUpCorner.y *= -1;
        leftDownCorner.y *= -1;
        std::cout << std::endl;
    }

    void viewOptions()
    {
        char ch;
        bool loop = true;
        std::cout << std::endl;
        std::cout << "Choose your option:" << std::endl;
        std::cout << "1.Change corners" << std::endl;
        std::cout << "E.Exit" << std::endl;
        std::cout << std::endl;
        while (loop)
        {
            ch = _getch();
            if (ch == '1')
            {
                changeBoardCapturingProcedure();
                loop = false;
            }
            else if (ch == 'E' || ch == 'e')
            {
                loop = false;
            }
        }
    }

    void view()
    {
        std::cout << std::endl;
        std::cout << "LDX: " << leftDownCorner.x << " LDY: " << leftDownCorner.y << " RUX: " << rightUpCorner.x << " RUY: " << rightUpCorner.y;
        std::cout << std::endl;
        screen->view();
        viewOptions();
        updateBoard();
    }
    //zmiana koordynatow punktu
    void changePointCoordianates(const int point, const float x,const float y)
    {
        if (point < maxNumOfPoints)
        {
            pointTab[point].x = x;
            pointTab[point].y = y;
        }
    }
};

class FunctionGrapher
{

};
float modulo(float x, const float mod)
{
    while (x > mod)
        x = x - mod;

    return x;
}
float function(const float x)
{
    return (modulo(2*x,2) - 1);
}
int main2()
{
    DynamicBoard* dynamicBoard = new DynamicBoard;
    int squish = 12;
    float bibu[maxNumOfPoints];
    int i = 0;
    for (float x = 0; x < 10; x = x + 0.001)
    {
        if (i > 3144 / 2 * 3/squish)
            bibu[i] =bibu[i- 3144 / 2 * 3/squish]*0.001 + bibu[i - 1];
        else
            bibu[i] = sin(x*squish);

            dynamicBoard->changePointCoordianates(i, x, bibu[i]);
        i++;
    }

    dynamicBoard->updateBoard();

    while (true)
        dynamicBoard->view();

    /*Screen* screen = new Screen(200, 50);
    screen->view();
    for(int x = 0 ; x < 10 ; x++)
        for (int y = 0; y < 10; y++)
            screen->colorPixel('X',2,5, x, y);
    screen->view();*/
    return 0;
}
int main3()
{
    DynamicBoard* dynamicBoard = new DynamicBoard;
    //int squish = 12;
    float Vx[maxNumOfPoints];
    float Vy[maxNumOfPoints];
    float Ax[maxNumOfPoints];
    float Ay[maxNumOfPoints];
    float Sx[maxNumOfPoints];
    float Sy[maxNumOfPoints];

    float sqrtOf2 = sqrt(2);
    Sx[0]= -sqrtOf2 / 2;
    Sy[0]= sqrtOf2 / 2;
    Vx[0]= 4 * sqrtOf2;
    Vy[0]= 4 * sqrtOf2;
    Ax[0]= 32 * sqrtOf2;
    Ay[0]= -32 * sqrtOf2;
    /*Sx[0] = -1;
    Sy[0] =0.000001;
    Vy[0] = 8;
    Vx[0] = 0.000008;
    Ax[0] = 64;
    Ay[0] = -0.000002;
    */
    float deltaT = 0.00014;

    dynamicBoard->changePointCoordianates(0, Sx[0], Sy[0]);
    int i = 0;
    for (float t = 0; i<maxNumOfPoints; t = t + deltaT)
    {
        if (i % 1000 == 0)
        {
            std::cout << i<< std::endl;
            std::cout << i << ":|Sx: " << Sx[i] << " |Sy: " << Sy[i] << std::endl;
            std::cout << i << ":|Vx: " << Vx[i] << " |Vy: " << Vy[i] << std::endl;
            std::cout << i << ":|Ax: " << Ax[i] << " |Ay: " << Ay[i] << std::endl;
        }

        i++;

        //std::cout << std::endl;

        Sx[i] = Sx[i - 1] + Vx[i - 1] * deltaT;
        Vx[i] = Vx[i - 1] + Ax[i - 1] * deltaT;
        if((Sy[i - 1] - 0)!=0)
            Ax[i] = (Sx[i - 1] - 0) * Ay[i - 1] /(Sy[i - 1] - 0);
        else
            Ax[i] = (Sx[i - 1] - 0) * Ay[i - 1];


        Sy[i] = Sy[i - 1] + Vy[i - 1] * deltaT;
        Vy[i] = Vy[i - 1] + Ay[i - 1] * deltaT;
        if ((Sx[i - 1] - 0) != 0)
        Ay[i] = (Sy[i - 1] - 0) * Ax[i - 1] / (Sx[i - 1] - 0);
        else
            Ay[i] = (Sy[i - 1] - 0) * Ax[i - 1];

        dynamicBoard->changePointCoordianates(i, Sx[i], Sy[i]);
        //_getch();
    }

    std::cout << i << ":|Sx: " << Sx[i] << " |Sy: " << Sx[i] << std::endl;
    std::cout << i << ":|Vx: " << Vx[i] << " |Vy: " << Vx[i] << std::endl;
    std::cout << i << ":|Ax: " << Sx[i] << " |Ay: " << Ax[i] << std::endl;
    std::cout << std::endl;

    dynamicBoard->updateBoard();

    while (true)
        dynamicBoard->view();

    return 0;
}
int main()
{
DynamicBoard* dynamicBoard = new DynamicBoard;
float A[maxNumOfPoints];
float B[maxNumOfPoints];
float C[maxNumOfPoints];
float D[maxNumOfPoints];
//float sqrtOf2 = sqrt(2);
//A[0] = 1;
//B[0] = 0;
//C[0] = -1;
//D[0] = 0;

float deltaT = 0.0002;

dynamicBoard->changePointCoordianates(0, 0, A[0]);
int i = 0;
A[0] = 1;
B[0] = 0;
for (float t = 0; i < maxNumOfPoints; t = t + deltaT)
{
    i++;
        A[i] = A[i - 1]*((float)1+deltaT);
        B[i] = A[i] / 3;
    //std::cout << std::endl;
    //A[i] = A[i - 1] + B[i - 1] * deltaT;
    //B[i] = B[i - 1] - A[i - 1] * deltaT;
    //C[i] = C[i - 1] + D[i - 1] * deltaT;
    //D[i] = D[i - 1] + A[i - 1] * deltaT;
    //std::cout <<i<<"|A:" << A[i] << "|B:" << B[i] << "|C:" << C[i] << "|D:" << D[i] << std::endl;

    dynamicBoard->changePointCoordianates(i, t, B[i]);
    //std::cout << "x: " << deltaT * i << " |y: " << A[i]<<std::endl;
    //_getch();
}

dynamicBoard->updateBoard();

while (true)
dynamicBoard->view();

return 0;
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
