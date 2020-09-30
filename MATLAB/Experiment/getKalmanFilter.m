function KF = getKalmanFilter(T)
% Design Kalman Filter
% Initial uncertainty and intial state
x0          = getInitialPosition();
theta0      = x0.theta0;
phi0        = x0.phi0;
mup_init    = [0; 0; theta0; phi0];
% Pp_init     = 1e2*eye(4);
Pp_init    = diag([1e0, 1e0, 1e0, 1e0]);
% Pp_init    = diag([1e2, 1e-4, 1e-4, 1e10]);

% Get plant model
plantParams = getPlantParams();
% Unpack params
Jo = plantParams.Jo;
rh = plantParams.rh;
ro = plantParams.ro;
g  = plantParams.g;
mo = plantParams.mo;

bot = mo*((rh+ro)^2) + Jo*((rh^2)/(ro^2));

A = [0, 0, 0, 0;
     0, 0, 0, (mo*g*(rh+ro))/bot;
     1, 0, 0, 0;
     0, 1, 0, 0];
 
B = [1; (Jo*((rh^2/ro^2)))/bot; 0; 0];

Cm = [1, 0, 0, 0;
      0, 0, 1, 0;
      0, 0, 0, 1];

Dm = [0; 0; 0];

[Aod, Bod] = c2d(A, B, T);


% Qo = diag([1e0, 1e10, 1e-1, 1e0]);
% % 
% Ro = diag([1e-1, 1e-2, 1e-3]);
Qo = diag([1e0, 1e2, 1e-1, 1e0]);

Ro = diag([1e-1, 1e-2, 1e-4]);
% Pack structure
KF.mup_init = mup_init;
KF.Pp_init = Pp_init;
KF.Aod = Aod;
KF.Bod = Bod;
KF.Qo = Qo;
KF.Ro = Ro;
KF.Cm = Cm;
KF.Dm = Dm;
KF.T = T;
KF.idx = [1,2,4];
