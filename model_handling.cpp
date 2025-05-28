#include <iostream>
#include <vector>
using namespace std;

//funkcja ktora dodaje do verticesow i indicesow prostokat. x1,y1,z1 to pierwszy wierzcholek itd.
void add_plane(vector<double>& vrt, vector<double>& ind, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4) {

	vrt.push_back(x1); vrt.push_back(y1); vrt.push_back(z1); vrt.push_back(0.0f); vrt.push_back(0.0f);
	vrt.push_back(x2); vrt.push_back(y2); vrt.push_back(z2); vrt.push_back(0.0f); vrt.push_back(1.0f);
	vrt.push_back(x3); vrt.push_back(y3); vrt.push_back(z3); vrt.push_back(1.0f); vrt.push_back(1.0f);
	vrt.push_back(x4); vrt.push_back(y4); vrt.push_back(z4); vrt.push_back(1.0f); vrt.push_back(0.0f);

	ind.push_back(ind.size()  );
	ind.push_back(ind.size()+1);
	ind.push_back(ind.size()+2);
	ind.push_back(ind.size()+3);

}