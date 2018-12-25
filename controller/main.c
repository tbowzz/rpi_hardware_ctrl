#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <string>
#include <vector>
#include <stdint.h>

struct ControlData
{
	float yaw;
	float throttle;
	float pitch;
	float roll;
};

int main()
{
	FILE *file;
	std::vector<uint8_t> buf;
	buf.resize(512);
	file = fopen("/tmp/ctrl_fifo","rb");
	
	while (true)
	{
		fread(buf.data(), 1, 83, file);
		ControlData ctrlData;
		memcpy(&ctrlData, buf.data(), sizeof(ControlData));
		printf("YAW: %.2f | THR: %.2f | PTC: %.2f | ROL: %.2f\n", 
				ctrlData.yaw, ctrlData.throttle, ctrlData.pitch, ctrlData.roll);
		usleep(10000);
	}

	return 0;
}