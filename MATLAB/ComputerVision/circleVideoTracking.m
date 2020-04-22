clear; clc;
load('deskDrawerParams.mat');
vid = webcam();
vid.Resolution = '320x240';

tsim = 100;

% Create rotation and Translation matrices
R = cameraParams.RotationMatrices(:,:,end);
T = cameraParams.TranslationVectors(end,:);

angle = zeros(1,tsim);

for i = 1:tsim
    %% Capture the image
    imOrig = snapshot(vid);
    
    %% Undistort the image
    [im, newOrigin] = undistortImage(imOrig, cameraParams, 'OutputView', 'full');

    %% Segment the coins   
    magnification = 50;

    % Convert to HSV colour space
    imHSV = rgb2hsv(im);

    % Get the saturation channel.
    sat = imHSV(:, :, 2);

    % Attempt levels of thresholding
    t = multithresh(sat,4);
    imCircles = (sat > t(3));

    %     figure(8); 
    %     imshow(imCoin2, 'InitialMagnification', magnification);
    %     title('Segmented Coins');

    
    %% Detect circles
    % Find connected components.
    blobAnalysis = vision.BlobAnalysis('AreaOutputPort', true,...
    'CentroidOutputPort', false,...
    'BoundingBoxOutputPort', true,...
    'MaximumBlobArea', 250,...
    'MinimumBlobArea', 100, 'ExcludeBorderBlobs', true);
    [areas, boxes] = blobAnalysis(imCircles);

    % Sort connected components in descending order by area
    [~, idx] = sort(areas, 'Descend');
    
    if size(boxes,1) <= 1      
        continue
    end
    % Get the two largest components.
    boxes = double(boxes(idx(1:2), :));
    % Reduce the size of the image for display.
    scale = magnification / 20;
    imDetectedCoins = imresize(im, scale);

    % Insert labels for the circles.
    imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'rectangle', ...
    scale * boxes(1,:), 'hand');
    imDetectedCoins = insertObjectAnnotation(imDetectedCoins, 'rectangle', ...
    scale * boxes(2,:), 'object');
    figure(1);
    imshow(imDetectedCoins);
    txt = ['Detected circles. Time = ', num2str(i)];
    title(txt);
    drawnow;
    pause(0.1);
    
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
    theta = atan2((wp_bo2_cent(1) - wp_bo1_cent(1)), (wp_bo2_cent(2) - wp_bo1_cent(2)));
    angle(i) = theta;
    fprintf('Measured angle between the two circle centres = %0.2f deg from positive y axis\n', rad2deg(theta));

    
end



















clear vid;