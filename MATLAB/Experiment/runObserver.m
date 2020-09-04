function mu = runObserver(mutm1, u, y, plantModel, obs)
Aod = obs.Aod;
Bod = obs.Bod;
Cm  = plantModel.Cm;
Dm  = plantModel.Dm;
Lo  = obs.Lo;

mu = Aod*mutm1 + Bod*u + Lo*(y - Cm*mutm1 - Dm*u);
