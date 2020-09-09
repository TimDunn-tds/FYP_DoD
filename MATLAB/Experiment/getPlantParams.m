function params = getPlantParams()
params.ro       = 0.06;    % m
params.rh       = 0.12;    % m
params.thick    = 0.01;     % m
params.rho      = 1200;     % kg/m3


params.mo   = pi*(params.ro^2)*params.thick*params.rho;  % kg
params.mh   = pi*(params.rh^2)*params.thick*params.rho;  % kg

params.Jo   = 0.5*params.mo*(params.ro^2);  % kg*m2
% params.Jh   = 3.911e-3;                     % kg*m2
params.Jh   = 0.0157;                     % kg*m2
% params.Jh   = 0.0225;                     % kg*m2

params.g    = 9.8;  % m/s^2

% params.Jm   = 0.0138;