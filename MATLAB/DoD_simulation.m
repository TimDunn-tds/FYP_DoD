%% Clear parameters
clc; clear all; 
% close all;

T = 0.01;
tsim = 10;

%% Parameters
mo = 0.2;                   % kg
mh = 0.4;                   % kg

ro = 0.1;                   % m
rh = 0.2;                   % m

g = 9.82;                   % m/s^2

theta0 = 10*(pi/180);        % rad
phi0 = 5*(pi/180);          % rad


%% Motor parameters
tau_max = 100;              % Nm
tau_min = -100;             % Nm


%% Linearised Plant Model (linearised about upright balancing position)
% State variables:
% x(1) = hand angular velocity [rad/s]
% x(2) = object angular velocity about hand CoM [rad/s]
% x(3) = hand angle [rad]
% x(4) = object CoM angle relative to hand CoM [rad]

MM = [
    (mo + mh)*rh^2, -mo*rh*(ro + rh);
    -mo*rh*(ro + rh), 2*mo*(rh + ro)^2];


% MM = [
%     (mo + mh)*rh^2, -mo*rh^2;
%     -mo*rh^2, mo*(rh^2 + (rh + ro)^2)];

KK = [0, 0; 0, -mo*g*(rh + ro)];

DD = zeros(2);

e = [1;0];

% Create matrices 
A = [MM\-DD, MM\-KK; eye(2,2), zeros(2,2)];

B = [MM\e; zeros(2,1)];

Cr = [1, 0, 0, 0]; % Regulate hand velocity
Dr = 0; % No feedthrough?

Cm = [0, 0, 1, 1]; % Measure hand and object positions
Dm = 0;

% Reduced-state continuous-time model for control design
idx = [1 2 4];                % Only include these states
Ac  = A(idx,idx);
Bc  = B(idx,:);
Cc  = Cr(:,idx);
Dc  = Dr;


[Acd, Bcd] = c2d(Ac,Bc,T);


%% Create MPC Controller

% System dimensions
n_in = size(Bcd,2);     % No. actuators
n_out = size(Cc,1);     % No. outputs to regulate 
n_states = size(Acd,1); % No. states


N_MPC = 20; % Horizon

% Quadratic cost weighting
Ru = 1;         % Penalty on input moves (i.e. ||u[k]-u[k-1]||)
Qy = 1;      	% Penalty on output error


%% MPC stuff I dont understand
% Consider the augmented system
%
% [ x[k+1] ] = [ A B ]*[ x[k]   ] + [ B ]*(u[k] - u[k-1])
% [ u[k]   ]   [ 0 I ] [ u[k-1] ]   [ I ]
% \________/   \_____/ \________/   \___/ \_____________/
%   z[k+1]   =   Az   *  z[k]     +  Bz  * du[k]
%
% y[k] = [ C D ]*[ x[k]   ] + D*(u[k] - u[k-1])
%                [ u[k-1] ]
%        \_____/ \________/   | \_____________/
%          Cz   *  z[k]     + Dz * du[k]

Az = [Acd, Bcd; zeros(n_in, n_states), eye(n_in)];
Bz = [Bcd; eye(n_in)];
Cz = [Cc, Dc];
Dz = Dc;

n_z = n_states + n_in;     % Number of augmented states

AA = zeros((N_MPC+1)*n_z,n_z);
for i = 1:N_MPC+1
    AA(1+(i-1)*n_z:i*n_z,:) = Az^(i-1);
end
AB = zeros((N_MPC+1)*n_z,n_in);
for i = 2:N_MPC+1
    AB(1+(i-1)*n_z:i*n_z,:) = Az^(i-2)*Bz;
end
BB = zeros((N_MPC+1)*n_z,N_MPC*n_in);
for i = 1:N_MPC
    BB(1+(i-1)*n_z:end,1+(i-1)*n_in:i*n_in) = AB(1:(N_MPC-i+2)*n_z,:);
end
WW = kron(eye(N_MPC)-diag(ones(1,N_MPC-1),-1),eye(n_in));
EE = zeros(N_MPC*n_in,n_states+n_in);
EE(1:n_in,1+n_states:end) = -eye(n_in);

Qz = Cz.'*Qy*Cz;
Sz = Cz.'*Qy*Dz;
Rz = Dz.'*Qy*Dz + Ru;
[~,Qf] = dlqr(Az,Bz,Qz,Rz,Sz);
% [~,Qf] = lqry(ss(Az,Bz,Cz,Dz,T),Qy,Ru);

QQ = blkdiag(kron(eye(N_MPC),Qz),Qf);
SS = kron(eye(N_MPC+1,N_MPC),Sz);
RR = kron(eye(N_MPC),Ru);

% Compute feedforward gains (we assume n_in == n_out)
NN = [Acd - eye(n_states), Bcd; Cc, Dc]\[zeros(n_states,n_out);eye(n_out)];

%-----------------------------------------------
% Generate the Hessian H and gradient coefficient
%-----------------------------------------------
H = WW.'*( BB.'*QQ*BB + BB.'*SS + SS.'*BB + RR )*WW;
Gamma = WW.'*[(BB.'*QQ + SS.')*(AA + BB*EE) + (BB.'*SS + RR)*EE, -(BB.'*QQ + SS.')*kron(ones(N_MPC+1,1),NN)];
% Note: The gradient is g = Gamma*[x1; u0; ref], but we do this
%       multiplication inside the controller

% The mpcqpsolver function used in Simulink requires the first input to be 
% the inverse of the lower-triangular Cholesky decomposition of the 
% Hessian matrix H.
L_H = chol(H,'lower');
Linv = L_H\eye(size(H,1));

opt = mpcqpsolverOptions;
opt.FeasibilityTol = 1e-6; %Default is 1e-6
opt.IntegrityChecks = true; %False is recommended for code generation only

%-----------------------------------------------
% Create plant constraints
%-----------------------------------------------

% Inequality constraints: Aineq*U <= bineq

% Aineq = zeros(1,N_MPC*n_in);    % TODO
% bineq = zeros(1,1);             % TODO
Aineq = [eye(N_MPC);-eye(N_MPC)];
fracU_min = zeros(N_MPC,1)+tau_min;
fracU_max = zeros(N_MPC,1)+tau_max;
bineq = [fracU_max; -fracU_min];








%% Run simulation
sim('DoD_simulink_model');


figure(1);  clf;
subplot(2,2,1); hold on;
plot(tout,dtheta.signals.values.*180/pi);
plot(tout,ref.signals.values.*180/pi,'r');
legend('dtheta');
grid on;
title('Hand angular velocity');

subplot(2,2,2);
plot(tout,dphi.signals.values.*180/pi); hold on;
yyaxis right;
plot(tout,phi.signals.values.*180/pi);
legend('dphi','phi');
grid on;
title('Object angular velocity and angle (about hand CoM)');

subplot(2,2,3);
plot(tout,theta.signals.values.*180/pi);
legend('theta');
grid on;
title('Hand angle');

subplot(2,2,4);
plot(tau.time,tau.signals.values);
legend('tau');
grid on;
title('Demanded Torque');



%% Visulisation?

runVis(rh, ro, phi, theta, tout);

% 



