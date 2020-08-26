% Import data
clc; clear;
data = readtable('torque_data.csv');
V = data.Voltage_V_;
Ia_meas = data.Ia_A_;
Tm_meas = data.Torque_Nm_;
Ia_fit = [0; Ia_meas];
Tm_fit = [0; Tm_meas];

idx_pos = find(V>0);
Ia_p = [0; Ia_meas(idx_pos)];
Tm_p = [0; Tm_meas(idx_pos)];
% Ia_p = [Ia_meas(idx_pos)];
% Tm_p = [Tm_meas(idx_pos)];
idx_neg = find(V<0);
Ia_n = [0; Ia_meas(idx_neg)];
Tm_n = [0; Tm_meas(idx_neg)];
% Ia_n = [Ia_meas(idx_neg)];
% Tm_n = [Tm_meas(idx_neg)];

Ia_pos =@(Tm) 1.204.*Tm.^2 + 0.4482.*Tm + 0.04895;
Ia_neg =@(Tm) -0.3305.*Tm.^2 + 1.353.*Tm - 0.002279;

tauVec_p = linspace(0,2,200);
tauVec_n = linspace(0,-2,200);

% tauM_p =@(Ia) -0.3295.*Ia.^2 + 1.039*Ia - 0.003;
% tauM_p =@(Ia) 0.5702.*exp(0.3512.*Ia) - 0.5703.*exp(-1.82.*Ia);
% tauM_n =@(Ia) 0.06085.*Ia.^2 + 0.6843.*Ia;

tauM_p =@(Ia) (-0.4482 + sqrt(4.816.*Ia - 0.034860))./2.408;
tauM_n =@(Ia) -(-1.353 + sqrt(-1.322.*Ia + 1.8276))./0.661;
test = Ia_pos(tauVec_p);
Ivec_p = linspace(0,6,200);
Ivec_n = linspace(0,-6,200);
Ia = [flip(Ivec_n), Ivec_p];
Tm = 0.04629.*Ia.^4 + 0.2721.*Ia.^3 + 0.06547.*Ia.^2 + 1.417.*Ia - 0.082;


figure(11); clf; hold on; grid on;
scatter(Tm_p,Ia_p,'DisplayName','Measured');
scatter(Tm_n,Ia_n,'DisplayName','Measured');
plot(tauVec_p,Ia_pos(tauVec_p),'DisplayName','Ia(Tm) Function');
plot(tauVec_n,Ia_neg(tauVec_n),'DisplayName','Ia(Tm) Function');

plot(tauM_p(Ivec_p),Ivec_p,'DisplayName','Tm(Ia) Function');
plot(tauM_n(Ivec_n),Ivec_n,'DisplayName','Tm(Ia) Function');
plot(Ia,Tm);
ylabel('Current [A]');
xlabel('Torque [Nm]');
legend;

fitx = [tauVec_n, tauVec_p];
fity = [Ia_neg(tauVec_n), Ia_pos(tauVec_p)];

%%
heavi_p =@(x) 1./(1 + exp(-2.*25.*x));
heavi_m =@(x) 1./(1 + exp(-2.*25.*-x));
tauM=@(Ia) (0.06085.*Ia.^2 + 0.6843.*Ia).*heavi_m(Ia) + (0.5702.*exp(0.3412.*Ia) - 0.5703.*exp(-1.82.*Ia)).*heavi_p(Ia);
Ia = linspace(-3,3,20000);
tau = tauM(Ia);
figure;
plot(Ia,tau);




