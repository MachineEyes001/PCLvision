#pragma once

#include <QWidget>
#include "ui_GenerateDlg.h"
#include <pcl/io/obj_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/PolygonMesh.h>
#include <pcl/io/vtk_lib_io.h>//loadPolygonFileOBJ所属头文件
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/io/pcd_io.h>
#include <pcl/common/transforms.h>
#include <vtkVersion.h>
#include <vtkPLYReader.h>
#include <vtkOBJReader.h>
#include <vtkTriangle.h>
#include <vtkTriangleFilter.h>
#include <vtkPolyDataMapper.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/console/print.h>
#include <pcl/console/parse.h>

class GenerateDlg : public QWidget
{
	Q_OBJECT

public:
	GenerateDlg(QWidget *parent = nullptr);
	~GenerateDlg();

	QString pathModel = "";
	int default_number_samples = 100000;
	double default_leaf_size = 0.01;

	inline double uniform_deviate(int seed)
	{
		double ran = seed * (1.0 / (RAND_MAX + 1.0));
		return ran;
	}

	inline void randomPointTriangle(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3, Eigen::Vector4f& p)
	{
		float r1 = static_cast<float> (uniform_deviate(rand()));
		float r2 = static_cast<float> (uniform_deviate(rand()));
		float r1sqr = std::sqrt(r1);
		float OneMinR1Sqr = (1 - r1sqr);
		float OneMinR2 = (1 - r2);
		a1 *= OneMinR1Sqr;
		a2 *= OneMinR1Sqr;
		a3 *= OneMinR1Sqr;
		b1 *= OneMinR2;
		b2 *= OneMinR2;
		b3 *= OneMinR2;
		c1 = r1sqr * (r2 * c1 + b1) + a1;
		c2 = r1sqr * (r2 * c2 + b2) + a2;
		c3 = r1sqr * (r2 * c3 + b3) + a3;
		p[0] = c1;
		p[1] = c2;
		p[2] = c3;
		p[3] = 0;
	}

	inline void randPSurface(vtkPolyData* polydata, std::vector<double>* cumulativeAreas, double totalArea, Eigen::Vector4f& p, Eigen::Vector3f& n)
	{
		float r = static_cast<float> (uniform_deviate(rand()) * totalArea);

		std::vector<double>::iterator low = std::lower_bound(cumulativeAreas->begin(), cumulativeAreas->end(), r);
		vtkIdType el = vtkIdType(low - cumulativeAreas->begin());

		double A[3], B[3], C[3];
		vtkSmartPointer<vtkIdList> ptIds = vtkSmartPointer<vtkIdList>::New();
		polydata->GetCellPoints(el, ptIds);
		polydata->GetPoint(ptIds->GetId(0), A);
		polydata->GetPoint(ptIds->GetId(1), B);
		polydata->GetPoint(ptIds->GetId(2), C);

		randomPointTriangle(float(A[0]), float(A[1]), float(A[2]),
			float(B[0]), float(B[1]), float(B[2]),
			float(C[0]), float(C[1]), float(C[2]), p);
	}

	void uniform_sampling(vtkSmartPointer<vtkPolyData> polydata, size_t n_samples, pcl::PointCloud<pcl::PointXYZ>& cloud_out)
	{
		polydata->BuildCells();
		vtkSmartPointer<vtkCellArray> cells = polydata->GetPolys();

		double p1[3], p2[3], p3[3], totalArea = 0;
		std::vector<double> cumulativeAreas(cells->GetNumberOfCells(), 0);
		size_t i = 0;
		//vtkIdType npts = 0, * ptIds = NULL;
		vtkSmartPointer<vtkIdList> ptIds = vtkSmartPointer<vtkIdList>::New();
		for (cells->InitTraversal(); cells->GetNextCell(ptIds); i++)
		{
			//polydata->GetPoint(ptIds[0], p1);
			//polydata->GetPoint(ptIds[1], p2);
			//polydata->GetPoint(ptIds[2], p3);
			polydata->GetPoint(ptIds->GetId(0), p1);
			polydata->GetPoint(ptIds->GetId(1), p2);
			polydata->GetPoint(ptIds->GetId(2), p3);
			totalArea += vtkTriangle::TriangleArea(p1, p2, p3);
			cumulativeAreas[i] = totalArea;
		}

		cloud_out.points.resize(n_samples);
		cloud_out.width = static_cast<uint32_t> (n_samples);
		cloud_out.height = 1;

		for (i = 0; i < n_samples; i++)
		{
			Eigen::Vector4f p;
			Eigen::Vector3f n;
			randPSurface(polydata, &cumulativeAreas, totalArea, p, n);
			cloud_out.points[i].x = p[0];
			cloud_out.points[i].y = p[1];
			cloud_out.points[i].z = p[2];
		}
	}


private slots:
	//确认
	void on_btn_ok_clicked();

	//取消
	void on_btn_cancel_clicked();

	void on_Browse_clicked();

signals:
	void Generate(int FileType);

private:
	Ui::GenerateDlgClass ui;
};