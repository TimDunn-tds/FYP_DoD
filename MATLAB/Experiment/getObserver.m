function obs = getObserver(T)

KF = getKalmanFilter(T);
A   = KF.Aod;
B   = KF.Bod;
T   = KF.T;
Cm  = KF.Cm;
Dm  = KF.Dm;

[obs.Aod, obs.Bod] = c2d(A, B, T);

obs.Qo = KF.Qo;
obs.Ro = KF.Ro;

[~,obs.Lo] = kalmd(ss(obs.Aod, [obs.Bod eye(length(obs.Bod))], Cm, [Dm zeros(size(Dm,1),length(obs.Bod))]), obs.Qo, obs.Ro, T);
