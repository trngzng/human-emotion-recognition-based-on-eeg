#include "filter.h"
#include <cmath>

const float firCoeff100HzSampling1000[51] = {6.23707493203100e-19,
                                             0.000651936536801578,
                                             0.00124319314520673,
                                             0.00154534133131494,
                                             0.00121969027941796,
                                             -1.30863330037917e-18,
                                             -0.00201548144449766,
                                             -0.00415052127829799,
                                             -0.00522690670351652,
                                             -0.00402425886959341,
                                             3.10179234333809e-18,
                                             0.00606470479754808,
                                             0.0119023545680876,
                                             0.0143515447823021,
                                             0.0106508925575157,
                                             -5.31825881490377e-18,
                                             -0.0152975048205391,
                                             -0.0297614666016229,
                                             -0.0360342519662000,
                                             -0.0272952563088151,
                                             7.11141785786268e-18,
                                             0.0441130473576423,
                                             0.0976505458279451,
                                             0.149177852216505,
                                             0.186419208415108,
                                             0.200000000000000,
                                             0.186419208415108,
                                             0.149177852216505,
                                             0.0976505458279451,
                                             0.0441130473576423,
                                             7.11141785786268e-18,
                                             -0.0272952563088151,
                                             -0.0360342519662000,
                                             -0.0297614666016229,
                                             -0.0152975048205391,
                                             -5.31825881490377e-18,
                                             0.0106508925575157,
                                             0.0143515447823021,
                                             0.0119023545680876,
                                             0.00606470479754808,
                                             3.10179234333809e-18,
                                             -0.00402425886959341,
                                             -0.00522690670351652,
                                             -0.00415052127829800,
                                             -0.00201548144449766,
                                             -1.30863330037917e-18,
                                             0.00121969027941796,
                                             0.00154534133131494,
                                             0.00124319314520673,
                                             0.000651936536801578,
                                             6.23707493203100e-19};

Filter::Filter(QObject *parent) : QObject(parent)
{
    reset();
}

void Filter::reset()
{
    input.clear();
    output.clear();

    for (int i = 0; i < 51; i++)
        input.append(0.0f); // Buffer đủ dài

    for (int i = 0; i < 3; i++)
        output.append(0.0f); // Notch cần 3
}

float Filter::applyFilter(FilterType type, float x)
{
    float y = 0.0f;

    // Shift input và output
    input.pop_back();
    input.prepend(x);

    output.pop_back();
    output.prepend(0.0f); // Placeholder, tính sau

    // === Bộ lọc Notch 50Hz (ví dụ) ===
    if (type == FilterType::Notch)
    {
        float fs = 1000.0f; // Tần số lấy mẫu
        float fc = 50.0f;   // Tần số cần loại bỏ
        float a = 0.99f;    // Độ sâu Notch

        float omega_c = 2.0f * M_PI * fc / fs;
        float cos_omega = cos(omega_c);

        y = input[0] - 2.0f * cos_omega * input[1] + input[2]
            + 2.0f * a * cos_omega * output[1] - a * a * output[2];
    }

    // === Thêm các filter khác sau này ===
    else if (type == FilterType::LowPass)
    {
        for (int i = 0; i < 51; i++)
        {
            y += firCoeff100HzSampling1000[i] * input[i];
        }
    }

    // Lưu output
    output[0] = y;

    return y;
}
