#pragma once

#include <vtkAutoInit.h> 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <QtWidgets/QMainWindow>
#include <QProcess>
#include <QSettings>
#include <QPlainTextEdit>
#include "ui_PCLvision.h"
#include <pcl/point_types.h>					//点云数据类型
#include <pcl/point_cloud.h>					//点云类
#include <pcl/visualization/pcl_visualizer.h>	//点云可视化类
#include <vtkRenderWindow.h>					//vtk可视化窗口
#include <pcl/common/common.h>					//点云极值
#include "QHeightRampDlg.h"						//高度渲染
#include <pcl/filters/passthrough.h>			//直通滤波
#include <vtkRenderWindow.h>
#include <QVTKRenderWidget.h>
#include <QVTKOpenGLNativeWidget.h> 
#include <vtkGenericOpenGLRenderWindow.h>//这个必须加
#include <vtkTextActor.h>
#include <QLineEdit>
#include <pcl/io/pcd_io.h>
#include "PassThroughDlg.h"
#include "VoxelGridDlg.h"
#include "RadiusFilterDlg.h"
#include "RemoveOutliersDlg.h"
#include "ProjectionDlg.h"
#include "UniformSamplingDlg.h"
#include "UpSamplingDlg.h"
#include "DepthMapDlg.h"
#include "NARFKeyPointDlg.h"
#include "PlaneDlg.h" //RANSAC
#include "LineDlg.h"
#include "SphereDlg.h"
#include "Circle2DDlg.h"
#include "Circle3DDlg.h"
#include "CylinderDlg.h"
#include "ConeDlg.h"
#include "ECEDlg.h"
#include "RegionGrowDlg.h"
#include "ConditionalDlg.h"
#include "Normal3DDlg.h" //法线估计
#include "IntegralImageDlg.h"
#include "NDTDlg.h"//配准
#include "ICPDlg.h"
#include "PolygonDlg.h"//曲面重建
#include "GP3Dlg.h"
#include "BOX3DDlg.h"//3D包容盒子
#include "TemplateMatchingDlg.h"//模板匹配
#include "GenerateDlg.h"
#include "Depth2CloudDlg.h"

//设置中文编码
#pragma execution_character_set("utf-8")

//点云类型定义
typedef pcl::PointXYZ PointT;
typedef pcl::PointCloud<PointT> PointCloudT;

class PCLvision : public QMainWindow
{
    Q_OBJECT

public:
    PCLvision(QWidget *parent = nullptr);
    ~PCLvision();

	void closeEvent(QCloseEvent* event);

	//创建状态栏
	QStatusBar* SB = NULL;
	//创建组件
	QLabel* label_num = NULL;
	QLineEdit* edit_num = NULL;
	QLabel* label_vie = NULL;
	QLineEdit* edit_vie = NULL;
	QLabel* label_space = NULL;

	QString text;

    //初始化
    void init();

	//点选--------------回调函数
	void pickPointCallback(const pcl::visualization::PointPickingEvent& event, void* viewer_void);

	//选择区域--------------回调函数
	void pickAreaCallback(const pcl::visualization::AreaPickingEvent& event, void* viewer_void);

public slots:
	//打开点云
	void on_action_open_triggered();

	//存储点云
	void on_action_save_triggered();

	//点云另存为
	void on_action_saveO_triggered();

	//重设视角
	void on_action_reset_triggered();

	//俯视图
	void on_action_up_triggered();

	//前视图
	void on_action_front_triggered();

	//左视图
	void on_action_left_triggered();

	//后视图
	void on_action_back_triggered();

	//右视图
	void on_action_right_triggered();

	//底视图
	void on_action_bottom_triggered();

	//前轴测
	void on_action_frontIso_triggered();

	//后轴测
	void on_action_backIso_triggered();

	//设置点云颜色
	void on_action_setColor_triggered();
	//设置背景颜色
	void on_action_BackgroudColor_triggered();

	//设置高度渲染
	void on_action_heightRamp_triggered();

	//进行高度渲染
	void setHeightRamp(int, double);

	//选点
	void on_action_pickPoints_triggered();

	//选择区域
	void on_action_pickArea_triggered();

	//ZED相机
	void on_action_ZED_triggered();

	//MECK相机
	void on_action_Meck_triggered();

	//从3D图生成点云
	void on_action_Generate_triggered();
	void Generate(int FileType);
	void on_action_Depth2Cloud_triggered();
	void Depth2Cloud();

	//滤波
	void on_action_PassThrough_triggered();
	void PassThrough(int dir, double min, double max, int KeepOrganized);
	void on_action_VoxelGrid_triggered();
	void VoxelGrid(double L, double W, double H, int KeepOrganized);
	void on_action_RemoveOutliers_triggered();
	void RemoveOutliers(int Knum, double Thresh, int KeepOrganized);
	void on_action_RadiusFilter_triggered();
	void RadiusFilter(double radius, int minKnum, int KeepOrganized);
	void on_action_Conditional_triggered();
	void Conditional(int KeepOrganized, double GT_X_d, double LT_X_d, double GT_Y_d, double LT_Y_d, double GT_Z_d, double LT_Z_d);
	void on_action_Projection_triggered();
	void Projection(int plane, int KeepOrganized);
	void on_action_UniformSampling_triggered();
	void UniformSampling(double radius, int KeepOrganized);
	void on_action_UpSampling_triggered();
	void UpSampling(double SearchRadius, double SamplingRadius, double SamplingStep, int KeepOrganized);


	//深度图
	void on_action_DepthMap_triggered();
	void DepthMap(double angularResolution, double maxAngleWidth, double maxAngleHeight, double X, double Y, double Z, double noiseLevel, double minRange, int borderSize);

	//关键点
	void on_action_NARFKeyPoint_triggered();
	void NARFKeyPoint(double angularResolution, double maxAngleWidth, double maxAngleHeight, double Support_size, int Add_points_on_straight_edges, double Distance_for_additional_points, int CAMERAorLASER, int UnseenSet, double noise_level, double min_range, int border_size);

	//RANSAC
	void on_action_Plane_triggered();//平面
	void on_action_Line_triggered();//直线
	void on_action_Circle2D_triggered();//2D圆
	void on_action_Circle3D_triggered();//3D圆
	void on_action_Sphere_triggered();//球
	void on_action_Cylinder_triggered();//圆柱体
	void on_action_Cone_triggered();//圆锥体
	void RANSAC(int model, double localpoint, int iteration, int minpointnum, double confidence, int KdTreeSearhNum, int ModelOPTI, double NormalWeight, int ifReverse, double MinR, double MaxR);
	void on_action_ECE_triggered();//欧式聚类提取
	void ECE(double ClusterTolerance, int MinClusterSize, int MaxClusterSize);
	void on_action_RegionGrow_triggered();//区域生长
	void RegionGrow(int KSearch, int NumberOfNeighbours, double SmoothnessThreshold, double CurvatureThreshold, int MinClusterSize, int MaxClusterSize);

	//表面法线估计
	void on_action_Normal3D_triggered();//3D法线
	void Normal3D(double SearchRadius, int Level, double Scale);
	void on_action_IntegralImage_triggered();//积分图
	void IntegralImage(int EstimationMethod, double DepthChangeFactor, double NormalSmoothingSize, int Level, double Scale);

	//配准
	void on_action_NDT_triggered();//正态分布变换配准(NDT)
	void NDT(double TransformationEpsilon, double StepSize, double Resolution, int MaximumIterations, int Axis, double AngleRotation, double X, double Y, double Z, double leaf);
	void on_action_ICP_triggered();//迭代最近点(ICP)
	void ICP(double leaf, double NormalRadius, double FeatureRadius, int RansacNum, int NumberOfSamples, int CorrespondenceRandomness, double SimilarityThreshold, double MaxCorrespondenceDistance, double InlierFraction);

	//曲面重建
	void on_action_Polygon_triggered();//多边形拟合
	void Polygon(double Alpha);
	void on_action_GP3_triggered();//三角化
	void GP3(int KSearch, double SearchRadius, double Mu, int MaximumNearestNeighbors, double MaximumSurfaceAngle, double MinimumAngle, double MaximumAngle, int NormalConsistency);

	//3D包容盒子
	void on_action_BOX3D_triggered();
	void BOX3D(int BOXtype, int show);

	//模板匹配
	void on_action_TemplateMatching_triggered();
	void TemplateMatching();

public:
	//可视化窗口类
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
	//当前的点云
	PointCloudT::Ptr m_currentCloud;
	QString path = "";
	PointCloudT::Ptr keypoints_ptr;

private:
    Ui::PCLvisionClass ui;


	//高度渲染的点云列
	QList<PointCloudT::Ptr> m_heightCloudList;

	//点、框选回调注册条件
	int ifRegisterAreaPicking = 0;
	int ifRegisterPointPicking = 0;

	//点云坐标极值
	PointT p_min, p_max;

	//高度渲染对话框
	QHeightRampDlg heightRampDlg;
	double maxLen;

	//滤波对话框
	PassThroughDlg passThroughDlg;
	VoxelGridDlg voxelGridDlg;
	RemoveOutliersDlg removeOutliersDlg;
	RadiusFilterDlg radiusFilterDlg;
	ProjectionDlg projectionDlg;
	ConditionalDlg conditionaldlg;
	UniformSamplingDlg uniformsamplingdlg;
	UpSamplingDlg upsamplingdlg;

	//深度图对话框
	DepthMapDlg depthmapDlg;

	//关键点对话框
	NARFKeyPointDlg NARFkeypointdlg;

	//RANSAC对话框
	PlaneDlg planedlg;//平面
	LineDlg linedlg;//直线
	Circle2DDlg circle2ddlg;//2D圆
	SphereDlg spheredlg;//球
	Circle3DDlg circle3ddlg;//3D圆
	CylinderDlg cylinderdlg;//圆柱体
	ConeDlg conedlg;//圆锥体
	ECEDlg ecedlg;//欧式聚类
	RegionGrowDlg regiongrowdlg;//区域生长

	//表面法线估计对话框
	Normal3DDlg normal3ddlg;
	IntegralImageDlg integralimagedlg;

	//配准对话框
	NDTDlg ndtdlg;
	ICPDlg icpdlg;

	//曲面重建对话框
	PolygonDlg polygondlg;
	GP3Dlg gp3dlg;

	//3D盒子对话框
	BOX3DDlg box3ddlg;

	//模板匹配对话框
	TemplateMatchingDlg templatematchingdlg;

	//点云生成对话框
	GenerateDlg generatedlg;
	Depth2CloudDlg depth2clouddlg;
		 
	pcl::PointXYZ Click3DPoint;
};