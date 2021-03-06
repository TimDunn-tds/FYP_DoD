function LQG = getLQG(T)
% Design LQG
% State variables:
% x(1) = hand angular velocity [rad/s]                  dtheta
% x(2) = object angular velocity about hand CoM [rad/s] dphi
% x(4) = object CoM angle relative to hand CoM [rad]    phi
Qc = [
    1, 0, 0;
    0, 1, 0;
    0, 0, 0.1]; % penalise state
Rc = 1e12;      % penalise actuator

plantModel = getPlantModel(T);
idx = plantModel.idx;
Ac  = plantModel.Ac;
Bc  = plantModel.Bc;
T   = plantModel.T;
Cc  = plantModel.Cc;
Dc  = plantModel.Dc;
Cr  = plantModel.Cr;

Kc = zeros(1,length(idx));
Kc(:,idx)   = lqrd(Ac, Bc, Qc, Rc, T);	% Feedback gain matrix

% Design feedforward gains (see eqn 8.73 from Franklin et. al. (1998))
[Acd, Bcd]  = c2d(Ac, Bc, T);
NN          = [Acd - eye(length(idx)), Bcd; Cc, Dc]\[zeros(length(idx),1);1];
Nx          = zeros(length(Cr),1);
Nx(idx,:)   = NN(1:length(idx));
Nu          = NN(end);


LQG.Kc = Kc;
LQG.Nx = Nx;
LQG.Nu = Nu;



