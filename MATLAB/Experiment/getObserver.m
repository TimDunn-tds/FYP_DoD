function obs = getObserver(T)

KF = getKalmanFilter(T);
A   = KF.Aod;
B   = KF.Bod;
T   = KF.T;
obs.Cm = [  1, 0, 0, 0;
            0, 0, 1, 0;
            0, 0, 0, 1];
  
obs.Dm  = [0; 0; 0];

[obs.Aod, obs.Bod] = c2d(A, B, T);

% dtheta, dphi, theta, phi
% obs.Qo = diag([1e2, 1e2, 1e-1, 1e-1]);
obs.Qo = diag([1e0, 1e-6, 1e0, 1e-8]);

% dtheta, theta, phi
obs.Ro = diag([1e-1, 1e-3, 1e8]);

sys = ss(obs.Aod, [obs.Bod eye(length(obs.Bod))], obs.Cm, [obs.Dm zeros(size(obs.Dm,1),length(obs.Bod))], -1);
[~,obs.Lo] = kalman(sys, obs.Qo, obs.Ro);


% obs.Qo = diag([1e8, 1e8, 1e6, 1e1]);

% obs.Ro = diag([1e-1, 1e-1, 1e0]);
