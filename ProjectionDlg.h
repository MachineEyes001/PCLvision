#pragma once

#include <QWidget>
#include "ui_ProjectionDlg.h"
#include <pcl/ModelCoefficients.h>             //ģ��ϵ��ͷ�ļ�
#include <pcl/filters/project_inliers.h>       //ͶӰ�˲���ͷ�ļ�

class ProjectionDlg : public QWidget
{
	Q_OBJECT

public:
	ProjectionDlg(QWidget *parent = nullptr);
	~ProjectionDlg();

private slots:
	//ȷ��
	void on_btn_ok_clicked();

	//ȡ��
	void on_btn_cancel_clicked();

signals:
	void Projection(int plane,int KeepOrganized);

private:
	Ui::ProjectionDlgClass ui;
};
