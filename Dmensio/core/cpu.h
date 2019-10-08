#ifndef _CPU_H_
#define _CPU_H_

#pragma comment(lib, "pdh.lib")

#include <Pdh.h>

class Cpu
{
public:
	Cpu();
	Cpu(const Cpu&);
	~Cpu();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCpuPercentage();

private:
	bool m_canReadCpu;
	HQUERY m_queryHandle;
	HCOUNTER m_counterHandle;
	unsigned long m_lastSampleTime;
	long m_cpuUsage;
};

#endif