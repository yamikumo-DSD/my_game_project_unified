#include "keyboard.h"

void gp::KeyboardConfig::load_keyboard_config(void)
{
	const std::string config_file = "./keyboard.config";
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
