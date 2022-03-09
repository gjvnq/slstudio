/*
 *
 SLStudio - Platform for Real-Time  Structured Light
 (c) 2013 -- 2014 Jakob Wilm, DTU, Kgs.Lyngby, Denmark
 *
*/

#ifndef SLPOSEWIDGET_H
#define SLPOSEWIDGET_H

#ifndef Q_MOC_RUN
    #include <QVTKRenderWidget.h>
    #include <pcl/visualization/pcl_visualizer.h>
    #include <pcl/geometry/planar_polygon.h>
    #include <Eigen/Eigen>
#endif

class SLPoseWidget : public QVTKRenderWidget {
    Q_OBJECT
    public:
        explicit SLPoseWidget(QWidget *parent = 0);
        ~SLPoseWidget();
    signals:

    public slots:
        void showPoseEstimate(Eigen::Affine3f T);

    private:
        pcl::visualization::PCLVisualizer *visualizer;

};

#endif // SLPOSEWIDGET_H
