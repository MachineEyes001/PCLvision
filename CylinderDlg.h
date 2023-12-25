#pragma once

#include <QWidget>
#include "ui_CylinderDlg.h"
#include <pcl/sample_consensus/method_types.h>//ģ�Ͷ���ͷ�ļ�
#include <pcl/sample_consensus/model_types.h>//����������Ʒ���ͷ�ļ�
#include <pcl/segmentation/sac_segmentation.h>//���ڲ���һ���Էָ�����ͷ�ļ�
#include <pcl/features/normal_3d.h>

class CylinderDlg : public QWidget
{
	Q_OBJECT

public:
	CylinderDlg(QWidget *parent = nullptr);
	~CylinderDlg();

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();

signals:
	void RANSAC(int model, double localpoint, int iteration, int minpointnum, double confidence, int KdTreeSearhNum=0, int ModelOPTI=0, double NormalWeight=0, int ifReverse=0, double MinR=0, double MaxR=0);

private:
	Ui::CylinderDlgClass ui;
};
