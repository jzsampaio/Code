
#include "widget.h"
#include "ui_widget.h"

void Widget::process()
{

    VideoCapture *video = videoInput;

    if(!video->isOpened())
        {
        reportBad("video não está pronto!");
        numberOfFailure++;
        if(numberOfFailure > 10){
            reportBad("Numero de falhas excedido! \n O programa sera pausado");
            clock->stop();
            }
        return;
        }
    *video >> currentFrame;

    if(currentFrame.empty())
        {
        report("frame nao pode ser capturado!");
        pauseVideo();
        return;
        }

    Cv2QtImage::setLabelImage(ui->InputImg, currentFrame);

    Mat frame;
    Mat outputFrame(currentFrame.rows,
                    currentFrame.cols,
                    CV_8UC3);
    currentFrame.copyTo(frame);

    //------------------------------------------------
    //-----------BORRANDO IMAGEM----------------------
    //------------------------------------------------

    if(CONTROL_FILTER_GAUSSIAN){
        medianBlur ( frame, frame, SizeGaussFilter);
        }
    frame.copyTo(outputFrame);
    //------------------------------------------------
    //-----------DETECÇÃO DE OBJETOS POR COR----------
    //------------------------------------------------
    if(!targets.empty())
        {
        //acha centro dos objetos
        vector<Point> centers =
            this->findTargets(frame);
       //atualiza os filtros de kalman
        vector<Point> kalmanCenters = this->findKalmanCenters(centers);

        drawDetectionResult(outputFrame, centers);
        drawKalmanResult(outputFrame, kalmanCenters);

        if(CONTROL_SEE_FUTURE){
            vector<vector<Point> > future = this->predictFuture(30);
            drawFuturePrediction(outputFrame, future);
            }
        if(CONTROL_SEE_PAST){
            addToPastHistory(kalmanCenters);
            drawPastHistory(outputFrame);
        }

   }//end if(!targents.empty())

    Cv2QtImage::setLabelImage(ui->OutputImg, outputFrame);
}
