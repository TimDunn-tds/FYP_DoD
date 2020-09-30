function u = runMPC(ref, mu0, u0, MPC)
Linv = MPC.Linv;
opt = MPC.opt;
n_in = MPC.n_in;
Gamma = MPC.Gamma;
Acd = MPC.Acd;
Bcd = MPC.Bcd;
Aineq = MPC.Aineq;
bineq = MPC.bineq;

% Predict state at next time step
mu1 = Acd*mu0 + Bcd*u0;

% Compute linear coefficient of QP
kk = [mu1; u0; ref];
g = Gamma*kk;

iA0 = false(size(Aineq,1),1);   % Initial active constraints

% The KWIK algorithm defines inequality constraints as Ax >= b rather than 
% Ax <= b, as in the quadprog command.
[uMPC, ~] = mpcqpsolver(Linv, g, -Aineq, -bineq, [], zeros(0,1), iA0, opt);

% Return next control action
u = uMPC(1:n_in);