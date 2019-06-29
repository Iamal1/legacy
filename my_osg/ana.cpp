#include<Windows.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include<algorithm>
#include <fstream>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osg/MatrixTransform> 
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileNameUtils>
#define PR 1e-9
#define PR1 -1e-9

struct TPoint
{
	double x, y, z;
	TPoint(){}
	TPoint(double _x, double _y, double _z) :x(_x), y(_y), z(_z){}
	inline bool operator == (const TPoint&v) const { return x == v.x && y == v.y && z == v.z; }
	inline bool operator != (const TPoint& v) const { return x != v.x || y != v.y || z != v.z; }
	inline bool operator <  (const TPoint& v) const
	{
		if (x<v.x) return true;
		else if (x>v.x) return false;
		else if (y<v.y) return true;
		else if (y>v.y) return false;
		else return (z<v.z);
	}
	TPoint operator-(const TPoint p) { return TPoint(x - p.x, y - p.y, z - p.z); }
	TPoint operator*(const TPoint p) { return TPoint(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x); }//���
	double operator^(const TPoint p) { return x*p.x + y*p.y + z*p.z; }//���
};
struct fac//
{
	int a, b, c;//͹��һ�����ϵ�������ı��
	bool ok;//�����Ƿ�������͹���е���
};
TPoint xmult(TPoint u, TPoint v)
{
	return TPoint(u.y*v.z - v.y*u.z, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x);
}
double dmult(TPoint u, TPoint v)
{
	return u.x*v.x + u.y*v.y + u.z*v.z;
}
TPoint subt(TPoint u, TPoint v)
{
	return TPoint(u.x - v.x, u.y - v.y, u.z - v.z);
}
double vlen(TPoint u)
{
	return sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
}
TPoint pvec(TPoint a, TPoint b, TPoint c)
{
	return xmult(subt(a, b), subt(b, c));
}
double Dis(TPoint a, TPoint b, TPoint c, TPoint d)
{
	return fabs(dmult(pvec(a, b, c), subt(d, a))) / vlen(pvec(a, b, c));
}
//͹��
struct T3dhull
{
	int n;//��ʼ����
	int trianglecnt;//͹������������
	TPoint *ply;//��ʼ��
	std::vector <fac> tri;//����������
	std::set <TPoint> hply; //͹����
	std::vector <fac> htri;//͹��������
	int **vis;//��i����j�������ĸ���
	T3dhull(int no)
	{
		n = no;
		ply = new TPoint[no];
		vis = new int*[no];
		for (int i = 0; i != no; i++)
			vis[i] = new int[no];
		//��ʼ����û�����
		for (int i = 0; i<n; i++)
		{

			for (int j = 0; j<n; j++)
			{
				vis[i][j] = 0;
			}
		}
	}
	~T3dhull()
	{
		delete[] ply;
		for (int i = 0; i != n; i++)
		{
			delete[] vis[i];
		}
		delete[] vis;
	}
	double dist(TPoint a){ return sqrt(a.x*a.x + a.y*a.y + a.z*a.z); }//���㳤��
	double area(TPoint a, TPoint b, TPoint c){ return dist((b - a)*(c - a)); }//���������*2
	double volume(TPoint a, TPoint b, TPoint c, TPoint d){ return (b - a)*(c - a) ^ (d - a); }//�������������*6
	double ptoplane(TPoint &p, fac &f)//����������ͬ��
	{
		TPoint m = ply[f.b] - ply[f.a], n = ply[f.c] - ply[f.a], t = p - ply[f.a];
		return (m*n) ^ t;
	}
	double ptoplane1(TPoint &p, fac &f)//�㵽�����
	{
		TPoint m = ply[f.b] - ply[f.a], n = ply[f.c] - ply[f.a], t = p - ply[f.a];
		return fabs(dmult((m*n),t) / vlen(m*n));
	}
	void deal(int p, int a, int b)
	{
		int f = vis[a][b];//�뵱ǰ��(cnt)����(ab)���Ǹ���
		fac add;
		if (tri[f].ok)
		{
			if ((ptoplane(ply[p], tri[f]))>PR1) { dfs(p, f); }//���p���ܿ�������f����������̽��f��3���ߣ��Ա�����µ�͹����
			else//������Ϊp��ֻ����cnt�棬������f�棬��p���a��b�����һ�������Ρ�
			{
				add.a = b, add.b = a, add.c = p, add.ok = 1;
				vis[p][b] = vis[a][p] = vis[b][a] = trianglecnt;
				//tri[trianglecnt++]=add;
				tri.push_back(add); trianglecnt++;
			}
		}
	}
	void dfs(int p, int cnt)//ά��͹���������p��͹�������͹��
	{
		tri[cnt].ok = 0;//��ǰ����Ҫɾ������Ϊ���ڸ����͹������
		//����ѱ߷�����(��b,��a)���Ա���deal()���ж��뵱ǰ��(cnt)����(ab)���Ǹ��档���ж��뵱ͷ��(cnt)���ڵ�3����(�����뵱ǰ��Ĺ����Ƿ���ģ�����ͼ��(1)�ķ��߳���(����ʱ��)����130��312,���ǹ���13����һ��������13,��һ��������31)

		deal(p, tri[cnt].b, tri[cnt].a);
		deal(p, tri[cnt].c, tri[cnt].b);
		deal(p, tri[cnt].a, tri[cnt].c);
	}
	bool same(int s, int e)//�ж��������Ƿ�Ϊͬһ��
	{
		TPoint a = ply[tri[s].a], b = ply[tri[s].b], c = ply[tri[s].c];
		return fabs(volume(a, b, c, ply[tri[e].a]))<PR
			&&fabs(volume(a, b, c, ply[tri[e].b]))<PR
			&&fabs(volume(a, b, c, ply[tri[e].c]))<PR;
	}
	void construct()//����͹��
	{
		int i, j;
		trianglecnt = 0;
		if (n<4) return;
		bool tmp = true;
		for (i = 1; i<n; i++)//ǰ���㲻����
		{
			if ((dist(ply[0] - ply[i]))>PR)
			{
				std::swap(ply[1], ply[i]); tmp = false; break;
			}
		}
		if (tmp) return;
		tmp = true;
		for (i = 2; i<n; i++)//ǰ���㲻����
		{
			if ((dist((ply[0] - ply[1])*(ply[1] - ply[i])))>PR)
			{
				std::swap(ply[2], ply[i]); tmp = false; break;
			}
		}
		if (tmp) return;
		tmp = true;
		for (i = 3; i<n; i++)//ǰ�ĵ㲻����
		{
			if (fabs((ply[0] - ply[1])*(ply[1] - ply[2]) ^ (ply[0] - ply[i]))>PR)
			{
				std::swap(ply[3], ply[i]); tmp = false; break;
			}
		}
		if (tmp) return;
		fac add;
		for (i = 0; i<4; i++)//������ʼ������(4����Ϊply[0],ply[1],ply[2],ply[3])
		{
			add.a = (i + 1) % 4, add.b = (i + 2) % 4, add.c = (i + 3) % 4, add.ok = 1;
			if ((ptoplane(ply[i], add))>0) std::swap(add.b, add.c);//��֤��ʱ�룬�����������⣬�����µ�ſɿ�����
			vis[add.a][add.b] = vis[add.b][add.c] = vis[add.c][add.a] = trianglecnt;//���������߱���
			//tri[trianglecnt++]=add;
			tri.push_back(add); trianglecnt++;
		}
		for (i = 4; i<n; i++)//��������͹��
		{
			for (j = 0; j<trianglecnt; j++)//��ÿ�����ж��Ƿ��ڵ�ǰ3ά͹���ڻ���(i��ʾ��ǰ��,j��ʾ��ǰ��)
			{
				if (tri[j].ok && (ptoplane(ply[i], tri[j]))>0)//�Ե�ǰ͹��������жϣ����Ƿ���ܷ񿴵������
				{
					dfs(i, j); break;//���ܿ�����ǰ�棬����͹������(�ݹ飬���ܲ�ֹ����һ����)����ǰ�������ɺ�break����ѭ��
				}
			}
		}
		int cnt = trianglecnt;//����������͹��
		for (i = 0; i<cnt; i++)
		{
			if (tri[i].ok)
			{
				htri.push_back(tri[i]);
				hply.insert(ply[tri[i].a]);
				hply.insert(ply[tri[i].b]);
				hply.insert(ply[tri[i].c]);
			}

		}
	}
	void sss()
	{
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				;
	}
	int isin(TPoint p,int width)//����һ�����
	{
		int flag = -1;
		for (unsigned int i = 0; i<htri.size(); i++)
		{
			fac f = htri[i];
			if (ptoplane(p, f) > PR )
			{
				flag = 1;
				break;
			}
			if (ptoplane1(p, f) < width)
			{
				flag = 0;
				break;
			}

		}
		return flag;
	}
	double mindtohull(TPoint p)
	{
		double flag = 1e6;
		for (unsigned int i = 0; i<htri.size(); i++)
		{
			fac f = htri[i];

			double tmp = ptoplane1(p, f);
			flag = min(tmp, flag);

		}
		return flag;
	}
};

//����ģ�ͽڵ㲢��ȡ������Ϣ
class MemoryVisitor : public osg::NodeVisitor
{
public:
	MemoryVisitor() :
		osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}


	void reset()
	{
		_nodes.clear();
		_geometryMap.clear();
		_arrayMap.clear();
		_primitiveSetMap.clear();
	}

	void apply(osg::Node& node)
	{
		_nodes.insert(&node);
		traverse(node);
	}

	void apply(osg::Geode& geode)
	{
		_nodes.insert(&geode);
		for (unsigned int i = 0; i<geode.getNumDrawables(); ++i)
		{
			apply(&geode, geode.getDrawable(i));
		}
	}

	void apply(osg::Geode* geode, osg::Drawable* drawable)
	{
		if (!drawable) return;
		_geodes.insert(geode);
		osg::Geometry* geometry = drawable->asGeometry();
		if (geometry)
		{
			_geometryMap[geometry].insert(geode);

			apply(geometry, geometry->getVertexArray());
			apply(geometry, geometry->getNormalArray());
			apply(geometry, geometry->getColorArray());
			apply(geometry, geometry->getSecondaryColorArray());
			apply(geometry, geometry->getFogCoordArray());

			for (unsigned int i = 0; i<geometry->getNumTexCoordArrays(); ++i)
			{
				apply(geometry, geometry->getTexCoordArray(i));
			}
			for (unsigned int i = 0; i<geometry->getNumVertexAttribArrays(); ++i)
			{
				apply(geometry, geometry->getVertexAttribArray(i));
			}

			for (unsigned int i = 0; i<geometry->getNumPrimitiveSets(); ++i)
			{
				apply(geometry, geometry->getPrimitiveSet(i));
			}
		}
	}

	void apply(osg::Geometry* geometry, osg::Array* array)
	{
		if (!array) return;
		_arrayMap[array].insert(geometry);
	}

	void apply(osg::Geometry* geometry, osg::PrimitiveSet* primitiveSet)
	{
		if (!primitiveSet) return;
		_primitiveSetMap[primitiveSet].insert(geometry);
	}

	void report(std::ostream& out)
	{
		out << "Nodes " << _nodes.size() << std::endl;
		out << "Geometries " << _geometryMap.size() << std::endl;
		out << "Arrays " << _arrayMap.size() << std::endl;
		out << "PrimitiveSets " << _primitiveSetMap.size() << std::endl;
	}

	//ֱ�ӻ�ȡ����geometry������һ��set��֮����main�������ȡÿ��geometry�ĵ�
	std::set<osg::Geometry*> getgeom()
	{
		std::set<osg::Geometry*> geoms;
		for (GeometryMap::iterator itr = _geometryMap.begin();
			itr != _geometryMap.end();
			++itr)
		{
			osg::Geometry* geometry = itr->first;
			geoms.insert(geometry);
		}
		return geoms;
	}
	//����ɾ��Ч����֤ʵ�ǿ���ɾ����
	/*
	void emt()//�ǿ����޸ĵ�
	{
		int i = 0;
		for (GeometryMap::iterator itr = _geometryMap.begin();
			itr != _geometryMap.end();
			++itr)
		{
			osg::Geometry* geometry = itr->first;
			Geodes& geodes = itr->second;
			for (Geodes::iterator gitr = geodes.begin();
				gitr != geodes.end();
				++gitr,++i)
			{
				osg::Geode* geode = const_cast<osg::Geode*>(*gitr);
				if (i < 6)
				{
					if (geode->removeDrawable(geometry))
						std::cout << "removed" << std::endl;
					else std::cout << "failed" << std::endl;
				}
			}
		}
	}*/

	//����ʣ�µ�geometry������
	int noofgeo()
	{
		int noo = 0;
		std::cout << "Nodes " << _nodes.size() << std::endl;
		std::cout << "Geodes " << _geodes.size() << std::endl;
		for (Geodes::iterator itr = _geodes.begin();
			itr != _geodes.end();
			++itr)
		{
			osg::Geode* geode = const_cast<osg::Geode*>(*itr);
			int sb = geode->getNumDrawables();
			//std::cout << sb << std::endl;
			noo += sb;
		}
			/*
			osg::Node *node = *itr;
			if (node->asGeode())
			{
				osg::Geode* geode = const_cast<osg::Geode*>(node->asGeode());
				int sb = geode->getNumDrawables();
				std::cout << sb << std::endl;
				noo += sb;
			}
			else if ((node->asGroup()))
			{
				for (unsigned int i = 0; i < node->asGroup()->getNumChildren(); i++)
				{
					if ((node->asGroup()->getChild(i)->asGeode()))
					{
						osg::Geode* geode = const_cast<osg::Geode*>(node->asGroup()->getChild(i)->asGeode());
						int sb = geode->getNumDrawables();
						std::cout << sb << std::endl;
						noo += sb;
					}
				}
			}*/
			
		return noo;
	}

	//ֱ�ӻ���͹����ɾ�����������
	void subs(std::set<TPoint> hps)
	{
		OSG_NOTICE << "subs Geometries" << std::endl;

		typedef std::vector< osg::ref_ptr<osg::Array> > ArrayVector;
		typedef std::vector< osg::ref_ptr<osg::Geometry> > GeometryVector;
		ArrayVector newArrays;
		GeometryVector newGeometries;
		//�ж���û�е���͹����
		for (GeometryMap::iterator itr = _geometryMap.begin();
			itr != _geometryMap.end();
			++itr)
		{
			osg::Geometry* geometry = itr->first;
			osg::Vec3Array *vertices = (osg::Vec3Array*) geometry->getVertexArray();
			osg::Vec3Array::iterator viter;
			bool flll = true;
			for (viter = vertices->begin(); viter != vertices->end(); ++viter)
			{
				osg::Vec3 vec = *viter;
				TPoint po;
				po.x = vec.x(); po.y = vec.y(); po.z = vec.z();
				int cnt = hps.count(po);
				if (cnt > 0)//�е���͹���ϣ�����
				{
					std::cout << "keep" << std::endl;
					flll = false;
					break;
				}
			}
			if (flll)//���е㶼����͹���ϣ�ɾ��
			{ 
				Geodes& geodes = itr->second;
				for (Geodes::iterator gitr = geodes.begin();
					gitr != geodes.end();
					++gitr)
				{
					osg::Geode* geode = const_cast<osg::Geode*>(*gitr);
					if(geode->removeDrawable(geometry))
						std::cout << "removed" << std::endl;
					else std::cout << "failed" << std::endl;
				}
			}
		}

	}
	//�����к�ȵ�͹�������жϺ��޸�
	void asubs(T3dhull & hull, int width)
	{
		OSG_NOTICE << "asubs Geometries" << std::endl;

		typedef std::vector< osg::ref_ptr<osg::Array> > ArrayVector;
		typedef std::vector< osg::ref_ptr<osg::Geometry> > GeometryVector;
		ArrayVector newArrays;
		GeometryVector newGeometries;
		int keeps, rems;
		keeps = rems = 0;
		//�ж���û�е���͹������
		for (GeometryMap::iterator itr = _geometryMap.begin();
			itr != _geometryMap.end();
			++itr)
		{
			osg::Geometry* geometry = itr->first;
			osg::Vec3Array *vertices = (osg::Vec3Array*) geometry->getVertexArray();
			osg::Vec3Array::iterator viter;
			bool flll = true;
			for (viter = vertices->begin(); viter != vertices->end(); ++viter)
			{
				osg::Vec3 vec = *viter;
				TPoint po;
				po.x = vec.x(); po.y = vec.y(); po.z = vec.z();
				int cnt = hull.isin(po,width);
				if (cnt == 0)//�е���͹������������
				{
					//std::cout << "keep" << std::endl;
					keeps++;
					flll = false;
					break;
				}
			}
			if (flll)//���е㶼����͹���ϣ�ɾ��
			{
				Geodes& geodes = itr->second;
				for (Geodes::iterator gitr = geodes.begin();
					gitr != geodes.end();
					++gitr)
				{
					osg::Geode* geode = const_cast<osg::Geode*>(*gitr);
					if (geode->removeDrawable(geometry))
					{
						//std::cout << "removed" << std::endl;
						rems++;
					}
					else std::cout << "failed" << std::endl;
				}
			}
		}
		std::cout << "keeps" << keeps << std::endl;
		std::cout << "removes" << rems << std::endl;
	}
	//������ԭ���ٷ��������Դ��ĺ�����д����ʱ����˲��ֲ���������Ŀ�޹أ�����ɾ��
	/*
	void reallocate()
	{
		OSG_NOTICE << "Reallocating Arrays" << std::endl;

		typedef std::vector< osg::ref_ptr<osg::Array> > ArrayVector;
		typedef std::vector< osg::ref_ptr<osg::Geometry> > GeometryVector;
		ArrayVector newArrays;
		GeometryVector newGeometries;
		for (GeometryMap::iterator itr = _geometryMap.begin();
			itr != _geometryMap.end();
			++itr)
		{
			osg::Geometry* geometry = itr->first;
			bool useVBO = geometry->getUseVertexBufferObjects();
			osg::Geometry* newGeometry = osg::clone(geometry, osg::CopyOp(osg::CopyOp::DEEP_COPY_ALL));
			newGeometry->setUseVertexBufferObjects(false);
			newGeometry->setUseVertexBufferObjects(useVBO);
			newGeometries.push_back(newGeometry);
		}

		GeometryVector::iterator geom_itr = newGeometries.begin();
		for (GeometryMap::iterator itr = _geometryMap.begin();
			itr != _geometryMap.end();
			++itr, ++geom_itr)
		{
			osg::Geometry* geometry = itr->first;
			Geodes& geodes = itr->second;
			for (Geodes::iterator gitr = geodes.begin();
				gitr != geodes.end();
				++gitr)
			{
				osg::Geode* geode = const_cast<osg::Geode*>(*gitr);
				geode->replaceDrawable(geometry, geom_itr->get());
			}
		}
	}
	*/
	typedef std::vector< osg::ref_ptr<osg::Geometry> > GeometryVector;
	typedef std::pair<osg::Array*, osg::Array*> ArrayPair;
	typedef std::vector< ArrayPair > ArrayVector;
	typedef std::pair<osg::PrimitiveSet*, osg::PrimitiveSet*> PrimitiveSetPair;
	typedef std::vector< PrimitiveSetPair > PrimitiveSetVector;
	/*
	osg::Array* cloneArray(ArrayVector& arrayVector, osg::Array* array)
	{
		if (!array) return 0;
		osg::Array* newArray = static_cast<osg::Array*>(array->cloneType());
		arrayVector.push_back(ArrayPair(array, newArray));
		return newArray;
	}

	osg::PrimitiveSet* clonePrimitiveSet(PrimitiveSetVector& psVector, osg::PrimitiveSet* ps)
	{
		if (!ps) return 0;
		osg::PrimitiveSet* newPS = static_cast<osg::PrimitiveSet*>(ps->cloneType());
		psVector.push_back(PrimitiveSetPair(ps, newPS));
		return newPS;
	}

	void reallocate2()
	{
		OSG_NOTICE << "Reallocating Arrays" << std::endl;

		ArrayVector arrayVector;
		PrimitiveSetVector primitiveSetVector;
		GeometryVector newGeometries;

		for (GeometryMap::iterator itr = _geometryMap.begin();
			itr != _geometryMap.end();
			++itr)
		{
			osg::Geometry* geometry = itr->first;
			osg::ref_ptr<osg::Geometry> newGeometry = osg::clone(geometry, osg::CopyOp::SHALLOW_COPY);
			newGeometries.push_back(newGeometry.get());

			newGeometry->setVertexArray(cloneArray(arrayVector, geometry->getVertexArray()));
			newGeometry->setNormalArray(cloneArray(arrayVector, geometry->getNormalArray()));
			newGeometry->setColorArray(cloneArray(arrayVector, geometry->getColorArray()));
			newGeometry->setSecondaryColorArray(cloneArray(arrayVector, geometry->getSecondaryColorArray()));
			newGeometry->setFogCoordArray(cloneArray(arrayVector, geometry->getFogCoordArray()));
			for (unsigned int i = 0; i<geometry->getNumTexCoordArrays(); ++i)
			{
				newGeometry->setTexCoordArray(i, cloneArray(arrayVector, geometry->getTexCoordArray(i)));
			}
			for (unsigned int i = 0; i<geometry->getNumVertexAttribArrays(); ++i)
			{
				newGeometry->setVertexAttribArray(i, cloneArray(arrayVector, geometry->getVertexAttribArray(i)));
			}

			for (unsigned int i = 0; i<geometry->getNumPrimitiveSets(); ++i)
			{
				newGeometry->setPrimitiveSet(i, clonePrimitiveSet(primitiveSetVector, geometry->getPrimitiveSet(i)));
			}
		}

		GeometryVector::iterator geom_itr = newGeometries.begin();
		for (GeometryMap::iterator itr = _geometryMap.begin();
			itr != _geometryMap.end();
			++itr, ++geom_itr)
		{
			osg::Geometry* geometry = itr->first;
			Geodes& geodes = itr->second;
			for (Geodes::iterator gitr = geodes.begin();
				gitr != geodes.end();
				++gitr)
			{
				osg::Geode* geode = const_cast<osg::Geode*>(*gitr);
				geode->replaceDrawable(geometry, geom_itr->get());
			}
		}
	}
	*/

protected:

	typedef std::set<osg::Node*>  Nodes;
	typedef std::set<osg::Geode*>  Geodes;
	typedef std::set<osg::Geometry*>  Geometries;
	typedef std::map<osg::Geometry*, Geodes> GeometryMap;
	typedef std::map<osg::Array*, Geometries> ArrayMap;
	typedef std::map<osg::PrimitiveSet*, Geometries> PrimitiveSetMap;

	Nodes              _nodes;
	Geodes			   _geodes;
	GeometryMap        _geometryMap;
	ArrayMap           _arrayMap;
	PrimitiveSetMap    _primitiveSetMap;
};


int main()
{
	typedef std::set<osg::Geometry*>  Geometries;
	typedef std::map<osg::PrimitiveSet*, Geometries> PrimitiveSetMap;
	typedef std::set<osg::Vec3>  Vec3set;
	typedef std::vector<osg::Vec3>  Vec3vc;

	osg::Node *root = osgDB::readNodeFile("xinlian-a2.osg");

	// construct the viewer.
	/*
	osgViewer::Viewer viewer;
	viewer.setSceneData(root);
	viewer.realize();
	*/
	MemoryVisitor m;
	root->accept(m);
	int mys = m.noofgeo();
	std::cout << "no of geoms before deleting :" << mys << std::endl;
	Geometries geoset = m.getgeom();
	Geometries::iterator itr;
	////��set�Ե����ȥ�غ����vector����
	Vec3set vec3set;
	Vec3vc  vec3vc;
	for (itr = geoset.begin(); itr != geoset.end(); itr++)
	{
		osg::Geometry * geo = *itr;
		osg::Vec3Array *vertices = (osg::Vec3Array*) geo->getVertexArray();
		osg::Vec3Array::iterator viter;
		for (viter = vertices->begin(); viter!= vertices->end(); ++viter)
		{
			osg::Vec3 vec = *viter;
			if (vec3set.count(vec) == 0)
			{
				vec3vc.push_back(vec);
				vec3set.insert(vec);
			}
		}
		//std::cout << vec3set.size() << std::endl;

	}
	int no = vec3set.size();
	T3dhull hull(no);
	
	//std::cout << "vertices set: "<< vec3set.size() << std::endl;
	std::cout << "vertices vc: " << vec3vc.size() << std::endl;
	//�ѵ㼯ȫ������͹���ṹ����Ϊԭʼ����
	int ii = 0;
	Vec3vc::iterator viter;
	for (viter = vec3vc.begin(); viter != vec3vc.end(); ++viter)
	{
		osg::Vec3 vec = *viter;
		hull.ply[ii].x = vec.x();
		hull.ply[ii].y = vec.y();
		hull.ply[ii].z = vec.z();
		ii++;
	}
	/*
	for (ii = 0; ii<hull.n; ii++)
	{
		hull.ply[ii].x = rand() % (1000);
		hull.ply[ii].y = rand() % (1000);
		hull.ply[ii].z = rand() % (1000);
	}*/
	////������㱣���txt�﷽�㵥������͹���㷨
	/*
	std::ofstream out("out1.txt");
	if (out.is_open())
	{
		for (int i = 0; i<hull.n; i++)
		{
			out << hull.ply[i].x << " " << hull.ply[i].y << " " << hull.ply[i].z << std::endl;
		}
		out.close();
	}
	*/
	////����͹������ʾ͹���ϵĵ�
	hull.construct();
	int hps = hull.hply.size();
	std::cout << "vertices on hull: " << hps << std::endl;

	//m.subs(hull.hply);
	////���õ�ǰ͹������һ����ȶ�ģ�ͽ����޸�ɾ��
	int wid = 5;
	m.asubs(hull,wid);
	int mys1 = m.noofgeo();
	std::cout << "no of geoms after deleting :"  << mys1 << std::endl;
	//m.emt();
	////��͹����OSG�ﻭ������
	osg::ref_ptr<osg::Geometry> geo = new osg::Geometry;
	osg::Vec3Array* vecarray = new osg::Vec3Array;     //������������
	osg::Vec4Array* colarray = new osg::Vec4Array;     //��ɫRGBֵ����
	osg::Vec3Array* norarray = new osg::Vec3Array;     //������������
	std::vector<fac>::iterator hpiter;
	for( hpiter=hull.htri.begin();hpiter != hull.htri.end(); ++hpiter)
	{
		fac f = *hpiter;
		vecarray->push_back(osg::Vec3(hull.ply[f.a].x, hull.ply[f.a].y, hull.ply[f.a].z));
		vecarray->push_back(osg::Vec3(hull.ply[f.b].x, hull.ply[f.b].y, hull.ply[f.b].z));
		vecarray->push_back(osg::Vec3(hull.ply[f.c].x, hull.ply[f.c].y, hull.ply[f.c].z));
		colarray->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));    //Red
		colarray->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));    //Green
		colarray->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));    //Blue
		//colarray->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));    //Green
	}
	norarray->push_back(osg::Vec3(1.0, 0.0, 0.0));
	geo->setVertexArray(vecarray);
	geo->setColorArray(colarray);
	geo->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	geo->setNormalArray(norarray);
	geo->setNormalBinding(osg::Geometry::BIND_OVERALL);
	int loops = 0;
	for (hpiter = hull.htri.begin(); hpiter != hull.htri.end(); ++hpiter)
	{
		fac f = *hpiter;
		osg::DrawElementsUInt* pyramidFaceOne =
			new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
		pyramidFaceOne->push_back(loops++);
		pyramidFaceOne->push_back(loops++);
		pyramidFaceOne->push_back(loops++);
		geo->addPrimitiveSet(pyramidFaceOne);
	}
	//geo->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, hps));
	//ƽ�����͹������ԭģ��һ��չʾ
	osg::MatrixTransform* translateMT = new osg::MatrixTransform;
	translateMT->setMatrix(osg::Matrix::translate(50, 0, 0));
	translateMT->addChild(geo);

	osg::Group *gr = new osg::Group;
	gr->addChild(root);
	gr->addChild(translateMT);

	//�����߷ŵ�������  
	osgViewer::Viewer viewer;
	viewer.setSceneData(gr);
	viewer.realize();
	viewer.run();
	//system("pause");
	return 0;
}
