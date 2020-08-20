
% cutoff at 1 amp going "forward" and 0.8 going "backward"
v = linspace(-1,1,2000);
vc = 0.1; % half of the input for cutoff
a1 = 1.0;
b1 = 0.0; % settling value of friction
a2 = 0.01;
a3 = 0.001;

alpha0 = 0.00001;
alpha1 = 0.1;
alpha2 = 1;
% 
% vc = 0.001;
% a1 = 0.01;
% b1 = 0.001;
% a2 = 0.0001;
% R1 + R2.*exp(-R3.*abs(Ia)) + R4.*exp(-R5.*abs(Ia).^2);


sinwave = 4*sin(2*pi*0.5*v);
signv = v./sqrt(a2+v.^2);

Fs = exp(-((v./vc).^2));
Fc = (b1.*v)./(sqrt(a1 + v.^2));

Ft = Fs + Fc;

Tf = alpha0.*signv + alpha1.*exp(-alpha2.*signv.*v).*signv;
Fn = Fs.*signv + Fc;
signFn = Fn./sqrt(a3+Fn.^2);
new = sinwave - sqrt(signFn.^2).*sinwave;

figure(2); clf; hold on;
% plot(v,Fs,'DisplayName','Stribeck'); hold on;
% plot(v,Fc,'DisplayName','Coulomb');
% plot(v,Ft,'DisplayName','Total','LineWidth',2);
% plot(v,-Ft,'DisplayName','-ve Total','LineWidth',2);

% subplot(2,1,2);

plot(v,sinwave,'DisplayName','SinWave'); hold on;
plot(v,Fn,'DisplayName','Friction','linewidth',2);
% plot(v,sinwave-Fn,'linewidth',2,'DisplayName','Sinwave - friction');
% plot(v,signv,'DisplayName','Sign V');
plot(v,signFn,'DisplayName','Sign Fn');
plot(v,new,'DisplayName','new','linewidth',2);
% plot(v,Tf,'DisplayName','NewModel');
% plot(v,sinwave-Tf,'DisplayName','Sinwave - new friction');
grid on;
legend;




%%
% Load data
type = "sinWave";
filename = sprintf("%s_data.mat",type);
data = importdata(filename);

time    = data(:,1);
enc     = data(:,2);
pos     = data(:,3);
vel     = data(:,4);
V       = data(:,5);
current = data(:,6);

k = find(V<0);
current(k) = current(k)*-1;
Ia = current;
w = vel;
% Friction function coefficients
Va_cutoff = 1; % half the cutoff input
a1 = 1.0;
b1 = 0.0; % settling value of friction
a2 = 0.01;
a3 = 0.0001;

% Friction functions
Fs = exp(-((V./Va_cutoff).^2));
% Fc = (b1.*V)./(sqrt(a1 + V.^2));

signV = V./sqrt(a2+V.^2);
% Fn = Fs.*signV + Fc;
Fn = Fs.*signV;

signFn = Fn./sqrt(a2+Fn.^2);

% Model
signW = w./sqrt(a2+w.^2);
% corrected = w - (sqrt(signFn.^2).*-(w.*signW);
% corrected = w - signFn.*w;
this = 1.*signFn.*(1-sqrt(signW.^2));

signMe = this./sqrt(a2 + this.^2);

figure(4); clf; hold on;
% plot(time,current,'DisplayName','Current'); hold on;
% plot(time,V,'DisplayName','Voltage');
plot(time,this,'DisplayName','this');
plot(time,signFn,'DisplayName','sign(V<2)');
plot(time,signMe,'DisplayName','look at me','linewidth',2);
% plot(time,corrected,'DisplayName','velocity with deadzone');
% plot(time,w,'DisplayName','Actual Velocity');
% plot(time,Fn,'DisplayName','Fn');
legend;

%%
Ia = linspace(-2,2,2000);
Ia_cut = 0.5;
a1 = 0.5; % magnitude of breakaway peak
a2 = 0.1; % roughly ss value
a3 = 0.1; % slope after peak
B1 = 100; % also not sure
B2 = 10; % not sure. 
B3 = 100; % sharpness of peak
signX = Ia./sqrt(0.01 + Ia.^2);
heavi_plus = 1./(1 + exp(-2.*100.*Ia));
heavi_mins = 1./(1+ exp(-2.*100.*-Ia));
Tf = a1.*(tanh(B1.*Ia) - tanh(B2.*Ia)) + a2.*tanh(B3.*Ia) + a3.*Ia;

Fs = exp(-((Ia./Ia_cut).^2));

figure(9); clf; grid on; hold on;
plot(Ia,Tf);
plot(Ia,heavi_plus);
plot(Ia,heavi_mins);




