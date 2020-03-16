%% Clear parameters
clc; clear all; 

T = 0.01;
tsim = 15;

%% Parameters
% mo = 0.1;                   % kg
% mh = 0.3;                   % kg

% ro = 0.075;                  % m
ro = 0.15;
rh = 0.15;                   % m
% 
% ro = ro/2;
% rh = rh/2;

thick = 0.01;               % m

p = 1180;                   % kg/m3

mo = pi*(ro^2)*thick*p;
mh = pi*(rh^2)*thick*p;

Jo = 0.5*mo*(ro^2);
Jh = 0.5*mh*(rh^2);

g = 9.82;                   % m/s^2

theta0 = 0*(pi/180);        % rad
phi0 = -1*(pi/180);          % rad


%% Motor parameters
tau_max = 1;              % Nm
tau_min = -1;             % Nm


%% Linearised Plant Model (linearised about upright balancing position)
% State variables:
% x(1) = hand angular velocity [rad/s]
% x(2) = object angular velocity about hand CoM [rad/s]
% x(3) = hand angle [rad]
% x(4) = object CoM angle relative to hand CoM [rad]

% MM = [
%     (mo + mh)*rh^2, -mo*rh*(ro + rh);
%     -mo*rh*(ro + rh), 2*mo*(rh + ro)^2];

% MM = [
%     Jh + Jo*(2*(rh/ro) + (rh^2/ro^2) + 1), -Jo*(rh/ro + (rh^2/ro^2));
%     -Jo*(rh/ro + (rh^2/ro^2)), Jo*(rh^2/ro^2) + mo*(rh + ro)^2];
% 
% MM = [
%     Jh + Jo*(-2*(rh/ro) + (rh^2/ro^2) + 1), 2*Jo*(rh/ro - (rh^2/ro^2));
%     2*Jo*(rh/ro - (rh^2/ro^2)), Jo*(rh^2/ro^2) + mo*((rh + ro)^2)];

MM = [
    Jh + Jo*(rh^2/ro^2), 2*Jo*(rh^2/ro^2);
    2*Jo*(rh^2/ro^2), Jo*(rh^2/ro^2) + mo*((rh + ro)^2)];


% KK = [-mo*g*(rh + ro), 0; 0, -mo*g*(rh + ro)];
KK = [0, 0; 0, -mo*g*(rh + ro)];


DD = [0.001, 0; 0, 0];

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


% [Acd, Bcd] = c2d(Ac,Bc,T);

%% Design LQG
Qc = [
    1, 0, 0;
    0, 1, 0;
    0, 0, 1];           % penalise state
Rc = 50;                 % penalise actuator 

Kc = zeros(1,length(idx));
Kc(:,idx)   = lqrd(Ac, Bc, Qc, Rc, T);	% Feedback gain matrix

% Design feedforward gains (see eqn 8.73 from Franklin et. al. (1998))
[Acd, Bcd]  = c2d(Ac, Bc, T);
NN          = [Acd - eye(length(idx)), Bcd; Cc, Dc]\[zeros(length(idx),1);1];
Nx          = zeros(length(Cr),1);
Nx(idx,:)   = NN(1:length(idx));
Nu          = NN(end);




%% Old MPC
% %% Create MPC Controller
% 
% % System dimensions
% n_in = size(Bcd,2);     % No. actuators
% n_out = size(Cc,1);     % No. outputs to regulate 
% n_states = size(Acd,1); % No. states
% 
% 
% N_MPC = 40; % Horizon
% 
% % Quadratic cost weighting
% Ru = 1;         % Penalty on input moves (i.e. ||u[k]-u[k-1]||)
% Qy = 1;      	% Penalty on output error


% %% MPC stuff I dont understand
% % Consider the augmented system
% %
% % [ x[k+1] ] = [ A B ]*[ x[k]   ] + [ B ]*(u[k] - u[k-1])
% % [ u[k]   ]   [ 0 I ] [ u[k-1] ]   [ I ]
% % \________/   \_____/ \________/   \___/ \_____________/
% %   z[k+1]   =   Az   *  z[k]     +  Bz  * du[k]
% %
% % y[k] = [ C D ]*[ x[k]   ] + D*(u[k] - u[k-1])
% %                [ u[k-1] ]
% %        \_____/ \________/   | \_____________/
% %          Cz   *  z[k]     + Dz * du[k]
% 
% Az = [Acd, Bcd; zeros(n_in, n_states), eye(n_in)];
% Bz = [Bcd; eye(n_in)];
% Cz = [Cc, Dc];
% Dz = Dc;
% 
% n_z = n_states + n_in;     % Number of augmented states
% 
% AA = zeros((N_MPC+1)*n_z,n_z);
% for i = 1:N_MPC+1
%     AA(1+(i-1)*n_z:i*n_z,:) = Az^(i-1);
% end
% AB = zeros((N_MPC+1)*n_z,n_in);
% for i = 2:N_MPC+1
%     AB(1+(i-1)*n_z:i*n_z,:) = Az^(i-2)*Bz;
% end
% BB = zeros((N_MPC+1)*n_z,N_MPC*n_in);
% for i = 1:N_MPC
%     BB(1+(i-1)*n_z:end,1+(i-1)*n_in:i*n_in) = AB(1:(N_MPC-i+2)*n_z,:);
% end
% WW = kron(eye(N_MPC)-diag(ones(1,N_MPC-1),-1),eye(n_in));
% EE = zeros(N_MPC*n_in,n_states+n_in);
% EE(1:n_in,1+n_states:end) = -eye(n_in);
% 
% Qz = Cz.'*Qy*Cz;
% Sz = Cz.'*Qy*Dz;
% Rz = Dz.'*Qy*Dz + Ru;
% [~,Qf] = dlqr(Az,Bz,Qz,Rz,Sz);
% % [~,Qf] = lqry(ss(Az,Bz,Cz,Dz,T),Qy,Ru);
% 
% QQ = blkdiag(kron(eye(N_MPC),Qz),Qf);
% SS = kron(eye(N_MPC+1,N_MPC),Sz);
% RR = kron(eye(N_MPC),Ru);
% 
% % Compute feedforward gains (we assume n_in == n_out)
% NN = [Acd - eye(n_states), Bcd; Cc, Dc]\[zeros(n_states,n_out);eye(n_out)];
% 
% %-----------------------------------------------
% % Generate the Hessian H and gradient coefficient
% %-----------------------------------------------
% H = WW.'*( BB.'*QQ*BB + BB.'*SS + SS.'*BB + RR )*WW;
% Gamma = WW.'*[(BB.'*QQ + SS.')*(AA + BB*EE) + (BB.'*SS + RR)*EE, -(BB.'*QQ + SS.')*kron(ones(N_MPC+1,1),NN)];
% % Note: The gradient is g = Gamma*[x1; u0; ref], but we do this
% %       multiplication inside the controller
% 
% % The mpcqpsolver function used in Simulink requires the first input to be 
% % the inverse of the lower-triangular Cholesky decomposition of the 
% % Hessian matrix H.
% L_H = chol(H,'lower');
% Linv = L_H\eye(size(H,1));
% 
% opt = mpcqpsolverOptions;
% opt.FeasibilityTol = 1e-6; %Default is 1e-6
% opt.IntegrityChecks = true; %False is recommended for code generation only
% 
% %-----------------------------------------------
% % Create plant constraints
% %-----------------------------------------------
% 
% % Inequality constraints: Aineq*U <= bineq
% 
% % Aineq = zeros(1,N_MPC*n_in);    % TODO
% % bineq = zeros(1,1);             % TODO
% Aineq = [eye(N_MPC);-eye(N_MPC)];
% fracU_min = zeros(N_MPC,1)+tau_min;
% fracU_max = zeros(N_MPC,1)+tau_max;
% bineq = [fracU_max; -fracU_min];
% 



%% Run simulation
sim('DoD_simulink_model');

figure(1);  clf;

subplot(2,2,1);
plot(tout,phi.signals.values.*180/pi); hold on;
% plot(tout, theta.signals.values.*180/pi);
legend('phi');
grid on;
title('Object angle (about hand CoM');

subplot(2,2,2);
plot(tout,dphi.signals.values.*180/pi); hold on;
% yyaxis right;
% plot(tout,phi.signals.values.*180/pi);
legend('dphi');
grid on;
title('Object angular velocity (about hand CoM)');

subplot(2,2,3); hold on;
plot(tout,dtheta.signals.values.*180/pi);
plot(tout,ref.signals.values.*180/pi,'r');
legend('dtheta');
grid on;
title('Hand angular velocity');

subplot(2,2,4);
plot(tau.time,tau.signals.values);
legend('tau');
grid on;
title('Demanded Torque');

% figure(3); clf; hold on;
% alph = (rh/ro).*(phi.signals.values - theta.signals.values);
% plot(theta.time, abs(alph), 'DisplayName', 'alpha');
% plot(theta.time, abs(theta.signals.values), 'DisplayName', 'theta');
% legend;


%% Visulisation?

% F = runVis(rh, ro, phi, theta, dphi, dtheta, tout);

%% Save video
% writerObj = VideoWriter('DoD_vid.avi');
% writerObj.FrameRate = 100;
% 
% open(writerObj);
% 
% for i=1:length(F)
%     frame = F(i);
%     writeVideo(writerObj, frame);
% end
% close(writerObj);




