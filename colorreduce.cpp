#include "colorreduce.h"
#include "ui_colorreduce.h"

/* HSV some color H value range
 * Orange  0-22
 * Yellow 22- 38
 * Green 38-75
 * Blue 75-130
 * Violet 130-160
 * Red 160-179
 *
 */

colorReduce::colorReduce(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::colorReduce),m_openFlag(false),m_frameFlag(false),m_redFlag(true),
    m_greenFlag(false),m_blueFlag(false),m_redValue(0),m_greenValue(0),
    m_blueValue(0),m_reduceFlag(false),m_redChannelFlag(false),m_greenChannelFlag(false),
    m_blueChannelFlag(false),m_threshold(5),m_isRGB(true),m_checkDirFlag(false)
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
    m_prevGrayImage = m_grayImage.clone();
    ui->m_RGBButton->setStyleSheet("background-color: rgb(255,255,0);");
    ui->m_HSVButton->setStyleSheet("background-color: rgb(255,255,255);");
    m_timer = new QTimer();
    initConnect();
}

void colorReduce::initConnect()
{
    connect(ui->m_openVedio,SIGNAL(clicked(bool)),this,SLOT(openCamera()));
    connect(ui->m_getFrame,SIGNAL(clicked(bool)),this,SLOT(getFrame()));
    connect(ui->m_reduce,SIGNAL(clicked(bool)),this,SLOT(displayRGBImage()));
    connect(ui->m_red,SIGNAL(clicked(bool)),this,SLOT(changeRed()));
    connect(ui->m_green,SIGNAL(clicked(bool)),this,SLOT(changeGreen()));
    connect(ui->m_blue,SIGNAL(clicked(bool)),this,SLOT(changeBlue()));
    connect(ui->m_colorValue,SIGNAL(valueChanged(int)),this,SLOT(changeColorValue()));
    connect(ui->m_closeCamera,SIGNAL(clicked(bool)),this,SLOT(cameraClose()));
    connect(ui->m_reduceClose,SIGNAL(clicked(bool)),this,SLOT(reduceClose()));
    connect(ui->m_channelR,SIGNAL(clicked(bool)),this,SLOT(displayChannelRData()));
    connect(ui->m_channelG,SIGNAL(clicked(bool)),this,SLOT(displayChannelGData()));
    connect(ui->m_channelB,SIGNAL(clicked(bool)),this,SLOT(displayChannelBData()));
    connect(ui->m_RGBButton,SIGNAL(clicked(bool)),this,SLOT(colorSpaceRGB()));
    connect(ui->m_HSVButton,SIGNAL(clicked(bool)),this,SLOT(colorSpaceHSV()));
    connect(ui->m_playVideo,SIGNAL(clicked(bool)),this,SLOT(playVideo()));
    connect(ui->m_closeVideo,SIGNAL(clicked(bool)),this,SLOT(closeVideo()));
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timerReduce()));
    connect(ui->m_dir,SIGNAL(clicked(bool)),this,SLOT(dirCheck()));
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

void colorReduce::redEvent()
{
    if(!m_redFlag)
    {
        m_redFlag = true;
        m_greenFlag = false;
        m_blueFlag = false;
        if(m_isRGB)
        {
            m_redValue = ui->m_redValueText->text().toInt();
            ui->m_colorValue->setValue(m_redValue);
        }
        else
        {
            m_value = ui->m_redValueText->text().toInt();
            ui->m_colorValue->setValue(m_value);
        }
        ui->m_red->setStyleSheet("background-color: rgb(255,0,0);");
        ui->m_green->setStyleSheet("background-color: rgb(255,255,255);");
        ui->m_blue->setStyleSheet("background-color: rgb(255,255,255);");
    }
}

void colorReduce::greenEvent()
{
    if(!m_greenFlag)
    {
        m_greenFlag = true;
        m_redFlag = false;
        m_blueFlag = false;
        if(m_isRGB)
        {
            m_greenValue = ui->m_greenValueText->text().toInt();
            ui->m_colorValue->setValue(m_greenValue);
        }
        else
        {
            m_saturation = ui->m_greenValueText->text().toInt();
            ui->m_colorValue->setValue(m_saturation);
        }
        ui->m_red->setStyleSheet("background-color: rgb(255,255,255);");
        ui->m_green->setStyleSheet("background-color: rgb(0,255,0);");
        ui->m_blue->setStyleSheet("background-color: rgb(255,255,255);");
    }
}

void colorReduce::blueEvent()
{
    if(!m_blueFlag)
    {
        m_blueFlag = true;
        m_redFlag = false;
        m_greenFlag = false;
        if(m_isRGB)
        {
            m_blueValue = ui->m_blueValueText->text().toInt();
            ui->m_colorValue->setValue(m_blueValue);
        }
        else
        {
            m_hue = ui->m_blueValueText->text().toInt();
            ui->m_colorValue->setValue(m_hue);
        }
        ui->m_red->setStyleSheet("background-color: rgb(255,255,255);");
        ui->m_green->setStyleSheet("background-color: rgb(255,255,255);");
        ui->m_blue->setStyleSheet("background-color: rgb(0,0,255);");
    }
}

void colorReduce::rgbEvent()
{
    if(!m_reduceFlag)
    {
        m_reduceFlag = true;
        m_redChannelFlag = false;
        m_greenChannelFlag = false;
        m_blueChannelFlag = false;
        imageReduce(m_frame, RGB);
    }
}

void colorReduce::drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                                 double, const Scalar& color)
{
    for(int y = 0; y < cflowmap.rows; y += step)
    {
        for(int x = 0; x < cflowmap.cols; x += step)
        {
            const Point2f& fxy = flow.at<Point2f>(y, x);
            line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),
                 color);
            circle(cflowmap, Point(x,y), 2, color, -1);
        }
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
            if(!m_isRGB)
            {
                //                cvtColor(m_frame,m_imageHSV,COLOR_BGR2HSV);
                //                split(m_imageHSV,m_channel);
                cvtColor(m_frame,m_frame,COLOR_BGR2HSV);
                split(m_frame,m_channel);
            }
            else
            {
                split(m_frame,m_channel);

            }
            m_frameFlag = true;
            imageShow(m_frame,RGB);
            m_reduceFlag = false;
            m_redChannelFlag = false;
            m_greenChannelFlag = false;
            m_blueChannelFlag = false;
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

void colorReduce::playVideo()
{
    if(m_openFlag)
    {
        m_timer->start(30);
    }
    else
    {
        qDebug() << "the camera is not open" << endl;
    }
}

void colorReduce::timerReduce()
{
    if(m_openFlag)
    {
        if(m_camera->read(m_frame))
        {
            if(m_checkDirFlag)
            {
                cvtColor(m_frame,m_grayImage,CV_BGR2GRAY);
                calcOpticalFlowFarneback(m_prevGrayImage, m_grayImage, m_flow, 0.5, 3, 15, 3, 5, 1.2, 0);
                cvtColor(m_prevGrayImage, m_cflow, CV_GRAY2BGR);
                drawOptFlowMap(m_flow, m_cflow, 16, 1.5, Scalar(0, 255, 0));
                swap(m_prevGrayImage,m_grayImage);
                imageShow(m_cflow,RGB);
            }
            else
            {
                imageShow(m_frame,RGB);
            }
        }

    }
    else
    {
        m_timer->stop();
        m_checkDirFlag = false;
    }
}

void colorReduce::closeVideo()
{
    m_timer->stop();
    m_checkDirFlag = false;
}

void colorReduce::imageReduce(Mat &t_mat, int t_type)
{
    if(m_frameFlag)
    {
        if(m_isRGB)
        {
            bool t_bThreshold;
            for(int i = 0; i < t_mat.rows; i++)
            {
                for(int j = 0; j < t_mat.cols; j++)
                {
                    if(t_type == RGB)
                    {
                        t_bThreshold = ((abs(t_mat.at<Vec3b>(i,j)[R] - m_redValue) <= m_threshold)
                                        && (abs(t_mat.at<Vec3b>(i,j)[G] - m_greenValue) <= m_threshold)
                                        && (abs(t_mat.at<Vec3b>(i,j)[B] - m_blueValue) <= m_threshold));
                    }
                    else if(t_type == GRAY)
                    {
                        uchar t_value;
                        if(m_redChannelFlag)
                        {
                            t_value = m_redValue;
                        }
                        else if(m_greenChannelFlag)
                        {
                            t_value = m_greenValue;
                        }
                        else
                        {
                            t_value = m_blueValue;
                        }
                        t_bThreshold = (abs(t_mat.at<uchar>(i,j) - t_value) <= m_threshold);
                    }
                    if(t_bThreshold)
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
            //            int iLowH,iHighH;
            //            iLowH = m_hue-m_threshold;
            //            if(iLowH < 0)
            //            {
            //                iLowH = 0;
            //            }
            //            iHighH = m_hue+m_threshold;
            //            if(iLowH > 179)
            //            {
            //                iLowH = 179;
            //            }

            //            int iLowS,iHighS = 255;
            //            iLowS = m_saturation;

            //            int iLowV,iHighV = 255;
            //            iLowV = m_value;

            int iLowH = 100;
            int iHighH = 140;

            int iLowS = 90;
            int iHighS = 255;

            int iLowV = 90;
            int iHighV = 255;

            Mat t_imgThresholded;
            equalizeHist(m_channel[2],m_channel[2]);
            merge(m_channel,3,m_imageHSV);

            inRange(m_imageHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), t_imgThresholded);

            //开闭操作，去除噪声，增加连通域，若在意处理速度，可以只用开操作
            Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
            morphologyEx(t_imgThresholded, t_imgThresholded, MORPH_OPEN, element);
            morphologyEx(t_imgThresholded, t_imgThresholded, MORPH_CLOSE, element);

            imageShow(t_imgThresholded,GRAY);
        }
    }
    else
    {
        qDebug() << "no image" << endl;
    }
}

void colorReduce::changeRed()
{
    redEvent();
}

void colorReduce::changeGreen()
{
    greenEvent();
}

void colorReduce::changeBlue()
{
    blueEvent();
}

void colorReduce::changeColorValue()
{
    if(m_threshold != ui->m_thresholdText->text().toInt())
    {
        m_threshold = ui->m_thresholdText->text().toInt();
    }
    if(m_redFlag)
    {
        m_redValue = m_value = ui->m_colorValue->value();
        if(m_redValue > 255)
        {
            m_redValue = 255;
            m_value = 255;
        }
        ui->m_redValueText->setText(QString::number(m_redValue));
    }
    else if(m_greenFlag)
    {
        m_greenValue = m_saturation = ui->m_colorValue->value();
        if(m_greenValue > 255)
        {
            m_greenValue = 255;
            m_saturation = 255;
        }
        ui->m_greenValueText->setText(QString::number(m_greenValue));
    }
    else if(m_blueFlag)
    {
        m_blueValue = ui->m_colorValue->value();
        if(m_isRGB)
        {
            ui->m_colorValue->setMaximum(255);
            if(m_blueValue > 255)
            {
                m_blueValue = 255;
            }
        }
        else
        {
            ui->m_colorValue->setMaximum(179);
            if(m_blueValue > 179)
            {
                m_blueValue = 179;
            }
        }

        ui->m_blueValueText->setText(QString::number(m_blueValue));
    }
    if(m_reduceFlag)
    {
        imageReduce(m_frame,RGB);
    }
    else
    {
        if(m_redChannelFlag)
        {
            imageReduce(m_channel[R],GRAY);
        }
        else if(m_greenChannelFlag)
        {
            imageReduce(m_channel[G],GRAY);
        }
        else if(m_blueChannelFlag)
        {
            imageReduce(m_channel[B],GRAY);
        }
    }
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
    if(m_reduceFlag || m_redChannelFlag || m_greenChannelFlag || m_blueChannelFlag)
    {
        imageShow(m_frame,RGB);
        m_reduceFlag = false;
        m_redChannelFlag = false;
        m_greenChannelFlag = false;
        m_blueChannelFlag = false;
    }
    else
    {
        qDebug() << "no image reducing" << endl;
    }
}

void colorReduce::displayChannelRData()
{
    if(!m_redChannelFlag)
    {
        m_redChannelFlag = true;
        m_greenChannelFlag = false;
        m_blueChannelFlag = false;
        m_reduceFlag = false;
        redEvent();
        imageReduce(m_channel[R],GRAY);
    }
}

void colorReduce::displayChannelGData()
{

    if(!m_greenChannelFlag)
    {
        m_redChannelFlag = false;
        m_greenChannelFlag = true;
        m_blueChannelFlag = false;
        m_reduceFlag = false;
        greenEvent();
        imageReduce(m_channel[G],GRAY);
    }
}

void colorReduce::displayChannelBData()
{
    if(!m_blueChannelFlag)
    {
        m_redChannelFlag = false;
        m_greenChannelFlag = false;
        m_blueChannelFlag = true;
        m_reduceFlag = false;
        blueEvent();
        imageReduce(m_channel[B],GRAY);
    }
}

void colorReduce::displayRGBImage()
{
    rgbEvent();
}

void colorReduce::colorSpaceRGB()
{
    if(!m_isRGB)
    {
        m_isRGB = true;
        ui->m_RGBButton->setStyleSheet("background-color: rgb(255,255,0);");
        ui->m_HSVButton->setStyleSheet("background-color: rgb(255,255,255);");
        m_redValue = 0;
        m_greenValue = 0;
        m_blueValue = 0;
        m_hue = 0;
        m_saturation = 0;
        m_value = 0;
        ui->m_redValueText->setText("0");
        ui->m_greenValueText->setText("0");
        ui->m_blueValueText->setText("0");
        ui->m_colorValue->setValue(0);
    }
}

void colorReduce::colorSpaceHSV()
{
    if(m_isRGB)
    {
        m_isRGB = false;
        ui->m_RGBButton->setStyleSheet("background-color: rgb(255,255,255);");
        ui->m_HSVButton->setStyleSheet("background-color: rgb(255,255,0);");
        m_redValue = 0;
        m_greenValue = 0;
        m_blueValue = 0;
        m_hue = 0;
        m_saturation = 0;
        m_value = 0;
        ui->m_redValueText->setText("0");
        ui->m_greenValueText->setText("0");
        ui->m_blueValueText->setText("0");
        ui->m_colorValue->setValue(0);
    }
}

void colorReduce::dirCheck()
{
    if(m_openFlag)
    {
        m_checkDirFlag = true;
    }
}

void get_keyPoint(void)
{
    vector<KeyPoint> keypoints;
    Ptr<FeatureDetector> detector = FeatureDetector::create( "FAST" );
}
