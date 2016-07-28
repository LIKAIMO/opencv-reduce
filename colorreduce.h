#ifndef COLORREDUCE_H
#define COLORREDUCE_H

#include <QWidget>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QImage>
#include <QString>

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
        RGB
    };

private:
    Ui::colorReduce *ui;

    //init camera and open
    VideoCapture *m_camera;
    //the camera is open
    bool m_openFlag;

    //save current frame
    Mat m_frame;
    Mat m_channel[3];
    Mat m_grayImage;
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

    //set threshold
    uchar m_threshold;

public:
    void initAll(void);
    void initConnect(void);
    void imageShow(Mat &t_mat, int t_type);

public slots:
    void openCamera(void);
    void getFrame(void);
    void imageReduce(void);
    void changeRed(void);
    void changeGreen(void);
    void changeBlue(void);
    void changeColorValue(void);
    void cameraClose(void);
    void reduceClose(void);
    void displayChannelRData(void);
    void displayChannelGData(void);
    void displayChannelBData(void);
};

#endif // COLORREDUCE_H
