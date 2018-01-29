
fs = 384000;  % Sampling Frequency

Fstop1 = 13000;       % First Stopband Frequency
Fpass1 = 14000;       % First Passband Frequency
Fpass2 = 22000;      % Second Passband Frequency
Fstop2 = 23000;      % Second Stopband Frequency
Astop1 = 60;          % First Stopband Attenuation (dB)
Apass  = 1;           % Passband Ripple (dB)
Astop2 = 80;          % Second Stopband Attenuation (dB)
match  = 'passband';  % Band to match exactly

% Constrói um objeto FDESIGN e requere seu método BUTTER.
h  = fdesign.bandpass(Fstop1, Fpass1, Fpass2, Fstop2, Astop1, Apass, ...
                      Astop2, fs);
Hd = design(h, 'butter', 'MatchExactly', match);
% freqz(Hd);
% fvtool(Hd);

load('audio.mat');

ydft = fft(audio);


ydft = ydft(1:length(audio)/2+1);
% Cria um vetor de frequência
freq = 0:fs/length(audio):fs/2;
% magnitude
figure();
subplot(211);
plot(freq,abs(ydft));
title('Transformada de fourier do sinal recebido');
% fase
subplot(212);
plot(freq,unwrap(angle(ydft)));
xlabel('Hz');

filtered= filter(Hd,audio);

ydft = fft(filtered);

ydft = ydft(1:length(filtered)/2+1);
% Cria um vetor de frequência
freq = 0:fs/length(filtered):fs/2;
% magnitude
figure();
subplot(211);
plot(freq,abs(ydft));
title('Transformada de fourier do sinal filtrado');
% fase
subplot(212);
plot(freq,unwrap(angle(ydft)));
xlabel('Hz');

demodulated = amdemod(filtered,18000,fs);
audiofinal = demodulated-1;

ydft = fft(audiofinal);
ydft = ydft(1:length(audiofinal)/2+1);
freq = 0:fs/length(audiofinal):fs/2;

% magnitude do sinal
figure();
subplot(211);
plot(freq, abs(ydft));
title('Transformada de Fourier do sinal demodulado');

%fase
subplot(212);
plot(freq, unwrap(angle(ydft)));
xlabel('Hz');

audiofinal = audiofinal.*0.8;
audiowrite('test4.wav',audiofinal,fs);
%sound(f, fs);
