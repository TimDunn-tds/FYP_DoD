function plantModel = getPlantModel()
plantParams = getPlantParams();
% Unpack params
Jh = plantParams.Jh;
Jo = plantParams.Jo;
rh = plantParams.rh;
ro = plantParams.ro;
g  = plantParams.g;
mo = plantParams.mo;
% mh = plantParams.mh;

% State variables:
% x(1) = hand angular velocity [rad/s]
% x(2) = object angular velocity about hand CoM [rad/s]
% x(3) = hand angle [rad]
% x(4) = object CoM angle relative to hand CoM [rad]

MM = [
    Jh + Jo*(rh^2/ro^2), 2*Jo*(rh^2/ro^2);
    2*Jo*(rh^2/ro^2), Jo*(rh^2/ro^2) + mo*((rh + ro)^2)];

KK = [0, 0; 0, -mo*g*(rh + ro)];

DD = [0.00, 0; 0, 0];

e = [1;0];

% Create matrices 
plantModel.A = [MM\-DD, MM\-KK; eye(2,2), zeros(2,2)];

plantModel.B = [MM\e; zeros(2,1)];

plantModel.Cr = [1, 0, 0, 0]; % Regulate hand velocity
plantModel.Dr = 0; % No feedthrough

% Cm = [0, 0, 1, 0;
%       0, 0, 0, 1]; % Measure hand and object positions
plantModel.Cm = [0, 0, 1, 0;
      0, 0, 0, 1;
      1, 0, 0, 0]; % Measure hand and object positions, and hand velocity
plantModel.Dm = [0;0;0];

% Reduced-state continuous-time model for control design
plantModel.idx = [1 2 4];                % Only include these states
plantModel.Ac  = plantModel.A(plantModel.idx,plantModel.idx);
plantModel.Bc  = plantModel.B(plantModel.idx,:);
plantModel.Cc  = plantModel.Cr(:,plantModel.idx);
plantModel.Dc  = plantModel.Dr;

plantModel.T = 0.01;


