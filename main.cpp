#include<iostream>
#include <stdlib.h>
using namespace std;

int main() {

	int f = 6, N = f*f; //number of particles
	double *x0 = new double [N + 1]; //koordinaty chastic x,y,z
	double *y0 = new double [N + 1];
	double *z0 = new double [N + 1];

	double *cosX = new double [N + 1]; //косинусы chastic x,y,z
	double *cosY = new double [N + 1];
	double *cosZ = new double [N + 1];

	double *mX = new double [N + 1]; //magnitnie momenty
	double *mY = new double [N + 1];
	double *mZ = new double [N + 1];

	double *hrrx = new double [N + 1]; //поля взаимодействия х
	double *hrry = new double [N + 1]; //поля взаимодействия y
	double *hrrz = new double [N + 1]; //поля взаимодействия z

	double *hrm = new double [N + 1]; //модули полей взаимодействия
	double I0s = 500, hc = 50, vol = 0.5, dstep = 1E-4, kv = 1E-12, md;
	//намагниченность насыщения в гаусс, критическое поле в эрстедах, объем в микронах кубических, параметр решетки в микронах
	//перевод к сантиметрам кубическим и к сантиметрам, модуль магнитного момента



	for (int l = 1; l <= f; l++)
		for (int i = 1; i <= f; i++) {
			x0[i + (l - 1) * f] = (i - 1) * dstep;
			y0[i + (l - 1) * f] = 0 * dstep;
			z0[i + (l - 1) * f] = (l - 1) * dstep;
		}


	for (int i = 1; i <= f * f; i++) {
		cosX[i] = 0.; // если все параллельны
		cosY[i] = 0.;
		cosZ[i] = 1.; // все по оси z и по полю

		mX[i] = kv * vol * cosX[i] * I0s;
		mY[i] = kv * vol * cosY[i] * I0s;
		mZ[i] = kv * vol * cosZ[i] * I0s;

		cout << "x0[" << i << "]=" << x0[i] << " y0[" << i << "]=" << y0[i] << " z0[" << i << "]=" << z0[i] << endl;
		cout << "mx[" << i << "]=" << mX[i] << " my[" << i << "]=" << mY[i] << " mz[" << i << "]=" << mZ[i] << endl << endl;
	}

	md = kv * vol*I0s;
	cout << " |m|=" << md << " |M|=" << f * f * md << endl;


	system("pause");
}