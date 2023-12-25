#pragma once

#include <QWidget>
#include "ui_TemplateMatchingDlg.h"
#include <limits>
#include <fstream>
#include <vector>
#include <Eigen/Core>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/features/fpfh.h>
#include <pcl/registration/ia_ransac.h>
#include <pcl/search/impl/search.hpp>

class TemplateMatchingDlg : public QWidget
{
	Q_OBJECT

public:
	TemplateMatchingDlg(QWidget *parent = nullptr);
	~TemplateMatchingDlg();

    QString pathTemplate = "";

private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

    void on_BrowseTemplate_clicked();

signals:
	void TemplateMatching();

private:
	Ui::TemplateMatchingDlgClass ui;
};

class FeatureCloud {
public:
    typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;
    typedef pcl::PointCloud<pcl::Normal> SurfaceNormals;
    typedef pcl::PointCloud<pcl::FPFHSignature33> LocalFeatures;
    typedef pcl::search::KdTree<pcl::PointXYZ> SearchMethod;

    FeatureCloud() :search_method_xyz_(new SearchMethod) {}

    ~FeatureCloud() {}

    // 处理给定的点云
    void setInputCloud(PointCloud::Ptr xyz);

    // 加载并处理给定PCD文件中的点云
    void loadInputCloud(const std::string& pcd_file);

    // 获取指向点云的指针
    PointCloud::Ptr getPointCloud() const;

    // 获取指向三维曲面法线的点云的指针
    SurfaceNormals::Ptr getSurfaceNormals() const;

    // 获取指向特征描述点云的指针
    LocalFeatures::Ptr getLocalFeatures() const;

protected:
    // 计算表面法线和局部特征
    void processInput();

    // 计算表面法线
    void computeSurfaceNormals();

    // 计算局部特征描述子
    /**
     * 根据表面法向量 计算本地特征描述
     */
    void computeLocalFeatures();

private:
    // 点云数据
    PointCloud::Ptr xyz_;
    SurfaceNormals::Ptr normals_;
    LocalFeatures::Ptr features_; // 快速点特征直方图 Fast Point Feature Histogram
    SearchMethod::Ptr search_method_xyz_; // KDTree方法查找邻域
};

class TemplateAlignment {
public:

    // 用于存储对齐结果的结构
    struct Result {
        // 匹配分数
        float fitness_score;
        // 转换矩阵
        Eigen::Matrix4f final_transformation;

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    };

    TemplateAlignment()
    {
    }

    ~TemplateAlignment() {}

    // 将给定的点云设置为模板将与之对齐的目标
    void setTargetCloud(FeatureCloud& target_cloud);

    // 将给定点云添加到模板点云列表中
    void addTemplateCloud(FeatureCloud& template_cloud);

    // 将给定的模板点云与setTargetCloud（）指定的目标对齐
    // 对齐的核心代码
    void align(FeatureCloud& template_cloud, TemplateAlignment::Result& result);

    // 将addTemplateCloud设置的所有模板点云与setTargetCloud（）指定的目标对齐
    void alignAll(std::vector<TemplateAlignment::Result, Eigen::aligned_allocator<Result> >& results);

    // 将所有模板点云与目标点云对齐，以找到具有最佳对齐分数的模板云
    int findBestAlignment(TemplateAlignment::Result& result);

private:
    // 模板云及其将要对齐的目标的列表
    std::vector<FeatureCloud> templates_;
    FeatureCloud target_;

    // 随机采样一致性（SAC-IA）算法注册例程及其参数
    pcl::SampleConsensusInitialAlignment<pcl::PointXYZ, pcl::PointXYZ, pcl::FPFHSignature33> sac_ia_;
};
