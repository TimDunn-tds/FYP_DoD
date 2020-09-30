function LQG = getLQG(T)

plantParams = getPlantParams();
% Unpack params
Jo = plantParams.Jo;
rh = plantParams.rh;
ro = plantParams.ro;
g  = plantParams.g;
mo = plantParams.mo;


% Design LQG
% State variables:
% x(1) = hand angular velocity [rad/s]                  dtheta
% x(2) = object angular velocity about hand CoM [rad/s] dphi
% x(4) = object CoM angle relative to hand CoM [rad]    phi
Qc = [
    1, 0, 0;
    0, 0, 0;
    0, 0, 1]; % penalise state
Rc = 1e10;      % penalise actuator

% plantModel = getPlantModel(T);
idx = [1 2 3];
% idx = [3];

bot = mo*((rh+ro)^2) + Jo*((rh^2)/(ro^2));

Ac = [0, 0, 0;
     0, 0, (mo*g*(rh+ro))/bot;
     0, 1, 0];

Bc  = [1; (Jo*(rh^2/ro^2))/bot; 0];
Cc  = [1, 0, 0];    % Regulate hand velocity
Dc  = 0;            % No feedthrough
Cr  = Cc;

Kc = zeros(1,length(idx));
Kc(:,idx)   = lqrd(Ac, Bc, Qc, Rc, T);	% Feedback gain matrix

% Design feedforward gains (see eqn 8.73 from Franklin et. al. (1998))
[Acd, Bcd]  = c2d(Ac, Bc, T);
NN          = [Acd - eye(length(idx)), Bcd; Cc, Dc]\[zeros(length(idx),1);1];
Nx          = zeros(length(Cr),1);
Nx(idx,:)   = NN(1:length(idx));
Nu          = NN(end);

LQG.Acd = Acd;
LQG.Bcd = Bcd;
LQG.Cc = Cc;
LQG.Dc = Dc;

LQG.Kc = Kc;
LQG.Nx = Nx;
LQG.Nu = Nu;



