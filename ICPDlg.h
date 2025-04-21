#pragma once

#include <QWidget>
#include "ui_ICPDlg.h"
#include <pcl/common/time.h>
#include <Eigen/Core>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/features/fpfh_omp.h>
#include <pcl/registration/icp.h>
#include <pcl/registration/sample_consensus_prerejective.h>

typedef pcl::PointNormal PointNT;
typedef pcl::PointCloud<PointNT> PointCloudNT;
typedef pcl::FPFHSignature33 FeatureT;
typedef pcl::FPFHEstimationOMP<PointNT, PointNT, FeatureT> FeatureEstimationT;
typedef pcl::PointCloud<FeatureT> FeatureCloudT;

class ICPDlg : public QWidget
{
	Q_OBJECT

public:
	ICPDlg(QWidget *parent = nullptr);
	~ICPDlg();

	QString pathtarget = "";
	QString pathscene = "";

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();
	// 浏览
	void on_BrowseTarget_clicked();
	// 浏览
	void on_BrowseScene_clicked();

signals:
	void ICP(double leaf, double NormalRadius, double FeatureRadius, int RansacNum, int NumberOfSamples, int CorrespondenceRandomness, double SimilarityThreshold, double MaxCorrespondenceDistance, double InlierFraction);


private:
	Ui::ICPDlgClass ui;
};
