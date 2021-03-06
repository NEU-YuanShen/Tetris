#include "tetris.h"
#include "ui_tetris.h"

#include <QDebug>
#include <QMessageBox>
#include <QTimerEvent>
#include <QPainter>
#include <QTime>

#define NUM_BRICK_TYPES 7
#define	NUM_TURNS_PER_STEP 3
#define COLOR_CHANGE 60

Tetris::Tetris(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Tetris)
{
    ui->setupUi(this);
    setFixedSize(848,440);
    init();
    setWindowTitle("DoubleGame");
    setWindowIcon(QIcon("F:\\qt_project\\Tetris\\images\\Tetris.ico"));
     connect(ui->action_start, SIGNAL(triggered()), this, SLOT(gameStart()));
     connect(ui->action_pause, SIGNAL(triggered()), this, SLOT(onPause()));
     connect(ui->action_quit, SIGNAL(triggered()), this, SLOT(close()));
     connect(ui->action_easy, SIGNAL(triggered()), this, SLOT(onDiffEasy()));
     connect(ui->action_middle, SIGNAL(triggered()), this, SLOT(onDiffMid()));
     connect(ui->action_difficult, SIGNAL(triggered()), this, SLOT(onDiffSup()));
    // connect(ui->, SIGNAL(triggered()), this, SLOT(gameStart()));

}

Tetris::~Tetris()
{
    delete ui;

    delete bin;
    delete bin1;
    delete bin2;
    delete bin3;
}
void Tetris::init()
{
    bin=new CBin(10,20);
    bin1=new CBin(10,20);
    bin2=new CBin(10,20);
    bin3=new CBin(10,20);
    activeBrick=NULL;
    activeBrick1=NULL;
    activeBrick2=NULL;
    activeBrick3=NULL;
    gameOver=1;
    gameOver1=1;
    brickInFlight=1;
    brickInFlight1=1;
    brickType=0;
    brickType1=0;
    brickType2=0;
    brickType3=0;
    initOrientation=0;
    initOrientation1=0;
    initOrientation2=0;
    initOrientation3=0;
    notCollide=0;
    notCollide1=0;
    numLines=0;
    numLines1=0;
    difficulty=500;
    RandomFlag=false;
    RandomFlag1=false;
    Pause1=Pause2=false;
    p=false;
    Flag=false;
    modal=false;
    GameModal=2;
    image =new unsigned char*[20];
    image1 =new unsigned char*[20];
    image2 =new unsigned char*[20];
    image3 =new unsigned char*[20];
    for(int i=0;i<20;i++)
    {
        image[i]=new unsigned char[10];
        image1[i]=new unsigned char[10];
        image2[i]=new unsigned char[10];
        image3[i]=new unsigned char[10];
    }
    bin->getImage(image);
    bin1->getImage(image1);
    bin2->getImage(image2);
    bin3->getImage(image3);
    ////////////
}

void Tetris::gameStart()
{
    gameOver=0;
    brickInFlight=0;
    numLines=0;
    //------------
    gameOver1=0;
    brickInFlight1=0;
    numLines1=0;
    for(unsigned int i=0;i<20;i++)
    {
        for(unsigned int j=0;j<10;j++)
        {
            image[i][j]=0;
            image1[i][j]=0;
            image2[i][j]=0;
            image3[i][j]=0;
        }
    }
    bin->setImage(image);
    bin1->setImage(image1);
    bin2->setImage(image2);
    bin3->setImage(image3);

    timerID = startTimer(difficulty);
}

void Tetris::onPause()
{
    if(!p)   //???????????????????????????
    {
    Pause1=true;  //??????????????????
    Pause2=true;
    p=true;
    }
    else   //???????????????????????????????????????
    {
    Pause1=false;
    Pause2 =false;
    p = false;
    }
}

void Tetris::onDiffEasy()
{
    difficulty=500;
}

void Tetris::onDiffMid()
{
    difficulty=300;
}

void Tetris::onDiffSup()
{
    difficulty=150;
}


/////////////////////////////////////////////

void Tetris::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
        onPause();

    if(event->key() == Qt::Key_Right)
        activeBrick->shiftRight(bin);
    else if(event->key() == Qt::Key_Left)
        activeBrick->shiftLeft(bin);
    else if(event->key() == Qt::Key_Up)
        activeBrick->rotateClockwise(bin);
    else if(event->key() == Qt::Key_Down)
        activeBrick->shiftDown(bin);

    if(event->key() == Qt::Key_D)
        activeBrick2->shiftRight(bin2);
    else if(event->key() == Qt::Key_A)
        activeBrick2->shiftLeft(bin2);
    else if(event->key() == Qt::Key_W)
        activeBrick2->rotateClockwise(bin2);
    else if(event->key() == Qt::Key_S)
        activeBrick2->shiftDown(bin2);

}

void Tetris::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerID)
    {
        unsigned int binWidth,binHeight,binWidth1,binHeight1;
        unsigned int binWidth2,binHeight2,binWidth3,binHeight3;

       // CDC *pDC=GetDC();
        binWidth=bin->getWidth();    //???????????????????????????
        binHeight=bin->getHeight();  //???????????????????????????
        //----------
        binWidth1=bin1->getWidth();  //??????????????????????????????
        binHeight1=bin1->getHeight();//??????????????????????????????

        binWidth2=bin2->getWidth();   //???????????????????????????
        binHeight2=bin2->getHeight();   //???????????????????????????
        //----------
        binWidth3=bin3->getWidth();   //??????????????????????????????
        binHeight3=bin3->getHeight();  //??????????????????????????????

        if(!brickInFlight&&!gameOver)  //???????????????????????????????????????????????????
        {

            if(!RandomFlag)  //??????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
            {

              brickType=((unsigned int)qrand()%NUM_BRICK_TYPES)+1;
              initOrientation=(unsigned int)(qrand()%4);
              RandomFlag=true;
            }
            else //????????????????????????????????????????????????????????????????????????
            {
                brickType=brickType1;
                initOrientation=initOrientation1;
            }
            qsrand(QTime::currentTime().second());                          //???????????????????????????
            brickType1=(qrand()%NUM_BRICK_TYPES)+1;
            initOrientation1=(unsigned int)(qrand()%4);


            //??????????????????
            {
               if(brickType==1)
                activeBrick=new CTBrick;
            else if(brickType==2)
                activeBrick=new CABrick;
            else if(brickType==3)
                activeBrick=new CSBrick;
            else if(brickType==4)
                activeBrick=new CLBrick;
            else if(brickType==5)
                activeBrick=new COBrick;
            else if(brickType==6)
                activeBrick=new CIBrick;
            else if(brickType==7)
                activeBrick=new CJBrick;
              /////////////////
            if(brickType1==1)
                activeBrick1=new CTBrick;
            else if(brickType1==2)
                activeBrick1=new CABrick;
            else if(brickType1==3)
                activeBrick1=new CLBrick;
            else if(brickType1==4)
                activeBrick1=new CSBrick;
            else if(brickType1==5)
                activeBrick1=new COBrick;
            else if(brickType1==6)
                activeBrick1=new CIBrick;
            else if(brickType1==7)
                activeBrick1=new CJBrick;
            }
            activeBrick->setColour((unsigned char)brickType); //???????????????????????????
            activeBrick->putAtTop(initOrientation,binWidth/2);//????????????????????????
            //-----
            activeBrick1->setColour((unsigned char)brickType1); //??????????????????????????????
            activeBrick1->putAtMid(initOrientation1,binWidth1/2);//?????????????????????????????????????????????
            ///******************************************
            notCollide=activeBrick->checkCollision(bin); //??????????????????
            if(notCollide)        //??????????????????
            {
                brickInFlight=1;
                bin->getImage(image);
                activeBrick->operator>>(image);
                bin1->getImage(image1);
                activeBrick1->operator>>(image1);
                this->repaint();
            }
            else              //????????????????????????????????????????????????new?????????

            {
                gameOver=1;
                delete activeBrick;
                delete activeBrick1;
                brickInFlight=0;
            }
        }
        if(brickInFlight&&!gameOver)
        {
            if(!Pause1)
            notCollide=activeBrick->shiftDown(bin); //??????????????????
            if(notCollide){
                bin->getImage(image);         //?????????????????????
                activeBrick->operator>>(image); //????????????
            }
            else
            {
                brickInFlight=0;
                bin->getImage(image);

                activeBrick->operator>>(image);
                bin->setImage(image);

                this->repaint();
                numLines=numLines+bin->removeFullLines();  //??????????????????
                bin->getImage(image);

            }
            this->repaint();
        }
        if(gameOver){           //????????????????????????
            killTimer(timerID);

            if(GameModal==2)
            if(QMessageBox::question(this,tr("??????"),tr("???????????????????????????????????????????"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                gameStart();
            else
                close();
            else
                if(QMessageBox::question(this,tr("??????"),tr("??????????????????????????????????"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                gameStart();
            else
                close();

        }
        ////////////////////////////////////////////////////
        if(GameModal==2)
        {
        if(!brickInFlight1&&!gameOver1) //???????????????????????????????????????????????????
        {

            if(!RandomFlag1)
            {
              brickType2=((unsigned int)qrand()%NUM_BRICK_TYPES)+1;
              initOrientation2=(unsigned int)(qrand()%4);
              RandomFlag1=true;
            }
            else
            {
                brickType2=brickType3;
                initOrientation2=initOrientation3;
            }
            qsrand(QTime::currentTime().msec());                          //????????????????????????????????????
            brickType3=(qrand()%NUM_BRICK_TYPES)+1;
            initOrientation3=(unsigned int)(qrand()%4);



            {
                if(brickType2==1)
                activeBrick2=new CTBrick;
            else if(brickType2==2)
                activeBrick2=new CABrick;
            else if(brickType2==3)
                activeBrick2=new CLBrick;
            else if(brickType2==4)
                activeBrick2=new CSBrick;
            else if(brickType2==5)
                activeBrick2=new COBrick;
            else if(brickType2==6)
                activeBrick2=new CIBrick;
            else if(brickType2==7)
                activeBrick2=new CJBrick;

            ///
            if(brickType3==1)
                activeBrick3=new CTBrick;
            else if(brickType3==2)
                activeBrick3=new CABrick;
            else if(brickType3==3)
                activeBrick3=new CLBrick;
            else if(brickType3==4)
                activeBrick3=new CSBrick;
            else if(brickType3==5)
                activeBrick3=new COBrick;
            else if(brickType3==6)
                activeBrick3=new CIBrick;
            else if(brickType3==7)
                activeBrick3=new CJBrick;
            }

            activeBrick2->setColour((unsigned char)brickType2);
            activeBrick2->putAtTop(initOrientation2,binWidth2/2);
            //-----
            activeBrick3->setColour((unsigned char)brickType3);
            activeBrick3->putAtMid(initOrientation3,binWidth3/2);
            notCollide1=activeBrick2->checkCollision(bin2);
            if(notCollide1)
            {
                brickInFlight1=1;
                bin2->getImage(image2);
                activeBrick2->operator>>(image2);
                bin3->getImage(image3);
                activeBrick3->operator>>(image3);
                this->repaint();
            }
            else

            {
                gameOver1=1;
                delete activeBrick2;
                delete activeBrick3;
                brickInFlight1=0;
            }
        }

        //---------------------------------
        if(brickInFlight1&&!gameOver1)
        {
            if(!Pause2)
            notCollide1=activeBrick2->shiftDown(bin2);
            if(notCollide1){
                bin2->getImage(image2);
                activeBrick2->operator>>(image2);
            }
            else
            {
                brickInFlight1=0;
                bin2->getImage(image2);

                activeBrick2->operator>>(image2);
                bin2->setImage(image2);

                this->repaint();
                numLines1=numLines1+bin2->removeFullLines();
                bin2->getImage(image2);

            }
            this->repaint();
        }
        if(gameOver1){        //???????????????????????????
            killTimer(timerID);

            if(QMessageBox::question(this,tr("??????"),tr("?????????????????????????????????????????????"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                gameStart();
            else
                close();
        }
        }
    }
}

void Tetris::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    unsigned int width,i,j;
    unsigned int height,width1,height1;
    unsigned int height2,width2,height3,width3;

    width=bin->getWidth();  //?????????????????????????????????????????????
    height=bin->getHeight();
    width1=bin1->getWidth();//?????????????????????????????????????????????
    height1=bin1->getHeight();

    width2=bin2->getWidth();//?????????????????????????????????????????????
    height2=bin2->getHeight();
    width3=bin3->getWidth();//?????????????????????????????????????????????
    height3=bin3->getHeight();

    int nSize=20;

    QRect rect(0,0,860,500);
    QImage imageBack;
    imageBack.load(":/images/background.png");
    QBrush brushBack(imageBack);
    painter.setBrush(brushBack);
    painter.drawRect(rect);


    //???????????????????????????
    QRect re;
    //pDC->FillSolidRect(re,RGB(210,255,255));

    char buf[100];

   // sprintf(buf,"  %d  ",numLines*10);
   // pDC->TextOut(330,90,buf);
    ui->label_scorel->setText(QString::number(numLines*10));
    ui->label_scorel_2->setText(QString::number(numLines1*10));
    //???????????????????????????

    QRect  rc,rc1,rc2,rc3;
    QString str[] = {":/images/blue.png",
                       ":/images/red.png",
                       ":/images/green.png",
                       ":/images/blue.png",
                       ":/images/ya.png",
                       ":/images/yello.png"
                       ":/images/green.png",
                       ":/images/green.png"};
    int b,b1,b2;

    for(i=0;i<height1;i++)
    {
        for(j=0;j<width1;j++)
        {
            rc1=QRect(j*nSize+300,i*nSize,nSize,nSize);		//???????????????
            if(image1[i][j]!=0)
            {
                b=image1[i][j];
               // qDebug()<<str[b];
                QImage img;
                img.load(":/images/red.png");
               // img.load(str[b]);
                QBrush brush(img);
                painter.setBrush(brush);
                painter.fillRect(rc1,brush);

              //  painter.setBrush(Qt::green);
              //  painter.drawRect(rc1);
            }
        }
    }
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            rc=QRect(j*nSize+80,i*nSize,nSize,nSize);
            if(image[i][j]!=0)
            {
                b=image[i][j];    //??????????????????????????????????????????
           //     qDebug()<<str[b];
                QImage img;
                img.load(":/images/red.png");
               // img.load(str[b]);
                QBrush brush(img);
                painter.setBrush(brush);
                painter.fillRect(rc,brush);
            }
        }
    }


    /////////////////////////////////////////
    if(GameModal==2)
    {
        for(i=0;i<height3;i++)
        {
            for(j=0;j<width3;j++)
            {
                rc3=QRect(j*nSize+420,i*nSize,nSize,nSize);
                if(image3[i][j]!=0)
                {
                    b=image3[i][j];//???????????????????????????????????????
                 //   qDebug()<<str[b];
                    QImage img;
                    img.load(":/images/green.png");
                   // img.load(str[b]);
                    QBrush brush(img);
                    painter.setBrush(brush);
                    painter.fillRect(rc3,brush);
                }
            }
        }
        for(i=0;i<height2;i++)
        {
            for(j=0;j<width2;j++)
            {
                rc2=QRect(j*nSize+540,i*nSize,nSize,nSize);
                if(image2[i][j]!=0)
                {
                    b=image2[i][j];//??????????????????????????????????????????
                   // qDebug()<<str[b];
                    QImage img;
                    img.load(":/images/green.png");
                   // img.load(str[b]);
                    QBrush brush(img);
                    painter.setBrush(brush);
                    painter.fillRect(rc2,brush);
                }


            }
        }
    }
}
