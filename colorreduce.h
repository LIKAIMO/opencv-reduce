#ifndef COLORREDUCE_H
#define COLORREDUCE_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/video/tracking.hpp>
#include <QDebug>
#include <QImage>
#include <QString>
#include <QTimer>

using namespace std;
using namespace cv;

namespace Ui {
class colorReduce;
}

class colorReduce : public QWidget
{
    Q_OBJECT

public:
    explicit colorReduce(QWidget *parent = 0);
    ~colorReduce();

    enum
    {
        B,
        G,
        R,
        GRAY,
        RGB,
        HSV
    };
    enum
    {
        H,
        S,
        V
    };
    //∂® ±∆˜…Ë÷√
    QTimer *m_timer;

private:
    Ui::colorReduce *ui;

    //init camera and open
    VideoCapture *m_camera;
    //the camera is open
    bool m_openFlag;

    //save current frame
    Mat m_frame;
    Mat m_imageHSV;
    Mat m_channel[3];
    Mat m_grayImage;
    //direction check
    Mat m_prevGrayImage;
    Mat m_cflow;
    Mat m_flow;
    bool m_frameFlag;

    //color RGB value set
    bool m_redFlag;
    bool m_greenFlag;
    bool m_blueFlag;
    uchar m_redValue;
    uchar m_greenValue;
    uchar m_blueValue;

    //reduce image
    bool m_reduceFlag;
    bool m_redChannelFlag;
    bool m_greenChannelFlag;
    bool m_blueChannelFlag;

    //set threshold
    uchar m_threshold;

    //set current color space
    bool m_isRGB; //true is RGB, false is HSV

    //HSV range
    uchar m_hue;//range: (m_hue - m_threshold)~(m_hue + m_threshold)
    uchar m_saturation; //range: m_saturation~255
    uchar m_value;//range: m_value~255

    //video stream flag
    bool m_checkDirFlag;

public:
    void initAll(void);
    void initConnect(void);
    void imageReduce(Mat &t_mat, int t_type);
    void imageShow(Mat &t_mat, int t_type);
    void redEvent(void);
    void greenEvent(void);
    void blueEvent(void);
    void rgbEvent(void);
    void get_keyPoint(void);
    void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,
                        double, const Scalar& color);
public slots:
    void openCamera(void);
    void getFrame(void);
    void playVideo(void);
    void timerReduce(void);
    void closeVideo(void);
    void changeRed(void);
    void changeGreen(void);
    void changeBlue(void);
    void changeColorValue(void);
    void cameraClose(void);
    void reduceClose(void);
    void displayChannelRData(void);
    void displayChannelGData(void);
    void displayChannelBData(void);
    void displayRGBImage(void);
    void colorSpaceRGB(void);
    void colorSpaceHSV(void);
    void dirCheck(void);
};

#endif // COLORREDUCE_H
