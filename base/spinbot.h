#pragma once

class spinbot
{
public:
	static void spinX(usercmd_s *cmd, int speed);
	static void spinY(usercmd_s *cmd, int speed, int closestClient);
	static float negate_number(float f);
	static void spinZ(usercmd_s *cmd, int speed);
	static void handleSpinbot(usercmd_s *cmd, int closestClient);
};
