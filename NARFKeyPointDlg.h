#pragma once

#include <QWidget>
#include "ui_NARFKeyPointDlg.h"
#include <pcl/features/range_image_border_extractor.h>
#include <pcl/keypoints/narf_keypoint.h>

class NARFKeyPointDlg : public QWidget
{
	Q_OBJECT

public:
	NARFKeyPointDlg(QWidget *parent = nullptr);
	~NARFKeyPointDlg();

	QString _far_ranges_pcd_path = "";

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();

	// ���
	void on_Browse_clicked();

signals:
	void NARFKeyPoint(double angularResolution, double maxAngleWidth, double maxAngleHeight, double Support_size, int Add_points_on_straight_edges, double Distance_for_additional_points, int CAMERAorLASER, int UnseenSet, double noise_level, double min_range, int border_size);

private:
	Ui::NARFKeyPointDlgClass ui;
};
