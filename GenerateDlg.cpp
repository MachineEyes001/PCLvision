#include "GenerateDlg.h"
#include <QFileDialog>

GenerateDlg::GenerateDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

GenerateDlg::~GenerateDlg()
{}

//确认
void GenerateDlg::on_btn_ok_clicked()
{
	int FileType = ui.FileType->currentIndex();
	bool ok;
	//获取lineedit数据
	QString NumberSamples_s = ui.NumberSamples->text();
	default_number_samples = NumberSamples_s.toInt(&ok);
	if (default_number_samples <= 0)
	{
		ui.NumberSamples->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.NumberSamples->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString LeafSize_s = ui.LeafSize->text();
	default_leaf_size = LeafSize_s.toDouble(&ok);
	if (default_leaf_size < 0.0)
	{
		ui.LeafSize->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.LeafSize->setStyleSheet("QLineEdit{border:1px solid black }");

	emit Generate(FileType);
}

//取消
void GenerateDlg::on_btn_cancel_clicked()
{
	close();
}

// 浏览
void GenerateDlg::on_Browse_clicked()
{
	//获取点云路径
	pathModel = QFileDialog::getOpenFileName(this, "选择模型文件", ".//", "文件(*.ply *.obj);;所有文件(*.*)");
	ui.PathModel->setText(pathModel);
}
