#pragma once

#include <QWidget>
#include "ui_NDTDlg.h"
//ʹ�á���̬�ֲ��任�㷨���͡������������ݵĹ���������Ӧ��ͷ�ļ�������������������������������滻, 
//����ʹ���������������(approximate voxel filter)�������Ϻ�
#include <pcl/registration/ndt.h> // ndt��׼ͷ�ļ�
#include <pcl/filters/approximate_voxel_grid.h>

class NDTDlg : public QWidget
{
	Q_OBJECT

public:
	NDTDlg(QWidget *parent = nullptr);
	~NDTDlg();

	QString pathtarget = "";
	QString pathscene = "";

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();
	// ���
	void on_BrowseTarget_clicked();
	// ���
	void on_BrowseScene_clicked();

signals:
	void NDT(double TransformationEpsilon, double StepSize, double Resolution, int MaximumIterations, int Axis, double AngleRotation, double X, double Y, double Z, double leaf);


private:
	Ui::NDTDlgClass ui;
};
