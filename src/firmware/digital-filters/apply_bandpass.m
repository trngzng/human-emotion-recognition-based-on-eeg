% Đường dẫn đầy đủ đến file CSV
filepath = 'ppg_golden_data.csv';
% Đọc dữ liệu từ file CSV
opts = detectImportOptions(filepath);
data = readmatrix(filepath);

% Định nghĩa các tham số
fs = 100.0;  % Tần số lấy mẫu (Hz)
order = 4;  % Bậc của bộ lọc

% Bộ lọc 
b_z_lo = [0.000177296607979,   0.000709186431917,   0.001063779647875,   0.000709186431917,   0.000177296607979]
a_z_lo = [1.000000000000000,  -3.349831562667920,   4.252610698953553,  -2.420450670140820,  0.520508279582855]

a_z_hi = [1, -1.999555722824731, 0.999555821493919];
b_z_hi = [0.999777886079662, -1.999555772159325, 0.999777886079662];
% Áp dụng bộ lọc
filtered_sig_high = filter(b_z_hi, a_z_hi, data);
filtered_sig_low = filter(b_z_lo, a_z_lo, filtered_sig_high);
fltered_data = filtered_sig_low;

% Tạo mảng thời gian
n = length(data); % Số phần tử trong dữ liệu
t = (0:n-1);  % Mảng thời gian

% Vẽ đồ thị
figure;

% Đồ thị ngõ vào (tín hiệu gốc)
% subplot(2, 1, 1);
% plot(t, data);
% title('Tín hiệu gốc');
% xlabel('Thời gian (s)');
% ylabel('Biên độ');

% % Đồ thị sau khi áp dụng bộ lọc thông thấp
% subplot(3, 1, 2);
% plot(t, filtered_sig_low);
% title('Tín hiệu sau khi áp dụng bộ lọc low');
% xlabel('Thời gian (s)');
% ylabel('Biên độ');

% Đồ thị sau khi áp dụng bộ lọc thông thấp
subplot(2, 1, 1);
% Tạo figure lớn hơn
% figure('Position', [0, 0, 1920, 1080]);  % Kích thước [x, y, width, height]
plot(t, data);
title('Tín hiệu raw');
xlabel('Thời gian (ms)');
ylabel('Biên độ');

subplot(2, 1, 2);
plot(t, fltered_data);
title('Tín hiệu sau khi áp dụng bộ lọc');
xlabel('Thời gian (ms)');
ylabel('Biên độ');
