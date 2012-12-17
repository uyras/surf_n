const int N = 10, M = 10, total = N * M;
const double
I0s = 500, //намагниченность насыщения в гаусс,
		hc = 50, //критическое поле в эрстедах,
		vol = 0.5, //объем в микронах кубических,
		dstep = 0.0001, //параметр решетки в сантиметрах (множитель)
		kv = 1E-12; //перевод к сантиметрам кубическим и к сантиметрам (множитель для перевода),
const short int repeatCheckerMemorySize = 4;
