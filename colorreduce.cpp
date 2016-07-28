#include "colorreduce.h"
#include "ui_colorreduce.h"

colorReduce::colorReduce(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::colorReduce),m_openFlag(false),m_frameFlag(false),m_redFlag(true),
    m_greenFlag(false),m_blueFlag(false),m_redValue(0),m_greenValue(0),
    m_blueValue(0),m_reduceFlag(false),m_threshold(5)
{
    ui->setupUi(this);
    initAll();
}

colorReduce::~colorReduce()
{
    if(m_openFlag)
    {
        m_camera->release();
    }
    delete ui;
}

void colorReduce::initAll()
{
    initConnect();
    ui->m_redValueText->setText(QString::number(m_redValue));
    ui->m_greenValueText->setText(QString::number(m_greenValue));
    ui->m_blueValueText->setText(QString::number(m_blueValue));
    ui->m_colorValue->setMinimum(0);
    ui->m_colorValue->setMaximum(255);
    ui->m_thresholdText->setText(QString::number(m_threshold));
    ui->m_red->setStyleSheet("background-color: rgb(255,0,0);");
    ui->m_green->setStyleSheet("background-color: rgb(255,255,255);");
    ui->m_blue->setStyleSheet("background-color: rgb(255,255,255);");
    m_grayImage = Mat::zeros(480,640,CV_8U);
}

void colorReduce::initConnect()
{
    connect(ui->m_openVedio,SIGNAL(clicked(bool)),this,SLOT(openCamera()));
    connect(ui->m_getFrame,SIGNAL(clicked(bool)),this,SLOT(getFrame()));
    connect(ui->m_reduce,SIGNAL(clicked(bool)),this,SLOT(imageReduce()));
    connect(ui->m_red,SIGNAL(clicked(bool)),this,SLOT(changeRed()));
    connect(ui->m_green,SIGNAL(clicked(bool)),this,SLOT(changeGreen()));
    connect(ui->m_blue,SIGNAL(clicked(bool)),this,SLOT(changeBlue()));
    connect(ui->m_colorValue,SIGNAL(valueChanged(int)),this,SLOT(changeColorValue()));
    connect(ui->m_closeCamera,SIGNAL(clicked(bool)),this,SLOT(cameraClose()));
    connect(ui->m_reduceClose,SIGNAL(clicked(bool)),this,SLOT(reduceClose()));
    connect(ui->m_channelR,SIGNAL(clicked(bool)),this,SLOT(displayChannelRData()));
    connect(ui->m_channelG,SIGNAL(clicked(bool)),this,SLOT(displayChannelGData()));
    connect(ui->m_channelB,SIGNAL(clicked(bool)),this,SLOT(displayChannelBData()));
}

void colorReduce::imageShow(Mat &t_mat, int t_type)
{
    if(RGB == t_type)
    {
        Mat t_mated;
        cvtColor(t_mat, t_mated, CV_BGR2RGB);
        QImage t_labelImage = QImage(t_mated.data, t_mated.cols, t_mated.rows,
                                     t_mated.cols * t_mated.channels(), QImage::Format_RGB888);
        ui->m_imageShow->setPixmap(QPixmap::fromImage(t_labelImage));
    }
    else if(GRAY == t_type)
    {
        QImage t_labelImage = QImage(t_mat.data, t_mat.cols, t_mat.rows,QImage::Format_Grayscale8);
        ui->m_imageShow->setPixmap(QPixmap::fromImage(t_labelImage));
    }
}

void colorReduce::openCamera()
{

    m_camera = new VideoCapture(0);
    if(m_camera->isOpened())
    {
        m_openFlag = true;
        qDebug() << "open camera success" << endl;
    }
    else
    {
        m_openFlag = false;
        qDebug() << "open camera fail" << endl;
    }
}

void colorReduce::getFrame()
{
    if(m_openFlag)
    {
        if(m_camera->read(m_frame))
        {
            m_frameFlag = true;
            split(m_frame,m_channel);
            imageShow(m_frame,RGB);
        }
        else
        {
            m_frameFlag = false;
            qDebug() << "get frame fail" << endl;
        }
    }
    else
    {
        qDebug() << "the camera is not open" << endl;
    }
}

void colorReduce::imageReduce()
{
    if(m_frameFlag)
    {
        if(!m_reduceFlag)
        {
            m_reduceFlag = true;
        }

        for(int i = 0; i < m_frame.rows; i++)
        {
            for(int j = 0; j < m_frame.cols; j++)
            {
                if((abs(m_frame.at<Vec3b>(i,j)[R] - m_redValue) <= m_threshold)
                    && (abs(m_frame.at<Vec3b>(i,j)[G] - m_greenValue) <= m_threshold)
                    && (abs(m_frame.at<Vec3b>(i,j)[B] - m_blueValue) <= m_threshold))
                {
                    m_grayImage.at<uchar>(i,j) = 255;
                }
                else
                {
                    m_grayImage.at<uchar>(i,j) = 0;
                }
            }
        }


        //降噪
        blur(m_grayImage,m_grayImage,Size(3,3));
        //二值化处理
        //threshold(m_grayImage, m_grayImage, m_redValue, 255, THRESH_BINARY);
        //显示处理后的结果
        imageShow(m_grayImage, GRAY);
    }
    else
    {
        qDebug() << "no image" << endl;
    }
}

void colorReduce::changeRed()
{
    if(!m_redFlag)
    {
        m_redFlag = true;
        m_greenFlag = false;
        m_blueFlag = false;
        m_redValue = ui->m_redValueText->text().toInt();
        ui->m_colorValue->setValue(m_redValue);
        ui->m_red->setStyleSheet("background-color: rgb(255,0,0);");
        ui->m_green->setStyleSheet("background-color: rgb(255,255,255);");
        ui->m_blue->setStyleSheet("background-color: rgb(255,255,255);");
    }
}

void colorReduce::changeGreen()
{
    if(!m_greenFlag)
    {
        m_greenFlag = true;
        m_redFlag = false;
        m_blueFlag = false;
        m_greenValue = ui->m_greenValueText->text().toInt();
        ui->m_colorValue->setValue(m_greenValue);
        ui->m_red->setStyleSheet("background-color: rgb(255,255,255);");
        ui->m_green->setStyleSheet("background-color: rgb(0,255,0);");
        ui->m_blue->setStyleSheet("background-color: rgb(255,255,255);");
    }
}

void colorReduce::changeBlue()
{
    if(!m_blueFlag)
    {
        m_blueFlag = true;
        m_redFlag = false;
        m_greenFlag = false;
        m_blueValue = ui->m_blueValueText->text().toInt();
        ui->m_colorValue->setValue(m_blueValue);
        ui->m_red->setStyleSheet("background-color: rgb(255,255,255);");
        ui->m_green->setStyleSheet("background-color: rgb(255,255,255);");
        ui->m_blue->setStyleSheet("background-color: rgb(0,0,255);");
    }
}

void colorReduce::changeColorValue()
{
    if(m_threshold != ui->m_thresholdText->text().toInt())
    {
        m_threshold = ui->m_thresholdText->text().toInt();
    }
    if(m_redFlag)
    {
        m_redValue = ui->m_colorValue->value();
        ui->m_redValueText->setText(QString::number(m_redValue));
    }
    else if(m_greenFlag)
    {
        m_greenValue = ui->m_colorValue->value();
        ui->m_greenValueText->setText(QString::number(m_greenValue));
    }
    else if(m_blueFlag)
    {
        m_blueValue = ui->m_colorValue->value();
        ui->m_blueValueText->setText(QString::number(m_blueValue));
    }
    imageReduce();
}

void colorReduce::cameraClose()
{
    if(m_openFlag)
    {
        m_openFlag = false;
        m_camera->release();
         qDebug() << "close success" << endl;
    }
    else
    {
        qDebug() << "the camera is closed" << endl;
    }
}

void colorReduce::reduceClose()
{
    if(m_reduceFlag)
    {
        imageShow(m_frame,RGB);
        m_reduceFlag = false;
    }
}

void colorReduce::displayChannelRData()
{
    imageShow(m_channel[R],GRAY);
//    ui->m_msg->clear();
//    QString t_str = "";
//    for(int i = 0; i < m_channel[R].rows; i++)
//    {
//        for(int j = 0; j < m_channel[R].cols; j++)
//        {
//            t_str += QString::number(m_channel[R].at<uchar>(i,j)) + " ";
//        }
//    }
//    ui->m_msg->setText(t_str);
}

void colorReduce::displayChannelGData()
{
    imageShow(m_channel[G],GRAY);
//    ui->m_msg->clear();
//    QString t_str = "";
//    for(int i = 0; i < m_channel[G].rows; i++)
//    {
//        for(int j = 0; j < m_channel[G].cols; j++)
//        {
//            t_str += QString::number(m_channel[G].at<uchar>(i,j)) + " ";
//        }
//    }
//    ui->m_msg->setText(t_str);
}

void colorReduce::displayChannelBData()
{
    imageShow(m_channel[B],GRAY);
//    ui->m_msg->clear();
//    QString t_str = "";
//    for(int i = 0; i < m_channel[B].rows; i++)
//    {
//        for(int j = 0; j < m_channel[B].cols; j++)
//        {
//            t_str += QString::number(m_channel[B].at<uchar>(i,j)) + " ";
//        }
//    }
//    ui->m_msg->setText(t_str);
}
