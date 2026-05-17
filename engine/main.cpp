#include "include/application/Engine.h"

int main(void)
{
	VX::Engine Engine = VX_Init(1080, 607.5, vxm::Dark);
	run(&Engine);
	
	return 0;
}