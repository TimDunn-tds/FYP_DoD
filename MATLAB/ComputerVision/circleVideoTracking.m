clear; clc; close all;
load('deskDrawerParamsHR.mat');
% load('cameraParams.mat');

vid = webcam(2);
% vid.Resolution = '320x240';
vid.Resolution = '640x480';

tsim = 100;

% Create rotation and Translation matrices
R = cameraParams.RotationMatrices(:,:,end);
T = cameraParams.TranslationVectors(end,:);

angle = zeros(1,tsim);
tic
for i = 1:tsim
    %% Capture the image
    imOrig = snapshot(vid);
    
    %% Undistort the image
    [im, newOrigin] = undistortImage(imOrig, cameraParams, 'OutputView', 'full');

    %% Segment the coins   
    magnification = 50;

    % Convert to HSV colour space
    imHSV = rgb2hsv(im);
%     imshow(imHSV, 'InitialMagnification', magnification);

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
    'MaximumBlobArea', 2500,...
    'MinimumBlobArea', 900, 'ExcludeBorderBlobs', true);
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
    scale = magnification / 40;
    imDetectedCoins = imresize(im, scale);

    % Insert labels for the circles.
    imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'circle', ...
    scale * circles(1,:), 'hand');
    imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'circle', ...
    scale * circles(2,:), 'object');
    figure(1);
    imshow(imDetectedCoins);
%     txt = ['Detected circles. Time = ', num2str(i)];
%     title(txt);
    drawnow limitrate;
%     pause(0.01);
    
    %% Measure the circles
    % Adjust upper left corners of bounding boxes for coordinate system shift 
    % caused by undistortImage with output view of 'full'. This would not be
    % needed if the output was 'same'. The adjustment makes the points compatible
    % with the cameraParameters of the original image.
    boxes = boxes + [newOrigin, 0, 0]; % zero padding is added for width and height

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

    %% Measure angle between the coins
    theta = atan2(-(wp_bo2_cent(1) - wp_bo1_cent(1)), -(wp_bo2_cent(2) - wp_bo1_cent(2)));
    angle(i) = theta;
    fprintf('Measured angle between the two circle centres = %0.2f deg from positive y axis\n', rad2deg(theta));

    
end
toc


















clear vid;