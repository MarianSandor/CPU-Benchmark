#include "MEM_test.h"
#include <windows.h>
#include <sys\timeb.h> 
#include <iostream>

const int data_size = 1000 * 1000 * 1024; //1 GB
const int stride = 8000 * 1024; //8 Mb

const int CONVERSIONFACTOR = 1000000000; //10^9
const int SCALEFACTOR = 100;

const int iter = 10;

volatile char* data;

long double write_cached()
{
	struct timeb start, end;
	long double diff = 0;

	ftime(&start);
	for (int i = 0; i < data_size; i++)
		data[i] = (char) 65;
	ftime(&end);

	diff = 1000.0 * (end.time - start.time) + (end.millitm - start.millitm);

	return diff;
}

long double read_cached()
{
	struct timeb start, end;
	long double diff = 0;

	volatile char buffer;

	ftime(&start);
	for (int i = 0; i < data_size; i++)
		buffer = data[i];
	ftime(&end);

	diff = 1000.0 * (end.time - start.time) + (end.millitm - start.millitm);

	return diff;
}

long double read_uncached()
{
	struct timeb start, end;
	long double diff = 0;

	volatile char buffer;

	ftime(&start);
	for (int j = 0; j < stride; j++)
		for (int i = j; i < data_size; i += stride)
			buffer = data[i];
	ftime(&end);

	diff = 1000.0 * (end.time - start.time) + (end.millitm - start.millitm);

	return diff;
}

long double write_uncached()
{
	struct timeb start, end;
	long double diff = 0;

	ftime(&start);
	for (int j = 0; j < stride; j++)
		for (int i = j; i < data_size; i += stride)
			data[i] = (char)65;
	ftime(&end);

	diff = 1000.0 * (end.time - start.time) + (end.millitm - start.millitm);

	return diff;
}

int MEM_run_test()
{
	long double rd_cached_time = 0;
	long double rd_uncached_time = 0;
	long double wr_cached_time = 0;
	long double wr_uncached_time = 0;

	std::ofstream logFile("MEM_test.log");

	logFile << "Running write_cached test..." << std::endl;
	for (int i = 0; i < iter; i++)
	{
		data = new char[data_size];
		wr_cached_time += write_cached();
		delete[] data;
	}
	wr_cached_time /= iter;
	wr_cached_time /= 1000;

	logFile << "Running write_uncached test..." << std::endl;
	for (int i = 0; i < iter; i++)
	{
		data = new char[data_size];
		wr_uncached_time += write_uncached();
		delete[] data;
	}
	wr_uncached_time /= iter;
	wr_uncached_time /= 1000;

	logFile << "Running read_cached test..." << std::endl;
	for (int i = 0; i < iter; i++)
	{
		data = new char[data_size];
		rd_cached_time += read_cached();
		delete[] data;
	}
	rd_cached_time /= iter;
	rd_cached_time /= 1000;

	logFile << "Running read_uncached test..." << std::endl;
	for (int i = 0; i < iter; i++)
	{
		data = new char[data_size];
		rd_uncached_time += read_uncached();
		delete[] data;
	}
	rd_uncached_time /= iter;
	rd_uncached_time /= 1000;

	long double rd_cached_speed = (data_size / CONVERSIONFACTOR) / rd_cached_time;
	long double rd_uncached_speed = (data_size / CONVERSIONFACTOR) / rd_uncached_time;
	long double wr_cached_speed = (data_size / CONVERSIONFACTOR) / wr_cached_time;
	long double wr_uncached_speed = (data_size / CONVERSIONFACTOR) / wr_uncached_time;

	logFile << std::endl << "Test results:" << std::endl;
	logFile << "Write cached time: " << wr_cached_time << " Speed: " << wr_cached_speed << " GB/sec" << std::endl;
	logFile << "Write uncached time: " << wr_uncached_time << " Speed: " << wr_uncached_speed << " GB/sec" << std::endl;
	logFile << "Read cached time: " << rd_cached_time << " Speed: " << rd_cached_speed << " GB/sec" << std::endl;
	logFile << "Read uncached time: " << rd_uncached_time << " Speed: " << rd_uncached_speed << " GB/sec" << std::endl;

	logFile.close();
	
	long double avg_read_score = (rd_cached_speed + rd_uncached_speed) / 2;
	long double avg_write_score = (wr_cached_speed + wr_uncached_speed) / 2;

	long score = round(avg_read_score * SCALEFACTOR + avg_write_score * SCALEFACTOR);

	return score;
}