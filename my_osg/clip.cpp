
#include<Windows.h>
#include <osg/MatrixTransform>
#include <osg/ClipNode>
#include <osg/Billboard>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Notify>
#include <osg/Material>
#include <osg/PolygonOffset>
#include <osg/PolygonMode>
#include <osg/LineStipple>
#include <osg/AnimationPath>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgViewer/Viewer>

#include <osgUtil/Optimizer>
#include <osgUtil/IntersectionVisitor>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/PlaneIntersector>
#include <osg/LineWidth>

#include <algorithm>
std::vector<osg::Vec3d> tp;
int edge[100][100] = { 0 };

double cross(osg::Vec3d p0, osg::Vec3d p1, osg::Vec3d p2)
{
	double ux, uy, vx, vy;

	ux = p1.x() - p0.x();
	uy = p1.y() - p0.y();
	vx = p2.x() - p0.x();
	vy = p2.y() - p0.y();

	return ux * vy - vx * uy;
}

double distance(osg::Vec3d p1, osg::Vec3d p2)
{
	double ux, uy;

	ux = p1.x() - p2.x();
	uy = p1.y() - p2.y();

	return sqrt(ux * ux + uy * uy);
}

double cosin(osg::Vec3d p0, osg::Vec3d p1, osg::Vec3d p2)
{
	double ux, uy, vx, vy;

	ux = p1.x() - p0.x();
	uy = p1.y() - p0.y();
	vx = p2.x() - p0.x();
	vy = p2.y() - p0.y();

	return (ux * vx + ux * uy)/(distance(p0,p1)*distance(p0,p2));
}
bool appr(double x)
{
	return (fabs(x) < 1e-8);
}
osg::Vec3d pr, pn;
int whichpart(osg::Vec3d p1)
{
	int part = 0;
	double yuxian, chaji;
	chaji = cross(pn, pr, p1);
	yuxian = cosin(pn, pr, p1);
	if (appr(chaji) && yuxian > 0) part = 8;
	else if (appr(chaji) && yuxian < 0) part = 4;
	else if (chaji > 0 && appr(yuxian)) part = 2;
	else if (chaji < 0 && appr(yuxian)) part = 6;
	else if (chaji > 0 && yuxian > 0) part = 1;
	else if (chaji > 0 && yuxian < 0) part = 3;
	else if (chaji < 0 && yuxian < 0) part = 5;
	else part = 7;
	return part;
}
//�Ƚϻ���prev��pn��ʱ����ת�Ƕ�˭С
bool compareccVec3(const osg::Vec3d& p1, const osg::Vec3d& p2)
{
	//����
	int pa1, pa2;
	pa1 = whichpart(p1);
	pa2 = whichpart(p2);
	if (pa1 < pa2) return true;
	else if (pa1 == pa2)
	{   //��ȷ�����С����0�Ƚϡ�
		if (pa1 == 2 || pa1 == 4 || pa1 == 6|| pa1 == 8)
		{
			return distance(pn, p1) > distance(pn, p2);
		}
		double yx1, yx2;
		yx1 = cosin(pn, pr, p1);
		yx2 = cosin(pn, pr, p2);
		if (pa1<4)
		{
			return yx1>yx2 ;
		}
		else
			return yx1 < yx2;
	}
	else return false;
}

bool nearb(osg::Vec3d p0, osg::Vec3d p1, osg::Vec3d p2,double wid)
{
	double xmax, xmin, ymax, ymin;
	xmax = max(p1.x(), p2.x());
	xmin = min(p1.x(), p2.x());
	ymax = max(p1.y(), p2.y());
	ymin = min(p1.y(), p2.y());
	if (p0.x() < xmax + wid&& p0.x() > xmin - wid &&p0.y() < ymax + wid&& p0.y() > ymin - wid)
		return true;
	else return false;
}

bool compareVec3(const osg::Vec3d& p1, const osg::Vec3d& p2)
{
	if (p1.x() < p2.x()) return true;
	else if (p1.x() == p2.x()) return p1.y() < p2.y();
	else return false;
}

double distVec3(const osg::Vec3d& p1, const osg::Vec3d& p2)
{
	return (p1.x() - p2.x())*(p1.x() - p2.x()) + (p1.y() - p2.y())*(p1.y() - p2.y());
}

//draw intersection plane
osg::ref_ptr<osg::Node> drawis(osg::ref_ptr<osg::Node>loadedModel, int h)
{
	osg::ref_ptr<osg::Group> rootnode = new osg::Group;
	osg::Plane plane(0, 0, 1, -h);
	//osg::Polytope boundingPolytope;
	// ��һ��Լ����  
	//boundingPolytope.add(plane);
	//����ƽ������  
	osg::ref_ptr<osgUtil::PlaneIntersector> intersector = new osgUtil::PlaneIntersector(plane);
	osgUtil::IntersectionVisitor intersectionVisitor;
	intersector->setRecordHeightsAsAttributes(true);
	intersectionVisitor.reset();
	intersectionVisitor.setIntersector(intersector.get());
	loadedModel->accept(intersectionVisitor);
	osgUtil::PlaneIntersector::Intersections& intersections = intersector->getIntersections();
	//�Խ�����б任���ֲ�����ת���������꣩  
	osgUtil::PlaneIntersector::Intersections::iterator itr;
	for (itr = intersections.begin();
		itr != intersections.end();
		++itr)
	{
		osgUtil::PlaneIntersector::Intersection& intersection = *itr;

		if (intersection.matrix.valid())
		{
			for (auto pitr = intersection.polyline.begin();
				pitr != intersection.polyline.end();
				++pitr)
			{
				*pitr = (*pitr) * (*intersection.matrix);
			}
			// matrix no longer needed.  
			intersection.matrix = 0;
		}
	}
	osgUtil::PlaneIntersector::Intersections::iterator itrs;
	for (itrs = intersections.begin();
		itrs != intersections.end();
		++itrs)
	{
		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array;
		osgUtil::PlaneIntersector::Intersection& intersection = *itrs;

		for (auto pitr = intersection.polyline.begin();
			pitr != intersection.polyline.end();
			++pitr)
		{
			vertexArray->push_back(*pitr);
		}
		//��������  
		osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array();
		colorArray->push_back(osg::Vec4(255, 0, 0, 0));
		geometry->setColorArray(colorArray, osg::Array::BIND_OVERALL);
		osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
		lineWidth->setWidth(2.0f);
		geometry->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
		geometry->setVertexArray(vertexArray);
		osg::ref_ptr<osg::DrawArrays> drawArray = new osg::DrawArrays(osg::DrawArrays::LINE_STRIP, 0, vertexArray->size());
		geometry->addPrimitiveSet(drawArray);
		rootnode->addChild(geometry);
		
	}
	return rootnode;
}

//�����Ѿ���ñ߽���Ϣ���߱�����edge���������tp���vector���棬������ȫ�ֱ����������ļ��ʼ�ĵط�
//��������߽���Ϣ������һ�����wid������һ��geometry�������ĳЩ���������߽����С��wid���ͱ������geometry������ɾ��
void dpis(osg::ref_ptr<osg::Node>loadedModel, int wid,int h)
{
	osg::Plane plane(0, 0, 1, -h);
	//osg::Polytope boundingPolytope;
	// ��һ��Լ����  
	//boundingPolytope.add(plane);
	//����ƽ������  
	osg::ref_ptr<osgUtil::PlaneIntersector> intersector = new osgUtil::PlaneIntersector(plane);
	osgUtil::IntersectionVisitor intersectionVisitor;
	intersector->setRecordHeightsAsAttributes(true);
	intersectionVisitor.reset();
	intersectionVisitor.setIntersector(intersector.get());
	loadedModel->accept(intersectionVisitor);
	osgUtil::PlaneIntersector::Intersections& intersections = intersector->getIntersections();
	//�Խ�����б任���ֲ�����ת���������꣩  
	std::set<osg::Vec3d, decltype(compareVec3)*>
		myVec3Array(compareVec3);
	osgUtil::PlaneIntersector::Intersections::iterator itr;
	for (itr = intersections.begin();
		itr != intersections.end();
		++itr)
	{
		osgUtil::PlaneIntersector::Intersection& intersection = *itr;

		if (intersection.matrix.valid())
		{
			for (auto pitr = intersection.polyline.begin();
				pitr != intersection.polyline.end();
				++pitr)
			{
				*pitr = (*pitr) * (*intersection.matrix);
				myVec3Array.insert(*pitr);
			}
			// matrix no longer needed.  
			intersection.matrix = 0;
		}
	}
	int keeps = 0;
	int rems = 0;
	osgUtil::PlaneIntersector::Intersections::iterator itrs;
	for (itrs = intersections.begin();
		itrs != intersections.end();
		++itrs)
	{
		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array;
		osgUtil::PlaneIntersector::Intersection& intersection = *itrs;

		for (auto pitr = intersection.polyline.begin();
			pitr != intersection.polyline.end();
			++pitr)
		{
			vertexArray->push_back(*pitr);
		}
		//delete  ...................

		osg::Vec3Array::iterator viter;
		//int wid = 0.5;
		bool doinb = true;
		for (viter = vertexArray->begin(); viter != vertexArray->end(); ++viter)
		{
			osg::Vec3 vec = *viter;
			int x, y, z;
			x = vec.x();
			y = vec.y();
			z = vec.z();
			if (z<=3||z>=18)//��¥��¥������
			{
				//std::cout << "keep" << std::endl;
				keeps++;
				doinb = false;
				break;
			}
			else
			{
				for (int i = 0; i < tp.size(); i++)
					for (int j = 0; j < i; j++)
					{
					if (edge[i][j]&&doinb)
					{
						if (nearb(vec, tp[i], tp[j], wid))
						{
							keeps++;
							doinb = false;
							break;
						}
					}
					}
			}
		}
		if (doinb)//���е㶼����bound������ɾ��
		{
			osg::ref_ptr<osg::Drawable> drawable = intersection.drawable;
			osg::Geometry* geometry0 = drawable->asGeometry();
			//printf(" dad num:%d\n", drawable->getNumParents());
			for (unsigned i = 0; i < drawable->getNumParents(); i++)
			{
				osg::Geode* dad = drawable->getParent(i)->asGeode();
				if (!dad) printf("no dad \n");
				else
				{
					osg::Geode* geode = const_cast<osg::Geode*>(dad);

					if (geode->removeDrawable(geometry0))
					{	
						rems++;
						//std::cout << "removed" << std::endl;
					}	
					else std::cout << "failed" << std::endl;
				}
			}
		}
	}
	printf("keeps: %d\t rems: %d at height z=%d\n", keeps, rems,h);
}

//������ͼ�Ĵ���ɵ�ͱߵ���ʽ��ֻ���xinlian-a����
//����˼���ǰѷ����������һ���㣬�����ĺ��������һ���ߣ��ߣ�������������
osg::ref_ptr<osg::Node> getpandl(osg::ref_ptr<osg::Node>loadedModel, int h)
{
	osg::ref_ptr<osg::Group> rootnode = new osg::Group;
	osg::Plane plane(0, 0, 1, -h);
	//osg::Polytope boundingPolytope;
	// ��һ��Լ����  
	//boundingPolytope.add(plane);
	//����ƽ������  
	osg::ref_ptr<osgUtil::PlaneIntersector> intersector = new osgUtil::PlaneIntersector(plane);
	osgUtil::IntersectionVisitor intersectionVisitor;
	intersector->setRecordHeightsAsAttributes(true);
	intersectionVisitor.reset();
	intersectionVisitor.setIntersector(intersector.get());
	loadedModel->accept(intersectionVisitor);
	osgUtil::PlaneIntersector::Intersections& intersections = intersector->getIntersections();
	//�Խ�����б任���ֲ�����ת���������꣩  
	osgUtil::PlaneIntersector::Intersections::iterator itr;
	for (itr = intersections.begin();
		itr != intersections.end();
		++itr)
	{
		osgUtil::PlaneIntersector::Intersection& intersection = *itr;

		if (intersection.matrix.valid())
		{
			for (auto pitr = intersection.polyline.begin();
				pitr != intersection.polyline.end();
				++pitr)
			{
				*pitr = (*pitr) * (*intersection.matrix);
			}
			// matrix no longer needed.  
			intersection.matrix = 0;
		}
	}
	//���ƽ���  
	osgUtil::PlaneIntersector::Intersections::iterator itrs;
	for (itrs = intersections.begin();
		itrs != intersections.end();
		++itrs)
	{
		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array;
		osgUtil::PlaneIntersector::Intersection& intersection = *itrs;
		osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array();
		for (auto pitr = intersection.polyline.begin();
			pitr != intersection.polyline.end();
			++pitr)
		{
			vertexArray->push_back(*pitr);
		}
		double xmin, ymin, xmax, ymax;
		xmin = ymin = 1e6;
		xmax = ymax = -1e6;
		osg::Vec3d pl, pr, pc;
		for (auto mitr = vertexArray->begin();
			mitr != vertexArray->end();
			++mitr)
		{
			xmax = (xmax > mitr->x()) ? xmax : mitr->x();
			ymax = (ymax > mitr->y()) ? ymax : mitr->y();
			xmin = (xmin < mitr->x()) ? xmin : mitr->x();
			ymin = (ymin < mitr->y()) ? ymin : mitr->y();
			pc = *mitr;
			//printf("xymax: %f %f\n", xmax, ymax);
		}
		double dx, dy;
		dx = xmax - xmin;
		dy = ymax - ymin;
		if (dx < 1e-6 || dy < 1e-6)//��
		{		
			colorArray->push_back(osg::Vec4(0, 255, 0, 0));
		}
		else if (dx / dy<2 && dx / dy>0.5)//����
		{
			//keep this
			colorArray->push_back(osg::Vec4(255, 0, 0, 0));
		}
		else
		{
			pl = pr = pc;
			pl.x() = xmin;
			pl.y() = ymin;
			pr.x() = xmax;
			pr.y() = ymax;
			vertexArray->clear();
			vertexArray->push_back(pl);
			vertexArray->push_back(pr);
			colorArray->push_back(osg::Vec4(0, 0, 255, 0));
		}
		geometry->setColorArray(colorArray, osg::Array::BIND_OVERALL);
		osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
		lineWidth->setWidth(2.0f);
		geometry->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
		geometry->setVertexArray(vertexArray);
		osg::ref_ptr<osg::DrawArrays> drawArray = new osg::DrawArrays(osg::DrawArrays::LINE_STRIP, 0, vertexArray->size());
		geometry->addPrimitiveSet(drawArray);
		rootnode->addChild(geometry);
	}

	return rootnode;
}

//������ͼ�Ĵ���ɵ�ͱߵ���ʽ����������߹�ϵ
//���ڴ˹�����߽磬ά���ڽӾ���edge�ͱ߽�㼯tp����������߽磬ֻ���xinlian-a����
osg::ref_ptr<osg::Node> procpl(osg::ref_ptr<osg::Node>loadedModel, int h)
{
	osg::ref_ptr<osg::Group> rootnode = new osg::Group;
	osg::Plane plane(0, 0, 1, -h);
	osg::ref_ptr<osgUtil::PlaneIntersector> intersector = new osgUtil::PlaneIntersector(plane);
	osgUtil::IntersectionVisitor intersectionVisitor;
	intersector->setRecordHeightsAsAttributes(true);
	intersectionVisitor.reset();
	intersectionVisitor.setIntersector(intersector.get());
	loadedModel->accept(intersectionVisitor);
	osgUtil::PlaneIntersector::Intersections& intersections = intersector->getIntersections();
	//�Խ�����б任���ֲ�����ת���������꣩  
	osgUtil::PlaneIntersector::Intersections::iterator itr;
	for (itr = intersections.begin();
		itr != intersections.end();
		++itr)
	{
		osgUtil::PlaneIntersector::Intersection& intersection = *itr;

		if (intersection.matrix.valid())
		{
			for (auto pitr = intersection.polyline.begin();
				pitr != intersection.polyline.end();
				++pitr)
			{
				*pitr = (*pitr) * (*intersection.matrix);
			}
			// matrix no longer needed.  
			intersection.matrix = 0;
		}
	}
	//����ɵ����
	std::vector<osg::Vec3d> parray;
	int adjmat[100][100] = { 0 };
	osg::Vec3d pl, pr, pc;
	double radx, rady;
	radx = rady = 0.0;
	//�ȵ�
	osgUtil::PlaneIntersector::Intersections::iterator itrs;
	for (itrs = intersections.begin();
		itrs != intersections.end();
		++itrs)
	{
		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array;
		osgUtil::PlaneIntersector::Intersection& intersection = *itrs;

		for (auto pitr = intersection.polyline.begin();
			pitr != intersection.polyline.end();
			++pitr)
		{
			vertexArray->push_back(*pitr);
		}
		double xmin, ymin, xmax, ymax;
		xmin = ymin = 1e6;
		xmax = ymax = -1e6;
		for (auto mitr = vertexArray->begin();
			mitr != vertexArray->end();
			++mitr)
		{
			xmax = (xmax > mitr->x()) ? xmax : mitr->x();
			ymax = (ymax > mitr->y()) ? ymax : mitr->y();
			xmin = (xmin < mitr->x()) ? xmin : mitr->x();
			ymin = (ymin < mitr->y()) ? ymin : mitr->y();
			//printf("xymax: %f %f\n", xmax, ymax);
			pc = *mitr;
		}
		double dx, dy;
		dx = xmax - xmin;
		dy = ymax - ymin;
		if (dx / dy<2 && dx / dy>0.5)//����
		{
			//keep this
			pc.x() = (xmax + xmin) / 2;
			pc.y() = (ymax + ymin) / 2;
			parray.push_back(pc);
			radx = max(dx, radx);
			rady = max(dy, rady);
		}
	}
	int nnn = parray.size();
	osg::Vec3d *poi;
	poi = new osg::Vec3d[nnn];
	for (int i = 0; i < parray.size(); i++)
	{
		poi[i] = parray[i];
	}
	std::sort(poi, poi + nnn, compareVec3);
	
	for (int i = 0; i < nnn; i++)
	{
		parray[i] = poi[i];
		//printf("parray: %f  %f\n", parray[i].x(), parray[i].y());
	}
	//delete[] poi;
	//������
	for (itrs = intersections.begin();
		itrs != intersections.end();
		++itrs)
	{
		osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
		osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array;
		osgUtil::PlaneIntersector::Intersection& intersection = *itrs;

		for (auto pitr = intersection.polyline.begin();
			pitr != intersection.polyline.end();
			++pitr)
		{
			vertexArray->push_back(*pitr);
		}
		double xmin, ymin, xmax, ymax;
		xmin = ymin = 1e6;
		xmax = ymax = -1e6;
		for (auto mitr = vertexArray->begin();
			mitr != vertexArray->end();
			++mitr)
		{
			xmax = (xmax > mitr->x()) ? xmax : mitr->x();
			ymax = (ymax > mitr->y()) ? ymax : mitr->y();
			xmin = (xmin < mitr->x()) ? xmin : mitr->x();
			ymin = (ymin < mitr->y()) ? ymin : mitr->y();
			//printf("xymax: %f %f\n", xmax, ymax);
			//pc = *mitr;
		}
		double dx, dy;
		dx = xmax - xmin;
		dy = ymax - ymin;
		pl = pr = pc;
		pl.x() = xmin;
		pl.y() = ymin;
		pr.x() = xmax;
		pr.y() = ymax;
		
		double mind1, mind2;
		mind1 = mind2 = 1e6;
		for (auto mitr = parray.begin();
			mitr != parray.end();
			++mitr)
		{
			pc = *mitr;
			mind1 = min(mind1, distVec3(pc, pl));
			mind2 = min(mind2, distVec3(pc, pr));
		}
		if (4 * mind1<radx*radx + rady*rady && 4 * mind2<radx*radx + rady*rady)
		{ 
			int ti, tj;
			for (int i = 0; i < parray.size(); i++)
			{
				pc = parray[i];
				if (abs(mind1 - distVec3(pc, pl)) < 1e-3){ ti = i; }
				if (abs(mind2 - distVec3(pc, pr)) < 1e-3){ tj = i; }
			}
			//if (ti == tj) printf("%f %f\n", pr.x(), pl.x());
			adjmat[ti][tj] = adjmat[tj][ti] = 1;
		}

	}
	/*
	for (int i = 0; i < parray.size();i++)
		for (int j = 0; j < parray.size(); j++) 
		{
			if (adjmat[i][j]) printf("i=%d j=%d\n",i,j);
		}
	printf("p: %d\n", parray.size());*/
	//������
	//������������������xinlian-a����
	int pct[1000] = { 0 };
	for (int i = 0; i < parray.size(); i++)
		for (int j = 0; j < parray.size(); j++)
		{
			if (adjmat[i][j]&&i!=j)
			{
				pct[i] += 1;
			}
		}
	
	for (int i = 0; i < parray.size(); i++)
	{
		if (pct[i]>1)
		{
			printf("i:%d,%d\n", i, pct[i]);
			pct[i] = 1000 + tp.size();
			tp.push_back(poi[i]);
			
		}
	}
	printf("tp: %d\n", tp.size()); 
	
	
	for (int i = 0; i < parray.size(); i++)
	for (int j = 0; j < parray.size(); j++)
	{
		if (adjmat[i][j]&&i!=j)
		{
			if (pct[i]>=1000&&pct[j]>=1000)
				edge[pct[i] - 1000][pct[j] - 1000] = edge[pct[j] - 1000][pct[i] - 1000] = 1;
		}
	}
	//���������ȡ���ı߽�
	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> vertexArray = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array();
	for (int i = 0; i < tp.size(); i++)
		for (int j = 0; j < i; j++)
		{
		if (edge[i][j])
		{
			printf("tp %f  %f\n", tp[i].x(), tp[i].y());
			
			vertexArray->push_back(tp[i]);
			vertexArray->push_back(tp[j]);
			colorArray->push_back(osg::Vec4(0, 0, 255, 0));
		}
		
		}
	geometry->setColorArray(colorArray, osg::Array::BIND_OVERALL);
	osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth();
	lineWidth->setWidth(2.0f);
	geometry->getOrCreateStateSet()->setAttributeAndModes(lineWidth, osg::StateAttribute::ON);
	geometry->setVertexArray(vertexArray);
	osg::ref_ptr<osg::DrawArrays> drawArray = new osg::DrawArrays(osg::DrawArrays::LINES, 0, vertexArray->size());
	geometry->addPrimitiveSet(drawArray);
	rootnode->addChild(geometry);
	return rootnode;
	//����д��ɨ����ת���㷨������û�õ��Գɹ����������⡣����㷨����Ҳ��һ������������ģ��
	/*
	std::vector<osg::Vec3d> bdp;
	//�ҵ������½�
	for (int i = 0; i < nnn; i++)
	{
		bool isolated = true;
		for (int j = 0; j < nnn; j++)
		{
			if (adjmat[i][j]) {
				isolated = false; break;
			}

		}
		if (!isolated) {
			bdp.push_back(poi[i]);
			break;
		}
	}
	pr = bdp[0];
	pr.y() = bdp[0].y() - 1;
	pn = bdp[0];
	osg::Vec3d px = pr;
	/*
	while (px != bdp[0])
	{
		int i,j;
		for (i = 0; i < nnn; i++)
			if (pn == poi[i]) break;
		int ct = 0;
		std::vector<osg::Vec3d> adjp;
		for (j = 0; j < nnn; j++)
		{
			if (adjmat[i][j] && poi[j] != pr) { ct++; adjp.push_back(poi[j]); }
		}
		if (ct == 1) 
		{
			px = poi[j];
			if (px != bdp[0])
				bdp.push_back(px);
			pr = pn;
			pn = px;
		}
		else
		{
			int nn = adjp.size();
			osg::Vec3d *pois;
			pois = new osg::Vec3d[nn];
			for (int i = 0; i < adjp.size(); i++)
			{
				pois[i] = parray[i];
			}
			std::sort(pois, pois + nn, compareccVec3);
			px = pois[0];
			if (px != bdp[0])
				bdp.push_back(px);
			pr = pn;
			pn = px;
			delete[] pois;
		}
	}

	//��P1
	//������ʣ�µĵ�ֱ���ص�p0
	for (auto mitr = bdp.begin();
		mitr != bdp.end();
		++mitr)
	{
		pc = *mitr;
		printf("parray: %f  %f\n", pc.x(), pc.y());
	}*/

}

int main(int argc, char **argv)
{
	
	osg::ref_ptr<osg::Node> loadedModel = osgDB::readRefNodeFile("xinlian-a2.osg");
	/*
	dpis(loadedModel, 1, 3);
	dpis(loadedModel, 2, 6);
	dpis(loadedModel, 2, 9);//���������⡣ֻ��������
	dpis(loadedModel, 2, 12);
	dpis(loadedModel, 2, 15);
	dpis(loadedModel, 2, 18);
	//system("pause");
	//return 0;
	*/
	
	osg::ref_ptr<osg::Group> rootnode = new osg::Group;
	//��ȡ�߽粢չʾ������bnode
	osg::ref_ptr<osg::Node> bnode = procpl(loadedModel,18);
	rootnode->addChild(bnode);
	//����������ģ�͵ĺ�����ͼdnode0
	osg::ref_ptr<osg::Node> dnode0 = getpandl(loadedModel, 15);
	rootnode->addChild(dnode0);
	//���ڱ߽紦��h=15m��¥��
	dpis(loadedModel, 1, 15);
	//����������ģ�͵ĺ�����ͼ
	osg::ref_ptr<osg::Node> dnode = getpandl(loadedModel, 15);
	osg::MatrixTransform* translated = new osg::MatrixTransform;
	translated->setMatrix(osg::Matrix::translate(0, -50, 0));
	translated->addChild(dnode);
	rootnode->addChild(translated);
	//����ԭģ�ͣ��ƶ�һ�����뷽��۲�
	osg::MatrixTransform* translateMT = new osg::MatrixTransform;
	translateMT->setMatrix(osg::Matrix::translate(50, 0, 0));
	translateMT->addChild(loadedModel);
	rootnode->addChild(translateMT);
	osgViewer::Viewer viewer;
	viewer.setSceneData(rootnode);
	//std::cout << rootnode->getNumChildren() << std::endl;
	return viewer.run();
	
}
