#include "VoxelGridDlg.h"

VoxelGridDlg::VoxelGridDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

VoxelGridDlg::~VoxelGridDlg()
{}

//确认
void VoxelGridDlg::on_btn_ok_clicked()
{
	int KeepOrganized = ui.KeepOrganized->currentIndex();
	bool ok;
	//获取lineedit数据
	QString W_s = ui.W->text();
	double W_d = W_s.toDouble(&ok);
	if (W_d < 0.0)
	{
		ui.W->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.W->setStyleSheet("QLineEdit{border:1px solid black }");
	QString L_s = ui.L->text();
	double L_d = L_s.toDouble(&ok);
	if (L_d < 0.0)
	{
		ui.L->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.L->setStyleSheet("QLineEdit{border:1px solid black }");
	QString H_s = ui.H->text();
	double H_d = H_s.toDouble(&ok);
	if (H_d < 0.0)
	{
		ui.H->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.H->setStyleSheet("QLineEdit{border:1px solid black }");
	emit VoxelGrid(L_d, W_d, H_d, KeepOrganized);

}

//取消
void VoxelGridDlg::on_btn_cancel_clicked()
{
	close();
}