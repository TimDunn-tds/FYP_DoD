function KF = getKalmanFilter(T)
% Design Kalman Filter
% Initial uncertainty and intial state
x0          = getInitialPosition();
theta0      = x0.theta0;
phi0        = x0.phi0;
mup_init    = [0; 0; theta0; phi0];
Pp_init     = 1e2*eye(4);

% Get plant model
plantModel = getPlantModel(T);
A = plantModel.A;
B = plantModel.B;
T = plantModel.T;

[Aod, Bod] = c2d(A, B, T);

Qo = diag([1e-2, 1e-2, 1e-5, 1e-5]);
Ro = diag([1e-6, 1e-6, 9.0851e-04]);

% Pack structure
KF.mup_init = mup_init;
KF.Pp_init = Pp_init;
KF.Aod = Aod;
KF.Bod = Bod;
KF.Qo = Qo;
KF.Ro = Ro;