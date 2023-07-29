#include "pid.h"

void PID_Init(PID *pid, float Kp, float Ki, float Kd)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;

    pid->target = 0;
    pid->bias = 0;
    pid->previous_bias = 0;
    pid->integral = 0;

    pid->integrate_limit = 1e4f;
}

void PID_Set_Kp(PID *pid, float Kp)
{
  pid->Kp = Kp;
}

void PID_Set_Ki(PID *pid, float Ki)
{
  pid->Ki = Ki;
}

void PID_Set_Kd(PID *pid, float Kd)
{
  pid->Kd = Kd;
}

void PID_Set_Coefficients(PID *pid, float Kp, float Ki, float Kd)
{
  pid->Kp = Kp;
  pid->Ki = Ki;
  pid->Kd = Kd;
}

void PID_Set_Target(PID *pid, float target)
{
  pid->target = target;
}

void PID_Set_IntegrateLimit(PID *pid, float limit)
{
  pid->integrate_limit = limit;
}

void Update_PID_DerivKnown(PID *pid, float current, float derivative, float *output)
{
  pid->bias = pid->target - current;

  pid->integral += pid->bias;
  if (pid->integral > pid->integrate_limit)
  {
    pid->integral = pid->integrate_limit;
  }
  else if (pid->integral < - pid->integrate_limit)
  {
    pid->integral = - pid->integrate_limit;
  }

  *output = pid->Kp*pid->bias + pid->Ki*pid->integral + pid->Kd*derivative;
}

void Update_PID(PID *pid, float current, float *output)
{
    pid->bias = pid->target - current;
    float derivative = pid->bias - pid->previous_bias;
    Update_PID_DerivKnown(pid, current, derivative, output);

    pid->previous_bias = pid->bias;
}
