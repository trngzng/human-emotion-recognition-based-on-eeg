clc;

% Rp = 1, Rs = 30
% cook: 0.25-0.2-0.15-0.1-0.075-0.05

fs = 100;

Wp_low = 3/(fs/2); Ws_low = 12/(fs/2);
Rp_low = 0.5; Rs_low = 40;
[n_low,Wn_low] = buttord(Wp_low,Ws_low,Rp_low,Rs_low);   % Gives minimum order of filter
disp([n_low, (Wn_low*(fs/2))]);
format long;
[b_low,a_low] = butter(n_low,Wn_low,'low');         % Butterworth filter design
freqz(b_low,a_low,1024,fs);           % Plots the frequency response
H_z_low = tf(b_low,a_low,1/fs);
disp('H_z_low');

disp(H_z_low);
b_low
a_low
H_z_low


% -------------------------------------------------------------------------------------------------------------
% Wp_high = 0.01/(fs/2); Ws_high = 0.0005/(fs/2);
% Rp_high = 0.5; Rs_high = 40;
% [n_high,Wn_high] = buttord(Wp_high,Ws_high,Rp_high,Rs_high);   % Gives minimum order of filter
% disp([n_high, (Wn_high*fs/2)]);
% [b_high,a_high] = butter(n_high,Wn_high,'high');         % Butterworth filter design
% freqz(b_high,a_high,1024,fs);           % Plots the frequency response
% H_z_high = tf(b_high,a_high,1/fs);
% % disp('H_z_high');
% format long;
% disp(H_z_high);
% 
% b_high
% a_high
% H_z_high
% -------------------------------------------------------------------------------------------------------------
% f0_notch = 50;
% bw_notch = 1;
% 
% Wp_notch = [(f0_notch - bw_notch/2)/(fs/2), (f0_notch + bw_notch/2)/(fs/2)];
% Ws_notch = [(f0_notch - bw_notch*1.5)/(fs/2), (f0_notch + bw_notch*1.5)/(fs/2)];
% Rp_notch = 0.5; Rs_notch = 40;
% 
% [n_notch,Wn_notch] = buttord(Wp_notch, Ws_notch, Rp_notch, Rs_notch);
% disp(['Order: ', num2str(n_notch), ', Normalized Cutoff Frequency: ', num2str(Wn_notch)]);
% disp(Wn_notch*250fs/2
% 
% [b_notch, a_notch] = butter(3, Wn_notch, 'stop');

% % freqz(b_notch, a_notch, 1024, fs); 
% H_z_notch = tf(b_notch,a_notch,1/500);
% disp('H_z_notch');
% disp(H_z_notch);
% format long;
% b_notch
% a_notch
% H_z_notch