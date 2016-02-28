#include "pad.h"

void gp::PadConfig::load_pad_config(void)
{
	const std::string config_file = "./pad.config";
	FILE* fp;

	if (!fopen_s(&fp, config_file.c_str(), "r"))
	{
		fread(this, sizeof(*this), 1, fp);
		fclose(fp);
	}
	else
	{
		fopen_s(&fp, config_file.c_str(), "w");
		fwrite(this, sizeof(*this), 1, fp);
		fclose(fp);
	}
}
