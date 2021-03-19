#include <iostream>
#include <string>
#include "CL\opencl.h"
using namespace std;
string getPlatformName(const cl_platform_id pid) {
	size_t param_value_size;
	clGetPlatformInfo(pid, CL_PLATFORM_NAME, 0, NULL, &param_value_size);
	char *param_value = new char[param_value_size];
	clGetPlatformInfo(pid, CL_PLATFORM_NAME, param_value_size, param_value, NULL);
	return param_value;
}
int main() {
	cl_uint num_platforms;
	cl_device_id *devices;
	char name_data[48], ext_data[4096];
	cl_int i, err;
	cl_uint num_devices, addr_data;

	clGetPlatformIDs(0, NULL, &num_platforms);
	cl_platform_id *platforms = new cl_platform_id[num_platforms];
	clGetPlatformIDs(num_platforms, platforms, NULL);
	for (cl_uint i = 0; i < num_platforms; i++) {
		string platname = getPlatformName(platforms[i]);
		cout << "<" << i << "> " << "Platform name is :" << platname << endl;

		err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 1, NULL, &num_devices);
		if (err < 0) {
			perror("Couldn't find any devices");
			exit(1);
		}

		/* Access connected devices */
		devices = (cl_device_id*)
			malloc(sizeof(cl_device_id) * num_devices);
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL,
			num_devices, devices, NULL);

		/*循环显示platform的所有device（CPU和显卡）信息。*/
		for (i = 0; i < (int)num_devices; i++) {

			err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME,
				sizeof(name_data), name_data, NULL);
			if (err < 0) {
				perror("Couldn't read extension data");
				exit(1);
			}
			clGetDeviceInfo(devices[i], CL_DEVICE_ADDRESS_BITS,
				sizeof(ext_data), &addr_data, NULL);

			clGetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS,
				sizeof(ext_data), ext_data, NULL);

			printf("NAME: %s\nADDRESS_WIDTH: %u\nEXTENSIONS: %s\n\n",
				name_data, addr_data, ext_data);
		}

	}

	getchar();
	return 0;
}
