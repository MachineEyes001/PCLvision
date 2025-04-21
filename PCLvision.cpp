#include "PCLvision.h"
#include <QFile>
#include <QMessageBox>
#include <QColorDialog>
#include <pcl/io/pcd_io.h>

PCLvision::PCLvision(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	// 读取上一次保存的值
	QSettings settings("MyCPvisionsetting", QSettings::IniFormat);
	QString zedpathValue = settings.value("zedpathValue").toString();
	if (!zedpathValue.isEmpty())
	{
		zedpath = zedpathValue;
	}
	QString meckpathValue = settings.value("meckpathValue").toString();
	if (!meckpathValue.isEmpty())
	{
		meckpath = meckpathValue;
	}

    //初始化
    init();
}
PCLvision::~PCLvision()
{
	// 保存当前输入的值
	QSettings settings("MyCPvisionsetting", QSettings::IniFormat);
	settings.setValue("zedpathValue", zedpath);
	settings.setValue("meckpathValue", meckpath);
}
void PCLvision::closeEvent(QCloseEvent* event)
{
	heightRampDlg.close();
	passThroughDlg.close();
	voxelGridDlg.close();
	removeOutliersDlg.close();
	radiusFilterDlg.close();
	conditionaldlg.close();
	uniformsamplingdlg.close();
	upsamplingdlg.close();
	projectionDlg.close();
	depthmapDlg.close();
	NARFkeypointdlg.close();
	planedlg.close();
	linedlg.close();
	spheredlg.close();
	circle2ddlg.close();
	circle3ddlg.close();
	cylinderdlg.close();
	conedlg.close();
	ecedlg.close();
	regiongrowdlg.close();
	normal3ddlg.close();
	integralimagedlg.close();
	ndtdlg.close();
	icpdlg.close();
	polygondlg.close();
	gp3dlg.close();
	box3ddlg.close();
	templatematchingdlg.close();
	generatedlg.close();
	depth2clouddlg.close();
}
//点选--------------回调函数
void PCLvision::pickPointCallback(const pcl::visualization::PointPickingEvent& event, void* viewer_void)  //点选
{
	if (event.getPointIndex() == -1)
		return;
	//获取选中点的坐标
	pcl::PointXYZ current_point;
	event.getPoint(current_point.x, current_point.y, current_point.z);
	Click3DPoint = current_point;//获取选中的目标点用于处理
	// 高亮显示选中点
	pcl::PointXYZRGB selected_point;
	selected_point.x = current_point.x;
	selected_point.y = current_point.y;
	selected_point.z = current_point.z;
	selected_point.r = 255;
	selected_point.g = 0;
	selected_point.b = 0;
	viewer->removeShape("selected_point");
	viewer->addSphere(selected_point, 0.05, 255, 0, 0, "selected_point");
	//viewer->spinOnce();
	ui.qvtkWidget->update();

	text.clear();
	ui.infoTextEdit->clear();
	text = QString("坐标(%1,%2,%3) \n").arg(current_point.x).arg(current_point.y).arg(current_point.z);
	text += "\n";

	ui.infoTextEdit->appendPlainText(text);
}
//静态成员函数作为回调函数
static void staticPickPointCallback(const pcl::visualization::PointPickingEvent& event, void* viewer_void)
{
	// 创建一个实例对象
	PCLvision* pcl_viewer = static_cast<PCLvision*>(viewer_void);
	pcl_viewer->pickPointCallback(event, viewer_void);
}
//框选--------------回调函数
void PCLvision::pickAreaCallback(const pcl::visualization::AreaPickingEvent& event, void* viewer_void)
{
	std::vector<int> indices;
	if (event.getPointsIndices(indices) == -1)
		return;
	PointCloudT::Ptr clicked_points_3d(new PointCloudT);
	for (int i = 0; i < indices.size(); ++i)
	{
		clicked_points_3d->points.push_back(m_currentCloud->points.at(indices[i]));
	}
	QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "删除源点云?", QMessageBox::Yes | QMessageBox::No);
	if (result == QMessageBox::Yes)
	{
		*m_currentCloud = *clicked_points_3d;
		m_currentCloud->resize(indices.size());
		viewer->removeAllPointClouds();
		viewer->removeAllShapes();
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(clicked_points_3d, 0, 255, 0);
		viewer->addPointCloud(clicked_points_3d, singelColor, "pick", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "pick");
		ui.qvtkWidget->update();
		int Cloudnum = indices.size();
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	else
	{
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(clicked_points_3d, 0, 255, 0);
		viewer->removeAllPointClouds();
		viewer->removeAllShapes();
		viewer->addPointCloud(m_currentCloud);
		viewer->addPointCloud(clicked_points_3d, singelColor, "pick", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "pick");
		ui.qvtkWidget->update();
		int Cloudnum = m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	SB->showMessage("               再次点按x键后结束框选...", 3000);
}
//静态成员函数作为回调函数
static void staticPickAreaCallback(const pcl::visualization::AreaPickingEvent& event, void* viewer_void)
{
	// 创建一个实例对象
	PCLvision* pcl_viewer = static_cast<PCLvision*>(viewer_void);
	pcl_viewer->pickAreaCallback(event, viewer_void);
}
//获取两个点平行于坐标轴的最短距离
double getMinValue(PointT p1, PointT p2)
{
	double min = 0;

	if (p1.x - p2.x > p1.y - p2.y)
	{
		min = p1.y - p2.y;
	}
	else
	{
		min = p1.x - p2.x;
	}

	if (min > p1.z - p2.z)
	{
		min = p1.z - p2.z;
	}

	return min;
}
//获取两个点平行于坐标轴的最长距离
double getMaxValue(PointT p1, PointT p2)
{
	double max = 0;

	if (p1.x - p2.x > p1.y - p2.y)
	{
		max = p1.x - p2.x;

	}
	else
	{
		max = p1.y - p2.y;
	}

	if (max < p1.z - p2.z)
	{
		max = p1.z - p2.z;
	}

	return max;
}
//初始化
void PCLvision::init()
{
	//点云初始化
	m_currentCloud.reset(new PointCloudT);

	auto renderer2 = vtkSmartPointer<vtkRenderer>::New();
	auto renderWindow2 = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	renderWindow2->AddRenderer(renderer2);
	viewer.reset(new pcl::visualization::PCLVisualizer(renderer2, renderWindow2, "viewer", false));
	ui.qvtkWidget->setRenderWindow(viewer->getRenderWindow());
	viewer->setupInteractor(ui.qvtkWidget->interactor(), ui.qvtkWidget->renderWindow());
	// 读取上一次保存的值
	QSettings settings("MyColorSetting", QSettings::IniFormat);
	QString ColorRed = settings.value("ColorRed").toString();
	QString ColorGreen = settings.value("ColorGreen").toString();
	QString ColorBlue = settings.value("ColorBlue").toString();
	if (!ColorRed.isEmpty() && !ColorGreen.isEmpty() && !ColorBlue.isEmpty())
	{
		double Red = ColorRed.toDouble();
		double Green = ColorGreen.toDouble();
		double Blue = ColorBlue.toDouble();
		viewer->setBackgroundColor(Red, Green, Blue);
	}

	//创建状态栏
	SB = statusBar();
	//创建组件
	label_num = new QLabel("点数");
	edit_num = new QLineEdit();
	edit_num->setFixedWidth(100);
	label_vie = new QLabel("视点");
	edit_vie = new QLineEdit();
	edit_vie->setFixedWidth(100);
	label_space = new QLabel("                                                                       ");
	//将组件添加到状态栏
	SB->addPermanentWidget(label_num);
	SB->addPermanentWidget(edit_num);
	SB->addPermanentWidget(label_vie);
	SB->addPermanentWidget(edit_vie);
	SB->addPermanentWidget(label_space);

	//槽函数
	connect(&heightRampDlg, SIGNAL(setHeightRamp(int, double)), this, SLOT(setHeightRamp(int, double)));
	connect(&passThroughDlg, SIGNAL(PassThrough(int, double, double, int)), this, SLOT(PassThrough(int, double, double, int)));
	connect(&voxelGridDlg, SIGNAL(VoxelGrid(double, double, double, int)), this, SLOT(VoxelGrid(double, double, double, int)));
	connect(&removeOutliersDlg, SIGNAL(RemoveOutliers(int, double, int)), this, SLOT(RemoveOutliers(int, double, int)));
	connect(&radiusFilterDlg, SIGNAL(RadiusFilter(double, int, int)), this, SLOT(RadiusFilter(double, int, int)));
	connect(&conditionaldlg, SIGNAL(Conditional(int, double, double, double, double, double, double)), this, SLOT(Conditional(int, double, double, double, double, double, double)));
	connect(&uniformsamplingdlg, SIGNAL(UniformSampling(double,int)), this, SLOT(UniformSampling(double,int)));
	connect(&upsamplingdlg, SIGNAL(UpSampling(double, double, double, int)), this, SLOT(UpSampling(double, double, double, int)));
	connect(&projectionDlg, SIGNAL(Projection(int,int)), this, SLOT(Projection(int, int)));
	connect(&depthmapDlg, SIGNAL(DepthMap(double, double, double, double, double, double, double, double, int)), this, SLOT(DepthMap(double, double, double, double, double, double, double, double, int)));
	connect(&NARFkeypointdlg, SIGNAL(NARFKeyPoint(double, double, double, double, int, double, int, int, double, double, int)), this, SLOT(NARFKeyPoint(double, double, double, double, int, double, int, int, double, double, int)));
	connect(&planedlg, SIGNAL(RANSAC(int, double,int,int, double, int, int, double, int, double, double)), this, SLOT(RANSAC(int, double,int,int, double, int, int, double, int, double, double)));
	connect(&linedlg, SIGNAL(RANSAC(int, double, int, int, double, int, int, double, int, double, double)), this, SLOT(RANSAC(int, double, int, int, double, int, int, double, int, double, double)));
	connect(&spheredlg, SIGNAL(RANSAC(int, double, int, int, double, int, int, double, int, double, double)), this, SLOT(RANSAC(int, double, int, int, double, int, int, double, int, double, double)));
	connect(&circle2ddlg, SIGNAL(RANSAC(int, double, int, int, double, int, int, double, int, double, double)), this, SLOT(RANSAC(int, double, int, int, double, int, int, double, int, double, double)));
	connect(&circle3ddlg, SIGNAL(RANSAC(int, double, int, int, double, int, int, double, int, double, double)), this, SLOT(RANSAC(int, double, int, int, double, int, int, double, int, double, double)));
	connect(&cylinderdlg, SIGNAL(RANSAC(int, double, int, int, double, int, int, double, int, double, double)), this, SLOT(RANSAC(int, double, int, int, double, int, int, double, int, double, double)));
	connect(&conedlg, SIGNAL(RANSAC(int, double, int, int, double, int, int, double, int, double, double)), this, SLOT(RANSAC(int, double, int, int, double, int, int, double, int, double, double)));
	connect(&ecedlg, SIGNAL(ECE(double, int, int)), this, SLOT(ECE(double, int, int)));
	connect(&regiongrowdlg, SIGNAL(RegionGrow(int, int, double, double, int, int)), this, SLOT(RegionGrow(int, int, double, double, int, int)));
	connect(&normal3ddlg, SIGNAL(Normal3D(double, int, double)), this, SLOT(Normal3D(double, int, double)));
	connect(&integralimagedlg, SIGNAL(IntegralImage(int, double, double, int, double)), this, SLOT(IntegralImage(int, double, double, int, double)));
	connect(&ndtdlg, SIGNAL(NDT(double, double, double, int, int, double, double, double, double, double)), this, SLOT(NDT(double, double, double, int, int, double, double, double, double, double)));
	connect(&icpdlg, SIGNAL(ICP(double, double, double, int, int, int, double, double, double)), this, SLOT(ICP(double, double, double, int, int, int, double, double, double)));
	connect(&polygondlg, SIGNAL(Polygon(double)), this, SLOT(Polygon(double)));
	connect(&gp3dlg, SIGNAL(GP3(int, double, double, int, double, double, double, int)), this, SLOT(GP3(int, double, double, int, double, double, double, int)));
	connect(&box3ddlg, SIGNAL(BOX3D(int, int)), this, SLOT(BOX3D(int, int)));
	connect(&templatematchingdlg, SIGNAL(TemplateMatching()), this, SLOT(TemplateMatching()));
	connect(&generatedlg, SIGNAL(Generate(int)), this, SLOT(Generate(int)));
	connect(&depth2clouddlg, SIGNAL(Depth2Cloud()), this, SLOT(Depth2Cloud()));

}
//打开点云
void PCLvision::on_action_open_triggered()
{
	//获取点云路径
	path = QFileDialog::getOpenFileName(this, "选择点云文件", ".//", "点云文件(*.txt *.pcd);;所有文件(*.*)");
	std::string mypath = path.toStdString();
	//读取点云数据
	QFile file(path);

	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		//清空点云
		m_currentCloud->clear();
		viewer->removeAllPointClouds();
		viewer->removeAllShapes();
		viewer->removeAllCoordinateSystems();
		int count = 0;
		int dataLine = 0;
		int ifhaveDATA = 0;
		QString view = "";
		QString Pnum = "";
		while (!file.atEnd())
		{
			count++;
			QByteArray line = file.readLine();
			QString str(line);
			QList<QString> strList = str.split(" ");
			if (strList[0] == "VIEWPOINT")
			{
				view = strList[1] + " " + strList[2] + " " + strList[3] + " " + strList[4] + " " + strList[5] + " " + strList[6] + " " + strList[7];
			}
			if (strList[0] == "POINTS")
			{
				Pnum = strList[1];
			}
			if (strList[0] == "DATA")
			{
				ifhaveDATA = 1;
				break;
			}
			if (count > 15)
			{
				count = 0;
				break;
			}
		}
		if (ifhaveDATA == 1)
		{
			// 将pcd中的数据加载到m_currentCloud中
			if (pcl::io::loadPCDFile<pcl::PointXYZ>(mypath, *m_currentCloud) == -1) //* 加载文件
			{
				SB->showMessage("               文件读取失败...", 3000);
				return;
			}
		}
		else
		{
			while (!file.atEnd())
			{
				dataLine++;
				QByteArray line = file.readLine();
				QString str(line);
				QList<QString> strList = str.split(" ");
				if (dataLine > count)
				{
					if (strList.size() != 3)
					{
						strList.clear();
						strList = str.split("\t");
						QMessageBox::warning(this, "警告", "点云文件格式错误！");
						break;
					}

					//点云赋值
					PointT point;
					point.x = strList.at(0).toDouble();
					point.y = strList.at(1).toDouble();
					point.z = strList.at(2).toDouble();

					m_currentCloud->push_back(point);
				}
			}
		}
		//添加到窗口
		viewer->addPointCloud(m_currentCloud);
		pcl::getMinMax3D(*m_currentCloud, p_min, p_max);

		double scale = getMinValue(p_max, p_min);
		maxLen = getMaxValue(p_max, p_min);

		//重设视角
		viewer->resetCamera();

		//刷新窗口
		ui.qvtkWidget->update();

		if (count == 0 || !m_currentCloud->empty())
		{
			int Cloudnum = m_currentCloud->width;
			QString view = "0 0 0 1 0 0 0";
			QString Pnum = QString::number(Cloudnum);
			edit_num->setText(Pnum);
			edit_vie->setText(view);
		}
		else if (count != 0 || !m_currentCloud->empty())
		{
			edit_num->setText(Pnum);
			edit_vie->setText(view);
		}
	}
	file.close();
	SB->showMessage("              ");
}
//存储点云
void PCLvision::on_action_save_triggered()
{
	std::string mypath = path.toStdString();
	pcl::PCDWriter writer;
	writer.write(mypath, *m_currentCloud, true);
}
//点云另存为
void PCLvision::on_action_saveO_triggered()
{
	//获取路径
	path = QFileDialog::getSaveFileName(this, "选择存储路径", "cloud.pcd", "点云文件(*.txt *.pcd);;所有文件(*.*)");
	// 检查用户是否选择了有效的路径
	if (!path.isEmpty()) {
		std::string mypath = path.toStdString();
		pcl::PCDWriter writer;
		//pcl::PointCloud<pcl::PointXYZ>::Ptr target_cloud(new pcl::PointCloud<pcl::PointXYZ>);
		//for (size_t i = 0; i < m_currentCloud->size(); ++i) {
		//	pcl::PointXYZ point;
		//	point.x = m_currentCloud->points[i].x/1000;
		//	point.y = m_currentCloud->points[i].y/1000;
		//	point.z = m_currentCloud->points[i].z/1000;
		//	target_cloud->push_back(point);
		//}
		writer.write(mypath, *m_currentCloud, true);
	}
	else {
		// 用户取消了另存操作，可以选择提示用户或执行其他逻辑
		// 例如：
		qDebug() << "取消另存";
	}
}
//重设视角
void PCLvision::on_action_reset_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->resetCamera();
		ui.qvtkWidget->update();
	}
}
//俯视图
void PCLvision::on_action_up_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), p_max.z + 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0, 0, 1, 0);
		ui.qvtkWidget->update();
	}
}
//前视图
void PCLvision::on_action_front_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), p_min.y - 2 * maxLen, 0.5 * (p_min.z + p_max.z), 0.5 * (p_min.x + p_max.x), 0, 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui.qvtkWidget->update();
	}
}
//左视图
void PCLvision::on_action_left_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(p_min.x - 2 * maxLen, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui.qvtkWidget->update();
	}
}
//后视图
void PCLvision::on_action_back_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), p_max.y + 2 * maxLen, 0.5 * (p_min.z + p_max.z), 0.5 * (p_min.x + p_max.x), 0, 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui.qvtkWidget->update();
	}
}
//右视图
void PCLvision::on_action_right_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(p_max.x + 2 * maxLen, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 0, 0, 1);
		ui.qvtkWidget->update();
	}
}
//底视图
void PCLvision::on_action_bottom_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), p_min.z - 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0, 0, 1, 0);
		ui.qvtkWidget->update();
	}
}
//前轴测
void PCLvision::on_action_frontIso_triggered()
{
	if (!m_currentCloud->empty())
	{
		viewer->setCameraPosition(p_min.x - 2 * maxLen, p_min.y - 2 * maxLen, p_max.z + 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), 1, 1, 0);
		ui.qvtkWidget->update();
	}
}
//后轴测
void PCLvision::on_action_backIso_triggered()
{
	viewer->setCameraPosition(p_max.x + 2 * maxLen, p_max.y + 2 * maxLen, p_max.z + 2 * maxLen, 0.5 * (p_min.x + p_max.x), 0.5 * (p_min.y + p_max.y), 0.5 * (p_min.z + p_max.z), -1, -1, 0);
	ui.qvtkWidget->update();
}
//设置点云颜色
void PCLvision::on_action_setColor_triggered()
{
	QColor color = QColorDialog::getColor(Qt::white, this, "设置点云颜色", QColorDialog::ShowAlphaChannel);

	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(m_currentCloud, color.red(), color.green(), color.blue());
	viewer->addPointCloud(m_currentCloud, singelColor, "myCloud", 0);
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, color.alpha() * 1.0 / 255, "myCloud");

	ui.qvtkWidget->update();

}
//设置背景颜色
void PCLvision::on_action_BackgroudColor_triggered()
{
	QColor color = QColorDialog::getColor(Qt::white, this, "设置背景颜色", QColorDialog::ShowAlphaChannel);
	viewer->setBackgroundColor(color.red()/255.0, color.green()/255.0, color.blue()/255.0);
	// 保存当前输入的值
	QSettings settings("MyColorSetting", QSettings::IniFormat);
	QString ColorRed = QString::number(color.red() / 255.0);
	settings.setValue("ColorRed", ColorRed);
	QString ColorGreen = QString::number(color.green() / 255.0);
	settings.setValue("ColorGreen", ColorGreen);
	QString ColorBlue = QString::number(color.blue() / 255.0);
	settings.setValue("ColorBlue", ColorBlue);
	//ui.qvtkWidget->update();
}
//设置高度渲染
void PCLvision::on_action_heightRamp_triggered()
{
	heightRampDlg.show();
}
//进行高度渲染
void PCLvision::setHeightRamp(int dir, double height)
{
	//清空点云
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	m_heightCloudList.clear();

	double min_range = 0;
	double max_range = 0;
	std::string field = "x";

	switch (dir)
	{
	case 0:
		min_range = p_min.x;
		max_range = p_max.x;
		field = "x";
		break;

	case 1:
		min_range = p_min.y;
		max_range = p_max.y;
		field = "y";
		break;

	case 2:
		min_range = p_min.z;
		max_range = p_max.z;
		field = "z";
		break;
	default:
		break;
	}

	for (double i = min_range - 1; i < max_range + height;)
	{
		PointCloudT::Ptr cloudTemp(new PointCloudT());

		pcl::PassThrough<PointT> pass;			//直通滤波器对象
		pass.setInputCloud(m_currentCloud);		//输入点云
		pass.setFilterFieldName(field);			//设置过滤字段
		pass.setFilterLimits(i, i + height);	//设置过滤范围
		//pass.setFilterLimitsNegative(false);	//设置保留字段
		pass.filter(*cloudTemp);				//执行滤波

		i += height;

		m_heightCloudList.append(cloudTemp);
	}

	//分段渲染
	for (int j = 0; j < m_heightCloudList.size(); j++)
	{
		pcl::visualization::PointCloudColorHandlerGenericField<PointT> fieldColor(m_heightCloudList.at(j), field);
		std::string index = std::to_string(j);
		viewer->addPointCloud(m_heightCloudList.at(j), fieldColor, index);
	}

}
//选点
void PCLvision::on_action_pickPoints_triggered()
{
	if (ifRegisterPointPicking == 0)
	{
		// 注册鼠标选取回调函数
		viewer->registerPointPickingCallback(staticPickPointCallback, this);
	}
	qDebug() << "Start picking points...";
	SB->showMessage("               请按下shift键同时操作选点...");
	ifRegisterPointPicking = 1;
}
//选择区域
void PCLvision::on_action_pickArea_triggered()
{
	if (ifRegisterAreaPicking == 0)
	{
		// 注册框选回调函数
		viewer->registerAreaPickingCallback(staticPickAreaCallback, this);
	}
	qDebug() << "Start picking area...";
	SB->showMessage("               请点按x键后框选...");
	ifRegisterAreaPicking = 1;
}
//ZED相机
void PCLvision::on_action_ZED_triggered()
{
	if (zedpath.isEmpty())
	{
		zedpath = QFileDialog::getOpenFileName(this, "选择相机SDK", ".//", "ZEDSDK(*.exe);;所有文件(*.*)");

	}
	QProcess::startDetached(zedpath);
}
//Meck相机
void PCLvision::on_action_Meck_triggered()
{
	if (meckpath.isEmpty())
	{
		meckpath = QFileDialog::getOpenFileName(this, "选择相机SDK", ".//", "MECKSDK(*.exe);;所有文件(*.*)");

	}
	QProcess::startDetached(meckpath);
}
//滤波
void PCLvision::on_action_PassThrough_triggered()
{
	passThroughDlg.show();
}
//进行直通滤波
void PCLvision::PassThrough(int dir, double min, double max, int KeepOrganized)
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	std::string field = "x";

	switch (dir)
	{
	case 0:
		field = "x";
		break;

	case 1:
		field = "y";
		break;

	case 2:
		field = "z";
		break;
	default:
		break;
	}
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
	// 创建滤波器
	pcl::PassThrough<pcl::PointXYZ> pass;
	pass.setInputCloud(m_currentCloud);    // 1. 设置输入源
	pass.setFilterFieldName(field);       // 2. 设置过滤域名
	pass.setFilterLimits(min, max);      // 3. 设置过滤范围
	pass.filter(*cloud_filtered);       // 4. 执行过滤，并将结果输出到cloud_filtered

	text.clear();
	ui.infoTextEdit->clear();
	text += QString("滤波前点数: %1 \n").arg(m_currentCloud->size());
	text += QString("滤波后点数: %1 \n").arg(cloud_filtered->size());
	text += "\n";

	ui.infoTextEdit->appendPlainText(text);

	if (KeepOrganized == 0) //设置保持点云的结构
	{
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_filtered, 255, 0, 0);
		viewer->addPointCloud(m_currentCloud);
		viewer->addPointCloud(cloud_filtered, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	else
	{
		m_currentCloud = cloud_filtered;
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_filtered, 255, 0, 0);
		viewer->addPointCloud(cloud_filtered, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = cloud_filtered->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}

}
void PCLvision::on_action_VoxelGrid_triggered()
{
	voxelGridDlg.show();
}
//进行降采样
void PCLvision::VoxelGrid(double L, double W, double H, int KeepOrganized)
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
	// 创建一个自定义长宽高的体素过滤器，m_currentCloud作为输入数据，cloud_filtered作为输出数据
	pcl::VoxelGrid<pcl::PointXYZ> sor;
	sor.setInputCloud(m_currentCloud);
	sor.setLeafSize(L, W, H);
	sor.filter(*cloud_filtered);

	text.clear();
	ui.infoTextEdit->clear();
	text += QString("采样前点数: %1 \n").arg(m_currentCloud->size());
	text += QString("采样后点数: %1 \n").arg(cloud_filtered->size());
	text += "\n";

	ui.infoTextEdit->appendPlainText(text);

	if (KeepOrganized == 0) //设置保持点云的结构
	{
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_filtered, 255, 0, 0);
		viewer->addPointCloud(m_currentCloud);
		viewer->addPointCloud(cloud_filtered, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	else
	{
		m_currentCloud = cloud_filtered;
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_filtered, 255, 0, 0);
		viewer->addPointCloud(cloud_filtered, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = cloud_filtered->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}

}
void PCLvision::on_action_RemoveOutliers_triggered()
{
	removeOutliersDlg.show();
}
//移除离群点
void PCLvision::RemoveOutliers(int Knum, double Thresh, int KeepOrganized)
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
	// 创建滤波器
	pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
	sor.setInputCloud(m_currentCloud);
	// 设置平均距离估计的最近邻的数量K
	sor.setMeanK(Knum);
	// 设置标准差阈值系数
	sor.setStddevMulThresh(Thresh);
	// 执行过滤
	sor.filter(*cloud_filtered);

	text.clear();
	ui.infoTextEdit->clear();
	text += QString("滤波前点数: %1 \n").arg(m_currentCloud->size());
	text += QString("滤波后点数: %1 \n").arg(cloud_filtered->size());
	text += "\n";

	ui.infoTextEdit->appendPlainText(text);

	if (KeepOrganized == 0) //设置保持点云的结构
	{
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_filtered, 255, 0, 0);
		viewer->addPointCloud(m_currentCloud);
		viewer->addPointCloud(cloud_filtered, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	else
	{
		m_currentCloud = cloud_filtered;
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_filtered, 255, 0, 0);
		viewer->addPointCloud(cloud_filtered, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = cloud_filtered->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
}
void PCLvision::on_action_RadiusFilter_triggered()
{
	radiusFilterDlg.show();
}
//进行半径滤波
void PCLvision::RadiusFilter(double radius, int minKnum, int KeepOrganized)
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::RadiusOutlierRemoval<pcl::PointXYZ> outrem;
	// 创建滤波器
	outrem.setInputCloud(m_currentCloud);
	outrem.setRadiusSearch(radius);
	outrem.setMinNeighborsInRadius(minKnum);
	// 添加滤波器
	outrem.filter(*cloud_filtered);

	text.clear();
	ui.infoTextEdit->clear();
	text += QString("滤波前点数: %1 \n").arg(m_currentCloud->size());
	text += QString("滤波后点数: %1 \n").arg(cloud_filtered->size());
	text += "\n";

	ui.infoTextEdit->appendPlainText(text);

	if (KeepOrganized == 0) //设置保持点云的结构
	{
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_filtered, 255, 0, 0);
		viewer->addPointCloud(m_currentCloud);
		viewer->addPointCloud(cloud_filtered, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	else
	{
		m_currentCloud = cloud_filtered;
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_filtered, 255, 0, 0);
		viewer->addPointCloud(cloud_filtered, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = cloud_filtered->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
}
void PCLvision::on_action_Conditional_triggered()
{
	conditionaldlg.show();
}
//进行条件滤波
void PCLvision::Conditional(int KeepOrganized, double GT_X_d, double LT_X_d, double GT_Y_d, double LT_Y_d, double GT_Z_d, double LT_Z_d)
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered_ptr(new pcl::PointCloud<pcl::PointXYZ>());
	//创建条件定义对象
	pcl::ConditionAnd<pcl::PointXYZ>::Ptr range_cond_cptr(new pcl::ConditionAnd<pcl::PointXYZ>());
	//为条件定义对象添加比较算子
	range_cond_cptr->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("x", pcl::ComparisonOps::GT, GT_X_d)));
	range_cond_cptr->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("x", pcl::ComparisonOps::LT, LT_X_d)));
	range_cond_cptr->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("y", pcl::ComparisonOps::GT, GT_Y_d)));
	range_cond_cptr->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("y", pcl::ComparisonOps::LT, LT_Y_d)));
	range_cond_cptr->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("z", pcl::ComparisonOps::GT, GT_Z_d)));
	range_cond_cptr->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("z", pcl::ComparisonOps::LT, LT_Z_d)));
	
	// 创建滤波器并用条件定义对象初始化
	pcl::ConditionalRemoval<pcl::PointXYZ> conditrem;//创建条件滤波器
	conditrem.setCondition(range_cond_cptr);        //并用条件定义对象初始化            
	conditrem.setInputCloud(m_currentCloud);             //输入点云
	// 执行滤波
	conditrem.filter(*cloud_filtered_ptr); 

	text.clear();
	ui.infoTextEdit->clear();
	text += QString("滤波前点数: %1 \n").arg(m_currentCloud->size());
	text += QString("滤波后点数: %1 \n").arg(cloud_filtered_ptr->size());
	text += "\n";

	ui.infoTextEdit->appendPlainText(text);

	if (KeepOrganized == 0) //设置保持点云的结构
	{
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_filtered_ptr, 255, 0, 0);
		viewer->addPointCloud(m_currentCloud);
		viewer->addPointCloud(cloud_filtered_ptr, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	else 
	{
		m_currentCloud = cloud_filtered_ptr;
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_filtered_ptr, 255, 0, 0);
		viewer->addPointCloud(cloud_filtered_ptr, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = cloud_filtered_ptr->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
}
void PCLvision::on_action_Projection_triggered()
{
	projectionDlg.show();
}
//进行投影
void PCLvision::Projection(int plane, int KeepOrganized)
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_projected(new pcl::PointCloud<pcl::PointXYZ>);
	// 定义模型系数对象
	pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
	if (plane == 0)
	{
		// 填充ModelCoefficients的值,使用ax+by+cz+d=0*面模型，其中 a=b=d=0,c=1 也就是X——Y*面
		coefficients->values.resize(4);
		coefficients->values[0] = coefficients->values[1] = 0;
		coefficients->values[2] = 1.0;
		coefficients->values[3] = 0;
	}
	else if (plane == 1)
	{
		// 填充ModelCoefficients的值,使用ax+by+cz+d=0*面模型，其中 c=b=d=0,a=1 也就是Y——Z*面
		coefficients->values.resize(4);
		coefficients->values[0] = 1.0;
		coefficients->values[1] = 0;
		coefficients->values[2] = 0;
		coefficients->values[3] = 0;
	}
	else if (plane == 2)
	{
		// 填充ModelCoefficients的值,使用ax+by+cz+d=0*面模型，其中 a=c=d=0,b=1 也就是X——Z*面
		coefficients->values.resize(4);
		coefficients->values[0] = 0;
		coefficients->values[1] = 1.0;
		coefficients->values[2] = 0;
		coefficients->values[3] = 0;
	}
	// 创建ProjectInliers对象，使用ModelCoefficients作为投影对象的模型参数
	pcl::ProjectInliers<pcl::PointXYZ> proj;     //创建投影滤波对象
	proj.setModelType(pcl::SACMODEL_PLANE);      //设置对象对应的投影模型
	proj.setInputCloud(m_currentCloud);                   //设置输入点云
	proj.setModelCoefficients(coefficients);       //设置模型对应的系数
	proj.filter(*cloud_projected);                 //投影结果存储

	text.clear();
	ui.infoTextEdit->clear();
	text += QString("投影前点数: %1 \n").arg(m_currentCloud->size());
	text += QString("投影后点数: %1 \n").arg(cloud_projected->size());
	text += "\n";

	ui.infoTextEdit->appendPlainText(text);

	if (KeepOrganized == 0) //设置保持点云的结构
	{
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_projected, 255, 0, 0);
		viewer->addPointCloud(m_currentCloud);
		viewer->addPointCloud(cloud_projected, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = cloud_projected->width + m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	else
	{
		m_currentCloud = cloud_projected;
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(cloud_projected, 255, 0, 0);
		viewer->addPointCloud(cloud_projected, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = cloud_projected->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
}
void PCLvision::on_action_UniformSampling_triggered()
{
	uniformsamplingdlg.show();
}
//进行均匀采样
void PCLvision::UniformSampling(double radius, int KeepOrganized)
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	pcl::PointCloud<pcl::PointXYZ>::Ptr filteredCloud(new pcl::PointCloud<pcl::PointXYZ>);
	// 统一采样对象
	pcl::UniformSampling<pcl::PointXYZ> filter;
	filter.setInputCloud(m_currentCloud);
	filter.setRadiusSearch(radius);
	filter.filter(*filteredCloud);

	text.clear();
	ui.infoTextEdit->clear();
	text += QString("采样前点数: %1 \n").arg(m_currentCloud->size());
	text += QString("采样后点数: %1 \n").arg(filteredCloud->size());
	text += "\n";

	ui.infoTextEdit->appendPlainText(text);

	if (KeepOrganized == 0) //设置保持点云的结构
	{
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(filteredCloud, 255, 0, 0);
		viewer->addPointCloud(m_currentCloud);
		viewer->addPointCloud(filteredCloud, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	else
	{
		m_currentCloud = filteredCloud;
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(filteredCloud, 255, 0, 0);
		viewer->addPointCloud(filteredCloud, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = filteredCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
}
void PCLvision::on_action_UpSampling_triggered()
{
	upsamplingdlg.show();
}
//进行上采样
void PCLvision::UpSampling(double SearchRadius, double SamplingRadius, double SamplingStep, int KeepOrganized)
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	pcl::PointCloud<pcl::PointXYZ>::Ptr filteredCloud(new pcl::PointCloud<pcl::PointXYZ>);

	// 滤波对象
	pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointXYZ> filter;
	filter.setInputCloud(m_currentCloud);
	//建立搜索对象
	pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
	filter.setSearchMethod(kdtree);
	//设置搜索邻域的半径为3cm
	filter.setSearchRadius(SearchRadius);
	// Upsampling 采样的方法有 DISTINCT_CLOUD, RANDOM_UNIFORM_DENSITY
	filter.setUpsamplingMethod(pcl::MovingLeastSquares<pcl::PointXYZ, pcl::PointXYZ>::SAMPLE_LOCAL_PLANE);
	// 采样的半径是
	filter.setUpsamplingRadius(SamplingRadius);
	// 采样步数的大小
	filter.setUpsamplingStepSize(SamplingStep);
	filter.process(*filteredCloud);

	text.clear();
	ui.infoTextEdit->clear();
	text += QString("采样前点数: %1 \n").arg(m_currentCloud->size());
	text += QString("采样后点数: %1 \n").arg(filteredCloud->size());
	text += "\n";

	ui.infoTextEdit->appendPlainText(text);

	if (KeepOrganized == 0) //设置保持点云的结构
	{
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(filteredCloud, 255, 0, 0);
		viewer->addPointCloud(m_currentCloud);
		viewer->addPointCloud(filteredCloud, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	else
	{
		m_currentCloud = filteredCloud;
		pcl::visualization::PointCloudColorHandlerCustom<PointT> singelColor(filteredCloud, 255, 0, 0);
		viewer->addPointCloud(filteredCloud, singelColor, "filter", 0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 5, "filter");
		ui.qvtkWidget->update();
		int Cloudnum = filteredCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
}
//深度图
void PCLvision::on_action_DepthMap_triggered()
{
	depthmapDlg.show();
}
void PCLvision::DepthMap(double angularResolution, double maxAngleWidth, double maxAngleHeight, double X, double Y, double Z, double noiseLevel, double minRange, int borderSize)
{
	//根据点云图，通过1deg的分辨率生成深度图
	//angular_resolution为模拟的深度传感器的角度分辨率，即深度图像中一个像素对应的角度大小
	double AngularResolution = (double)(angularResolution * (M_PI / 180.0));  
	//max_angle_width为模拟的深度传感器的水平最大采样角度
	double MaxAngleWidth = (double)(maxAngleWidth * (M_PI / 180.0));  
	//max_angle_height为模拟传感器的垂直方向最大采样角度,都转为弧度
	double MaxAngleHeight = (double)(maxAngleHeight * (M_PI / 180.0));  
	//传感器的采集位置
	Eigen::Affine3f sensorPose = (Eigen::Affine3f)Eigen::Translation3f(X, Y, Z);
	//深度图像遵循坐标系统
	pcl::RangeImage::CoordinateFrame coordinate_frame = pcl::RangeImage::CAMERA_FRAME;
	double NoiseLevel = noiseLevel;    //noise_level获取深度图像深度时，近邻点对查询点距离值的影响水平
	double MinRange = minRange;    //min_range设置最小的获取距离，小于最小获取距离的位置为传感器的盲区
	int BorderSize = borderSize;     //border_size获得深度图像的边缘的宽度

	boost::shared_ptr<pcl::RangeImage> range_image_ptr(new pcl::RangeImage);
	pcl::RangeImage& rangeImg = *range_image_ptr;
	range_image_ptr->createFromPointCloud(*m_currentCloud, AngularResolution, MaxAngleWidth, MaxAngleHeight, sensorPose, coordinate_frame, NoiseLevel, MinRange, BorderSize);
	//深度图存储
	if (depthmapDlg.DepthMappath.isEmpty())
	{
		depthmapDlg.DepthMappath = "RangeImageRGB.png";
	}
	std::string mypath = depthmapDlg.DepthMappath.toStdString();
	float* ranges = rangeImg.getRangesArray();
	unsigned char* rgb_image = pcl::visualization::FloatImageUtils::getVisualImage(ranges, rangeImg.width, rangeImg.height);
	pcl::io::saveRgbPNGFile(mypath, rgb_image, rangeImg.width, rangeImg.height);
	// --------------------------
	// -----显示深度图-----
	// --------------------------
	//用以图像的方式可视化深度图像，图像的颜色取决于深度值
	pcl::visualization::RangeImageVisualizer range_image_widget("Range image");
	range_image_widget.showRangeImage(rangeImg);      //图像可视化方式显示深度图像
	range_image_widget.setSize(800, 600);

	while (!range_image_widget.wasStopped()) 
	{
		// 事件监听与处理
		range_image_widget.spinOnce();
	}
}
//关键点
void PCLvision::on_action_NARFKeyPoint_triggered()
{
	NARFkeypointdlg.show();
}
void PCLvision::NARFKeyPoint(double angularResolution, double maxAngleWidth, double maxAngleHeight, double Support_size, int Add_points_on_straight_edges,double Distance_for_additional_points, int CAMERAorLASER, int UnseenSet, double noise_level, double min_range, int border_size)
{
	//angular_resolution为模拟的深度传感器的角度分辨率，即深度图像中一个像素对应的角度大小
	double AngularResolution = (double)(angularResolution * (M_PI / 180.0));
	//max_angle_width为模拟的深度传感器的水平最大采样角度
	double MaxAngleWidth = (double)(maxAngleWidth * (M_PI / 180.0));
	//max_angle_height为模拟传感器的垂直方向最大采样角度,都转为弧度
	double MaxAngleHeight = (double)(maxAngleHeight * (M_PI / 180.0));
	pcl::PointCloud<pcl::PointWithViewpoint> far_ranges;
	Eigen::Affine3f scene_sensor_pose(Eigen::Affine3f::Identity());
	scene_sensor_pose = Eigen::Affine3f(Eigen::Translation3f(m_currentCloud->sensor_origin_[0], m_currentCloud->sensor_origin_[1], m_currentCloud->sensor_origin_[2])) *Eigen::Affine3f(m_currentCloud->sensor_orientation_);
	if (NARFkeypointdlg._far_ranges_pcd_path.isEmpty())
	{
		NARFkeypointdlg._far_ranges_pcd_path = "_far_ranges.pcd";
	}
	std::string mypath = NARFkeypointdlg._far_ranges_pcd_path.toStdString();
	if (pcl::io::loadPCDFile(mypath.c_str(), far_ranges) == -1)
		SB->showMessage("               远距离深度文件不存在...", 5000);
	pcl::RangeImage::CoordinateFrame coordinate_frame = pcl::RangeImage::CAMERA_FRAME;
	if (CAMERAorLASER == 0)//相机坐标系
	{
		coordinate_frame = pcl::RangeImage::CAMERA_FRAME;
	}
	else//激光坐标系
	{
		coordinate_frame = pcl::RangeImage::LASER_FRAME;
	}
	bool setUnseenToMaxRange = false;
	if (UnseenSet == 0)//是否将不可见区域设置为最大深度
		setUnseenToMaxRange = true;
	else
		setUnseenToMaxRange = false;
	// -----------------------------------------------
	// ---------------从点云创建深度图----------------
	// -----------------------------------------------
	boost::shared_ptr<pcl::RangeImage> range_image_ptr(new pcl::RangeImage);
	pcl::RangeImage& range_image = *range_image_ptr;
	range_image.createFromPointCloud(*m_currentCloud, AngularResolution, MaxAngleWidth, MaxAngleHeight, scene_sensor_pose, coordinate_frame, noise_level, min_range, border_size);
	range_image.integrateFarRanges(far_ranges);
	if (setUnseenToMaxRange)
		range_image.setUnseenToMaxRange();
	// --------------------------------
	// --------提取 NARF 关键点--------
	// --------------------------------
	pcl::RangeImageBorderExtractor range_image_border_extractor;  //创建了一个RangeImageBorderExtractor对象，用于提取边界特征
	pcl::NarfKeypoint narf_keypoint_detector(&range_image_border_extractor);  //创建了一个NarfKeypoint对象，用于进行NARF关键点检测。通过将之前创建的RangeImageBorderExtractor对象传递给构造函数，指定了在关键点检测过程中使用的边界提取器
	narf_keypoint_detector.setRangeImage(&range_image);  //将输入的范围图像range_image设置为关键点检测器的范围图像
	narf_keypoint_detector.getParameters().support_size = Support_size;  //设置关键点检测器的参数中的support_size属性，表示关键点周围的支持区域大小
	if (Add_points_on_straight_edges == 0)
	{
		narf_keypoint_detector.getParameters().add_points_on_straight_edges = true;  //设置关键点检测器的参数中的add_points_on_straight_edges属性为true，表示在直线边缘上添加额外的关键点
	}
	else
	{
		narf_keypoint_detector.getParameters().add_points_on_straight_edges = false;  //设置关键点检测器的参数中的add_points_on_straight_edges属性为true，表示在直线边缘上添加额外的关键点

	}
	narf_keypoint_detector.getParameters().distance_for_additional_points = Distance_for_additional_points;  //设置关键点检测器的参数中的distance_for_additional_points属性，表示添加额外关键点的最大距离

	pcl::PointCloud<int> keypoint_indices;
	narf_keypoint_detector.compute(keypoint_indices);
	// -------------------------------------
	// ---------在3D视图中显示关键点--------
	// -------------------------------------
	pcl::PointCloud<pcl::PointXYZ>::Ptr keypoints_ptr(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::PointCloud<pcl::PointXYZ>& keypoints = *keypoints_ptr;
	keypoints.points.resize(keypoint_indices.points.size());
	for (size_t i = 0; i < keypoint_indices.points.size(); ++i)
		keypoints.points[i].getVector3fMap() = range_image.points[keypoint_indices.points[i]].getVector3fMap();
	QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "删除非关键点?", QMessageBox::Yes | QMessageBox::No);
	if (result == QMessageBox::Yes)
	{
		*m_currentCloud = *keypoints_ptr;
		m_currentCloud->resize(keypoint_indices.size());
		viewer->removeAllPointClouds();
		viewer->removeAllShapes();
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> keypoints_color_handler(keypoints_ptr, 0, 255, 0);
		viewer->addPointCloud<pcl::PointXYZ>(keypoints_ptr, keypoints_color_handler, "keypoints");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "keypoints");
		ui.qvtkWidget->update();
		int Cloudnum = keypoint_indices.size();
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);

		text.clear();
		ui.infoTextEdit->clear();
		text += QString("关键点数量: %1 \n").arg(keypoint_indices.size());
		text += "\n";

		ui.infoTextEdit->appendPlainText(text);
	}
	else
	{
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> keypoints_color_handler(keypoints_ptr, 0, 255, 0);
		viewer->removeShape("keypoints");
		viewer->addPointCloud<pcl::PointXYZ>(keypoints_ptr, keypoints_color_handler, "keypoints");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "keypoints");
		ui.qvtkWidget->update();
		QString Keypointnum = QString::number(keypoint_indices.size());
		int Cloudnum = m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);

		text.clear();
		ui.infoTextEdit->clear();
		text += QString("关键点数量: %1 \n").arg(keypoint_indices.size());
		text += "\n";

		ui.infoTextEdit->appendPlainText(text);
	}
}
//RANSAC
void PCLvision::on_action_Plane_triggered()
{
	planedlg.show();
}
void PCLvision::on_action_Line_triggered()
{
	linedlg.show();
}
void PCLvision::on_action_Circle2D_triggered()
{
	circle2ddlg.show();
}
void PCLvision::on_action_Circle3D_triggered()
{
	circle3ddlg.show();
}
void PCLvision::on_action_Sphere_triggered()
{
	spheredlg.show();
}
void PCLvision::on_action_Cylinder_triggered()
{
	cylinderdlg.show();
}
void PCLvision::on_action_Cone_triggered()
{
	conedlg.show();
}
void PCLvision::RANSAC(int model, double localpoint, int iteration, int minpointnum, double confidence, int KdTreeSearhNum, int ModelOPTI, double NormalWeight, int ifReverse, double MinR, double MaxR)
{
	text.clear();
	ui.infoTextEdit->clear();
	pcl::PointCloud<pcl::PointXYZ>::Ptr final(new pcl::PointCloud<pcl::PointXYZ>);
	// 平面
	if (model == 0)
	{
		std::vector<int> interiorPoint;
		//外点点云存储
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out(new pcl::PointCloud<pcl::PointXYZ>());
		cloud_out->resize(m_currentCloud->size());
		*cloud_out = *m_currentCloud;
		int j = 0;
		while (cloud_out->size() > minpointnum)//循环条件
		{
			//创建RandomSampleConsensus对象并计算适当的模型
			pcl::SampleConsensusModelPlane<pcl::PointXYZ>::Ptr model(new pcl::SampleConsensusModelPlane<pcl::PointXYZ>(cloud_out));
			//根据命令行参数，来随机估算对应平面模型，并存储估计的局内点
			pcl::RandomSampleConsensus<pcl::PointXYZ> ransac(model);
			ransac.setDistanceThreshold(localpoint);    //与平面距离小于localpoint的点称为局内点考虑
			ransac.setMaxIterations(iteration);		//最大迭代次数
			ransac.setProbability(confidence);
			ransac.computeModel();                   //执行随机参数估计
			//-------------------------根据索引提取内点--------------------------
			pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_plane(new pcl::PointCloud<pcl::PointXYZ>);
			ransac.getInliers(interiorPoint);			//提取内点索引
			//若内点尺寸过小，不用继续拟合，跳出循环
			if (interiorPoint.size() < minpointnum) {
				break;
			}
			pcl::copyPointCloud<pcl::PointXYZ>(*cloud_out, interiorPoint, *cloud_plane);

			//提取外点
			pcl::PointCloud<pcl::PointXYZ>::Ptr outplanes(new pcl::PointCloud<pcl::PointXYZ>);
			pcl::PointIndices::Ptr inplanePtr(new pcl::PointIndices);
			inplanePtr->indices = interiorPoint;
			pcl::ExtractIndices<pcl::PointXYZ> extract;
			extract.setInputCloud(cloud_out);
			extract.setIndices(inplanePtr);
			extract.setNegative(true);
			extract.filter(*outplanes);
			cloud_out->clear();
			*cloud_out = *outplanes;
			if (ifReverse == 0) { *final += *cloud_plane; }
			else { *final = *cloud_out; }

			text += QString("平面%1点数: %2 \n").arg(j+1).arg(cloud_plane->size());

			j++;
		}	
	}
	// 直线
	else if (model == 1)
	{
		std::vector<int> interiorPoint;
		// 创建RandomSampleConsensus对象并计算适当的模型
		pcl::SampleConsensusModelLine<pcl::PointXYZ>::Ptr model(new pcl::SampleConsensusModelLine<pcl::PointXYZ>(m_currentCloud));
		//根据命令行参数，来随机估算对应直线模型，并存储估计的局内点
		pcl::RandomSampleConsensus<pcl::PointXYZ> ransac(model);
		ransac.setDistanceThreshold(localpoint);
		ransac.setMaxIterations(iteration);
		ransac.setProbability(confidence);
		ransac.computeModel();
		ransac.getInliers(interiorPoint);			//提取内点索引
		pcl::copyPointCloud<pcl::PointXYZ>(*m_currentCloud, interiorPoint, *final);

		pcl::getMinMax3D(*final, p_min, p_max);
		float dx = p_max.x - p_min.x;
		float dy = p_max.y - p_min.y;
		float dz = p_max.z - p_min.z;
		double d = std::sqrt(dx* dx+ dy* dy+ dz* dz);
		text = "直线长度: ";
		text += QString("%6.3").arg(d);
		text += "\n";

		//外点点云存储
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out(new pcl::PointCloud<pcl::PointXYZ>());
		//提取外点
		pcl::PointCloud<pcl::PointXYZ>::Ptr outplanes(new pcl::PointCloud<pcl::PointXYZ>);
		pcl::PointIndices::Ptr inplanePtr(new pcl::PointIndices);
		inplanePtr->indices = interiorPoint;
		pcl::ExtractIndices<pcl::PointXYZ> extract;
		extract.setInputCloud(m_currentCloud);
		extract.setIndices(inplanePtr);
		extract.setNegative(true);
		extract.filter(*outplanes);
		*cloud_out = *outplanes;
		if (ifReverse == 0) {}
		else { *final = *cloud_out; }
	}
	// 2D圆
	else if (model == 2)
	{
		//类创建
		pcl::SACSegmentationFromNormals<pcl::PointXYZ, pcl::Normal>seg;
		pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);//创建索引对象
		pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
		pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);
		pcl::PointIndices::Ptr inliers_Circle2D(new pcl::PointIndices);

		//	估计法向量
		pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
		pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
		ne.setInputCloud(m_currentCloud);
		ne.setSearchMethod(tree);
		ne.setKSearch(KdTreeSearhNum);
		ne.compute(*cloud_normals);

		if (ModelOPTI == 0) { seg.setOptimizeCoefficients(true); }   //设置对估计模型优化
		else { seg.setOptimizeCoefficients(false); }
		seg.setModelType(pcl::SACMODEL_CIRCLE2D);//拟合2D圆
		seg.setMethodType(pcl::SAC_RANSAC);//RANSAC
		seg.setNormalDistanceWeight(NormalWeight);//设置法向量权重
		seg.setMaxIterations(iteration);//设置最大迭代次数
		seg.setDistanceThreshold(localpoint);//eps
		seg.setProbability(confidence);
		seg.setRadiusLimits(MinR, MaxR);//添加拟合圆的半径限制，防止拟合过大或过小的圆
		seg.setInputNormals(cloud_normals);//输入法向量
		seg.setInputCloud(m_currentCloud);//传入点集
		seg.segment(*inliers, *coefficients);//分割

		// 获取3D圆内点和系数
		seg.segment(*inliers_Circle2D, *coefficients);

		text = "2D圆参数: \n";
		for (int i = 0; i < coefficients->values.size(); ++i) {
			text += QString("values[%1]: ").arg(i);
			text += QString::number(coefficients->values[i]) + " \n";
		}
		text += "\n";

		pcl::ExtractIndices<pcl::PointXYZ> extract;
		// 提取内点
		extract.setInputCloud(m_currentCloud);
		extract.setIndices(inliers_Circle2D);
		if (ifReverse == 0) { extract.setNegative(false); }
		else { extract.setNegative(true); }
		extract.filter(*final);
	}
	// 3D圆
	else if (model == 3)
	{
		//类创建
		pcl::SACSegmentationFromNormals<pcl::PointXYZ, pcl::Normal>seg;
		pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);//创建索引对象
		pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
		pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);
		pcl::PointIndices::Ptr inliers_Circle3D(new pcl::PointIndices);

		//	估计法向量
		pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
		pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
		ne.setInputCloud(m_currentCloud);
		ne.setSearchMethod(tree);
		ne.setKSearch(KdTreeSearhNum);
		ne.compute(*cloud_normals);

		if (ModelOPTI == 0) { seg.setOptimizeCoefficients(true); }   //设置对估计模型优化
		else { seg.setOptimizeCoefficients(false); }
		seg.setModelType(pcl::SACMODEL_CIRCLE3D);//拟合3D圆
		seg.setMethodType(pcl::SAC_RANSAC);//RANSAC
		seg.setNormalDistanceWeight(NormalWeight);//设置法向量权重
		seg.setMaxIterations(iteration);//设置最大迭代次数
		seg.setDistanceThreshold(localpoint);//eps
		seg.setProbability(confidence);
		seg.setRadiusLimits(MinR, MaxR);//添加拟合圆的半径限制，防止拟合过大或过小的圆
		seg.setInputNormals(cloud_normals);//输入法向量
		seg.setInputCloud(m_currentCloud);//传入点集
		seg.segment(*inliers, *coefficients);//分割

		// 获取3D圆内点和系数
		seg.segment(*inliers_Circle3D, *coefficients);

		text = "3D圆参数: \n";
		for (int i = 0; i < coefficients->values.size(); ++i) {
			text += QString("values[%1]: ").arg(i);
			text += QString::number(coefficients->values[i]) + " \n";
		}
		text += "\n";

		pcl::ExtractIndices<pcl::PointXYZ> extract;
		// 提取内点
		extract.setInputCloud(m_currentCloud);
		extract.setIndices(inliers_Circle3D);
		if (ifReverse == 0) { extract.setNegative(false); }
		else { extract.setNegative(true); }
		extract.filter(*final);
	}
	// 球体
	else if (model == 4) 
	{
		//类创建
		pcl::SACSegmentationFromNormals<pcl::PointXYZ, pcl::Normal>seg;
		pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);//创建索引对象
		pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
		pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);
		pcl::PointIndices::Ptr inliers_Sphere(new pcl::PointIndices);

		//	估计法向量
		pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
		pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
		ne.setInputCloud(m_currentCloud);
		ne.setSearchMethod(tree);
		ne.setKSearch(KdTreeSearhNum);
		ne.compute(*cloud_normals);

		if (ModelOPTI == 0) { seg.setOptimizeCoefficients(true); }   //设置对估计模型优化
		else { seg.setOptimizeCoefficients(false); }
		seg.setModelType(pcl::SACMODEL_SPHERE);//拟合球
		seg.setMethodType(pcl::SAC_RANSAC);//RANSAC
		seg.setNormalDistanceWeight(NormalWeight);//设置法向量权重
		seg.setMaxIterations(iteration);//设置最大迭代次数
		seg.setDistanceThreshold(localpoint);//eps
		seg.setProbability(confidence);
		seg.setRadiusLimits(MinR, MaxR);//添加拟合圆的半径限制，防止拟合过大或过小的圆
		seg.setInputNormals(cloud_normals);//输入法向量
		seg.setInputCloud(m_currentCloud);//传入点集
		seg.segment(*inliers, *coefficients);//分割

		// 获取球内点和系数
		seg.segment(*inliers_Sphere, *coefficients);

		text = "球参数: \n";
		for (int i = 0; i < coefficients->values.size(); ++i) {
			text += QString("values[%1]: ").arg(i);
			text += QString::number(coefficients->values[i]) + " \n";
		}
		text += "\n";

		pcl::ExtractIndices<pcl::PointXYZ> extract;
		// 提取内点
		extract.setInputCloud(m_currentCloud);
		extract.setIndices(inliers_Sphere);
		if (ifReverse == 0) { extract.setNegative(false); }
		else { extract.setNegative(true); }
		extract.filter(*final);
	}
	// 圆柱体
	else if (model == 5)
	{
		// 创建圆柱模型的分割对象，并设置所有参数
		//pcl::PassThrough<pcl::PointXYZ> pass;             //直通滤波对象
		pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;  //法线估计对象
		pcl::SACSegmentationFromNormals<pcl::PointXYZ, pcl::Normal> seg;    //分割对象
		pcl::ExtractIndices<pcl::PointXYZ> extract;      //点提取对象
		//pcl::ExtractIndices<pcl::Normal> extract_normals;    ///点提取对象
		pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());

		pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);
		pcl::ModelCoefficients::Ptr coefficients_cylinder(new pcl::ModelCoefficients);
		pcl::PointIndices::Ptr inliers_cylinder(new pcl::PointIndices);

		ne.setSearchMethod(tree);
		ne.setInputCloud(m_currentCloud);
		ne.setKSearch(KdTreeSearhNum);
		ne.compute(*cloud_normals);


		// 创建用于圆柱体分割的分割对象并设置所有参数
		if (ModelOPTI == 0) { seg.setOptimizeCoefficients(true); }   //设置对估计模型优化
		else { seg.setOptimizeCoefficients(false); }
		seg.setModelType(pcl::SACMODEL_CYLINDER);  //设置分割模型为圆柱形
		seg.setMethodType(pcl::SAC_RANSAC);       //参数估计方法
		seg.setNormalDistanceWeight(NormalWeight);       //设置表面法线权重系数
		seg.setMaxIterations(iteration);              //设置迭代的最大次数
		seg.setDistanceThreshold(localpoint);         //设置内点到模型的距离允许最大值
		seg.setProbability(confidence);
		seg.setRadiusLimits(MinR, MaxR);             //设置估计出的圆柱模型的半径的范围
		seg.setInputCloud(m_currentCloud);
		seg.setInputNormals(cloud_normals);

		// 获取圆柱体内点和系数
		seg.segment(*inliers_cylinder, *coefficients_cylinder);

		text = "圆柱体参数: \n";
		for (int i = 0; i < coefficients_cylinder->values.size(); ++i) {
			text += QString("values[%1]: ").arg(i);
			text += QString::number(coefficients_cylinder->values[i]) + " \n";
		}
		text += "\n";

		// 提取圆柱体内点
		extract.setInputCloud(m_currentCloud);
		extract.setIndices(inliers_cylinder);
		if(ifReverse==0){ extract.setNegative(false); }
		else{ extract.setNegative(true); }
		extract.filter(*final);
	}
	// 圆锥体
	else if (model == 6)
	{
		// 创建法线估计对象
		pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
		// 创建KdTree对象用于近邻搜索
		pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
		// 输出存储对象
		pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);

		// 估计法线
		ne.setSearchMethod(tree);
		ne.setInputCloud(m_currentCloud);
		ne.setKSearch(KdTreeSearhNum);
		ne.compute(*cloud_normals);

		// 创建分割对象
		pcl::SACSegmentationFromNormals<pcl::PointXYZ, pcl::Normal> seg;
		if (ModelOPTI == 0) { seg.setOptimizeCoefficients(true); }   //设置对估计模型优化
		else { seg.setOptimizeCoefficients(false); }
		seg.setModelType(pcl::SACMODEL_CONE);  //设置分割模型为锥体
		seg.setMethodType(pcl::SAC_RANSAC);       //参数估计方法
		seg.setNormalDistanceWeight(NormalWeight);       //设置表面法线权重系数
		seg.setMaxIterations(iteration);              //设置迭代的最大次数
		seg.setDistanceThreshold(localpoint);         //设置内点到模型的距离允许最大值
		seg.setProbability(confidence);
		seg.setRadiusLimits(MinR, MaxR);             //设置估计出的圆锥体模型的半径的范围
		seg.setInputCloud(m_currentCloud);
		seg.setInputNormals(cloud_normals);

		// 定义存储对象
		pcl::PointIndices::Ptr inliers(new pcl::PointIndices());
		pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());

		// 执行圆锥体检测
		seg.segment(*inliers, *coefficients);

		text = "圆锥体参数: \n";
		for (int i = 0; i < coefficients->values.size(); ++i) {
			text += QString("values[%1]: ").arg(i);
			text += QString::number(coefficients->values[i]) + " \n";
		}
		text += "\n";

		// 提取圆锥体点云
		pcl::ExtractIndices<pcl::PointXYZ> extract;
		extract.setInputCloud(m_currentCloud);
		extract.setIndices(inliers);
		if (ifReverse == 0) { extract.setNegative(false); }
		else { extract.setNegative(true); }
		extract.filter(*final);
	}
	// 平行线
	else if (model == 7)
	{
		// 创建分割对象
		pcl::SACSegmentation<pcl::PointXYZ> seg;
		// 定义存储对象
		pcl::PointIndices::Ptr inliers(new pcl::PointIndices());
		pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients());
		if (ModelOPTI == 0) { seg.setOptimizeCoefficients(true); }   //设置对估计模型优化
		else { seg.setOptimizeCoefficients(false); }
		seg.setModelType(pcl::SACMODEL_PARALLEL_LINE); // 设置模型类型为平行线
		seg.setMethodType(pcl::SAC_RANSAC); // 使用RANSAC算法进行估计
		seg.setMaxIterations(iteration); // 设置最大迭代次数
		seg.setDistanceThreshold(localpoint); // 设置距离阈值
		seg.setProbability(confidence);
		// 设置输入点云
		seg.setInputCloud(m_currentCloud);
		// 执行平行线检测
		seg.segment(*inliers, *coefficients);

		// 提取平行线点云
		pcl::ExtractIndices<pcl::PointXYZ> extract;
		extract.setInputCloud(m_currentCloud);
		extract.setIndices(inliers);
		if (ifReverse == 0) { extract.setNegative(false); }
		else { extract.setNegative(true); }
		extract.filter(*final);
	}
	
	text += QString("分割后点数: %1 \n").arg(final->size());
	ui.infoTextEdit->appendPlainText(text);

	QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "删除非采样点?", QMessageBox::Yes | QMessageBox::No);
	if (result == QMessageBox::Yes)
	{
		*m_currentCloud = *final;
		m_currentCloud->resize(final->size());
		viewer->removeAllPointClouds();
		viewer->removeAllShapes();
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> final_color_handler(final, 0, 255, 0);
		viewer->addPointCloud<pcl::PointXYZ>(final, final_color_handler, "final");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "final");
		ui.qvtkWidget->update();
		int Cloudnum = final->size();
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	else
	{
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> final_color_handler(final, 0, 255, 0);
		viewer->removeAllPointClouds();
		viewer->removeAllShapes();
		viewer->addPointCloud(m_currentCloud);
		viewer->addPointCloud<pcl::PointXYZ>(final, final_color_handler, "final");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "final");
		ui.qvtkWidget->update();
		int Cloudnum = m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
}
void PCLvision::on_action_ECE_triggered()
{
	ecedlg.show();
}
void PCLvision::ECE(double ClusterTolerance, int MinClusterSize, int MaxClusterSize)
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	text.clear();
	ui.infoTextEdit->clear();
	// 为提取算法的搜索方法创建一个KdTree对象
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
	tree->setInputCloud(m_currentCloud);

	/**
	 * 在这里，我们创建一个PointIndices的vector，该vector在vector <int>中包含实际的索引信息。
	 * 每个检测到的簇的索引都保存在这里-请注意，cluster_indices是一个vector，包含多个检测到的簇的PointIndices的实例。
	 * 因此，cluster_indices[0]包含我们点云中第一个 cluster(簇)的所有索引。
	 *
	 * 从点云中提取簇（集群）,并将点云索引保存在 cluster_indices 中。
	 */
	std::vector<pcl::PointIndices> cluster_indices;
	pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;//因为点云是PointXYZ类型的，所以这里用PointXYZ创建一个欧氏聚类对象，并设置提取的参数和变量
	ec.setClusterTolerance(ClusterTolerance); // 设置一个合适的聚类搜索半径 ClusterTolerance，如果搜索半径取一个非常小的值，那么一个实际独立的对象就会被分割为多个聚类；如果将值设置得太高，那么多个对象就会被分割为一个聚类，所以需要进行测试找出最合适的ClusterTolerance
	ec.setMinClusterSize(MinClusterSize);    // 每个簇（集群）的最小大小
	ec.setMaxClusterSize(MaxClusterSize);  // 每个簇（集群）的最大大小
	ec.setSearchMethod(tree);     // 设置点云搜索算法
	ec.setInputCloud(m_currentCloud);   // 设置输入点云
	ec.extract(cluster_indices);        // 设置提取到的簇，将每个簇以索引的形式保存到cluster_indices;

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_save(new pcl::PointCloud<pcl::PointXYZ>);//用于保存提取后的点云
	// 为了从点云索引向量中分割出每个簇，必须迭代访问点云索引，
	int j = 0;
	for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin();it != cluster_indices.end(); ++it) {

		// 每次创建一个新的点云数据集，并且将所有当前簇的点写入到点云数据集中。
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
		const std::vector<int>& indices = it->indices;

		for (std::vector<int>::const_iterator pit = indices.begin(); pit != indices.end(); ++pit)
			cloud_cluster->points.push_back(m_currentCloud->points[*pit]);

		cloud_cluster->width = cloud_cluster->points.size();
		cloud_cluster->height = 1;
		cloud_cluster->is_dense = true;

		text += QString("点云簇 %1 : %2 个点 \n").arg(j+1).arg(cloud_cluster->points.size());
		text += "\n";

		std::stringstream ss;
		ss << "cloud_cluster_" << j;
		// 生成随机颜色
		pcl::visualization::PointCloudColorHandlerRandom<pcl::PointXYZ> single_color(cloud_cluster);
		viewer->addPointCloud<pcl::PointXYZ>(cloud_cluster, single_color, ss.str());
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, ss.str());
		j++;

		*cloud_save += *cloud_cluster;
	}
	viewer->addCoordinateSystem(0.5);

	*m_currentCloud = *cloud_save;
	m_currentCloud->resize(cloud_save->size());
	ui.qvtkWidget->update();
	int Cloudnum = cloud_save->size();
	QString Pnum = QString::number(Cloudnum);
	edit_num->setText(Pnum);

	text += QString("云簇数量: %1 \n").arg(j);
	ui.infoTextEdit->appendPlainText(text);
}
void PCLvision::on_action_RegionGrow_triggered()
{
	regiongrowdlg.show();
}
void PCLvision::RegionGrow(int KSearch, int NumberOfNeighbours, double SmoothnessThreshold, double CurvatureThreshold, int MinClusterSize, int MaxClusterSize)
{
	//设置搜索的方式或者说是结构　kd树　搜索
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
	//求法线　和　曲率　
	pcl::PointCloud <pcl::Normal>::Ptr normals(new pcl::PointCloud <pcl::Normal>);
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normal_estimator;
	normal_estimator.setSearchMethod(tree);
	normal_estimator.setInputCloud(m_currentCloud);
	normal_estimator.setKSearch(KSearch);//临近点
	normal_estimator.compute(*normals);

	//区域增长聚类分割对象　<点，法线>
	pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> reg;
	reg.setMinClusterSize(MinClusterSize);     //最小的聚类的点数
	reg.setMaxClusterSize(MaxClusterSize);//最大的聚类的点数
	reg.setSearchMethod(tree);     //搜索方式
	reg.setNumberOfNeighbours(NumberOfNeighbours); //设置搜索的邻域点的个数
	reg.setInputCloud(m_currentCloud);      //输入点
	//reg.setIndices (indices);
	reg.setInputNormals(normals);  //输入的法线
	reg.setSmoothnessThreshold(SmoothnessThreshold / 180.0 * M_PI);//设置平滑度 法线差值阈值
	reg.setCurvatureThreshold(CurvatureThreshold);                //设置曲率的阀值

	std::vector <pcl::PointIndices> clusters;
	reg.extract(clusters);//提取点的索引

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_save(new pcl::PointCloud<pcl::PointXYZ>);//用于保存提取后的点云
	// 为了从点云索引向量中分割出每个簇，必须迭代访问点云索引，
	int j = 0;
	for (std::vector<pcl::PointIndices>::const_iterator it = clusters.begin(); it != clusters.end(); ++it) {

		// 每次创建一个新的点云数据集，并且将所有当前簇的点写入到点云数据集中。
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_cluster(new pcl::PointCloud<pcl::PointXYZ>);
		const std::vector<int>& indices = it->indices;

		for (std::vector<int>::const_iterator pit = indices.begin(); pit != indices.end(); ++pit)
			cloud_cluster->points.push_back(m_currentCloud->points[*pit]);

		cloud_cluster->width = cloud_cluster->points.size();
		cloud_cluster->height = 1;
		cloud_cluster->is_dense = true;

		text += QString("点云簇 %1 : %2 个点 \n").arg(j + 1).arg(cloud_cluster->points.size());
		text += "\n";

		std::stringstream ss;
		ss << "cloud_cluster_" << j;
		// 生成随机颜色
		pcl::visualization::PointCloudColorHandlerRandom<pcl::PointXYZ> single_color(cloud_cluster);
		viewer->addPointCloud<pcl::PointXYZ>(cloud_cluster, single_color, ss.str());
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, ss.str());
		j++;

		*cloud_save += *cloud_cluster;
	}
	viewer->addCoordinateSystem(0.5);

	*m_currentCloud = *cloud_save;
	m_currentCloud->resize(cloud_save->size());
	ui.qvtkWidget->update();
	int Cloudnum = cloud_save->size();
	QString Pnum = QString::number(Cloudnum);
	edit_num->setText(Pnum);

	text += QString("云簇数量: %1 \n").arg(j);
	ui.infoTextEdit->appendPlainText(text);

}
//表面法线估计
void PCLvision::on_action_Normal3D_triggered()
{
	normal3ddlg.show();
}
void PCLvision::Normal3D(double SearchRadius, int Level, double Scale)
{
	// 法线对象
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);

	// 用于法线估计的对象
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normalEstimation;
	normalEstimation.setInputCloud(m_currentCloud);
	// 对于每个点，使用半径为SearchRadius的所有邻域
	normalEstimation.setRadiusSearch(SearchRadius);
	// 通过kd树使搜索更加高效
	// 法线估计对象将使用它来查找最近的邻点
	pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
	normalEstimation.setSearchMethod(kdtree);
	// 计算法线
	normalEstimation.compute(*normals);

	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	viewer->addPointCloud(m_currentCloud);
	// 参数int level表示每Level个点绘制一个法向量
	// 参数float scale表示法向量长度缩放为Scale倍
	viewer->addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(m_currentCloud, normals, Level, Scale, "normals");
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 1, 0, 0, "normals");//点云法线为红色
	ui.qvtkWidget->update();
}
void PCLvision::on_action_IntegralImage_triggered()
{
	integralimagedlg.show();
}
void PCLvision::IntegralImage(int EstimationMethod, double DepthChangeFactor, double NormalSmoothingSize, int Level, double Scale)
{
	// 法线对象
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);

	// 创建一个用于法线估计的对象并计算法线
	pcl::IntegralImageNormalEstimation<pcl::PointXYZ, pcl::Normal> ne;
	// 有以下可选的估算方式：
	/**
	enum NormalEstimationMethod
	{
	  COVARIANCE_MATRIX,
	  AVERAGE_3D_GRADIENT,
	  AVERAGE_DEPTH_CHANGE
	};
	 COVARIANCE_MATRIX模式创建9个积分图像，以根据其局部邻域的协方差矩阵为特定点计算法线。
	 AVERAGE_3D_GRADIENT模式创建6个积分图像以计算水平和垂直3D渐变的平滑版本，并使用这两个渐变之间的叉积计算法线。
	 AVERAGE_DEPTH_CHANGE模式仅创建单个积分图像，并根据平均深度变化计算法线。
	 */
	if (EstimationMethod == 0){ ne.setNormalEstimationMethod(ne.COVARIANCE_MATRIX); }
	else if (EstimationMethod == 1) { ne.setNormalEstimationMethod(ne.AVERAGE_3D_GRADIENT); }
	else if (EstimationMethod == 2) { ne.setNormalEstimationMethod(ne.AVERAGE_DEPTH_CHANGE); }
	ne.setMaxDepthChangeFactor(DepthChangeFactor);//设置深度变化的阀值
	ne.setNormalSmoothingSize(NormalSmoothingSize);//设置计算法线的区域
	ne.setInputCloud(m_currentCloud);
	ne.compute(*normals);//计算

	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	viewer->addPointCloud(m_currentCloud);
	viewer->addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(m_currentCloud, normals, Level, Scale, "normals");
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR, 1, 0, 0, "normals");//点云法线为红色
	ui.qvtkWidget->update();
}
//配准
void PCLvision::on_action_NDT_triggered()
{
	ndtdlg.show();
}
void PCLvision::NDT(double TransformationEpsilon, double StepSize, double Resolution, int MaximumIterations, int Axis, double AngleRotation, double X, double Y, double Z, double leaf)
{
	text.clear();
	ui.infoTextEdit->clear();

	// 加载目标点云 target_cloud
	std::string mytargetpath = ndtdlg.pathtarget.toStdString();
	pcl::PointCloud<pcl::PointXYZ>::Ptr target_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	// 加载场景点云 target_cloud
	std::string myscenepath = ndtdlg.pathscene.toStdString();
	pcl::PointCloud<pcl::PointXYZ>::Ptr scene_cloud(new pcl::PointCloud<pcl::PointXYZ>);

	if (pcl::io::loadPCDFile<pcl::PointXYZ>(mytargetpath, *target_cloud) == -1 || pcl::io::loadPCDFile<pcl::PointXYZ>(myscenepath, *scene_cloud) == -1) {
		QMessageBox::warning(this, "警告", "无效的点云文件！");
		return;
	}
	text += QString("目标点云点数:%1 \n").arg(target_cloud->size());
	text += "\n";
	text += QString("场景点云点数:%1 \n").arg(scene_cloud->size());
	text += "\n";

	// 过滤输入点云到原始大小的约10%，以提高配准速度
	pcl::PointCloud<pcl::PointXYZ>::Ptr filtered_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::ApproximateVoxelGrid<pcl::PointXYZ> approximate_voxel_filter;
	approximate_voxel_filter.setLeafSize(leaf, leaf, leaf);
	approximate_voxel_filter.setInputCloud(scene_cloud);
	approximate_voxel_filter.filter(*filtered_cloud);

	// 使用默认值创建NDT对象。内部数据结构直到稍后才会初始化。
	pcl::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ> ndt;

	// 根据输入数据的尺度设置NDT相关参数
	// 设置终止条件的最小转换差
	ndt.setTransformationEpsilon(TransformationEpsilon);
	// 设置More Thuente行搜索的最大步长
	ndt.setStepSize(StepSize);
	// 设置NDT网格结构的分辨率（VoxelGridCovariance）
	ndt.setResolution(Resolution);
	/*-------------------------------------------------------------------------------------------------------------------------
	以上代码设置一些尺度相关的参数,因为 NDT 算法使用一个体素化数据结构和More-Thuente行搜索，因此需要缩放一些参数来适应数据集。
	以上参数看起来在我们使用的房间尺寸比例下运行地很好，但是它们如果需要处理例如一个咖啡杯的扫描之类更小物体，需要对参数进行很
	大程度的缩小。在变换中 Epsilon 参数分别从长度和弧度，定义了变换矢量[ x, y, z,roll,pitch, yaw]的最小许可的递增量，一旦递增量
	减小到这个临界值以下 ，那么配准算法就将终止。步长StepSize参数定义了 More-Thuente 行搜索允许的最大步长，这个行搜索算法确定了
	最大值以下的最佳步长，当靠近最优解时该算法会缩短迭代步长，在更大的最大步长将会在较少的迭代次数下遍历较大的距离，但是却有过
	度迭代和在不符合要求的局部最小值处结束的风险。
	-------------------------------------------------------------------------------------------------------------------------*/

	// 设置配准迭代的最大次数
	ndt.setMaximumIterations(MaximumIterations);
	/*-------------------------------------------------------------------------------------------------------------------------
	这个MaximumIterations参数控制了优化程序运行的最大迭代次数，一般来说，在达到这个限制值之前优化程序就会在 epsilon 变换阈值下
	终止。添加此最大迭代次数限制能够增加程序鲁棒性,阻止了它在错误的方向运行太久。
	-------------------------------------------------------------------------------------------------------------------------*/

	// 设置过滤后的输入源点云（第二次扫描数据）
	ndt.setInputSource(filtered_cloud);
	// 设置目标点云（第一次扫描数据），作为对其配准的目标
	ndt.setInputTarget(target_cloud);
	/*-------------------------------------------------------------------------------------------------------------------------
	这里，我们把点云赋给 NDT 配准对象，目标点云的坐标系是被匹配的输入点云的参考坐标系，匹配完成后输入点云将被变换到与目标点云同
	一坐标系下，当加载目标点云后，NDT 算法的内部数据结构被初始化。
	-------------------------------------------------------------------------------------------------------------------------*/

	// 设置使用机器人测距法得到的粗略初始变换矩阵
	Eigen::AngleAxisf init_rotation; // 先声明一个空的 AngleAxisf 对象
	if(Axis==0){ init_rotation = Eigen::AngleAxisf(AngleRotation, Eigen::Vector3f::UnitX()); }
	else if (Axis == 1){ init_rotation = Eigen::AngleAxisf(AngleRotation, Eigen::Vector3f::UnitY());}
	else if (Axis == 2){ init_rotation = Eigen::AngleAxisf(AngleRotation, Eigen::Vector3f::UnitZ());}
	else if (Axis == 3){ init_rotation = Eigen::AngleAxisf(AngleRotation, Eigen::Vector3f::UnitW());}
	Eigen::Translation3f init_translation(X, Y, Z);
	Eigen::Matrix4f init_guess = (init_translation * init_rotation).matrix();
	/*-------------------------------------------------------------------------------------------------------------------------
	以上代码块中创建了一个点云配准变换矩阵的初始估计，虽然算法运行并不需要这样的一个初始变换矩阵，但是有了它容易得到更好的结果，
	尤其是当参考坐标系之间有较大差异时（本例即是），在机器人应用程序（例如用于生成此数据集的应用程序）中，通常使用里程表数据生
	成初始转换。
	-------------------------------------------------------------------------------------------------------------------------*/

	// 计算所需的刚体变换，以使输入点云与目标点云对齐
	pcl::PointCloud<pcl::PointXYZ>::Ptr output_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	ndt.align(*output_cloud, init_guess);
	text += QString("匹配分数:%1 \n").arg(ndt.getFitnessScore());
	text += "\n";

	// 使用找到的变换矩阵，来对未过滤的输入点云进行变换
	pcl::transformPointCloud(*scene_cloud, *output_cloud, ndt.getFinalTransformation());

	*m_currentCloud = *output_cloud+*target_cloud;
	m_currentCloud->resize(output_cloud->size()+ target_cloud->size());
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	// 着色并可视化目标点云（红色）
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> target_color(target_cloud, 255, 0, 0);
	viewer->addPointCloud<pcl::PointXYZ>(target_cloud, target_color, "target cloud");
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,3, "target cloud");

	// 着色并可视化转换后的输入点云（绿色）
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> output_color(output_cloud, 0, 255, 0);
	viewer->addPointCloud<pcl::PointXYZ>(output_cloud, output_color, "output cloud");
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,3, "output cloud");

	// 可视化
	viewer->addCoordinateSystem(1.0, "global");
	viewer->initCameraParameters();

	ui.qvtkWidget->update();
	int Cloudnum = output_cloud->size() + target_cloud->size();
	QString Pnum = QString::number(Cloudnum);
	edit_num->setText(Pnum);
	ui.infoTextEdit->appendPlainText(text);
}
void PCLvision::on_action_ICP_triggered()
{
	icpdlg.show();
}
void PCLvision::ICP(double leaf, double NormalRadius, double FeatureRadius, int RansacNum, int NumberOfSamples, int CorrespondenceRandomness, double SimilarityThreshold, double MaxCorrespondenceDistance, double InlierFraction)
{
	// 实例化必要的数据容器，检查输入参数并加载对象和场景点云
    PointCloudNT::Ptr object(new PointCloudNT);
    PointCloudNT::Ptr object_aligned(new PointCloudNT);
    PointCloudNT::Ptr scene(new PointCloudNT);
    FeatureCloudT::Ptr object_features(new FeatureCloudT);
    FeatureCloudT::Ptr scene_features(new FeatureCloudT);

	std::string mytargetpath = icpdlg.pathtarget.toStdString();
	std::string myscenepath = icpdlg.pathscene.toStdString();
 
    // 加载对象和场景
    if (pcl::io::loadPCDFile<PointNT>(mytargetpath, *object) < 0 || pcl::io::loadPCDFile<PointNT>(myscenepath, *scene) < 0)
    {
		QMessageBox::warning(this, "警告", "无效的点云文件！");
        return;
    }
 
    // 为了加快处理速度，使用PCL的：pcl::VoxelGrid类将对象和场景点云的采样率下采样
    pcl::VoxelGrid<PointNT> grid;
    grid.setLeafSize(leaf, leaf, leaf);
    grid.setInputCloud(object);
    grid.filter(*object);
    grid.setInputCloud(scene);
    grid.filter(*scene);
 
    // 估计场景的法线
    pcl::console::print_highlight("Estimating scene normals...\n");
    pcl::NormalEstimationOMP<PointNT, PointNT> nest;
    nest.setRadiusSearch(NormalRadius);
    nest.setInputCloud(scene);
    nest.compute(*scene);
 
    // 特征估计
    // 对于下采样点云中的每个点，使用PCL的pcl::FPFHEstimationOMP<>类来计算对齐过程中用于匹配的快速点特征直方图（FPFH）描述符。
    pcl::console::print_highlight("Estimating features...\n");
    FeatureEstimationT fest;
    fest.setRadiusSearch(FeatureRadius);
    fest.setInputCloud(object);
    fest.setInputNormals(object);
    fest.compute(*object_features);
    fest.setInputCloud(scene);
    fest.setInputNormals(scene);
    fest.compute(*scene_features);
 
    // 对齐配准对象创建与配置
    // SampleConsensusPrerejective 实现了有效的RANSAC姿态估计循环
    pcl::console::print_highlight("Starting alignment...\n");
    pcl::SampleConsensusPrerejective<PointNT, PointNT, FeatureT> align;
    align.setInputSource(object);
    align.setSourceFeatures(object_features);
    align.setInputTarget(scene);
    align.setTargetFeatures(scene_features);
    align.setMaximumIterations(RansacNum); // RANSAC 迭代次数
    align.setNumberOfSamples(NumberOfSamples); // 样本数-在对象和场景之间进行采样的点对应数。至少需要3个点才能计算姿态
    align.setCorrespondenceRandomness(CorrespondenceRandomness); // 对应随机性-我们可以在N个最佳匹配之间随机选择，而不是将每个对象FPFH描述符匹配到场景中最接近的匹配特征。这增加了必要的迭代，但也使算法对异常匹配具有鲁棒性
    align.setSimilarityThreshold(SimilarityThreshold); // 多边形相似度阈值-对齐类使用pcl::registration::CorrespondenceRejectorPoly类，根据采样点之间距离的位置不变的几何一致性，尽早消除不良姿态。在物体和场景上， 将该值设置为越接近1，则贪婪程度越高，从而使算法变得更快。但是，这也会增加存在噪音时消除好姿态的风险
    align.setMaxCorrespondenceDistance(MaxCorrespondenceDistance * leaf); // 内在阈值-这是欧几里德距离阈值，用于确定变换后的对象点是否正确对齐到最近的场景点。在此示例中，我们使用的启发式值为点云分辨率的2.5倍
    align.setInlierFraction(InlierFraction); // Inlier分数-在许多实际情况下，由于杂波，遮挡或两者兼而有之，场景中观察到的对象的大部分都不可见。在这种情况下，我们需要考虑不会将所有对象点都用于对准场景的姿态假设（猜想）。正确对齐的点的绝对数量是使用inlier阈值确定的，并且如果该数量与对象中总点数的比大于指定的inlier分数，则我们接受姿态假设（猜想）为有效
	// 执行配准并输出结果
	{
		pcl::ScopeTime t("Alignment");
		align.align(*object_aligned);
	}//对齐的对象存储在点云object_aligned中。如果找到了一个具有足够inliers的姿态（占对象点总数的25％以上），则该算法会收敛，并且我们可以打印并可视化结果
	
	if (align.hasConverged())
    {
		// 打印结果
		printf("\n");
		Eigen::Matrix4f transformation = align.getFinalTransformation();
		
		text.clear();
		ui.infoTextEdit->clear();
		text += "\n";
		text += QString("    | %6.3 %6.3 %6.3 | \n").arg(transformation(0, 0)).arg(transformation(0, 1)).arg(transformation(0, 2));
		text += QString("R = | %6.3 %6.3 %6.3 | \n").arg(transformation(1, 0)).arg(transformation(1, 1)).arg(transformation(1, 2));
		text += QString("    | %6.3 %6.3 %6.3 | \n").arg(transformation(2, 0)).arg(transformation(2, 1)).arg(transformation(2, 2));
		text += "\n";
		text += QString("t = < %0.3, %0.3, %0.3 >\n").arg(transformation(0, 3)).arg(transformation(1, 3)).arg(transformation(2, 3));
		text += "\n";
		text += QString("内点: %1/%2\n").arg(align.getInliers().size()).arg(object->size());

		ui.infoTextEdit->appendPlainText(text);

		m_currentCloud->clear();
		for (size_t i = 0; i < object_aligned->size(); ++i) {
			pcl::PointXYZ point;
			point.x = object_aligned->points[i].x;
			point.y = object_aligned->points[i].y;
			point.z = object_aligned->points[i].z;
			m_currentCloud->push_back(point);
		}
		for (size_t i = 0; i < scene->size(); ++i) {
			pcl::PointXYZ point;
			point.x = scene->points[i].x;
			point.y = scene->points[i].y;
			point.z = scene->points[i].z;
			m_currentCloud->push_back(point);
		}

        // 显示
		viewer->removeAllPointClouds();
		viewer->removeAllShapes();
		// 着色并可视化目标点云（红色）
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointNormal> target_color(scene, 255, 0, 0);
		viewer->addPointCloud<pcl::PointNormal>(scene, target_color, "scene cloud");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "scene cloud");

		// 着色并可视化转换后的输入点云（绿色）
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointNormal> output_color(object_aligned, 0, 255, 0);
		viewer->addPointCloud<pcl::PointNormal>(object_aligned, output_color, "object cloud");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "object cloud");

		ui.qvtkWidget->update();
		int Cloudnum = object_aligned->size() + scene->size();
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
    }
    else
    {
		QMessageBox::information(this, "提示", "配准失败！");
        return;
    }
}
//曲面重建
void PCLvision::on_action_Polygon_triggered()
{
	polygondlg.show();
}
void PCLvision::Polygon(double Alpha)
{
	// 存储提取多边形上的点云
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_hull(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::ConcaveHull<pcl::PointXYZ> chull;        //创建多边形提取对象
	chull.setInputCloud(m_currentCloud);       //设置输入点云为提取后点云
	chull.setAlpha(Alpha);
	chull.reconstruct(*cloud_hull);   //创建提取创建多边形

	QMessageBox::StandardButton result = QMessageBox::question(this, "提示", "删除非采样点?", QMessageBox::Yes | QMessageBox::No);
	if (result == QMessageBox::Yes)
	{
		*m_currentCloud = *cloud_hull;
		m_currentCloud->resize(cloud_hull->size());
		viewer->removeAllPointClouds();
		viewer->removeAllShapes();
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> final_color_handler(cloud_hull, 255, 255, 0);
		viewer->addPointCloud<pcl::PointXYZ>(cloud_hull, final_color_handler, "hull");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "hull");
		ui.qvtkWidget->update();
		int Cloudnum = cloud_hull->size();
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
	else
	{
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> final_color_handler(cloud_hull, 255, 255, 0);
		viewer->removeShape("hull");
		viewer->addPointCloud<pcl::PointXYZ>(cloud_hull, final_color_handler, "hull");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 5, "hull");
		ui.qvtkWidget->update();
		int Cloudnum = m_currentCloud->width;
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
	}
}
void PCLvision::on_action_GP3_triggered()
{
	gp3dlg.show();
}
void PCLvision::GP3(int KSearch, double SearchRadius, double Mu, int MaximumNearestNeighbors, double MaximumSurfaceAngle, double MinimumAngle, double MaximumAngle, int NormalConsistency)
{
	// 正态估计
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;   //法线估计对象
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);   //存储估计的法线
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);  //定义kd树指针
	tree->setInputCloud(m_currentCloud);   //用cloud构建tree对象
	n.setInputCloud(m_currentCloud);
	n.setSearchMethod(tree);
	n.setKSearch(KSearch);
	n.compute(*normals);   //估计法线存储到其中
	//* 法线不应包含点法线+曲面曲率

	// 连接XYZ字段和法线字段*
	pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
	pcl::concatenateFields(*m_currentCloud, *normals, *cloud_with_normals);    //连接字段
	//* cloud_with_normals = cloud + normals

	// 定义搜索树对象
	pcl::search::KdTree<pcl::PointNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointNormal>);
	tree2->setInputCloud(cloud_with_normals);   //点云构建搜索树

	// 初始化对象
	pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;   //定义三角化对象
	pcl::PolygonMesh triangles;                //存储最终三角化的网络模型

	// 设置连接点之间的最大距离（最大边长）
	gp3.setSearchRadius(SearchRadius);  //设置连接点之间的最大距离，（即是三角形最大边长）

	// 设置各参数值
	gp3.setMu(Mu);  //设置被样本点搜索其近邻点的最远距离为2.5，为了使用点云密度的变化
	gp3.setMaximumNearestNeighbors(MaximumNearestNeighbors);    //设置样本点可搜索的邻域个数
	gp3.setMaximumSurfaceAngle(MaximumSurfaceAngle * M_PI / 180.0); // 设置某点法线方向偏离样本点法线的最大角度
	gp3.setMinimumAngle(MinimumAngle * M_PI / 180.0); // 设置三角化后得到的三角形内角的最小的角度为
	gp3.setMaximumAngle(MaximumAngle * M_PI / 180.0); // 设置三角化后得到的三角形内角的最大角度为
	if(NormalConsistency==0){gp3.setNormalConsistency(false);}
	else { gp3.setNormalConsistency(true); }

	// 获取结果
	gp3.setInputCloud(cloud_with_normals);     //设置输入点云为有向点云
	gp3.setSearchMethod(tree2);   //设置搜索方式
	gp3.reconstruct(triangles);  //重建提取三角化

	// 附加顶点信息
	std::vector<int> parts = gp3.getPartIDs();
	std::vector<int> states = gp3.getPointStates();

	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	// 添加网格到可视化窗口
	viewer->addPolygonMesh(triangles, "mesh");
	// 设置观察点和方向
	//viewer->setCameraPosition(0, 0, -2, 0, -1, 0);
	ui.qvtkWidget->update();
}
//3D包容盒子
void PCLvision::on_action_BOX3D_triggered()
{
	box3ddlg.show();
}
void PCLvision::BOX3D(int BOXtype, int show)
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	text.clear();
	ui.infoTextEdit->clear();
	// 创建惯性矩估算对象，设置输入点云，并进行计算
	pcl::MomentOfInertiaEstimation <pcl::PointXYZ> feature_extractor;
	feature_extractor.setInputCloud(m_currentCloud);
	feature_extractor.compute();

	std::vector <float> moment_of_inertia;
	std::vector <float> eccentricity;
	// 获取惯性矩
	feature_extractor.getMomentOfInertia(moment_of_inertia);
	// 获取离心率
	feature_extractor.getEccentricity(eccentricity);

	if (BOXtype == 0)
	{
		pcl::PointXYZ min_point;
		pcl::PointXYZ max_point;
		pcl::getMinMax3D(*m_currentCloud, min_point, max_point);
		double radius = sqrt(pow(max_point.x - min_point.x, 2) + pow(max_point.y - min_point.y, 2) + pow(max_point.z - min_point.z, 2)) / 2;
		pcl::PointXYZ center((max_point.x + min_point.x) / 2, (max_point.y + min_point.y) / 2, (max_point.z + min_point.z) / 2);
		text += "半径: ";
		text += QString("%1 \n").arg(radius);
		text += "\n";
		viewer->addSphere(center, radius, 1.0, 1.0, 0.0, "Bounding Sphere");
		viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY, 0.5, "Bounding Sphere");
	}
	else if (BOXtype == 1)
	{
		pcl::PointXYZ min_point_AABB;
		pcl::PointXYZ max_point_AABB;
		// 获取AABB盒子
		feature_extractor.getAABB(min_point_AABB, max_point_AABB);
		text += "长(dx): ";
		text += QString("%1 \n").arg(max_point_AABB.x - min_point_AABB.x);
		text += "宽(dy): ";
		text += QString("%1 \n").arg(max_point_AABB.y - min_point_AABB.y);
		text += "高(dz): ";
		text += QString("%1 \n").arg(max_point_AABB.z - min_point_AABB.z);
		text += "\n";
		// 添加AABB包容盒
		viewer->addCube(min_point_AABB.x, max_point_AABB.x, min_point_AABB.y, max_point_AABB.y, min_point_AABB.z, max_point_AABB.z, 1.0, 1.0, 0.0, "AABB");
		viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_WIREFRAME, "AABB");

	}
	else if (BOXtype == 2)
	{
		pcl::PointXYZ min_point_OBB;
		pcl::PointXYZ max_point_OBB;
		pcl::PointXYZ position_OBB;
		Eigen::Matrix3f rotational_matrix_OBB;
		// 获取OBB盒子
		feature_extractor.getOBB(min_point_OBB, max_point_OBB, position_OBB, rotational_matrix_OBB);

		// 添加OBB包容盒
		Eigen::Vector3f position(position_OBB.x, position_OBB.y, position_OBB.z);
		Eigen::Quaternionf quat(rotational_matrix_OBB);
		// position：中心位置
		// quat：旋转矩阵
		// max_point_OBB.x - min_point_OBB.x  宽度
		// max_point_OBB.y - min_point_OBB.y  高度
		// max_point_OBB.z - min_point_OBB.z  深度
		text += "\n";
		text += QString("\t[%1\t%2\t%3\t] \n").arg(rotational_matrix_OBB(0, 0)).arg(rotational_matrix_OBB(0, 1)).arg(rotational_matrix_OBB(0, 2));
		text += QString("R=\t[%1\t%2\t%3\t] \n").arg(rotational_matrix_OBB(1, 0)).arg(rotational_matrix_OBB(1, 1)).arg(rotational_matrix_OBB(1, 2));
		text += QString("\t[%1\t%2\t%3\t] \n \n").arg(rotational_matrix_OBB(2, 0)).arg(rotational_matrix_OBB(2, 1)).arg(rotational_matrix_OBB(2, 2));
		text += "长(x): ";
		text += QString("%1 \n").arg(max_point_OBB.x - min_point_OBB.x);
		text += "宽(y): ";
		text += QString("%1 \n").arg(max_point_OBB.y - min_point_OBB.y);
		text += "高(z): ";
		text += QString("%1 \n").arg(max_point_OBB.z - min_point_OBB.z);
		text += "\n";
		viewer->addCube(position, quat, max_point_OBB.x - min_point_OBB.x, max_point_OBB.y - min_point_OBB.y, max_point_OBB.z - min_point_OBB.z, "OBB");
		viewer->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_REPRESENTATION, pcl::visualization::PCL_VISUALIZER_REPRESENTATION_WIREFRAME, "OBB");

	}
	else if (BOXtype == 3)
	{
		pcl::ConvexHull<pcl::PointXYZ> hull;
		hull.setInputCloud(m_currentCloud);
		hull.setDimension(3);

		std::vector<pcl::Vertices> polygons;
		pcl::PointCloud<pcl::PointXYZ>::Ptr surface_hull(new pcl::PointCloud<pcl::PointXYZ>);
		hull.reconstruct(*surface_hull, polygons);

		cout << surface_hull->size();

		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> color_handlerK(surface_hull, 255, 0, 0);
		viewer->addPointCloud(surface_hull, color_handlerK, "point");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 6, "point");

		if (show == 0) {viewer->addPolygon<pcl::PointXYZ>(surface_hull, 0, 0, 255, "polyline");}
		else { viewer->addPolygonMesh<pcl::PointXYZ>(surface_hull, polygons, "polyline"); }
	}

	float major_value, middle_value, minor_value;
	Eigen::Vector3f major_vector, middle_vector, minor_vector;
	Eigen::Vector3f mass_center;
	feature_extractor.getEigenValues(major_value, middle_value, minor_value);
	// 获取主轴major_vector，中轴middle_vector，辅助轴minor_vector
	feature_extractor.getEigenVectors(major_vector, middle_vector, minor_vector);
	// 获取质心
	feature_extractor.getMassCenter(mass_center);

	viewer->addCoordinateSystem(1.0);
	viewer->addPointCloud<pcl::PointXYZ>(m_currentCloud, "sample cloud");

	pcl::PointXYZ center(mass_center(0), mass_center(1), mass_center(2));
	pcl::PointXYZ x_axis(major_vector(0) + mass_center(0), major_vector(1) + mass_center(1), major_vector(2) + mass_center(2));
	pcl::PointXYZ y_axis(middle_vector(0) + mass_center(0), middle_vector(1) + mass_center(1), middle_vector(2) + mass_center(2));
	pcl::PointXYZ z_axis(minor_vector(0) + mass_center(0), minor_vector(1) + mass_center(1), minor_vector(2) + mass_center(2));
	viewer->addLine(center, x_axis, 1.0f, 0.0f, 0.0f, "major eigen vector");
	viewer->addLine(center, y_axis, 0.0f, 1.0f, 0.0f, "middle eigen vector");
	viewer->addLine(center, z_axis, 0.0f, 0.0f, 1.0f, "minor eigen vector");

	ui.qvtkWidget->update();

	text += QString("质心( %1 , %2 , %3 ) \n").arg(mass_center(0)).arg(mass_center(1)).arg(mass_center(2));
	text += "\n";

	ui.infoTextEdit->appendPlainText(text);
}
//模板匹配
void PCLvision::on_action_TemplateMatching_triggered()
{
	templatematchingdlg.show();
}
void PCLvision::TemplateMatching()
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();
	// 加载对象模板
	std::vector<FeatureCloud> object_templates;
	object_templates.resize(0);

	// 加载特征云
	std::string mytemplatepath = templatematchingdlg.pathTemplate.toStdString();
	FeatureCloud template_cloud;
	template_cloud.loadInputCloud(mytemplatepath);
	object_templates.push_back(template_cloud);

	// 对齐到目标特征点云
	FeatureCloud target_cloud;
	target_cloud.setInputCloud(m_currentCloud);

	// 设置TemplateAlignment输入
	TemplateAlignment template_align;
	for (size_t i = 0; i < object_templates.size(); i++) {
		FeatureCloud& object_template = object_templates[i];
		// 添加模板点云
		template_align.addTemplateCloud(object_template);
	}
	// 设置目标点云
	template_align.setTargetCloud(target_cloud);

	// 查找最佳模板对齐方式
	// 核心代码
	TemplateAlignment::Result best_alignment;
	int best_index = template_align.findBestAlignment(best_alignment);
	const FeatureCloud& best_template = object_templates[best_index];

	// 打印匹配度分数（小于0.00002的值是好的）
	text.clear();
	ui.infoTextEdit->clear();
	text += "\n";
	text += QString("匹配度分数(<0.00002): %6.3\n").arg(best_alignment.fitness_score);
	text += QString("最佳匹配索引: %1\n").arg(best_index);

	// 打印旋转矩阵和平移向量
	Eigen::Matrix3f rotation = best_alignment.final_transformation.block<3, 3>(0, 0);
	Eigen::Vector3f translation = best_alignment.final_transformation.block<3, 1>(0, 3);
	Eigen::Vector3f euler_angles = rotation.eulerAngles(2, 1, 0) * 180 / M_PI;

	text += "\n";
	text += QString("    | %1 %2 %3 | \n").arg(rotation(0, 0)).arg(rotation(0, 1)).arg(rotation(0, 2));
	text += QString("R = | %1 %2 %3 | \n").arg(rotation(1, 0)).arg(rotation(1, 1)).arg(rotation(1, 2));
	text += QString("    | %1 %2 %3 | \n").arg(rotation(2, 0)).arg(rotation(2, 1)).arg(rotation(2, 2));
	text += "\n";
	text += "yaw(z) pitch(y) roll(x) = ";
	text += QString::number(euler_angles[0]) + ", ";
	text += QString::number(euler_angles[1]) + ", ";
	text += QString::number(euler_angles[2]) + "\n";
	text += "\n";
	text += QString("t = < %1, %2, %3 >\n").arg(translation(0)).arg(translation(1)).arg(translation(2));
	text += "\n";
	text += "白色-目标点云\n红色-模板点云\n蓝色-旋转后点云\n";

	ui.infoTextEdit->appendPlainText(text);

	// 保存对齐的模板以进行可视化
	pcl::PointCloud<pcl::PointXYZ> transformed_cloud;
	// 将模板中保存的点云图进行旋转矩阵变换，把变换结果保存到transformed_cloud
	pcl::transformPointCloud(*best_template.getPointCloud(), transformed_cloud, best_alignment.final_transformation);
	//pcl::io::savePCDFileBinary("output.pcd", transformed_cloud);

	// 旋转后的点云rotated --------------------------------
	pcl::PointCloud<pcl::PointXYZ>::Ptr t_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	for (size_t i = 0; i < transformed_cloud.size(); ++i) {
		pcl::PointXYZ point;
		point.x = transformed_cloud.points[i].x;
		point.y = transformed_cloud.points[i].y;
		point.z = transformed_cloud.points[i].z;
		t_cloud->push_back(point);
	}
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> transformed_cloud_handler(t_cloud, 0, 0, 255);
	viewer->addPointCloud(t_cloud, transformed_cloud_handler, "transformed_cloud");
	// 设置渲染属性（点大小）
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "transformed_cloud");

	// 目标点云target --------------------------------
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> target_cloud_handler(m_currentCloud, 255, 255, 255);
	viewer->addPointCloud(m_currentCloud, target_cloud_handler, "target_cloud");
	// 设置渲染属性（点大小）
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "target_cloud");

	// 模板点云template --------------------------------
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> template_cloud_handler(template_cloud.getPointCloud(), 255, 0, 0);
	viewer->addPointCloud(template_cloud.getPointCloud(), template_cloud_handler, "template_cloud");
	// 设置渲染属性（点大小）
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "template_cloud");
	
	// 设置坐标系系统
	viewer->addCoordinateSystem(1.0);
	ui.qvtkWidget->update();

	*m_currentCloud = *t_cloud + *m_currentCloud + *best_template.getPointCloud();
	m_currentCloud->resize(t_cloud->size() + m_currentCloud->size() + best_template.getPointCloud()->size());
	int Cloudnum = t_cloud->size() + m_currentCloud->size() + best_template.getPointCloud()->size();
	QString Pnum = QString::number(Cloudnum);
	edit_num->setText(Pnum);
}
//从3D图生成点云
void PCLvision::on_action_Generate_triggered()
{
	generatedlg.show();
}
void PCLvision::Generate(int FileType)
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();

	std::string mymodelpath = generatedlg.pathModel.toStdString();
	int SAMPLE_POINTS_ = generatedlg.default_number_samples;
	float leaf_size = generatedlg.default_leaf_size;
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_1(new pcl::PointCloud<pcl::PointXYZ>);

	vtkSmartPointer<vtkPolyData> polydata1;
	if (FileType == 0)
	{
		pcl::PolygonMesh mesh;
		pcl::io::loadPolygonFilePLY(mymodelpath.c_str(), mesh);
		pcl::io::mesh2vtk(mesh, polydata1);
	}
	else if (FileType == 1)
	{
		vtkSmartPointer<vtkOBJReader> readerQuery = vtkSmartPointer<vtkOBJReader>::New();
		readerQuery->SetFileName(mymodelpath.c_str());
		readerQuery->Update();
		polydata1 = readerQuery->GetOutput();
	}

	//确保多边形是三角形！
	vtkSmartPointer<vtkTriangleFilter> triangleFilter = vtkSmartPointer<vtkTriangleFilter>::New();
#if VTK_MAJOR_VERSION < 6
	triangleFilter->SetInput(polydata1);
#else
	triangleFilter->SetInputData(polydata1);
#endif
	triangleFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper> triangleMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	triangleMapper->SetInputConnection(triangleFilter->GetOutputPort());
	triangleMapper->Update();
	polydata1 = triangleMapper->GetInput();

	generatedlg.uniform_sampling(polydata1, SAMPLE_POINTS_, *cloud_1);

	// Voxelgrid
	pcl::VoxelGrid<pcl::PointXYZ> grid_;
	grid_.setInputCloud(cloud_1);
	grid_.setLeafSize(leaf_size, leaf_size, leaf_size);
	grid_.filter(*m_currentCloud);

	//添加到窗口
	viewer->addPointCloud(m_currentCloud);
	pcl::getMinMax3D(*m_currentCloud, p_min, p_max);

	double scale = getMinValue(p_max, p_min);
	maxLen = getMaxValue(p_max, p_min);

	//重设视角
	viewer->resetCamera();

	//刷新窗口
	ui.qvtkWidget->update();

	if (!m_currentCloud->empty())
	{
		int Cloudnum = m_currentCloud->width;
		QString view = "0 0 0 1 0 0 0";
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
		edit_vie->setText(view);
	}

	SB->showMessage("              ");
}
void PCLvision::on_action_Depth2Cloud_triggered()
{
	depth2clouddlg.show();
}
void PCLvision::Depth2Cloud()
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();

	std::string mydepthpath = depth2clouddlg.pathDepth.toStdString();
	cv::Mat depth = cv::imread(mydepthpath);       // 从相机得到depth深度图

	std::string myrgbpath = depth2clouddlg.pathRGB.toStdString();
	cv::Mat rgb = cv::imread(myrgbpath);          // 从相机得到RGB彩色图

	std::string mycameradatapath = depth2clouddlg.pathCameraData.toStdString();
	cv::Mat cameraMatrix = cv::Mat_<double>(3, 3);; // 从文件加载相机内参
	cv::FileStorage paramFs(mycameradatapath.c_str(), cv::FileStorage::READ);
	paramFs["cameraMatrix"] >> cameraMatrix;

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr pCloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	depth2clouddlg.convert(cameraMatrix, rgb, depth, pCloud);

	// 转换后的点云 --------------------------------
	pcl::PointCloud<pcl::PointXYZ>::Ptr mycloud(new pcl::PointCloud<pcl::PointXYZ>);
	for (size_t i = 0; i < pCloud->size(); ++i) {
		pcl::PointXYZ point;
		point.x = pCloud->points[i].x;
		point.y = pCloud->points[i].y;
		point.z = pCloud->points[i].z;
		mycloud->push_back(point);
	}

	//添加到窗口
	*m_currentCloud = *mycloud;
	m_currentCloud->resize(mycloud->width);
	viewer->addPointCloud(m_currentCloud);
	pcl::getMinMax3D(*m_currentCloud, p_min, p_max);

	double scale = getMinValue(p_max, p_min);
	maxLen = getMaxValue(p_max, p_min);

	//重设视角
	viewer->resetCamera();

	//刷新窗口
	ui.qvtkWidget->update();

	if (!m_currentCloud->empty())
	{
		int Cloudnum = m_currentCloud->width;
		QString view = "0 0 0 1 0 0 0";
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
		edit_vie->setText(view);
	}

	SB->showMessage("              ");
}
//显示当前处理点云
void PCLvision::ShowCurrentCloud()
{
	viewer->removeAllPointClouds();
	viewer->removeAllShapes();

	//添加到窗口
	viewer->addPointCloud(m_currentCloud);
	pcl::getMinMax3D(*m_currentCloud, p_min, p_max);

	double scale = getMinValue(p_max, p_min);
	maxLen = getMaxValue(p_max, p_min);

	//重设视角
	viewer->resetCamera();

	//刷新窗口
	ui.qvtkWidget->update();

	if (!m_currentCloud->empty())
	{
		int Cloudnum = m_currentCloud->width;
		QString view = "0 0 0 1 0 0 0";
		QString Pnum = QString::number(Cloudnum);
		edit_num->setText(Pnum);
		edit_vie->setText(view);
	}

	SB->showMessage("              ");
}