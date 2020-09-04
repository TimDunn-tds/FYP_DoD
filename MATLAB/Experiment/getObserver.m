function obs = getObserver(T)

plantModel = getPlantModel(T);
A   = plantModel.A;
B   = plantModel.B;
T   = plantModel.T;
Cm  = plantModel.Cm;
Dm  = plantModel.Dm;

[obs.Aod, obs.Bod] = c2d(A, B, T);

obs.Qo = diag([1e-2, 1e-2, 1e-5, 1e-5]);
obs.Ro = diag([1e-6, 1e-6, 9.0851e-04]);

[~,obs.Lo] = kalmd(ss(obs.Aod, [obs.Bod eye(length(obs.Bod))], Cm, [Dm zeros(size(Dm,1),length(obs.Bod))]), obs.Qo, obs.Ro, T);
