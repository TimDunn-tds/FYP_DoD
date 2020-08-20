% Clean up
clc;
clear;

%% Get data
% Pick sysID input
% type = "chirp_sig";
% type = "sinWave_load_05hz";
type = "sinWave";
% type = "sinWave1hz";
% type = "sinWave2hz";
% type = "sinWave3hz";
% type = "sinWave4hz";
% type = "deadZone";
% type = "rampDown";
% type = "rampUp";
% type = "stepDown";
% type = "stepUp";
% type = "constant4V";
% type = "sinWave1hzPos";
% type = "sinWave1hzNeg";
% type = "constant6V";

filename = sprintf("%s_data.mat",type);
data = importdata(filename);
time    = data(:,1);
enc     = data(:,2);
pos     = data(:,3);
vel     = data(:,4);
V       = data(:,5);
current = data(:,6);
% Correct current
k       = find(V<0);
current(k) = current(k)*-1;
% Define intial conditions
w_a0 = 0;
Ia0 = 0;
Va0 = 0;
x0 = [Ia0; w_a0];

% Simulation time
dt = 0.01;
t_sim = time();
input = [V, vel, current];


%% Run system identification
% Define initial parameter guess
%             Kt,  B,   a1,   b1,    a2,    w_c
% param_vec = [0.1; 0.01; 0.01; 0.01; 0.001; 0.001];

% a1 a2 a3 b1 b2 b3
% 1.9477 1e-3 0.0012 99.1563 2.0073 107.1265
Jh = 0.0039;
param_vec = [1; 0.001; 0.001; 1000; 1; 100; Jh];

% La = 0.0135 or 0.021615
% Ra = 2.5625;
% Kw = 0.0086;

% Compute cost
y_true = [V, current, vel];

% V_y = @(y) (y(:,1) - y_true(:,1)).'*(y(:,1) - y_true(:,1));
% V_y = @(y) (y(:,2) - y_true(:,2)).'*(y(:,2) - y_true(:,2));
% V_y = @(y) (y(:,3) - y_true(:,3)).'*(y(:,3) - y_true(:,3));
V_y =@(y) 0.1*((y(:,3) - y_true(:,3)).'*(y(:,3) - y_true(:,3)))...
            + 0.9*((y(:,2) - y_true(:,2)).'*(y(:,2) - y_true(:,2)));

V_theta = @(theta) V_y(runSim(theta,x0,t_sim,input));

%% Run optimisation with fmincon
% A = -eye(length(param_vec));
% b = zeros(length(param_vec),1);
% % lb = [0; 0; 1e-9];
% options = optimoptions('fmincon','Display','iter','StepTolerance',1e-10);
% param_opt = fmincon(V_theta, param_vec, A, b, [], [], [], [], [], options);
% final_cost = V_theta(param_opt)

%% Run optimisation with patternsearch
A = -eye(length(param_vec));
b = zeros(length(param_vec),1);
lb = 1e-10.*ones(length(param_vec),1);
% ub = [0.02; inf];
options = optimoptions('patternsearch','Display','iter','StepTolerance',1e-10);
param_opt = patternsearch(V_theta, param_vec, A, b, [], [], lb, [], [], options);
final_cost = V_theta(param_opt)

%% Plot results 
% y_cts = runSim(param_opt,x0,t_sim,input);
y_cts = runSim(param_opt,x0,t_sim,input);

figure(1); clf;
ax1 = subplot(3,1,1);
plot(t_sim,y_true(:,1),'DisplayName','Measured'); hold on;
plot(t_sim,y_cts(:,1),'DisplayName','Simulated');
xlabel('Time [s]');
ylabel('Voltage [V]');
grid on;
legend('location','best');

ax2 = subplot(3,1,2);
plot(t_sim,y_true(:,3),'DisplayName','Measured'); hold on;
plot(t_sim,y_cts(:,3),'DisplayName','Simulated');
ylabel('Velocity [rad/s]');
xlabel('Time [s]');
grid on;
legend('location','best');

ax3 = subplot(3,1,3);
plot(t_sim,y_true(:,2),'DisplayName','Measured Current'); hold on;
plot(t_sim,y_cts(:,2),'DisplayName','Simulated Current');
ylabel('Current [A]');
xlabel('Time [s]');
grid on;
title("Lol current sense doesnt even work when it's braking");
legend;

linkaxes([ax1,ax2,ax3],'x');

%% Prompt for save of file
answer = questdlg("Save parameters?","Save prompt","Yes","No","No");
if answer == "Yes"
    saveFileName = sprintf("%s_params.mat", type);
    save(saveFileName, 'param_opt');
end

%% Additional functions
function y = runSim(param_vec, x0, t_sim, input)

    % Unpack params
    % Kt, B, a1, a2, a3, VaC
%     Kt      = param_vec(1);
    a1      = param_vec(1);
    a2      = param_vec(2);
    a3      = param_vec(3);
    B1      = param_vec(4);
    B2      = param_vec(5);
    B3      = param_vec(6);
    Jh      = param_vec(7);
    a4      = param_vec(8);
    a5      = param_vec(9);
    a6      = param_vec(10);
    B4      = param_vec(11);
    B5      = param_vec(12);
    B6      = param_vec(13);
    
    
    % Fixed parameters
    Ra = 2.5625;
    Kw = 0.0086;
    La = 0.0135;
    N  = 98.78;
    J  = Jh/(N^2); % Inertia at the motor (low torque/High speed) side of gearbox

    % Unpack input
    V = input(:,1);
    % Interpolation function
    Va  =@(t) interp1(t_sim,V,t);
    
    % Useful functions
    heavi_p =@(x) 1./(1 + exp(-2.*100.*x));
    heavi_m =@(x) 1./(1 + exp(-2.*100.*-x));
%     signX =@(x,a1) x./sqrt(a1 + x.^2);
%     signX =@(x) x./sqrt(0.01 + x.^2);
%     stri =@(x,xc) exp(-((x./xc).^2));
%     invStri =@(x,xc) exp(-((xc./x).^2));
    
    % CCRs
%     tauM    =@(Ia) Kt.*Ia;
    eb      =@(w_m) Kw.*w_m;
%     tauF    =@(w_m,Ia,Va) B1.*w_m + tauM(Ia).*signX(Va,Va_a1).*stri(Va,Va_cutoff);
%     tauF    =@(w_m,Ia,Va) B1.*w_m + tauM(Ia).*signX(w_m,wm_cutoff).*stri(Va,Va_cutoff);

%     signX   =@(x) x./(sqrt(0.01 + x.^2));              % sign of x (smooth)
%     Fkitc   =@(w_m) B.*w_m;
%     Fstri   =@(Va) exp(-((Va./VaC).^2)).*signX(Va);    % stribeck friction
%     signFt  =@(Va) Fstri(Va)./sqrt(a3 + Fstri(Va).^2);  % Sign of combined
%     smth    =@(w_m,Ia,Va) signX(Fstri(Va)).*(1-sqrt(signX(w_m).^2));
%     tauF    =@(w_m,Ia,Va) B2.*w_m+ B1.*tauM(Ia).*signX(smth(w_m,Ia,Va));
%     tauF    =@(w_m,Ia,Va) B2.*w_m;
%     tauF    =@(w_m,Ia,Va) B.*w_m + sqrt(signFt(Va).^2).*tauM(Ia);    % Total friction force
%     tauF    =@(w_m) Fstri(w_m) + Fcoul(w_m);            % total friction
%     tauF    =@(w_m) Fstri(w_m) + Fcoul(w_m) + Fkitc(w_m);
%     tauF    =@(w_m) Fstri(w_m);            % total friction

%     tauF    =@(w_m,Ia,Va) a0.*(w_m) + tauM(Ia).*double(abs(Ia<0.8) && abs(w_m<0.13));
%     tauF    =@(w_m,Ia,Va) (a0 + a1.*exp(-a2.*abs(w_m))).*sign1(w_m) ...
%                             + (a3 + a4.*exp(-a5.*abs(w_m))).*sign2(w_m);
%     tauF    =@(w_m,Ia,Va) (a0 + a1.*exp(-a2.*sqrt(w_m.*w_m))).*signX(w_m);
%     tauF    =@(w_m,Ia,Va) (a0 + a1.*exp(-a2.*abs(w_m))).*sign(w_m);
    tauF =@(w_m) (a1.*(tanh(B1.*w_m) - tanh(B2.*w_m)) + a2.*tanh(B3.*w_m) + a3.*w_m).*heavi_p(w_m)...
                + (a4.*(tanh(B4.*w_m) - tanh(B5.*w_m)) + a5.*tanh(B6.*w_m) + a6.*w_m).*heavi_m(w_m);
%     tauF =@(w_m) (a1.*(tanh(B1.*w_m) - tanh(B2.*w_m)) + a2.*tanh(B3.*w_m) + a3.*w_m);

    % SSRs
%     dw_m =@(w_m,Ia,Va) (tauM(Ia) - tauF(w_m,Ia,Va))/J; % w_m (not speed of hand)
    dw_m =@(w_m,Ia,Va) (motorTorque(Ia,Va) - tauF(w_m))./J; 
%     dw_m =@(w_m,Ia,Va) (tauM(Ia) - (B + B1.*exp(-B2*abs(w_m))).*w_m)/J; 

%     dw_m    =@(w_m,Ia,Va) motorTorque(w_m,Ia,Va,Kt,B,Tf)./J;
    dIa     =@(w_m,Ia,Va) (Va - Ra*Ia - eb(w_m))/La;
    
    % Create ode function
    dx =@(w_m,Ia,Va) [dIa(w_m,Ia,Va); dw_m(w_m,Ia,Va)];
    
    % Create wrapper function
    dx_wrap = @(t,x) dx(x(2),x(1),Va(t));

    % Run simulation
    [res.t,res.x] = ode15s(dx_wrap, t_sim, x0);
    
    % Calculate dIa
    w_sim = res.x(:,2);
    Ia_sim = res.x(:,1);
    dIa_eq = (Va(t_sim) - Ra.*Ia_sim - eb(w_sim))./La;
    
    % Simulated Voltage function
    V_hat =@(Ia,w_m) Ra.*Ia + Kw.*w_m + La.*dIa_eq;
    
    % Define output
    % V_hat, Ia, w_m
    res.x(:,1) = V_hat(Ia_sim,w_sim);
    res.x(:,2) = Ia_sim;
    res.x(:,3) = w_sim;
    output =@(x) [x(1); x(2); x(3)./N];
    % Generate output
    y = zeros(length(res.t),3);
    for i=1:length(res.t)
        y(i,:) = output(res.x(i,:)).';
    end
    
end








