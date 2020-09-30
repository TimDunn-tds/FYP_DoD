function plantModel = getPlantModel(T)
plantParams = getPlantParams();
% Unpack params
Jo = plantParams.Jo;
rh = plantParams.rh;
ro = plantParams.ro;
g  = plantParams.g;
mo = plantParams.mo;

% State variables:
% x(1) = hand angular velocity [rad/s]      dtheta
% x(2) = object angular velocity [rad/s]    dphi
% x(3) = object angle [rad/s]               phi
% 
% % Create matrices 
% bot = mo*((rh+ro)^2) + Jo*((rh^2)/(ro^2));
% 
% plantModel.A = [0, 0, 0;
%                 0, 0, (mo*g*(rh+ro))/bot;
%                 0, 1, 0];
% 
% plantModel.B = [1; (Jo*(rh^2/ro^2))/bot; 0];
% 
% plantModel.Cr = [1, 0, 0]; % Regulate hand velocity
% 
% plantModel.Dr = 0; % No feedthrough

% Cm = [0, 0, 1, 0;
%       0, 0, 0, 1]; % Measure hand and object positions
% plantModel.Cm = [0, 0, 1, 0;
%                 0, 0, 0, 1;
% %                 1, 0, 0, 0]; % Measure hand and object positions, and hand velocity
% plantModel.Dm = [0;0;0];

% Reduced-state continuous-time model for control design
% plantModel.idx = [1 2 3];                % Only include these states
% plantModel.Ac  = plantModel.A(plantModel.idx,plantModel.idx);
% plantModel.Bc  = plantModel.B(plantModel.idx,:);
% plantModel.Cc  = plantModel.Cr(:,plantModel.idx);
% plantModel.Dc  = plantModel.Dr;

plantModel.T = T;


