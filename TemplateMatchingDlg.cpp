#include "TemplateMatchingDlg.h"
#include <QFileDialog>

double tempnormal_radius = 0.0;
double tempfeature_radius = 0.0;
double tempMinSampleDistance = 0.0;
double tempMaxCorrespondenceDistance = 0.0;
int tempMaximumIterations = 0;

TemplateMatchingDlg::TemplateMatchingDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

TemplateMatchingDlg::~TemplateMatchingDlg()
{}

//确认
void TemplateMatchingDlg::on_btn_ok_clicked()
{
	bool ok;
	//获取lineedit数据
	QString NormalRadius_s = ui.NormalRadius->text();
	tempnormal_radius = NormalRadius_s.toDouble(&ok);
	if (tempnormal_radius < 0.0)
	{
		ui.NormalRadius->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.NormalRadius->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString FeatureRadius_s = ui.FeatureRadius->text();
	tempfeature_radius = FeatureRadius_s.toDouble(&ok);
	if (tempfeature_radius < 0.0)
	{
		ui.FeatureRadius->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.FeatureRadius->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString MinSampleDistance_s = ui.MinSampleDistance->text();
	tempMinSampleDistance = MinSampleDistance_s.toDouble(&ok);
	if (tempMinSampleDistance < 0.0)
	{
		ui.MinSampleDistance->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MinSampleDistance->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString MaxCorrespondenceDistance_s = ui.MaxCorrespondenceDistance->text();
	tempMaxCorrespondenceDistance = MaxCorrespondenceDistance_s.toDouble(&ok);
	if (tempMaxCorrespondenceDistance < 0.0)
	{
		ui.MaxCorrespondenceDistance->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MaxCorrespondenceDistance->setStyleSheet("QLineEdit{border:1px solid black }");
	//获取lineedit数据
	QString MaximumIterations_s = ui.MaximumIterations->text();
	tempMaximumIterations = MaximumIterations_s.toInt(&ok);
	if (tempMaximumIterations < 0)
	{
		ui.MaximumIterations->setStyleSheet("QLineEdit { color: red; }");
	}
	else
		ui.MaximumIterations->setStyleSheet("QLineEdit{border:1px solid black }");
	emit TemplateMatching();
}
//取消
void TemplateMatchingDlg::on_btn_cancel_clicked()
{
	close();
}
// 浏览
void TemplateMatchingDlg::on_BrowseTemplate_clicked()
{
	//获取点云路径
	pathTemplate = QFileDialog::getOpenFileName(this, "选择点云文件", ".//", "点云文件(*.txt *.pcd);;所有文件(*.*)");
	ui.PathTemplate->setText(pathTemplate);
}

// 处理给定的点云
void FeatureCloud::setInputCloud(PointCloud::Ptr xyz) {
	xyz_ = xyz;
	processInput();
}

// 加载并处理给定PCD文件中的点云
void FeatureCloud::loadInputCloud(const std::string& pcd_file) {
	xyz_ = PointCloud::Ptr(new PointCloud);
	pcl::io::loadPCDFile(pcd_file, *xyz_);
	processInput();
}

// 获取指向点云的指针
FeatureCloud::PointCloud::Ptr FeatureCloud::getPointCloud() const {
	return (xyz_);
}

// 获取指向三维曲面法线的点云的指针
FeatureCloud::SurfaceNormals::Ptr FeatureCloud::getSurfaceNormals() const {
	return (normals_);
}

// 获取指向特征描述点云的指针
FeatureCloud::LocalFeatures::Ptr FeatureCloud::getLocalFeatures() const {
	return (features_);
}

void FeatureCloud::processInput() {
	computeSurfaceNormals();
	computeLocalFeatures();
}

// 计算表面法线
void FeatureCloud::computeSurfaceNormals() {
	// 创建表面法向量
	normals_ = SurfaceNormals::Ptr(new SurfaceNormals);

	// 计算表面法向量
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> norm_est;
	norm_est.setInputCloud(xyz_);
	norm_est.setSearchMethod(search_method_xyz_);
	norm_est.setRadiusSearch(tempnormal_radius);
	norm_est.compute(*normals_);
}

// 计算局部特征描述子
/**
	* 根据表面法向量 计算本地特征描述
	*/
void FeatureCloud::computeLocalFeatures() {
	features_ = LocalFeatures::Ptr(new LocalFeatures);

	pcl::FPFHEstimation<pcl::PointXYZ, pcl::Normal, pcl::FPFHSignature33> fpfh_est;
	fpfh_est.setInputCloud(xyz_);
	fpfh_est.setInputNormals(normals_);
	fpfh_est.setSearchMethod(search_method_xyz_);
	fpfh_est.setRadiusSearch(tempfeature_radius);
	fpfh_est.compute(*features_);
}

// 将给定的点云设置为模板将与之对齐的目标
void TemplateAlignment::setTargetCloud(FeatureCloud& target_cloud) {
	target_ = target_cloud;
	// 初始化随机采样一致性（SAC-IA）算法中的参数
	sac_ia_.setMinSampleDistance(tempMinSampleDistance);
	sac_ia_.setMaxCorrespondenceDistance(tempMaxCorrespondenceDistance);
	sac_ia_.setMaximumIterations(tempMaximumIterations);
	// 设置输入target点云
	sac_ia_.setInputTarget(target_cloud.getPointCloud());
	// 设置特征target
	sac_ia_.setTargetFeatures(target_cloud.getLocalFeatures());
}

// 将给定点云添加到模板点云列表中
void TemplateAlignment::addTemplateCloud(FeatureCloud& template_cloud) {
	templates_.push_back(template_cloud);
}

// 将给定的模板点云与setTargetCloud（）指定的目标对齐
// 对齐的核心代码
void TemplateAlignment::align(FeatureCloud& template_cloud, TemplateAlignment::Result& result) {
	// 初始化随机采样一致性（SAC-IA）算法中的参数
	sac_ia_.setMinSampleDistance(tempMinSampleDistance);
	sac_ia_.setMaxCorrespondenceDistance(tempMaxCorrespondenceDistance);
	sac_ia_.setMaximumIterations(tempMaximumIterations);
	// 设置输入源
	sac_ia_.setInputSource(template_cloud.getPointCloud());
	// 设置特征源
	sac_ia_.setSourceFeatures(template_cloud.getLocalFeatures());

	pcl::PointCloud<pcl::PointXYZ> registration_output;
	sac_ia_.align(registration_output);

	// 根据最远相应距离计算匹配分数
	result.fitness_score = (double)sac_ia_.getFitnessScore(tempMaxCorrespondenceDistance);
	// 获取最终转换矩阵
	result.final_transformation = sac_ia_.getFinalTransformation();
}

// 将addTemplateCloud设置的所有模板点云与setTargetCloud（）指定的目标对齐
void TemplateAlignment::alignAll(std::vector<TemplateAlignment::Result, Eigen::aligned_allocator<Result> >& results) {
	results.resize(templates_.size());
	for (size_t i = 0; i < templates_.size(); ++i) {
		align(templates_[i], results[i]);
	}
}

// 将所有模板点云与目标点云对齐，以找到具有最佳对齐分数的模板云
int TemplateAlignment::findBestAlignment(TemplateAlignment::Result& result) {
	// 将所有模板与目标点云对齐
	std::vector<Result, Eigen::aligned_allocator<Result> > results;
	alignAll(results);

	// 找到最合适的模板
	float lowest_score = std::numeric_limits<float>::infinity();
	int best_template = 0;
	for (size_t i = 0; i < results.size(); ++i) {
		const Result& r = results[i];
		if (r.fitness_score < lowest_score) {
			lowest_score = r.fitness_score;
			best_template = (int)i;
		}
	}

	// 输出最佳匹配
	result = results[best_template];
	return (best_template);
}