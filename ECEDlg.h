#pragma once

#include <QWidget>
#include "ui_ECEDlg.h"
#include <pcl/filters/extract_indices.h>
#include <pcl/segmentation/extract_clusters.h>

class ECEDlg : public QWidget
{
	Q_OBJECT

public:
	ECEDlg(QWidget *parent = nullptr);
	~ECEDlg();

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();

signals:
	void ECE(double ClusterTolerance, int MinClusterSize, int MaxClusterSize);

private:
	Ui::ECEDlgClass ui;
};
