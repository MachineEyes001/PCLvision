#pragma once

#include <QWidget>
#include "ui_PlaneDlg.h"
#include <pcl/filters/extract_indices.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <pcl/sample_consensus/sac_model_line.h>
#include <pcl/sample_consensus/sac_model_circle.h>
#include <pcl/sample_consensus/sac_model_circle3d.h>
#include <pcl/sample_consensus/sac_model_sphere.h>
#include <pcl/sample_consensus/sac_model_cylinder.h>
#include <pcl/sample_consensus/sac_model_cone.h>
#include <pcl/sample_consensus/sac_model_parallel_line.h>
#include <pcl/sample_consensus/sac_model_perpendicular_plane.h>
#include <pcl/sample_consensus/sac_model_normal_plane.h>
#include <pcl/sample_consensus/sac_model_normal_sphere.h>
#include <pcl/sample_consensus/sac_model_registration.h>
#include <pcl/sample_consensus/sac_model_registration_2d.h>
#include <pcl/sample_consensus/sac_model_parallel_plane.h>
#include <pcl/sample_consensus/sac_model_normal_parallel_plane.h>
#include <pcl/sample_consensus/sac_model_stick.h>
#include <pcl/sample_consensus/sac_model_ellipse3d.h>

class PlaneDlg : public QWidget
{
	Q_OBJECT

public:
	PlaneDlg(QWidget *parent = nullptr);
	~PlaneDlg();

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

signals:
	void RANSAC(int model, double localpoint, int iteration, int minpointnum , double confidence, int KdTreeSearhNum = 0, int ModelOPTI = 0, double NormalWeight = 0, int ifReverse = 0, double MinR = 0, double MaxR = 0);

private:
	Ui::PlaneDlgClass ui;
};
