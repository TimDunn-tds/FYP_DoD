clc;
load('gtruth2.mat');
positiveInstances = gTruth.LabelData(:,1:2);

imDir = fullfile('circles');
addpath(imDir);

negativeFolder = fullfile('notCircles');
negativeImages = imageDatastore(negativeFolder);

trainCascadeObjectDetector('circleDetector.xml', positiveInstances,...
    negativeFolder, 'FalseAlarmRate', 0.1, 'NumCascadeStages', 3);

detector = vision.CascadeObjectDetector('circleDetector.xml');


%% 
img = imread('circ.jpg');

bbox = detector(img);

detectedImg = insertObjectAnnotation(img, 'rectangle', bbox, 'circle');

figure; imshow(detectedImg);

rmpath(imDir);


%%
vid = videoinput('winvideo');
im = getsnapshot(vid);

bbox = detector(im);

detectedImg = insertObjectAnnotation(im, 'rectangle', bbox, 'circle');

figure; imshow(detectedImg);

