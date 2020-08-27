clear; clc; close all;
load('apparatusParams.mat');
% load('cameraParams.mat');

vid = webcam(1);
% vid.Resolution = '320x240';
vid.Resolution = '640x480';
preview(vid);
tsim = 1000;


% Create rotation and Translation matrices
R = cameraParams.RotationMatrices(:,:,end);
T = cameraParams.TranslationVectors(end,:);

angle = zeros(tsim,1);
frames = uint8(zeros(480,640,3,tsim));
tic
for i = 1:tsim
    %% Capture the image
    imOrig = snapshot(vid);
    
    %% Undistort the image
    [im, newOrigin] = undistortImage(imOrig, cameraParams, 'OutputView', 'same');

    %% Segment the coins   
    magnification = 100;

    % Convert to HSV colour space
    imHSV = rgb2hsv(im);
    imshow(imHSV, 'InitialMagnification', magnification);

    % Get the saturation channel.
    sat = imHSV(:, :, 2);

    % Attempt levels of thresholding
    t = multithresh(sat,2);
    imCircles = (sat > t(2));

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
    'MaximumBlobArea', 4500,...
    'MinimumBlobArea', 2500,...
    'ExcludeBorderBlobs', true,...
    'MaximumCount', 2);
%     [areas, centroid, boxes] = blobAnalysis(imCircles(1:460,138:555));
    [areas, centroid, boxes] = blobAnalysis(imCircles);

    % Sort connected components in descending order by area
    [~, idx] = sort(areas, 'Descend');
    
    if size(boxes,1) ~= 2    
        continue
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
    drawnow limitrate;
%     pause(0.04);
    frames(:,:,:,i) = imDetectedCoins;
    
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
    centWP = pointsToWorld(cameraParams, R, T, centroids);
    

    %% Measure angle between the coins
    theta_marker = atan2(centWP(1,2) - centWP(1,1), centWP(2,1) - centWP(2,2)) + pi/2;
    
    theta_marker = asin(0.13*sin(pi-theta_marker)/0.18);
    
    angle(i) = theta_marker;
    fprintf('Phi = %0.2f deg from positive y axis\n', rad2deg(theta_marker));
    
    if (29 - abs(theta_marker ))<2
        pause
    end
    
end
elapsed_time = toc



%% Save video

writerObj = VideoWriter('CV_testing.mp4');
writerObj.FrameRate = tsim/elapsed_time;
open(writerObj);

for i=1:tsim
    frame = frames(:,:,:,i);
    writeVideo(writerObj, frame);
end
close(writerObj);

% clear vid;