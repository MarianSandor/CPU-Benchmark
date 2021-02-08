#include "CPU_test.h"

using namespace zlib;

const int FLOPS_iterations = 500000;
const int IOPS_iterations = 500000;
const int OP_count = 20;

const int CONVERSIONFACTOR = 1000000000; //10^9
const int SCALEFACTOR = 10;

const int bufferSize = 1024000;
const int no_buffers = 1;

DWORD WINAPI IOPS_test(LPVOID lpParam)
{
	volatile int a = 2, b = 1, c = 10, d = 4, e = 9,
				 f = 34, g = 25, h = 65, i = 95, j = 19,
				 k = 103, l = 148, m = 162, n = 194, o = 109,
				 p = 397, q = 482, r = 316, s = 380, t = 500,
				 u = 829, v = 670, w = 795, x = 559, y = 980,
				 z = 0;

	volatile int aa = 175, bb = 238, cc = 615, dd = 91, ee = 682,
				 ff = 996, gg = 552, hh = 74, ii = 641, jj = 562,
				 kk = 617, ll = 134, mm = 703, nn = 598, oo = 518,
				 pp = 172, qq = 11, rr = 676, ss = 440, tt = 881,
				 uu = 1, vv = 377, ww = 546, xx = 444, yy = 401,
				 zz = 757;

	for (int iter = 0; iter < IOPS_iterations; iter++) {
		a = f + c;
		b = b * d;
		e = g + h;
		i = j * l;
		k = m + p;
		o = n * t;
		q = w + x;
		r = r + s;
		u = v * v;
		z = y + y;

		aa = gg * ee;
		bb = dd * ff;
		cc = cc + hh;
		ii = oo + jj;
		kk = kk * ll;
		mm = nn + vv;
		pp = qq * qq;
		rr = ss + yy;
		uu = uu * tt;
		ww = xx + zz;
	}

	return 0;
}

DWORD WINAPI FLOPS_test(LPVOID lpParam)
{
	volatile double a = 2.4674, b = 1.5628, c = 10.9195, d = 4.6066, e = 9.5876,
					f = 34.4316, g = 25.3321, h = 65.6137, i = 95.9990, j = 19.5092,
					k = 103.1446, l = 148.4245, m = 162.8240, n = 194.9045, o = 109.9823,
					p = 397.6285, q = 482.5532, r = 316.9441, s = 380.1449, t = 500.3026,
					u = 829.4936, v = 670.7679, w = 795.7918, x = 559.6795, y = 980.4651,
					z = 0.5184;

	volatile double aa = 175.7936, bb = 238.3600, cc = 615.8491, dd = 91.4851, ee = 682.5048,
					ff = 996.4621, gg = 552.4293, hh = 74.7858, ii = 641.2004, jj = 562.9837,
					kk = 617.4853, ll = 134.7520, mm = 703.7725, nn = 598.9522, oo = 518.3790,
					pp = 172.6552, qq = 11.9147, rr = 676.2270, ss = 440.4683, tt = 881.8722,
					uu = 1.6556, vv = 377.1306, ww = 546.1055, xx = 444.1120, yy = 401.5463,
					zz = 757.4748;

	for (int iter = 0; iter < FLOPS_iterations; iter++) {
		a = f + c;
		b = b * d;
		e = g + h;
		i = j * l;
		k = m + p;
		o = n * t;
		q = w + x;
		r = r + s;
		u = v * v;
		z = y + y;

		aa = gg * ee;
		bb = dd * ff;
		cc = cc + hh;
		ii = oo + jj;
		kk = kk * ll;
		mm = nn + vv;
		pp = qq * qq;
		rr = ss + yy;
		uu = uu * tt;
		ww = xx + zz;
	}

	return 0;
}

long double IOPS(int no_Threads, int no_iterations)
{
	HANDLE* hThreadArray;

	struct timeb start, end;
	long double Total_diff = 0;
	long double Gflops;

	for (int j = 0; j < no_iterations; j++) {
		hThreadArray = new HANDLE[no_Threads];

		ftime(&start);
		for (int i = 0; i < no_Threads; i++)
		{
			hThreadArray[i] = CreateThread(NULL, 0, IOPS_test, NULL, 0, NULL);
		}

		WaitForMultipleObjects(no_Threads, hThreadArray, TRUE, INFINITE);
		ftime(&end);

		for (int i = 0; i < no_Threads; i++)
		{
			CloseHandle(hThreadArray[i]);
		}

		delete[] hThreadArray;

		Total_diff += 1000.0 * (end.time - start.time) + (end.millitm - start.millitm);
	}

	Total_diff /= no_iterations; // get avg
	Total_diff /= 1000; //convert to seconds

	Gflops = no_Threads * FLOPS_iterations * OP_count;
	Gflops /= CONVERSIONFACTOR;
	Gflops /= Total_diff;

	return Gflops;
}

long double FLOPS(int no_Threads, int no_iterations)
{
	HANDLE* hThreadArray;

	struct timeb start, end;
	long double Total_diff = 0;
	long double Gflops;

	for (int j = 0; j < no_iterations; j++) {
		hThreadArray = new HANDLE[no_Threads];

		ftime(&start);
		for (int i = 0; i < no_Threads; i++)
		{
			hThreadArray[i] = CreateThread(NULL, 0, FLOPS_test, NULL, 0, NULL);
		}

		WaitForMultipleObjects(no_Threads, hThreadArray, TRUE, INFINITE);
		ftime(&end);

		for (int i = 0; i < no_Threads; i++)
		{
			CloseHandle(hThreadArray[i]);
		}

		delete[] hThreadArray;
		
		Total_diff += 1000.0 * (end.time - start.time) + (end.millitm - start.millitm);
	}

	Total_diff /= no_iterations; // get avg
	Total_diff /= 1000; //convert to seconds

	Gflops = no_Threads * FLOPS_iterations * OP_count;
	Gflops /= CONVERSIONFACTOR;
	Gflops /= Total_diff;

	return Gflops;
}

long double compress(char** data)
{
	struct timeb start, end;
	long double diff = 0;

	CGZip gzip;

	gzip.Open(_T("gziptest.txt.gz"), CGZip::ArchiveModeWrite);

	ftime(&start);
	for (int i = 0; i < no_buffers; i++)
	{
		gzip.WriteString((const TCHAR*)data[i]);
	}
	ftime(&end);

	gzip.Close();

	diff = 1000.0 * (end.time - start.time) + (end.millitm - start.millitm);
	diff /= 1000; 

	return diff;
}

char** generateData()
{
	srand(time(NULL));
	
	char** data = new char* [no_buffers];

	for (int i = 0; i < no_buffers; i++)
		data[i] = new char[bufferSize];

	for (int i = 0; i < no_buffers; i++)
	{
		for (int j = 0; j < bufferSize; j++)
		{
			data[i][j] = (char)(65 + (double)rand() / RAND_MAX * (90 - 65));
		}
		data[i][bufferSize] = '\0';
	}

	return data;
}

void deleteData(char** data)
{
	for (int i = 0; i < no_buffers; i++)
		delete[] data[i];

	delete[] data;
}

int CPU_run_test()
{

	long double giops_1, giops_2, giops_4, giops_8, giops_16, giops_32, giops_64;
	long double gflops_1, gflops_2, gflops_4, gflops_8, gflops_16, gflops_32, gflops_64;

	std::ofstream logFile("CPU_test.log");

	logFile << "Running IOPS test with " << 1 << " threads..." << std::endl;
	giops_1 = IOPS(1,100);
	logFile << "Running FLOPS test with " << 1 << " threads..." << std::endl;
	gflops_1 = FLOPS(1,100);
	
	logFile << "Running IOPS test with " << 2 << " threads..." << std::endl;
	giops_2 = IOPS(2, 100);
	logFile << "Running FLOPS test with " << 2 << " threads..." << std::endl;
	gflops_2 = FLOPS(2, 100);

	logFile << "Running IOPS test with " << 4 << " threads..." << std::endl;
	giops_4 = IOPS(4, 100);
	logFile << "Running FLOPS test with " << 4 << " threads..." << std::endl;
	gflops_4 = FLOPS(4, 100);

	logFile << "Running IOPS test with " << 8 << " threads..." << std::endl;
	giops_8 = IOPS(8, 100);
	logFile << "Running FLOPS test with " << 8 << " threads..." << std::endl;
	gflops_8 = FLOPS(8, 100);

	logFile << "Running IOPS test with " << 16 << " threads..." << std::endl;
	giops_16 = IOPS(16, 100);
	logFile << "Running FLOPS test with " << 16 << " threads..." << std::endl;
	gflops_16 = FLOPS(16, 100);

	logFile << "Running IOPS test with " << 32 << " threads..." << std::endl;
	giops_32 = IOPS(32, 100);
	logFile << "Running FLOPS test with " << 32 << " threads..." << std::endl;
	gflops_32 = FLOPS(32, 100);

	logFile << "Running IOPS test with " << 64 << " threads..." << std::endl;
	giops_64 = IOPS(64, 100);
	logFile << "Running FLOPS test with " << 64 << " threads..." << std::endl;
	gflops_64 = FLOPS(64, 100);

	char** data = generateData();
	logFile << "Running Compression test..." << std::endl;
	long double compression_speed = no_buffers / compress(data);
	deleteData(data);

	logFile << std::endl << "Test results:" << std::endl;
	logFile << "For " << 1 << " number of threads, GIOPS = " << giops_1 << ", GFLOPS = " << gflops_1 << std::endl;
	logFile << "For " << 2 << " number of threads, GIOPS = " << giops_2 << ", GFLOPS = " << gflops_2 << std::endl;
	logFile << "For " << 4 << " number of threads, GIOPS = " << giops_4 << ", GFLOPS = " << gflops_4 << std::endl;
	logFile << "For " << 8 << " number of threads, GIOPS = " << giops_8 << ", GFLOPS = " << gflops_8 << std::endl;
	logFile << "For " << 16 << " number of threads, GIOPS = " << giops_16 << ", GFLOPS = " << gflops_16 << std::endl;
	logFile << "For " << 32 << " number of threads, GIOPS = " << giops_32 << ", GFLOPS = " << gflops_32 << std::endl;
	logFile << "For " << 64 << " number of threads, GIOPS = " << giops_64 << ", GFLOPS = " << gflops_64 << std::endl;
	logFile << "Compression speed = " << compression_speed << " MB/s" << std::endl;

	logFile.close();

	long double avg_giops_score = giops_1 + giops_2 + giops_4 + giops_8 + giops_16 + giops_32 + giops_64;
	long double avg_gflops_score = gflops_1 + gflops_2 + gflops_4 + gflops_8 + gflops_16 + gflops_32 + gflops_64;

	avg_giops_score /= 7;
	avg_gflops_score /= 7;

	long score = round(avg_gflops_score * SCALEFACTOR + avg_giops_score * SCALEFACTOR + compression_speed * SCALEFACTOR);

	return score;
}