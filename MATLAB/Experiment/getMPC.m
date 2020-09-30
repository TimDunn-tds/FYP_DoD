function MPC = getMPC(LQG,T)
% Unpack params
Acd = LQG.Acd;
Bcd = LQG.Bcd;
Cc = LQG.Cc;
Dc = LQG.Dc;

% Create MPC Controller
% System dimensions
n_in = size(Bcd,2);     % No. actuators
n_out = size(Cc,1);     % No. outputs to regulate 
n_states = size(Acd,1); % No. states


N_MPC = 5; % Horizon

% Quadratic cost weighting
Ru = 1e8;           % Penalty on input moves (i.e. ||u[k]-u[k-1]||)
Qy = 1e-8;      	% Penalty on output error 7 -8


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
alphaMax = 4;
Aineq = [eye(N_MPC);-eye(N_MPC)];
fracU_min = zeros(N_MPC,1) + alphaMax/T;
fracU_max = zeros(N_MPC,1) - alphaMax/T;
bineq = [fracU_max; -fracU_min];

MPC.Gamma = Gamma;
MPC.Linv = Linv;
MPC.Acd = Acd;
MPC.Bcd = Bcd;
MPC.Aineq = Aineq;
MPC.bineq = bineq;
MPC.n_in = n_in;
MPC.opt = opt;



