clc; close all;
if (exist('mypi','var'))
%     delete(vid);   % Close video connection
    clear vid;
    clear mypi
end
% load('apparatusParams2.mat');
% load('cameraParams.mat');
load('piCam.mat');

% vid = videoinput('winvideo',1, 'RGB24_320x240');
% % vid = videoinput('winvideo',3, 'RGB24_640x480');
% triggerconfig(vid, 'manual');
% vid.FramesPerTrigger = 1;
% vid.TriggerRepeat = inf;
% start(vid);
% preview(vid);
tsim = 1000;

% cam = imaqhwinfo; % Get Camera information
% cameraName = char(cam.InstalledAdaptors(end));
% cameraInfo = imaqhwinfo(cameraName);
% cameraId = 2;
% % cameraFormat ='RGB24_640x480';
% cameraFormat ='MJPG_1920x1080';
% 
% vid = imaq.VideoDevice(cameraName, cameraId, cameraFormat, ... % Input Video from current adapter
%                 'ReturnedColorSpace', 'RGB');
% vid = ipcam("http://rbpi:8080/?action=stream");
% preview(vid)

% PiCam
mypi = raspi('rbpi.local','pi','password');
% ROI = [235 0 488-235 0]./640;
vid = cameraboard(mypi,...
    'Quality', 10,...
    'Rotation', 90,...
    'FrameRate',90,...
    'AWBMode', 'fluorescent',...
    'ExposureMode','sports');

snapshot(vid);
snapshot(vid);
snapshot(vid);
snapshot(vid);
snapshot(vid);



% Create rotation and Translation matrices
R = cameraParams.RotationMatrices(:,:,end);
T = cameraParams.TranslationVectors(end,:);

angle = zeros(tsim,1);
frames  = single(zeros(240,320,3));

loopRate = 30;
r = robotics.Rate(loopRate);





%%
for i = 1:tsim
    %% Capture the image
%     tic
    imOrig = snapshot(vid);
%     toc
%     imOrig = step(vid);
    
    %% Undistort the image
%     tic
    [im, newOrigin] = undistortImage(imOrig, cameraParams, 'OutputView', 'same');

    %% Segment the coins   
    magnification = 100;

    % Convert to HSV colour space
    imHSV = rgb2hsv(im);
    imshow(imHSV, 'InitialMagnification', magnification);
%     imshow(imOrig, 'InitialMagnification', magnification);

    % Get the saturation channel.
    sat = imHSV(:, :, 2);

    % Attempt levels of thresholding
%     t = multithresh(sat,1);
    t = graythresh(sat);

    imCircles = (sat > t(1));

    % Threshold the image
%     t = graythresh(sat);
%     imCircles = (sat > t);
    
%     figure(8); 
%     imshow(imCircles, 'InitialMagnification', magnification);
%     title('Segmented Coins');

    
    %% Detect circles
    % Find connected components.
    blobAnalysis = vision.BlobAnalysis('AreaOutputPort', true,...
    'CentroidOutputPort', true,...
    'BoundingBoxOutputPort', true,...
    'MaximumBlobArea', 6000,...
    'MinimumBlobArea', 3000,...
    'ExcludeBorderBlobs', true,...
    'MaximumCount', 3);
%     [areas, centroid, boxes] = blobAnalysis(imCircles(1:460,138:555));
    [areas, centroid, boxes] = blobAnalysis(imCircles);
%     areas
    % Sort connected components in descending order by area
    [~, idx] = sort(areas, 'Descend');
    
    if size(boxes,1) < 2    
        fprintf("failure\n")
        continue;
    end

    % Get the two largest components.
    boxes = double(boxes(idx(1:2), :));
    centroid = double(centroid(idx(1:2),:));
    circles = [centroid, [mean(boxes(1,3:4))/2; mean(boxes(2,3:4))/2]];

    % Reduce the size of the image for display.
    scale = magnification / 100;
    imDetectedCoins = imresize(im, scale);
%     imshow(imDetectedCoins);

    % Insert labels for the circles.
%     imDetectedCoins = insertObjectAnnotation(imDetectedCoins(1:460,138:555), 'circle', ...
    imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'circle', ...
    scale * circles(1,:), 'hand');
    imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'circle', ...
    scale * circles(2,:), 'object');
    imDetectedCoins = insertMarker(imDetectedCoins, centroid);
%     figure(1);
%     imshow(imDetectedCoins);
%     txt = ['Detected circles. Time = ', num2str(i)];
%     title(txt);
%     drawnow limitrate;
%     pause(0.04);
    
    %% Measure the circles
    % Adjust upper left corners of bounding boxes for coordinate system shift 
    % caused by undistortImage with output view of 'full'. This would not be
    % needed if the output was 'same'. The adjustment makes the points compatible
    % with the cameraParameters of the original image.
    boxes = boxes + [newOrigin, 0, 0]; % zero padding is added for width and height
%     centroids = centroid + newOrigin;
    
    % Get the top-left and the top-right corners of box 1.
    box1 = double(boxes(1, :));
    imagePoints1 = [box1(1:2); ...
                    box1(1) + box1(3), box1(2)];

    % Get the world coordinates of the corners            
    worldPoints1 = pointsToWorld(cameraParams, R, T, imagePoints1);
    
    % Get the top-left and the top-right corners.
    box2 = double(boxes(2, :));
    imagePoints2 = [box2(1:2); ...
                    box2(1) + box2(3), box2(2)];

    % Apply the inverse transformation from image to world            
    worldPoints2 = pointsToWorld(cameraParams, R, T, imagePoints2);   
    
    ip_box1_cent = [box1(1) + box1(3)/2, box1(2) + box1(4)/2];
    ip_box2_cent = [box2(1) + box2(3)/2, box2(2) + box2(4)/2];

    wp_bo1_cent = pointsToWorld(cameraParams, R, T, ip_box1_cent);
    wp_bo2_cent = pointsToWorld(cameraParams, R, T, ip_box2_cent);
    
    % Align centroids to new origin of image with cameraparams
    centroids = centroid + newOrigin;
    % Calculate world points from image points
%     centWP = pointsToWorld(cameraParams, R, T, centroids);
    

    %% Measure angle between the coins
%     theta_marker = atan2(centWP(1,2) - centWP(2,2), centWP(2,1) - centWP(2,2)) + pi/2;
    theta_marker = atan2(centroids(1,2) - centroids(2,2), centroids(2,1) - centroids(1,1)) + pi/2;

    theta_marker = -1*asin(0.13*sin(pi-theta_marker)/0.18);
    
    angle(i) = theta_marker;
    fprintf('Phi = %0.2f deg from positive y axis\n', rad2deg(theta_marker));
    
    text = ['Angle: ', num2str(angle(i)*180/pi)];
    imDetectedCoins = insertText(imDetectedCoins, [1,240], text);
    frames = imDetectedCoins;
%     toc
%     imshow(imDetectedCoins, 'InitialMagnification', magnification);
%     waitfor(r);

    
end
%%
delete(vid)
clear('vid')
