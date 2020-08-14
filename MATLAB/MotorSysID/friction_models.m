%% Load data
type = "sinWave";
filename = sprintf("%s_data.mat",type);
data = importdata(filename);

time    = data(1:200,1);
enc     = data(1:200,2);
pos     = data(1:200,3);
vel     = data(1:200,4);
V       = data(1:200,5);
current = data(1:200,6);

k = find(V<0);
current(k) = current(k)*-1;
Ia = current;
w = vel;


%% Coefficients
V_a1 = 0.01;
V_b1 = 0.0;
I_a1 = 0.00001;
I_b1 = 0.001;
%% Functions
% Friction function coefficients
Va_cutoff = 0.5/2; % cutoff input/2
Ia_cutoff = 0.8/2; % cutoff/2

% Functions
signX =@(x,a1) x./sqrt(a1 + x.^2);

stri =@(x,xc) exp(-((x./xc).^2));
% invStri =@(x,xc) xc.*sqrt(-log(x));
invStri =@(x,xc) exp(-((xc./x).^2));

coul =@(x,a1,b1) (b1.*x)./(sqrt(a1 + x.^2));

%% Calculations
signV = signX(V,V_a1);
striV = stri(V,Va_cutoff);
invStriV = invStri(V,Va_cutoff);
signStriV = signX(striV,V_a1);
signInvStriV = signX(invStriV,V_a1);
signVxstriV = signV.*striV;

signIa = signX(Ia,I_a1);
stri_Ia = stri(Ia,Ia_cutoff);

%% Plotting
fig = figure(5);
clf;
% subplot(2,1,1); 
hold on; grid on;
plot(time,V,'DisplayName','Voltage');
plot(time,signV,'DisplayName','signV');
plot(time,striV,'DisplayName','striV');
% plot(time,invStriV,'DisplayName','invStriV');
% plot(time,signInvStriV,'DisplayName','signInvStriV');
% plot(time,signStriV,'DisplayName','signStriV');
plot(time,signVxstriV,'DisplayName','signVxstriV');


legend;
% 
% subplot(2,1,2); hold on; grid on;
% plot(time,current,'DisplayName','Ia');
% plot(time,signIa,'DisplayName','signIa');
% plot(time,stri_Ia,'DisplayName','Ia<0.8');
% 
% legend;








